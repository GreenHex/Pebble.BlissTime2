module.exports = function( minified ) {
  var clayConfig = this;
//  var _ = minified._;
//  var $ = minified.$;
//  var HTML = minified.HTML;
  
  function toggle_visibility() {
    var showItems;
    var hideItems;
    
    if ( this.get() == '1' ) { // weather
      showItems = [ 'TEMPERATURE_UNITS', 'OWM_API_KEY', 'UPDATE_INTERVAL', 'UPDATE_START_TIME', 'UPDATE_END_TIME' ];
      hideItems = [ 'STOCK_CODE' ];
    } else if ( this.get() == '2' ) { // stocks
      showItems = [ 'STOCK_CODE', 'UPDATE_INTERVAL', 'UPDATE_START_TIME', 'UPDATE_END_TIME' ];
      hideItems = [ 'TEMPERATURE_UNITS', 'OWM_API_KEY' ];
    } else { // nothing
      showItems = [  ];
      hideItems = [ 'TEMPERATURE_UNITS', 'OWM_API_KEY', 'STOCK_CODE', 'UPDATE_INTERVAL', 'UPDATE_START_TIME', 'UPDATE_END_TIME' ]; 
    }
    showItems.map( function( item ) {
      clayConfig.getItemByMessageKey( item ).show();
    });
    hideItems.map( function( item ) {
      clayConfig.getItemByMessageKey( item ).hide();
    });
  }
  
  clayConfig.on( clayConfig.EVENTS.AFTER_BUILD, function() {
      var displayType = clayConfig.getItemByMessageKey( 'DISPLAY_TYPE' );
      toggle_visibility.call( displayType );
      displayType.on( 'change', toggle_visibility );
  });
};