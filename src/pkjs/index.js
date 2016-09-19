var myAPIKey = 'a64e1f53a22fcccc25458ea5e0b2daeb';

var getWeatherGroupFromID = function( id ) {
 switch( id ) {
    case 951: return "Calm";
    case 952: return "Light breeze";
    case 900: return "Tornado";
  	case 901: return "Tropical storm";
  	case 902: return "Hurricane";
  	case 903: return "Cold";
  	case 904: return "Hot";
  	case 905: return "Windy";
  	case 906: return "Hail";
  	case 801:
  	case 802:
  	case 803:
  	case 804: return "Cloudy";
  	case 800: return "Clear sky";
  	case 701:
  	case 711:
  	case 721:
  	case 731:
  	case 741:
  	case 751:
  	case 761:
  	case 762:
  	case 771:
  	case 781: return "Hazy";
  	case 600: 
  	case 601:
  	case 602:
  	case 611:
  	case 612:
  	case 615:
  	case 616:
  	case 620:
  	case 621:
  	case 622: return "Snowy";
  	case 500:
  	case 501:
  	case 502:
  	case 503:
  	case 504:
  	case 511:
  	case 520:
  	case 521:
  	case 522:
  	case 531: return "Rainy";
  	case 300:
  	case 301:
  	case 302:
  	case 310:
  	case 311:
  	case 312:
  	case 313:
  	case 314:
  	case 321: return "Drizzle";
  	case 200:
  	case 201:
  	case 202:
  	case 210:
  	case 211:
  	case 212:
  	case 221:
  	case 230:
  	case 231:
  	case 232: return "Thunderstorm";
  	default: return "Unknown";
  }
};

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude + '&appid=' + myAPIKey;
  
  console.log(url);
  
  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      
      console.log(JSON.stringify(json));
      
      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp - 273.15);
      console.log("Temperature is " + temperature);
      
      // Conditions
      var conditions = getWeatherGroupFromID( json.weather[0].id); // json.weather[0].main;      
      console.log("Conditions are " + conditions);
      
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_TEMPERATURE": temperature,
        "KEY_CONDITIONS": conditions
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending weather info to Pebble!");
        }
      );
    }      
  );
}

function locationError(err) {
  console.log("Error requesting location!");
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getWeather();
  }                     
);
