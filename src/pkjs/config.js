module.exports = [
  {
    "type": "heading",
    "defaultValue": "Bliss Time 2"
  },
  {
    "type": "text",
    "defaultValue": "Configure your watch app settings."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Watch Settings"
      },
      {
        "type": "select",
        "messageKey": "CLOCK_TYPE_DIGITAL_OR_ANALOG",
        "defaultValue": '0',
        "label": "Display Type",
        "options": [
          { 
            "label": "Digital", 
            "value": '0'
          },
          { 
            "label": "Analog",
            "value": '1'
          }
        ]
      },
      {
        "type": "select",
        "messageKey": "DIGITAL_CLOCK_TYPE_12_OR_24",
        "defaultValue": '0',
        "label": "Watch Type",
        "options": [
          { 
            "label": "Use Watch Preferences", 
            "value": '0'
          },
          { 
            "label": "12 Hour", 
            "value": '1'
          },
          { 
            "label": "24 Hour",
            "value": '2'
          }
        ]
      },
      {
        "type": "slider",
        "messageKey": "ANALOG_SECONDS_DISPLAY_TIMEOUT_SECS",
        "defaultValue": '20',
        "label": "Seconds Display (seconds)",
        "description": "Number of seconds to display seconds hand after activation by tapping or shaking watch. Set value to '0' to disable.",
        "min": '0',
        "max": '60',
        "step": '5'
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "STATUS DISPLAY"
      },
      {
        "type": "select",
        "messageKey": "STATUS_DISPLAY_TYPE",
        "defaultValue": '0',
        "label": "Display",
        "options": [
          { 
            "label": "Nothing", 
            "value": '0'
          },
          { 
            "label": "Weather", 
            "value": '1'
          },
          { 
            "label": "Stock",
            "value": '2'
          },
        ]
          },
          {
          "type": "input",
          "messageKey": "OWM_API_KEY",
          "defaultValue": "",
          "label": "OpenWeatherMap API Key",
          "description": 'Weather forecast on the Pebble will not work without a valid API key. You can get your free API key from <a href="http://openweathermap.org">openweathermap.org.</a>',  
          "attributes": {
          "placeholder": "eg: a64e1f53a22fcccc25458ea5e0b2daeb",
          "limit": '32',
          "type": "text"
          }
          },
          {
          "type": "select",
          "messageKey": "TEMPERATURE_UNITS",
          "defaultValue": '0',
          "label": "Temperature Units",
          "options": [
          { 
          "label": "Celsius", 
          "value": '0'
          },
          { 
          "label": "Fahrenheit",
          "value": '1'
          },
          { 
          "label": "Kelvin",
          "value": '2'
          }
        ]
      },
      {
        "type": "input",
        "messageKey": "STOCK_CODE",
        "defaultValue": "",
        "label": "Google Stock Code",
        "attributes": {
          "placeholder": "eg: INDEXBOM:SENSEX",
          "limit": '25',
          "type": "text"
        }
      },
      {
        "type": "slider",
        "messageKey": "STATUS_UPDATE_INTERVAL",
        "defaultValue": '30',
        "label": "Update Interval (minutes)",
        "description": "Time interval between weather or stock updates.",
        "min": '10',
        "max": '60',
        "step": '5'
      },
      {
        "type": "select",
        "messageKey": "STATUS_UPDATE_START_TIME",
        "defaultValue": '0',
        "label": "Start Updates At",
        "options": [
          { "label": "Midnight", "value": '0' },
          { "label": "1 AM", "value": '1' },
          { "label": "2 AM", "value": '2' },
          { "label": "3 AM", "value": '3' },
          { "label": "4 AM", "value": '4' },
          { "label": "5 AM", "value": '5' },
          { "label": "6 AM", "value": '6' },
          { "label": "7 AM", "value": '7' },
          { "label": "8 AM", "value": '8' },
          { "label": "9 AM", "value": '9' },
          { "label": "10 AM", "value": '10' },
          { "label": "11 AM", "value": '11' },
          { "label": "Noon", "value": '12' },
          { "label": "1 PM", "value": '13' },
          { "label": "2 PM", "value": '14' },
          { "label": "3 PM", "value": '15' },
          { "label": "4 PM", "value": '16' },
          { "label": "5 PM", "value": '17' },
          { "label": "6 PM", "value": '18' },
          { "label": "7 PM", "value": '19' },
          { "label": "8 PM", "value": '20' },
          { "label": "9 PM", "value": '21' },
          { "label": "10 PM", "value": '22' },
          { "label": "11 PM", "value": '23' }
        ]
      },
      {
        "type": "select",
        "messageKey": "STATUS_UPDATE_END_TIME",
        "defaultValue": '23',
        "label": "Stop Updates At",
        "options": [
          { "label": "Midnight", "value": '0' },
          { "label": "1 AM", "value": '1' },
          { "label": "2 AM", "value": '2' },
          { "label": "3 AM", "value": '3' },
          { "label": "4 AM", "value": '4' },
          { "label": "5 AM", "value": '5' },
          { "label": "6 AM", "value": '6' },
          { "label": "7 AM", "value": '7' },
          { "label": "8 AM", "value": '8' },
          { "label": "9 AM", "value": '9' },
          { "label": "10 AM", "value": '10' },
          { "label": "11 AM", "value": '11' },
          { "label": "Noon", "value": '12' },
          { "label": "1 PM", "value": '13' },
          { "label": "2 PM", "value": '14' },
          { "label": "3 PM", "value": '15' },
          { "label": "4 PM", "value": '16' },
          { "label": "5 PM", "value": '17' },
          { "label": "6 PM", "value": '18' },
          { "label": "7 PM", "value": '19' },
          { "label": "8 PM", "value": '20' },
          { "label": "9 PM", "value": '21' },
          { "label": "10 PM", "value": '22' },
          { "label": "11 PM", "value": '23' }
        ]
      },
      {
        "type": "checkboxgroup",
        "messageKey": "STATUS_UPDATE_ON_DAYS",
        "label": "Update On Days",
        "defaultValue": [ true, true, true, true, true, true, true ],
        "options": [ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" ]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Chime settings"
      },
      {
        "type": "select",
        "messageKey": "CHIME_INTERVAL",
        "defaultValue": '0',
        "label": "Chime Interval",
        "options": [
          { 
            "label": "Off", 
            "value": '0'
          },
          { 
            "label": "Every half hour",
            "value": '1'
          },
          { 
            "label": "Every hour",
            "value": '2'
          }
        ]
      },
      {
        "type": "select",
        "messageKey": "CHIME_START_TIME",
        "defaultValue": '0',
        "label": "Chime Start Time",
        "options": [
          { "label": "Midnight", "value": '0' },
          { "label": "1 AM", "value": '1' },
          { "label": "2 AM", "value": '2' },
          { "label": "3 AM", "value": '3' },
          { "label": "4 AM", "value": '4' },
          { "label": "5 AM", "value": '5' },
          { "label": "6 AM", "value": '6' },
          { "label": "7 AM", "value": '7' },
          { "label": "8 AM", "value": '8' },
          { "label": "9 AM", "value": '9' },
          { "label": "10 AM", "value": '10' },
          { "label": "11 AM", "value": '11' },
          { "label": "Noon", "value": '12' },
          { "label": "1 PM", "value": '13' },
          { "label": "2 PM", "value": '14' },
          { "label": "3 PM", "value": '15' },
          { "label": "4 PM", "value": '16' },
          { "label": "5 PM", "value": '17' },
          { "label": "6 PM", "value": '18' },
          { "label": "7 PM", "value": '19' },
          { "label": "8 PM", "value": '20' },
          { "label": "9 PM", "value": '21' },
          { "label": "10 PM", "value": '22' },
          { "label": "11 PM", "value": '23' }
        ]
      },
      {
        "type": "select",
        "messageKey": "CHIME_END_TIME",
        "defaultValue": '23',
        "label": "Chime End Time",
        "options": [
          { "label": "Midnight", "value": '0' },
          { "label": "1 AM", "value": '1' },
          { "label": "2 AM", "value": '2' },
          { "label": "3 AM", "value": '3' },
          { "label": "4 AM", "value": '4' },
          { "label": "5 AM", "value": '5' },
          { "label": "6 AM", "value": '6' },
          { "label": "7 AM", "value": '7' },
          { "label": "8 AM", "value": '8' },
          { "label": "9 AM", "value": '9' },
          { "label": "10 AM", "value": '10' },
          { "label": "11 AM", "value": '11' },
          { "label": "Noon", "value": '12' },
          { "label": "1 PM", "value": '13' },
          { "label": "2 PM", "value": '14' },
          { "label": "3 PM", "value": '15' },
          { "label": "4 PM", "value": '16' },
          { "label": "5 PM", "value": '17' },
          { "label": "6 PM", "value": '18' },
          { "label": "7 PM", "value": '19' },
          { "label": "8 PM", "value": '20' },
          { "label": "9 PM", "value": '21' },
          { "label": "10 PM", "value": '22' },
          { "label": "11 PM", "value": '23' }
        ]
      },
      {
        "type": "checkboxgroup",
        "messageKey": "CHIME_ON_DAYS",
        "label": "Chime On Days",
        "defaultValue": [ true, true, true, true, true, true, true ],
        "options": [ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" ]
      },
      {
        "type": "slider",
        "messageKey": "CHIME_OFFSET",
        "defaultValue": '0',
        "label": "Chime Offset (minutes)",
        "description": "Number of minutes earlier to chime.",
        "min": '0',
        "max": '5',
        "step": '1'
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings and Exit"
  }
];