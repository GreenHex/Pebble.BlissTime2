var PBL_64_COLOURS = {
  GColorClear: 0x000000,
  GColorBlack: 0x000000,
  GColorWhite: 0xFFFFFF,
  GColorLightGray: 0xAAAAAA,
  GColorDarkGray: 0x555555,
  GColorPastelYellow: 0xFFFFAA,
  GColorIcterine: 0xFFFF55,
  GColorRajah: 0xFFAA55,
  GColorOrange: 0xFF5500,
  GColorRed: 0xFF0000,
  GColorFolly: 0xFF0055,
  GColorSunsetOrange: 0xFF5555,
  GColorMelon: 0xFFAAAA,
  GColorYellow: 0xFFFF00,
  GColorChromeYellow: 0xFFAA00,
  GColorWindsorTan: 0xAA5500,
  GColorRoseVale: 0xAA5555,
  GColorDarkCandyAppleRed: 0xAA0000,
  GColorFashionMagenta: 0xFF00AA,
  GColorBrilliantRose: 0xFF55AA,
  GColorRichBrilliantLavender: 0xFFAAFF,
  GColorBulgarianRose: 0x550000,
  GColorJazzberryJam: 0xAA0055,
  GColorMagenta: 0xFF00FF,
  GColorShockingPink: 0xFF55FF,
  GColorImperialPurple: 0x550055,
  GColorPurple: 0xAA00AA,
  GColorPurpureus: 0xAA55AA,
  GColorOxfordBlue: 0x000055,
  GColorIndigo: 0x5500AA,
  GColorVividViolet: 0xAA00FF,
  GColorLavenderIndigo: 0xAA55FF,
  GColorDukeBlue: 0x0000AA,
  GColorElectricUltramarine: 0x5500FF,
  GColorLiberty: 0x5555AA,
  GColorCobaltBlue: 0x0055AA,
  GColorCadetBlue: 0x55AAAA,
  GColorMayGreen: 0x55AA55,
  GColorIslamicGreen: 0x00AA00,
  GColorGreen: 0x00FF00,
  GColorBrightGreen: 0x55FF00,
  GColorInchworm: 0xAAFF55,
  GColorSpringBud: 0xAAFF00,
  GColorKellyGreen: 0x55AA00,
  GColorDarkGreen: 0x005500,
  GColorMidnightGreen: 0x005555,
  GColorBrass: 0xAAAA55,
  GColorArmyGreen: 0x555500,
  GColorLimerick: 0xAAAA00,
  GColorMintGreen: 0xAAFFAA,
  GColorScreaminGreen: 0x55FF55,
  GColorMalachite: 0x00FF55,
  GColorJaegerGreen: 0x00AA55,
  GColorTiffanyBlue: 0x00AAAA,
  GColorVividCerulean: 0x00AAFF,
  GColorBlue: 0x0000FF,
  GColorVeryLightBlue: 0x5555FF,
  GColorBabyBlueEyes: 0xAAAAFF,
  GColorMediumAquamarine: 0x55FFAA,
  GColorMediumSpringGreen: 0x00FFAA,
  GColorCyan: 0x00FFFF,
  GColorPictonBlue: 0x55AAFF,
  GColorBlueMoon: 0x0055FF,
  GColorElectricBlue: 0x55FFFF,
  GColorCeleste: 0xAAFFFF
};
Object.freeze( PBL_64_COLOURS );

// what a piece of shit...
var temp_colour = function( temp ) {
  var hex_colour = 0;
  if ( temp < -15 ) { // unimaginably cold
    hex_colour = PBL_64_COLOURS.GColorOxfordBlue;
  } else if ( temp > -10 ) { // brutally cold
    hex_colour = PBL_64_COLOURS.GColorDukeBlue;
  } else if ( temp > -5 ) { // frigid cold
    hex_colour = PBL_64_COLOURS.GColorBlue;
  } else if ( temp > 0 ) { // bitterly cold
    hex_colour = PBL_64_COLOURS.GColorVeryLightBlue;
  } else if ( temp > 5 ) { // extremely cold
    hex_colour = PBL_64_COLOURS.GColorTifannyBlue;
  } else if ( temp > 10 ) { // very cold
    hex_colour = PBL_64_COLOURS.GColorCyan;
  } else if ( temp > 12 ) { // cold
    hex_colour = PBL_64_COLOURS.GColorCobaltBlue;
  } else if ( temp > 14 ) { // getting cold
    hex_colour = PBL_64_COLOURS.GColorVividCerulean;
  } else if ( temp > 15 ) { // cool
    hex_colour = PBL_64_COLOURS.GColorBabyBlueEyes;
  } else if ( temp > 17 ) { // mild cool
    hex_colour = PBL_64_COLOURS.GColorLiberty;
  } else if ( temp > 20 ) { // mild
    hex_colour = PBL_64_COLOURS.GColorCeleste;
  } else if ( temp > 22.5 ) { // warm mild
    hex_colour = PBL_64_COLOURS.GColorMintGreen;
  } else if ( temp > 25.9 ) { // warm
    hex_colour = PBL_64_COLOURS.GColorChromeYellow;
  } else if ( temp > 29.9 ) { // very warm-hot
    hex_colour = PBL_64_COLOURS.GColorRed;
  } else if ( temp > 35 ) { // hot
    hex_colour = PBL_64_COLOURS.GColorRed;
  } else if ( temp > 39.9 ) { // very hot
    hex_colour = PBL_64_COLOURS.GColorWindsorTan;
  } else if ( temp > 44.9 ) { // extreme lethal-hot
    hex_colour = PBL_64_COLOURS.GColorDarkCandyAppleRed;
  } else if ( temp > 45 ) { // lethal hot
    hex_colour = PBL_64_COLOURS.GColorBulgarianRose;
  } else { // unknown, probably junk values
    hex_colour = PBL_64_COLOURS.GColorBlack;
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

module.exports.PBL_64_COLOURS = PBL_64_COLOURS;
module.exports.temp_colour = temp_colour;
module.exports.weatherID = weatherID;
module.exports.getWeatherGroupFromID = getWeatherGroupFromID;
