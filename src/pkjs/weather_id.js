var PBL_COLOURS = {
  CLEAR: 0x000000,
  BLACK: 0x000000,
  WHITE: 0xFFFFFF,
  LIGHT_GRAY: 0xAAAAAA,
  DARK_GRAY: 0x555555,
  PASTEL_YELLOW: 0xFFFFAA,
  ICTERINE: 0xFFFF55,
  RAJAH: 0xFFAA55,
  ORANGE: 0xFF5500,
  RED: 0xFF0000,
  FOLLY: 0xFF0055,
  SUNSET_ORANGE: 0xFF5555,
  MELON: 0xFFAAAA,
  YELLOW: 0xFFFF00,
  CHROME_YELLOW: 0xFFAA00,
  WINDSOR_TAN: 0xAA5500,
  ROSE_VALE: 0xAA5555,
  DARK_CANDY_APPLE_RED: 0xAA0000,
  FASHION_MAGENTA: 0xFF00AA,
  BRILLIANT_ROSE: 0xFF55AA,
  RICH_BRILLIANT_LAVENDER: 0xFFAAFF,
  BULGARIAN_ROSE: 0x550000,
  JAZBERRY_JAM: 0xAA0055,
  MAGENTA: 0xFF00FF,
  SHOCKING_PINK: 0xFF55FF,
  IMPERIAL_PURPLE: 0x550055,
  PURPLE: 0xAA00AA,
  PURPUREUS: 0xAA55AA,
  OXFORD_BLUE: 0x000055,
  INDIGO: 0x5500AA,
  VIVID_VIOLET: 0xAA00FF,
  LAVENDAR_INDIGO: 0xAA55FF,
  DUKE_BLUE: 0x0000AA,
  ELECTRIC_ULTRAMARINE: 0x5500FF,
  LIBERTY: 0x5555AA,
  COBALT_BLUE: 0x0055AA,
  CADET_BLUE: 0x55AAAA,
  MAY_GREEN: 0x55AA55,
  ISLAMIC_GREEN: 0x00AA00,
  GREEN: 0x00FF00,
  BRIGHT_GREEN: 0x55FF00,
  INCHWORM: 0xAAFF55,
  SPRING_BUD: 0xAAFF00,
  KELLY_GREEN: 0x55AA00,
  DARK_GREEN: 0x005500,
  MIDNIGHT_GREEN: 0x005555,
  BRASS: 0xAAAA55,
  ARMY_GREEN: 0x555500,
  LIMERICK: 0xAAAA00,
  MINT_GREEN: 0xAAFFAA,
  SCREAMIN_GREEN: 0x55FF55,
  MALACHITE: 0x00FF55,
  JAEGER_GREEN: 0x00AA55,
  TIFFANY_BLUE: 0x00AAAA,
  VIVID_CERULEAN: 0x00AAFF,
  BLUE: 0x0000FF,
  VERY_LIGHT_BLUE: 0x5555FF,
  BABY_BLUE_EYES: 0xAAAAFF,
  MEDIUM_AQUAMARINE: 0x55FFAA,
  MEDIUM_SPRING_GREEN: 0x00FFAA,
  CYAN: 0x00FFFF,
  PICTON_BLUE: 0x55AAFF,
  BLUE_MOON: 0x0055FF,
  ELECTRIC_BLUE: 0x55FFFF,
  CELESTE: 0xAAFFFF
};
Object.freeze( PBL_COLOURS );

// what a piece of shit...
var temp_colour = function( temp ) {
  var hex_colour = 0;
  if ( temp < -15 ) { // unimaginably cold
    hex_colour = PBL_COLOURS.OXFORD_BLUE;
  } else if ( temp > -10 ) { // brutally cold
    hex_colour = PBL_COLOURS.DUKE_BLUE;
  } else if ( temp > -5 ) { // frigid cold
    hex_colour = PBL_COLOURS.BLUE;
  } else if ( temp > 0 ) { // bitterly cold
    hex_colour = PBL_COLOURS.VERY_LIGHT_BLUE;
  } else if ( temp > 5 ) { // extremely cold
    hex_colour = PBL_COLOURS.TIFANNY_BLUE;
  } else if ( temp > 10 ) { // very cold
    hex_colour = PBL_COLOURS.CYAN;
  } else if ( temp > 12 ) { // cold
    hex_colour = PBL_COLOURS.COBALT_BLUE;
  } else if ( temp > 14 ) { // getting cold
    hex_colour = PBL_COLOURS.VIVID_CERULEAN;
  } else if ( temp > 15 ) { // cool
    hex_colour = PBL_COLOURS.BABY_BLUE_EYES;
  } else if ( temp > 17 ) { // mild cool
    hex_colour = PBL_COLOURS.LIBERTY;
  } else if ( temp > 20 ) { // mild
    hex_colour = PBL_COLOURS.CELESTE;
  } else if ( temp > 22.5 ) { // warm mild
    hex_colour = PBL_COLOURS.MINT_GREEN;
  } else if ( temp > 25.9 ) { // warm
    hex_colour = PBL_COLOURS.CHROME_YELLOW;
  } else if ( temp > 29.9 ) { // very warm-hot
    hex_colour = PBL_COLOURS.RED;
  } else if ( temp > 35 ) { // hot
    hex_colour = PBL_COLOURS.RED;
  } else if ( temp > 39.9 ) { // very hot
    hex_colour = PBL_COLOURS.WINDSOR_TAN;
  } else if ( temp > 44.9 ) { // extreme lethal-hot
    hex_colour = PBL_COLOURS.DARK_CANDY_APPLE_RED;
  } else if ( temp > 45 ) { // lethal hot
    hex_colour = PBL_COLOURS.BULGARIAN_ROSE;
  } else { // unknown, probably junk values
    hex_colour = PBL_COLOURS.BLACK;
  }
  return parseInt( hex_colour );  
};

