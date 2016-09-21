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
          "defaultValue": "Weather settings"
        },
        {
          "type": "select",
          "messageKey": "TEMPERATURE_UNITS",
          "defaultValue": 0,
          "label": "Temperature Units",
          "options": [
            { 
              "label": "Celsius", 
              "value": 0
            },
            { 
              "label": "Fahrenheit",
              "value": 1
            },
            { 
              "label": "Kelvin",
              "value": 2
            }
          ]
        },
      ]
    },
    {
      "type": "section",
      "items": [
        {
          "type": "heading",
          "defaultValue": "Clock Settings"
        },
        {
          "type": "select",
          "messageKey": "CLOCK_TYPE",
          "defaultValue": 0,
          "label": "Clock Type",
          "options": [
            { 
              "label": "12 Hour", 
              "value": 0
            },
            { 
              "label": "24 Hour",
              "value": 1
            }
          ]
        },
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
          "defaultValue": 2,
          "label": "Chime Interval",
          "options": [
            { 
              "label": "Never", 
              "value": 0
            },
            { 
              "label": "Every half hour",
              "value": 1
            },
            { 
              "label": "Every hour",
              "value": 2
            }
          ]
        },
        {
          "type": "select",
          "messageKey": "START_TIME",
          "defaultValue": 0,
          "label": "Start Time",
          "options": [
            { "label": "Midnight", "value": 0 },
            { "label": "1 AM", "value": 1 },
            { "label": "2 AM", "value": 2 },
            { "label": "3 AM", "value": 3 },
            { "label": "4 AM", "value": 4 },
            { "label": "5 AM", "value": 5 },
            { "label": "6 AM", "value": 6 },
            { "label": "7 AM", "value": 7 },
            { "label": "8 AM", "value": 8 },
            { "label": "9 AM", "value": 9 },
            { "label": "10 AM", "value": 10 },
            { "label": "11 AM", "value": 11 },
            { "label": "Noon", "value": 12 },
            { "label": "1 PM", "value": 13 },
            { "label": "2 PM", "value": 14 },
            { "label": "3 PM", "value": 15 },
            { "label": "4 PM", "value": 16 },
            { "label": "5 PM", "value": 17 },
            { "label": "6 PM", "value": 18 },
            { "label": "7 PM", "value": 19 },
            { "label": "8 PM", "value": 20 },
            { "label": "9 PM", "value": 21 },
            { "label": "10 PM", "value": 22 },
            { "label": "11 PM", "value": 23 }
          ]
        },
        {
          "type": "select",
          "messageKey": "END_TIME",
          "defaultValue": 23,
          "label": "End Time",
          "options": [
            { "label": "Midnight", "value": 0 },
            { "label": "1 AM", "value": 1 },
            { "label": "2 AM", "value": 2 },
            { "label": "3 AM", "value": 3 },
            { "label": "4 AM", "value": 4 },
            { "label": "5 AM", "value": 5 },
            { "label": "6 AM", "value": 6 },
            { "label": "7 AM", "value": 7 },
            { "label": "8 AM", "value": 8 },
            { "label": "9 AM", "value": 9 },
            { "label": "10 AM", "value": 10 },
            { "label": "11 AM", "value": 11 },
            { "label": "Noon", "value": 12 },
            { "label": "1 PM", "value": 13 },
            { "label": "2 PM", "value": 14 },
            { "label": "3 PM", "value": 15 },
            { "label": "4 PM", "value": 16 },
            { "label": "5 PM", "value": 17 },
            { "label": "6 PM", "value": 18 },
            { "label": "7 PM", "value": 19 },
            { "label": "8 PM", "value": 20 },
            { "label": "9 PM", "value": 21 },
            { "label": "10 PM", "value": 22 },
            { "label": "11 PM", "value": 23 }
          ]
        },
        {
          "type": "checkboxgroup",
          "messageKey": "ON_DAYS",
          "label": "On Days",
          "defaultValue": [false, false, false, false, false, false, false],
          "options": ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]
        },
        {
          "type": "slider",
          "messageKey": "CHIME_OFFSET",
          "defaultValue": 0,
          "label": "Chime Offset",
          "description": "Number of minutes earlier to chime",
          "min": 0,
          "max": 5,
          "step": 1
        }
      ]
    },
    {
    "type": "submit",
    "defaultValue": "Save Settings"
    }
]