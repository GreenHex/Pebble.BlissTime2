var Clay = require( 'pebble-clay' );
var clayConfig = require( './config' );
var clayManipulator = require( './config_manipulator' );
var clay = new Clay( clayConfig, clayManipulator, { autoHandleEvents: false } );
// var messageKeys = require( 'message_keys' );
var weatherID = require( './weather_id' );
// var messageKeys = require('message_keys'); // don't know why this doesn't work

// http://api.openweathermap.org/data/2.5/weather?lat=18.5204&lon=73.8567&appid=a64e1f53a22fcccc25458ea5e0b2daeb
// http://api.openweathermap.org/data/2.5/weather?lat=18.5204&lon=73.8567&appid=52c0913c975e1794a95d284f47aa4828
// {"cod":401, "message": "Invalid API key. Please see http://openweathermap.org/faq#error401 for more info."}
// {"coord":{"lon":73.86,"lat":18.52},"weather":[{"id":500,"main":"Rain","description":"light rain","icon":"10n"}],"base":"stations","main":{"temp":294.561,"pressure":946.28,"humidity":96,"temp_min":294.561,"temp_max":294.561,"sea_level":1021.07,"grnd_level":946.28},"wind":{"speed":2.82,"deg":289.001},"rain":{"3h":0.105},"clouds":{"all":76},"dt":1474821950,"sys":{"message":0.0088,"country":"IN","sunrise":1474764848,"sunset":1474808247},"id":8131502,"name":"Satara Division","cod":200}
// {"10000":true,"10001":true,"10002":true,"10003":true,"10004":true,"10005":true,"10006":true,"10009":"0","10010":"2","10011":"9","10012":"22","10013":"0","10014":0,"10016":"0","10017":"","10019":30,"10020":"","10021":"9","10022":"17"}

var DEBUG = 0;

var CMD_TYPES = {
  CMD_UNDEFINED : 0,
  CMD_WEATHER : 1,
  CMD_STOCKS : 2
};
Object.freeze(CMD_TYPES);

// this is stupid, but can't seem to make "message_keys" work...
var MSG_TYPES = {
  ON_DAYS               : 10000, // ON_DAYS[0..7]: 10000 to 10006
  //                      : 10007, // used on pebble
  //                      : 10008, // used on pebble
  TEMPERATURE_UNIT      : 10009,
  CHIME_INTERVAL        : 10010,
  CHIME_START_TIME      : 10011,
  CHIME_END_TIME        : 10012,
  CLOCK_TYPE            : 10013,
  CHIME_OFFSET          : 10014,
  REQUEST               : 10015, // used on pebble
  DISPLAY_TYPE          : 10016,
  STOCK_CODE            : 10017,
  CMP                   : 10018, // used on pebble
  UPDATE_INTERVAL       : 10019,
  OWM_API_KEY           : 10020,
  UPDATE_START_HOUR     : 10021,
  UPDATE_END_HOUR       : 10022
};
Object.freeze(CMD_TYPES);

var local_stored_settings = [ 
                          MSG_TYPES.ON_DAYS, MSG_TYPES.ON_DAYS + 1, MSG_TYPES.ON_DAYS + 2, MSG_TYPES.ON_DAYS + 3,
                          MSG_TYPES.ON_DAYS + 4, MSG_TYPES.ON_DAYS + 5, MSG_TYPES.ON_DAYS + 6, MSG_TYPES.TEMPERATURE_UNIT,
                          MSG_TYPES.CHIME_INTERVAL, MSG_TYPES.CHIME_INTERVAL, MSG_TYPES.CHIME_END_TIME, MSG_TYPES.CLOCK_TYPE,
                          MSG_TYPES.CHIME_OFFSET, MSG_TYPES.DISPLAY_TYPE, MSG_TYPES.STOCK_CODE, MSG_TYPES.UPDATE_INTERVAL,
                          MSG_TYPES.OWM_API_KEY, MSG_TYPES.UPDATE_START_HOUR, MSG_TYPES.UPDATE_END_HOUR
                        ];

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
      pos.coords.latitude + "&lon=" + pos.coords.longitude + '&appid=' + localStorage.getItem( MSG_TYPES.OWM_API_KEY );
  
  if (DEBUG) console.log( "index.js: locationSuccess(): " + url );
  
  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
     
      var json;
      
      try {
        json = JSON.parse( responseText );
      } catch ( err ) {
        if (DEBUG) console.log( 'index.js: locationSuccess(): Error parsing responseText, invalid JSON data.' );
        return;
      }
             
      if (DEBUG) console.log( "index.js: locationSuccess(): " + JSON.stringify(json) );
      
      var weather = "Loading...";
      if ( json.cod == 200 ) { // success
        var temperature = Math.round( json.main.temp );
        var conditions = weatherID.getWeatherGroupFromID( json.weather[0].id );
        var temperature_unit = localStorage.getItem( MSG_TYPES.TEMPERATURE_UNIT );
        
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
  if ( !localStorage.getItem( MSG_TYPES.OWM_API_KEY ) ) return;
  
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    { timeout: 15000, maximumAge: 60000 }
  );
}

/////// STOCK STUFF
function getCMP(){
  var stock_code = localStorage.getItem( MSG_TYPES.STOCK_CODE );
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
      
      var sign = "=";
      
      if ( json['0'].c > 0 ) {
        sign = '+';
      } else if ( json['0'].c < 0 ) {
        sign = '-';
      }
      
      if (DEBUG) console.log( "index.js: CMP: " + JSON.stringify( json ) );
      
      var dictionary = {   
        "CMP": stock_code.substring( stock_code.indexOf(":") + 1 ) + ":" + json['0'].l + sign
      };
        
      Pebble.sendAppMessage( dictionary,
        function(e) {
          if (DEBUG) console.log( "index.js: getCMP(): CMP sent to Pebble successfully. " + JSON.stringify( dictionary ) );
        },
        function(e) {
          if (DEBUG) console.log( "index.js: getCMP(): Error sending CMP to Pebble. " + JSON.stringify( e ) );
        });
    });
}

/// Send configuration data
function sendConfig() {
  var dictionary = {};
  
  local_stored_settings.map( function ( item ) {
    dictionary[item] = localStorage.getItem( item );
    // if (DEBUG) console.log( "index.js: sendConfig(): " + item + ": " + dictionary[item] );
  });
  
  Pebble.sendAppMessage( dictionary,
        function(e) {
          if (DEBUG) console.log( "index.js: sendConfig(): Configuration settings sent to Pebble successfully. " + JSON.stringify( dictionary ) );
        },
        function(e) {
          if (DEBUG) console.log( "index.js: sendConfig(): Error sending configuration settings to Pebble. " + JSON.stringify( e ) );
        });
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    if (DEBUG) console.log( "index.js: addEventListener( ready ): PebbleKit JS ready." );
    sendConfig();
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
  
  var dictionary = clay.getSettings( e.response );
  if (DEBUG) console.log( "index.js/clay: " + JSON.stringify( dictionary ) );
  
  // save for later...
  local_stored_settings.map( function( item ) {
    localStorage.setItem( item, dictionary[ item ] );
    // if (DEBUG) console.log( "index.js/clay: " + item + " " + localStorage.getItem( item ) );
  });
  
  sendConfig();
  
});
