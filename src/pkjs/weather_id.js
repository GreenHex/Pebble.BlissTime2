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
    case 801:
    case 802:
    case 803:
    case 804: return "Cloudy";
    case 800: return "Clear sky";
    case 900: return "Tornado";
    case 901:	return "Tropical storm";
    case 902: return "Hurricane";
    case 903: return "Cold";
    case 904: return "Hot";
    case 905: return "Windy";
    case 906: return "Hail";
    case 951: return "Calm";
    case 952: return "Light breeze";
    case 900: return "Tornado";
    case 901: return "Tropical storm";
    case 902: return "Hurricane";
    case 903: return "Cold";
    case 904: return "Hot";
    case 905: return "Windy";
    case 906: return "Hail";
    default: return "Unknown";
  }
};

module.exports.weatherID = weatherID;
module.exports.getWeatherGroupFromID = getWeatherGroupFromID;