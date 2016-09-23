#pragma once

enum CMD_TYPE {
  CMD_UNDEFINED = 0,
  CMD_WEATHER = 1,
  CMD_STOCKS = 2
};

void callback_init();
void callback_deinit();
void send_request( enum CMD_TYPE requestType );