var weatherID = [
  { 200 : "thunderstorm with light rain" },
  { 201 : "thunderstorm with rain" },
  { 202 : "thunderstorm with heavy rain" },
  { 210 : "light thunderstorm" },
  { 211 : "thunderstorm" },
  { 212 : "heavy thunderstorm" },
  { 221 : "ragged thunderstorm" },
  { 230 : "thunderstorm with light drizzle" },
  { 231 : "thunderstorm with drizzle" },
  { 232 : "thunderstorm with heavy drizzle" },
  { 300 : "light intensity drizzle" },
  { 301 : "drizzle" },
  { 302 : "heavy intensity drizzle" },
  { 310 : "light intensity drizzle rain" },
  { 311 : "drizzle rain" },
  { 312 : "heavy intensity drizzle rain" },
  { 313 : "shower rain and drizzle" },
  { 314 : "heavy shower rain and drizzle" },
  { 321 : "shower drizzle" },
  { 500 : "light rain" },
  { 501 : "moderate rain" },
  { 502 : "heavy intensity rain" },
  { 503 : "very heavy rain" },
  { 504 : "extreme rain" },
  { 511 : "freezing rain" },
  { 520 : "light intensity shower rain" },
  { 521 : "shower rain" },
  { 522 : "heavy intensity shower rain" },
  { 531 : "ragged shower rain" },
  { 600 : "light snow" },
  { 601 : "snow" },
  { 602 : "heavy snow" },
  { 611 : "sleet" },
  { 612 : "shower sleet" },
  { 615 : "light rain and snow" },
  { 616 : "rain and snow" },
  { 620 : "light shower snow" },
  { 621 : "shower snow" },
  { 622 : "heavy shower snow" },
  { 701 : "mist" },
  { 711 : "smoke" },
  { 721 : "haze" },
  { 731 : "sand, dust whirls" },
  { 741 : "fog" },
  { 751 : "sand" },
  { 761 : "dust" },
  { 762 : "volcanic ash" },
  { 771 : "squalls" },
  { 781 : "tornado" },
  { 800 : "clear sky" },
  { 801 : "few clouds" },
  { 802 : "scattered clouds" },
  { 803 : "broken clouds" },
  { 804 : "overcast clouds" },
  { 900 : "tornado" },
  { 901 : "tropical storm" },
  { 902 : "hurricane" },
  { 903 : "cold" },
  { 904 : "hot" },
  { 905 : "windy" },
  { 906 : "hail" },
  { 951 : "calm" },
  { 952 : "light breeze" },
  { 953 : "gentle breeze" },
  { 954 : "moderate breeze" },
  { 955 : "fresh breeze" },
  { 956 : "strong breeze" },
  { 957 : "high wind, near gale" },
  { 958 : "gale" },
  { 959 : "severe gale" },
  { 960 : "storm" },
  { 961 : "violent storm" },
  { 962 : "hurricane" },
  { default : "Unknown" }
];

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
    case 800: return "Clear sky";
    case 801:
    case 802:
    case 803:
    case 804: return "Cloudy";
    case 900: return "Tornado";
    case 901:	return "Tropical storm";
    case 902: return "Hurricane";
    case 903: return "Cold";
    case 904: return "Hot";
    case 905: return "Windy";
    case 906: return "Hail";
    case 951: return "Calm";
    case 952: return "Light breeze";
    default: return "Unknown";
  }
};

module.exports.PBL_COLOURS = PBL_COLOURS;
module.exports.temp_colour = temp_colour;
module.exports.weatherID = weatherID;
module.exports.getWeatherGroupFromID = getWeatherGroupFromID;
