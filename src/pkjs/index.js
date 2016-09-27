var Clay = require( 'pebble-clay' );
var clayConfig = require( './config' );
var clayManipulator = require( './config_manipulator' );
var clay = new Clay( clayConfig, clayManipulator, { autoHandleEvents: false } );
var weatherID = require( './weather_id' );

// http://api.openweathermap.org/data/2.5/weather?lat=18.5204&lon=73.8567&appid=a64e1f53a22fcccc25458ea5e0b2daeb
// http://api.openweathermap.org/data/2.5/weather?lat=18.5204&lon=73.8567&appid=52c0913c975e1794a95d284f47aa4828
// {"cod":401, "message": "Invalid API key. Please see http://openweathermap.org/faq#error401 for more info."}
// {"coord":{"lon":73.86,"lat":18.52},"weather":[{"id":500,"main":"Rain","description":"light rain","icon":"10n"}],"base":"stations","main":{"temp":294.561,"pressure":946.28,"humidity":96,"temp_min":294.561,"temp_max":294.561,"sea_level":1021.07,"grnd_level":946.28},"wind":{"speed":2.82,"deg":289.001},"rain":{"3h":0.105},"clouds":{"all":76},"dt":1474821950,"sys":{"message":0.0088,"country":"IN","sunrise":1474764848,"sunset":1474808247},"id":8131502,"name":"Satara Division","cod":200}


var DEBUG = 0;

var CMD_TYPES = {
  CMD_UNDEFINED : 0,
  CMD_WEATHER : 1,
  CMD_STOCKS : 2
};
Object.freeze(CMD_TYPES);

/*
ON_DAYS[0]            : 10000
ON_DAYS[1]            : 10001
ON_DAYS[2]            : 10002
ON_DAYS[3]            : 10003
ON_DAYS[4]            : 10004
ON_DAYS[5]            : 10005
ON_DAYS[6]            : 10006
                      : 10007
                      : 10008
TEMPERATURE_UNIT      : 10009
CHIME_INTERVAL        : 10010
CHIME_START_TIME      : 10011
CHIME_END_TIME        : 10012
CLOCK_TYPE            : 10013
CHIME_OFFSET          : 10014
REQUEST               : 10015*
DISPLAY_TYPE          : 10016
STOCK_CODE            : 10017
CMP                   : 10018*
UPDATE_INTERVAL       : 10019
OWM_API_KEY:          : 10020
UPDATE_START_HOUR     : 
UPDATE_END_HOUR       :
*/

var xhrRequest = function ( url, type, callback ) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback( this.responseText );
  };
  xhr.open( type, url );
  xhr.send();
};

/////// WEATHER STUFF
function locationSuccess( pos ) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude + '&appid=' + localStorage.getItem('OWM_API_KEY'); // myAPIKey;
  
  if (DEBUG) console.log( "index.js: locationSuccess(): " + url );
  
  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
     
      var json;
      
      try {
        json = JSON.parse( responseText );
      } catch (err) {
        if (DEBUG) console.log( 'index.js: locationSuccess(): Error parsing responseText, invalid JSON data.' );
        return;
      }
             
      if (DEBUG) console.log( "index.js: locationSuccess(): " + JSON.stringify(json) );
      
      var weather = "";
      if ( json.cod == 200 ) {
        var temperature = 0;
        var conditions = "";
        temperature = Math.round( json.main.temp );
        var temperature_unit = localStorage.getItem( 'TEMPERATURE_UNIT' );
        if ( temperature_unit == 1 ) { // deg Fahrenheit
          temperature = Math.round( json.main.temp * 9/5 - 459.67 );
          temperature = temperature + "°F";
        } else if ( temperature_unit == 2 ) { // Kelvin
          temperature = Math.round( json.main.temp );
          temperature = temperature + " K";
        } else { // deg Centigrade
          temperature = Math.round( json.main.temp - 273.15 );
          temperature = temperature + "°C";  
        }
        conditions = weatherID.getWeatherGroupFromID( json.weather[0].id );
        weather = temperature + ", " + conditions;
      } else { // error
        if (DEBUG) console.log( 'index.js: locationSuccess(): XMLHttpRequest returned error: ' + json.cod + ": " + json.message );
        weather  = json.cod + ": " + json.message;
      }
      
      // Assemble dictionary using our keys
      var dictionary = {
        "TEMPERATURE": weather
      };

      // Send to Pebble
      Pebble.sendAppMessage( dictionary,
        function(e) {
          if (DEBUG) console.log( "index.js: locationSuccess(): Weather info sent to Pebble successfully." );
        },
        function(e) {
          if (DEBUG) console.log( "index.js: locationSuccess(): Error sending weather info to Pebble. " + JSON.stringify(e) );
        }
      );
    }      
  );
}

