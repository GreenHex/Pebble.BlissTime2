var Clay = require( 'pebble-clay' );
var clayConfig = require( './config' );
var clayManipulator = require( './config_manipulator' );
var clay = new Clay( clayConfig, clayManipulator, { autoHandleEvents: false } );
var MESSAGE_KEYS = require( 'message_keys' );
var weatherID = require( './weather_id' );


var DEBUG = 0;

var CMD_TYPES = {
  CMD_UNDEFINED : 0,
  CMD_WEATHER : 1,
  CMD_STOCKS : 2,
  CMD_CONFIG : 3
};
Object.freeze( CMD_TYPES );

// clay should be able to give these, but settings are stored locally to enable sending configuration settings on call.
// But need to hook into clay, to get rid of all this.
var local_config_settings = [ // status
  MESSAGE_KEYS.STATUS_DISPLAY_TYPE,
  MESSAGE_KEYS.STATUS_UPDATE_INTERVAL,
  MESSAGE_KEYS.STATUS_UPDATE_START_TIME, 
  MESSAGE_KEYS.STATUS_UPDATE_END_TIME,
  MESSAGE_KEYS.STATUS_UPDATE_ON_DAYS, MESSAGE_KEYS.STATUS_UPDATE_ON_DAYS + 1, MESSAGE_KEYS.STATUS_UPDATE_ON_DAYS + 2,
  MESSAGE_KEYS.STATUS_UPDATE_ON_DAYS + 3, MESSAGE_KEYS.STATUS_UPDATE_ON_DAYS + 4, MESSAGE_KEYS.STATUS_UPDATE_ON_DAYS + 5,
  MESSAGE_KEYS.STATUS_UPDATE_ON_DAYS + 6,
  // clock
  MESSAGE_KEYS.CLOCK_TYPE_DIGITAL_OR_ANALOG,
  MESSAGE_KEYS.DIGITAL_CLOCK_TYPE_12_OR_24,
  MESSAGE_KEYS.ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS,
  // chime
  MESSAGE_KEYS.CHIME_INTERVAL,
  MESSAGE_KEYS.CHIME_START_TIME,
  MESSAGE_KEYS.CHIME_END_TIME,
  MESSAGE_KEYS.CHIME_ON_DAYS, MESSAGE_KEYS.CHIME_ON_DAYS + 1, MESSAGE_KEYS.CHIME_ON_DAYS + 2,
  MESSAGE_KEYS.CHIME_ON_DAYS + 3, MESSAGE_KEYS.CHIME_ON_DAYS + 4, MESSAGE_KEYS.CHIME_ON_DAYS + 5,
  MESSAGE_KEYS.CHIME_ON_DAYS + 6,
  MESSAGE_KEYS.CHIME_OFFSET,
  // others
  MESSAGE_KEYS.TEMPERATURE_UNITS,
  MESSAGE_KEYS.STOCK_CODE, 
  MESSAGE_KEYS.OWM_API_KEY
];

var xhrRequest = function ( url, type, callback ) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback( this.responseText );
  };
  xhr.open( type, url );
  xhr.send();
};

function sendDictionaryToPebble( dictionary ) {
  Pebble.sendAppMessage( dictionary,
                        function(e) {
                          if (DEBUG) console.log( "index.js: sendDictionaryToPebble(): Message sent to Pebble successfully. " + JSON.stringify( dictionary ) );
                        },
                        function(e) {
                          if (DEBUG) console.log( "index.js: sendDictionaryToPebble(): Error sending message to Pebble. " + JSON.stringify( e ) );
                        });
}

