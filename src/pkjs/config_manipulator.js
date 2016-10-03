module.exports = function( minified ) {
  var clayConfig = this;
//  var _ = minified._;
//  var $ = minified.$;
//  var HTML = minified.HTML;
  
  function show( showItems ) {
      showItems.map( function( item ) {
        clayConfig.getItemByMessageKey( item ).show();
      });
  }
  
  function hide( hideItems ) {
      hideItems.map( function( item ) {
        clayConfig.getItemByMessageKey( item ).hide();
      });
  }
  
  var toggle_visibility = function() {
    var showItems = [];
    var hideItems = [];
    
    if ( this.get() == '1' ) { // weather
      showItems = [ 'TEMPERATURE_UNITS', 'OWM_API_KEY', 'STATUS_UPDATE_INTERVAL', 'STATUS_UPDATE_START_TIME', 'STATUS_UPDATE_END_TIME', 'STATUS_UPDATE_ON_DAYS' ];
      hideItems = [ 'STOCK_CODE' ];
    } else if ( this.get() == '2' ) { // stocks
      showItems = [ 'STOCK_CODE', 'STATUS_UPDATE_INTERVAL', 'STATUS_UPDATE_START_TIME', 'STATUS_UPDATE_END_TIME', 'STATUS_UPDATE_ON_DAYS' ];
      hideItems = [ 'TEMPERATURE_UNITS', 'OWM_API_KEY' ];
    } else { // nothing
      showItems = [  ];
      hideItems = [ 'TEMPERATURE_UNITS', 'OWM_API_KEY', 'STOCK_CODE', 'STATUS_UPDATE_INTERVAL', 'STATUS_UPDATE_START_TIME', 'STATUS_UPDATE_END_TIME', 'STATUS_UPDATE_ON_DAYS' ]; 
    }
    show( showItems );
    hide( hideItems );
  };
  
  var toggle_visibility_clk_type = function() {
    var showItems;
    var hideItems;
    if ( this.get() == '1' ) { // analog
      showItems = [  ];
      hideItems = [ 'DIGITAL_CLOCK_TYPE_12_24' ];
    } else { // digital
      showItems = [ 'DIGITAL_CLOCK_TYPE_12_24' ];
      hideItems = [  ];
    }
    show( showItems );
    hide( hideItems );
  };
  
  clayConfig.on( clayConfig.EVENTS.AFTER_BUILD, function() {
      var displayType = clayConfig.getItemByMessageKey( 'STATUS_DISPLAY_TYPE' );
      toggle_visibility.call( displayType );
      displayType.on( 'change', toggle_visibility );
      
      var typeAnalog = clayConfig.getItemByMessageKey( 'CLOCK_TYPE_DIGITAL_ANALOG' );
      toggle_visibility_clk_type.call( typeAnalog );
      typeAnalog.on( 'change', toggle_visibility_clk_type );
    
      // not using CLOCK_TYPE_DIGITAL_ANALOG, so hide it.
      // clayConfig.getItemByMessageKey( 'CLOCK_TYPE_DIGITAL_ANALOG' ).hide();
  });
};
