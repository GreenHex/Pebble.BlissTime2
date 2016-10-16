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

  var toggle_status_type_visibility = function() {
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

  var toggle_clock_type_visibility = function() {
    var showItems;
    var hideItems;
    if ( this.get() == '1' ) { // analog
      showItems = [ 'ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS' ];
      hideItems = [ 'DIGITAL_CLOCK_TYPE_12_OR_24' ];
    } else { // digital
      showItems = [ 'DIGITAL_CLOCK_TYPE_12_OR_24' ];
      hideItems = [ 'ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS' ];
    }
    show( showItems );
    hide( hideItems );
  };

  var toggle_chime_settings_visibility = function() {
    var showItems;
    var hideItems;
    if ( this.get() == '0' ) { // no chime
      showItems = [ ];
      hideItems = [ 'CHIME_START_TIME', 'CHIME_END_TIME', 'CHIME_ON_DAYS', 'CHIME_OFFSET' ];
    } else { 
      showItems = [ 'CHIME_START_TIME', 'CHIME_END_TIME', 'CHIME_ON_DAYS', 'CHIME_OFFSET' ];
      hideItems = [ ];
    }
    show( showItems );
    hide( hideItems );
  };

  clayConfig.on( clayConfig.EVENTS.AFTER_BUILD, function() {
    var statusType = clayConfig.getItemByMessageKey( 'STATUS_DISPLAY_TYPE' );
    toggle_status_type_visibility.call( statusType );
    statusType.on( 'change', toggle_status_type_visibility );

    var clockType = clayConfig.getItemByMessageKey( 'CLOCK_TYPE_DIGITAL_OR_ANALOG' );
    toggle_clock_type_visibility.call( clockType );
    clockType.on( 'change', toggle_clock_type_visibility );

    var chimeSettings = clayConfig.getItemByMessageKey( 'CHIME_INTERVAL' );
    toggle_chime_settings_visibility.call( chimeSettings );
    chimeSettings.on( 'change', toggle_chime_settings_visibility );
  });
}