function locationError( err ) {
  if (DEBUG) console.log( "index.js: Error requesting location!" );
}

function getWeather() {
  if (DEBUG) console.log( "index.js: getWeather()." );
  if ( !localStorage.getItem( 'OWM_API_KEY' ) ) return;
  
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    { timeout: 15000, maximumAge: 60000 }
  );
}

/////// STOCK STUFF
function getCMP(){
  var stock_code = localStorage.getItem('STOCK_CODE');
  var url = "https://finance.google.com/finance/info?client=ig&q=" + stock_code;
  
  if (DEBUG) console.log( "index.js: URL: " + url );
  
  xhrRequest( url, 'GET', 
    function(responseText) {
      var json;
      
      try {
        json = JSON.parse( responseText.replace( /\//g,"" ) ); // Get rid of the initial "//"
      } catch (err) {
        if (DEBUG) console.log( 'index.js: getCMP(): Error parsing responseText, invalid JSON data.' );
        return;
      }
      
      var c_r = json['0'].c;
      var sign = "";

      if ( c_r > 0 ) {
        sign = '+';
      } else if ( c_r < 0 ) {
        sign = '-';
      } else {
        sign = '=';
      }
      
      if (DEBUG) console.log( "index.js: CMP: " + JSON.stringify( json ) );
      
      var dictionary = {   
        "CMP": stock_code.substring( stock_code.indexOf(":") + 1 ) + ":" + json['0'].l + sign
      };
        
      Pebble.sendAppMessage(dictionary,
        function(e) {
          if (DEBUG) console.log( "index.js: getCMP(): CMP sent to Pebble successfully. " + JSON.stringify( dictionary ) );
        },
        function(e) {
          if (DEBUG) console.log( "index.js: getCMP(): Error sending CMP to Pebble. " + JSON.stringify( e ) );
        s});
    });
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    if (DEBUG) console.log( "index.js: addEventListener( ready ): PebbleKit JS ready." );
    if ( localStorage.getItem( 'DISPLAY_TYPE' ) == 1 ) {
      if (DEBUG) console.log( "index.js: ready getWeather()" );
      getWeather();
    } else if ( localStorage.getItem( 'DISPLAY_TYPE' ) == 2 ) {
      if (DEBUG) console.log( "index.js: ready getCMP()" );
      getCMP();
    }
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener( 'appmessage',
  function(e) {
    if (DEBUG) console.log( "index.js: addEventListener( appmessage ): AppMessage received: " + JSON.stringify( e.payload ) );
    var dict = e.payload;
    if( dict.REQUEST ) {
      var value = dict.REQUEST;
      // if (DEBUG) console.log( "index.js: addEventListener( appmessage ): Here it is: " + value );
      if ( value == CMD_TYPES.CMD_WEATHER ) {
        if (DEBUG) console.log( "index.js: addEventListener( appmessage ): getWeather()" );
        getWeather();
      } else if ( value == CMD_TYPES.CMD_STOCKS ) {
        if (DEBUG) console.log( "index.js: addEventListener( appmessage ): getStocks()" );
        getCMP();
      }
    }
  }                     
);

/////// Pebble/clay stuff
Pebble.addEventListener( 'showConfiguration', function( e ) {
  Pebble.openURL( clay.generateUrl() );
});

Pebble.addEventListener( 'webviewclosed', function( e ) {
  if ( e && !e.response ) {
    return;
  }
  
  var dict = clay.getSettings( e.response );
  if (DEBUG) console.log( "index.js/clay: " + JSON.stringify( dict ) );
  localStorage.setItem( 'TEMPERATURE_UNIT', dict[ 10009 ] );
  localStorage.setItem( 'STOCK_CODE', dict[ 10017 ].toUpperCase() );
  localStorage.setItem( 'DISPLAY_TYPE', dict[ 10016 ] ); // nothing, weather or stock
  localStorage.setItem( 'OWM_API_KEY', dict[ 10020 ] );
  
  Pebble.sendAppMessage(dict, function( e ) {
    if (DEBUG) console.log( 'index.js/clay: Sent config data to Pebble' );
  }, function( e ) {
    if (DEBUG) console.log( 'index.js/clay: Failed to send config data!' );
    if (DEBUG) console.log( JSON.stringify( e ) );
  });
});
