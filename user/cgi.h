#ifndef CGI_H
#define CGI_H



#define READFLASH   1
#define WRITEFLASH  2
#define WRITESPI    3
#define SETDAC      4
#define CAPTURE_ONE_TRACE    5
#define CAPTURE_TWO_TRACES    6
#define CAPTURE_THREE_TRACES    7
#define CAPTURE_FOUR_TRACES    8
#define SETGAIN     9

#define START_I2C 10
#define STOP_I2C 11
#define SEND_I2C 12

#define READRF 20
#define GET_VOLTAGE 21
#define GET_FREQ 22
#define GET_TEMP 23
#define GET_CTMU_CALIB  24

#define SEND_CHAR  25
#define SEND_INT  26



#define CAPTURE_EIGHT_TRACES 27
#define GET_CAPTURE_STATUS  28
#define RETRIEVE_CHANNEL    29
#define RETRIEVE_BUFFER     30


#define START_LOGIC_ANALYSER 31
#define STOP_LOGIC_ANALYSER 32
#define GET_LA_STATUS       33


#define SEND_SPI8 34
#define SEND_SPI16 35
#define START_SPI 36
#define STOP_SPI 37

#define ESP_HEADER 254

#include "httpd.h"

int cgiLed(HttpdConnData *connData);
void tplLed(HttpdConnData *connData, char *token, void **arg);
int cgiReadFlash(HttpdConnData *connData);
void tplCounter(HttpdConnData *connData, char *token, void **arg);


void getString(char * buffP);
void tplDAQ(HttpdConnData *connData, char *token, void **arg) ;
int cgiDAQ(HttpdConnData *connData) ;


#endif
