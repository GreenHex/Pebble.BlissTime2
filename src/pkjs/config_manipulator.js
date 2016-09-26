module.exports = function( minified ) {
  var clayConfig = this;
//  var _ = minified._;
//  var $ = minified.$;
//  var HTML = minified.HTML;
    
  function toggle_visibility() {
    if ( this.get() == '1' ) { // weather
      clayConfig.getItemByMessageKey( 'TEMPERATURE_UNITS' ).show();
      clayConfig.getItemByMessageKey( 'OWM_API_KEY' ).show();
      clayConfig.getItemByMessageKey( 'STOCK_CODE' ).hide();
      clayConfig.getItemByMessageKey( 'UPDATE_INTERVAL' ).show();
      clayConfig.getItemByMessageKey( 'UPDATE_START_TIME' ).show();
      clayConfig.getItemByMessageKey( 'UPDATE_END_TIME' ).show();
    } else if ( this.get() == '2' ) { // stocks
      clayConfig.getItemByMessageKey( 'TEMPERATURE_UNITS').hide();
      clayConfig.getItemByMessageKey( 'OWM_API_KEY' ).hide();
      clayConfig.getItemByMessageKey( 'STOCK_CODE' ).show();
      clayConfig.getItemByMessageKey( 'UPDATE_INTERVAL' ).show();
      clayConfig.getItemByMessageKey( 'UPDATE_START_TIME' ).show();
      clayConfig.getItemByMessageKey( 'UPDATE_END_TIME' ).show();
    } else { // nothing
      clayConfig.getItemByMessageKey( 'TEMPERATURE_UNITS' ).hide();
      clayConfig.getItemByMessageKey( 'OWM_API_KEY' ).hide();
      clayConfig.getItemByMessageKey( 'STOCK_CODE' ).hide();
      clayConfig.getItemByMessageKey( 'UPDATE_INTERVAL' ).hide();
      clayConfig.getItemByMessageKey( 'UPDATE_START_TIME' ).hide();
      clayConfig.getItemByMessageKey( 'UPDATE_END_TIME' ).hide();
    }
  }
  
  clayConfig.on( clayConfig.EVENTS.AFTER_BUILD, function() {
      var displayType = clayConfig.getItemByMessageKey( 'DISPLAY_TYPE' );
      toggle_visibility.call( displayType );
      displayType.on( 'change', toggle_visibility );
  });
};