/////// WEATHER STUFF
function locationSuccess( pos ) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude + '&appid=' + localStorage.getItem( MESSAGE_KEYS.OWM_API_KEY );

  if (DEBUG) console.log( "index.js: locationSuccess(): " + url );

  // Send request to OpenWeatherMap
  xhrRequest( url, 'GET', 
             function( responseText ) {

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

                 weather = [  Math.round( json.main.temp - 273.15 ) + "°C",
                            Math.round( json.main.temp * 9/5 - 459.67 ) + "°F",
                            Math.round( json.main.temp ) + " K"
                           ][ localStorage.getItem( MESSAGE_KEYS.TEMPERATURE_UNITS ) ] +
                   ", " + weatherID.getWeatherGroupFromID( json.weather[0].id );

               } else { // error
                 if (DEBUG) console.log( 'index.js: locationSuccess(): XMLHttpRequest returned error: ' + json.cod + ": " + json.message );
                 weather  = json.cod + ": " + json.message;
               }

               // Assemble dictionary using our keys
               var dictionary = {
                 "STATUS_TEXT": weather,
                 "STATUS_COLOUR": weatherID.temp_colour( json.main.temp - 273.15 )
               };

               sendDictionaryToPebble( dictionary );
             }      
            );
}

function locationError( err ) {
  if (DEBUG) console.log( "index.js: Error requesting location!" );
}

function getWeather() {
  if (DEBUG) console.log( "index.js: getWeather()." );
  if ( !localStorage.getItem( MESSAGE_KEYS.OWM_API_KEY ) ) return;

  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    { timeout: 15000, maximumAge: 60000 }
  );
}

/////// STOCK STUFF
function getCMP() {
  var stock_code = localStorage.getItem( MESSAGE_KEYS.STOCK_CODE );
  if ( !stock_code ) return;

  var url = "https://finance.google.com/finance/info?client=ig&q=" + stock_code;

  if (DEBUG) console.log( "index.js: URL: " + url );

  xhrRequest( url, 'GET', 
             function( responseText ) {
               var json;

               try {
                 json = JSON.parse( responseText.replace( /\//g,"" ) ); // Get rid of the initial "//"
               } catch (err) {
                 if (DEBUG) console.log( 'index.js: getCMP(): Error parsing responseText, invalid JSON data.' );
                 return;
               }

               if (DEBUG) console.log( "index.js: CMP: " + JSON.stringify( json ) );

               var sign_idx = ( json['0'].c > 0 ) ? 1 : ( json['0'].c < 0 ) ? 2 : 0;
               var sign = [ "=", "+", "-" ][ sign_idx ];
               var colour = parseInt( [ weatherID.PBL_64_COLOURS.GColorDukeBlue, weatherID.PBL_64_COLOURS.GColorDarkGreen, 
                                       weatherID.PBL_64_COLOURS.GColorRed ][ sign_idx ] );

               var dictionary = {   
                 "STATUS_TEXT": stock_code.substring( stock_code.indexOf(":") + 1 ) + ":" + json['0'].l + sign,
                 "STATUS_COLOUR": colour
               };

               sendDictionaryToPebble( dictionary );
             });
}

/// Send configuration data
function sendConfig() {
  var dictionary = {};

  local_config_settings.map( function ( item ) {
    dictionary[item] = localStorage.getItem( item );
    if (DEBUG) console.log( "index.js: sendConfig(): " + item + ": " + dictionary[item] );
  });

  sendDictionaryToPebble( dictionary );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
                        function(e) {
                          if (DEBUG) console.log( "index.js: addEventListener( ready ): PebbleKit JS ready." );
                          sendConfig();
                        });

// Listen for when an AppMessage is received
Pebble.addEventListener( 'appmessage',
                        function(e) {
                          if (DEBUG) console.log( "index.js: addEventListener( appmessage ): AppMessage received: " + JSON.stringify( e.payload ) );
                          var dict = e.payload;
                          if( dict.REQUEST ) {
                            [ getWeather, getCMP, sendConfig ][ dict.REQUEST - 1 ]();
                          }
                        });

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
  local_config_settings.map( function( item ) {
    localStorage.setItem( item, dictionary[ item ] );
    // if (DEBUG) console.log( "index.js/clay: " + item + " " + localStorage.getItem( item ) );
  });
  sendConfig();
});
