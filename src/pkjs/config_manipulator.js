
module.exports = function(minified) {
  var clayConfig = this;
//  var _ = minified._;
//  var $ = minified.$;
//  var HTML = minified.HTML;
    
  function toggle_visibility() {
    if (this.get() == '1') {
      clayConfig.getItemByMessageKey('TEMPERATURE_UNITS').hide();
      clayConfig.getItemByMessageKey('STOCK_CODE').show();
    } else {
      clayConfig.getItemByMessageKey('TEMPERATURE_UNITS').show();
      clayConfig.getItemByMessageKey('STOCK_CODE').hide();
    }
  }
  
  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
      var coolStuffToggle = clayConfig.getItemByMessageKey('DISPLAY_TYPE');
      toggle_visibility.call(coolStuffToggle);
      coolStuffToggle.on( 'change', toggle_visibility );
  });
};