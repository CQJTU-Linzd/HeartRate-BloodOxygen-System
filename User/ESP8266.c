#include "ESP8266.h"
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "string.h"

void ESP8266_Init() {
	
}

void ESP8266_SendData(int data) {
	printf("AT+MQTTSUB=0,\"/k0m17RRZzf9/wifi/user/get\",1\r\n");		//订阅指令
	delay_ms(5000);			//延迟
	
	//printf("AT+MQTTPUB=0,\"/k0m17RRZzf9/wifi/user/update\",\"123\",1,0\r\n");		//发数据{"LED":1}
	
	char str[128];
	strcat(str, "AT+MQTTPUB=0,\"/k0m17RRZzf9/wifi/user/update\",\"");
	char sdata[20];
	sprintf(sdata, "%d", data);
	strcat(str, sdata);
	strcat(str, "\",1,0\r\n");
	
	printf((const char*)str);

}