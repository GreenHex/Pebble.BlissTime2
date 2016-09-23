var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clayManipulator = require( './config_manipulator' );
var clay = new Clay( clayConfig, clayManipulator, { autoHandleEvents: false } );

var DEBUG = 0;

var myAPIKey = 'a64e1f53a22fcccc25458ea5e0b2daeb';

var CMD_TYPE = {
  CMD_UNDEFINED : 0,
  CMD_WEATHER : 1,
  CMD_STOCKS : 2
};
Object.freeze(CMD_TYPE);

var getWeatherGroupFromID = function( id ) {
 switch( id ) {
    case 200:
  	case 201:
  	case 202:
  	case 210:
  	case 211: return "Thunderstorm";
  	case 212:
  	case 221:
  	case 230:
  	case 231:
  	case 232: return "Thunderstorm";
  	case 300:
  	case 301: return "Drizzle";
  	case 302:
  	case 310:
  	case 311:
  	case 312:
  	case 313:
  	case 314:
  	case 321: return "Drizzle";
  	case 500: return "Light rain";
  	case 501: return "Moderate rain";
  	case 502:
  	case 503:
  	case 504: return "Extreme rain";
  	case 511:
  	case 520:
  	case 521:
  	case 522:
  	case 531: return "Rainy";
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
    case 701: return "Misty";
  	case 711: return "Smokey";
  	case 721: return "Hazy";
  	case 731: return "Dusty";
  	case 741: return "Foggy";
  	case 751: return "Sandy";
  	case 761: return "Dusty";
  	case 762: 
  	case 771:
  	case 781: return "Hazy";
  	case 801:
  	case 802:
  	case 803:
  	case 804: return "Cloudy";
  	case 800: return "Clear sky";
    case 900: return "Tornado";
    case 901:	return "Tropical storm";
    case 902: return "Hurricane";
    case 903: return "Cold";
    case 904: return "Hot";
    case 905: return "Windy";
    case 906: return "Hail";
    case 951: return "Calm";
    case 952: return "Light breeze";
    case 900: return "Tornado";
  	case 901: return "Tropical storm";
  	case 902: return "Hurricane";
  	case 903: return "Cold";
  	case 904: return "Hot";
  	case 905: return "Windy";
  	case 906: return "Hail";
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

/////// WEATHER STUFF
function locationSuccess(pos) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude + '&appid=' + myAPIKey;
  
  if (DEBUG) console.log("index.js: locationSuccess(): " + url);
  
  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
      
      if (DEBUG) console.log("index.js: locationSuccess(): " + JSON.stringify(json));
      
      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp);
       
      var temperature_unit = localStorage.getItem('TEMPERATURE_UNIT');
      if ( temperature_unit == 1 ) { // deg Fahrenheit
        temperature = Math.round(json.main.temp * 9/5 - 459.67);
        temperature = temperature + "°F";
      } else if ( temperature_unit == 2 ) { // Kelvin
        temperature = Math.round(json.main.temp);
        temperature = temperature + " K";
      } else { // deg Centigrade
        temperature = Math.round(json.main.temp - 273.15);
        temperature = temperature + "°C";
      }
      
      if (DEBUG) console.log("index.js: locationSuccess(): Temperature is " + temperature);
      
      // Conditions
      var conditions = getWeatherGroupFromID( json.weather[0].id ); // json.weather[0].main;      
      if (DEBUG) console.log("index.js: locationSuccess(): Conditions are " + conditions);
      
      
      // Assemble dictionary using our keys
      var dictionary = {
        "TEMPERATURE": temperature,
        "CONDITIONS": conditions
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          if (DEBUG) console.log("index.js: locationSuccess(): Weather info sent to Pebble successfully.");
        },
        function(e) {
          if (DEBUG) console.log("index.js: locationSuccess(): Error sending weather info to Pebble. " + JSON.stringify(e));
        }
      );
    }      
  );
}

function locationError(err) {
  if (DEBUG) console.log("index.js: Error requesting location!");
}

function getWeather() {
  if (DEBUG) console.log("index.js: getWeather().");
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

/////// STOCK STUFF
function getCMP(){
  var url = "https://finance.google.com/finance/info?client=ig&q=" + localStorage.getItem('STOCK_CODE');
  
  if (DEBUG) console.log("index.js: URL: " + url);
  
  xhrRequest(url, 'GET', 
    function(responseText) {
      var json = JSON.parse(responseText.replace(/\//g,""));
       if (DEBUG) console.log("index.js: CMP: " + JSON.stringify(json));
       var stock_code = localStorage.getItem('STOCK_CODE');
        var dictionary = {
          "STOCK_CODE": stock_code,
          "CMP": stock_code.substring(stock_code.indexOf(":") + 1) + ":" + json['0'].l
        };
        
        Pebble.sendAppMessage(dictionary,
          function(e) {
              if (DEBUG) console.log("index.js: getCMP(): CMP sent to Pebble successfully. " + JSON.stringify(dictionary));
          },
          function(e) {
              if (DEBUG) console.log("index.js: getCMP(): Error sending CMP to Pebble. " + JSON.stringify(e));
        });
    });
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    if (DEBUG) console.log("index.js: addEventListener(ready): PebbleKit JS ready.");
    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    if (DEBUG) console.log("index.js: addEventListener(appmessage): AppMessage received: " + JSON.stringify(e.payload));
    var dict = e.payload;
    if(dict.REQUEST) {
      var value = dict.REQUEST;
      if (DEBUG) console.log( "index.js: addEventListener(appmessage): Here it is: " + value );
      if ( value == CMD_TYPE.CMD_WEATHER) {
        if (DEBUG) console.log( "index.js: addEventListener(appmessage): getWeather()" );
        getWeather();
      } else if ( value == CMD_TYPE.CMD_STOCKS) {
        if (DEBUG) console.log( "index.js: addEventListener(appmessage): getStocks()" );
        getCMP();
      }
    }
  }                     
);

/////// Pebble/clay stuff
Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
  if (e && !e.response) {
    return;
  }
  
  var dict = clay.getSettings(e.response);
  if (DEBUG) console.log("index.js/clay: " + JSON.stringify(dict));
  localStorage.setItem('TEMPERATURE_UNIT', dict[10009]);
  localStorage.setItem('STOCK_CODE', dict[10017]);
  
  Pebble.sendAppMessage(dict, function(e) {
    console.log('index.js/clay: Sent config data to Pebble');
  }, function(e) {
    console.log('index.js/clay: Failed to send config data!');
    console.log(JSON.stringify(e));
  });
});
