#pragma once

enum CMD_TYPE {
  CMD_UNDEFINED = 0,
  CMD_WEATHER = 1,
  CMD_STOCKS = 2,
  CMD_CONFIG = 3
};

void messaging_init( void );
void messaging_deinit( void );
void send_request( enum CMD_TYPE requestType );