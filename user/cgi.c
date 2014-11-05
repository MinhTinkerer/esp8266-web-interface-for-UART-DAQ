/*
Some random cgi routines.
*/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "httpd.h"
#include "cgi.h"
#include "io.h"
#include "espmissingincludes.h"
#include "stdout.h"




#define SEND_HEADER os_printf("\n!");
//cause I can't be bothered to write an ioGetLed()
static char currLedState=0;

//Cgi that turns the LED on or off according to the 'led' param in the POST data
int ICACHE_FLASH_ATTR cgiLed(HttpdConnData *connData) {
	int len;
	char buff[1024];
	
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->postBuff, "led", buff, sizeof(buff));
	if (len!=0) {
		currLedState=atoi(buff);
		ioLed(currLedState);
	}

	httpdRedirect(connData, "led.tpl");
	return HTTPD_CGI_DONE;
}



//Template code for the led page.
void ICACHE_FLASH_ATTR tplLed(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "ledstate")==0) {
		if (currLedState) {
			os_strcpy(buff, "on");
		} else {
			os_strcpy(buff, "off");
		}
	}
	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));
}

static long hitCounter=0;

//Template code for the counter on the index page.
void ICACHE_FLASH_ATTR tplCounter(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	if (os_strcmp(token, "counter")==0) {
		hitCounter++;
		os_sprintf(buff, "%ld", hitCounter);
	}
	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));
}


//Cgi that reads the SPI flash. Assumes 512KByte flash.
int ICACHE_FLASH_ATTR cgiReadFlash(HttpdConnData *connData) {
	int *pos=(int *)&connData->cgiData;
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if (*pos==0) {
		//os_printf("Start flash download.\n");
		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", "application/bin");
		httpdEndHeaders(connData);
		*pos=0x40200000;
		return HTTPD_CGI_MORE;
	}
	espconn_sent(connData->conn, (uint8 *)(*pos), 1024);
	*pos+=1024;
	if (*pos>=0x40200000+(512*1024)) return HTTPD_CGI_DONE; else return HTTPD_CGI_MORE;
}







//Code for accessing DAQ
void ICACHE_FLASH_ATTR tplDAQ(HttpdConnData *connData, char *token, void **arg) {
	char buff[800];
	char *buffP=&buff[0];
	if (token==NULL) return;

	os_strcpy(buff, "0");
	if (os_strcmp(token, "version")==0) {
			os_printf("\n%cv",ESP_HEADER);
			while(1){
					*buffP = getChar();
					if(*buffP=='\0')break;
					buffP++;
				}
	}
	
	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));
}






void getString(char * buffP){
	while(1){
			*buffP = getChar();
			if(*buffP=='\0')break;
			buffP++;
		}
}

void inttoString(int i,char * c){
	c[0]=i&0xFF;
	c[1]=(i>>8)&0xFF;
	c[2]='\0';
}
//Cgi script for accessing DAQ functions
int ICACHE_FLASH_ATTR cgiDAQ(HttpdConnData *connData) {
	int len;
	char cmdname[50],arg1[20],arg2[20],arg3[20],arg4[20];
	char buff[4000];
	char *buffP=&buff[0];
	int val;
	os_strcpy(buff, "ERR");
	
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->postBuff, "command", cmdname, sizeof(cmdname));
			httpdFindArg(connData->postBuff, "arg1", arg1, sizeof(arg1));
			httpdFindArg(connData->postBuff, "arg2", arg2, sizeof(arg2));
			httpdFindArg(connData->postBuff, "arg3", arg3, sizeof(arg3));
			httpdFindArg(connData->postBuff, "arg4", arg4, sizeof(arg4));
	if (len!=0) {

	if (os_strcmp(cmdname, "version")==0) {
			os_printf("\n%cv",ESP_HEADER);
			getString(buffP);
		}
	else if (os_strcmp(cmdname, "read flash")==0) {
			os_printf("\n%c%c%c",ESP_HEADER,READFLASH,atoi(arg1));
			getString(buffP);
		}
	else if (os_strcmp(cmdname, "set dac")==0) {
			val=(atoi(arg1)<<15)|(0<<14)|(1<<13)|(1<<12)|atoi(arg2); //channel-15,buf-14,g-13,on/off-12,value 0-11
			os_printf("\n%c%c%c%c",ESP_HEADER,SETDAC,val&0xFF,(val>>8)&0xFF);
			os_strcpy(buff, "1");
		}
	else if (os_strcmp(cmdname, "capacitance")==0) {
			val=atoi(arg2);
			os_printf("\n%c%c%c%c%c%c",ESP_HEADER,GET_CTMU_CALIB,atoi(arg1),0,val&0xFF,(val>>8)&0xFF);
			getString(buffP);
		}
	else if (os_strcmp(cmdname, "set gain")==0) {
			os_printf("\n%c%c%c%c",ESP_HEADER,SETGAIN,atoi(arg1),atoi(arg2));
			os_sprintf(buff, "gain of channel %d set",atoi(arg1));
		}
	else if (os_strcmp(cmdname, "get voltage")==0) {
			os_printf("\n%c%c%c",ESP_HEADER,GET_VOLTAGE,atoi(arg1));
			getChar(); // 0 sent by device recovering from sleep mode.
			getString(buffP);
		}
	else if (os_strcmp(cmdname, "start scope")==0) {
			val=atoi(arg1);
			os_printf("\n%c%c%c%c",ESP_HEADER,CAPTURE_FOUR_TRACES,val&0xFF,(val>>8)&0xFF); //samples
			val=atoi(arg2);
			os_printf("%c%c%c",val&0xFF,(val>>8)&0xFF,atoi(arg3));//delay, channel
			val=atoi(arg4);
			os_printf("%c%c",  val&0xFF,(val>>8)&0xFF);			  //trigger level
			os_sprintf(buff, "scope started");
		}
	else if (os_strcmp(cmdname, "scope status")==0) {
			os_printf("\n%c%c",ESP_HEADER,GET_CAPTURE_STATUS);
			getString(buffP);
		}
	else if (os_strcmp(cmdname, "retrieve channel")==0) {
			os_printf("\n%c%c%c",ESP_HEADER,RETRIEVE_CHANNEL,atoi(arg1)); //channel
			val=atoi(arg2);
			os_printf("%c%c",val&0xFF,(val>>8)&0xFF);//samples
			val=atoi(arg3);
			os_printf("%c%c",val&0xFF,(val>>8)&0xFF); //offset
			getString(buffP);
		}
	
	espconn_sent(connData->conn, (uint8 *)buff, os_strlen(buff));

	}

	//httpdRedirect(connData, "led.tpl");
	return HTTPD_CGI_DONE;
}









