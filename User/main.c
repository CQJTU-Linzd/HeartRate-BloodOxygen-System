#include "stm32f10x.h"
#include "exti.h"
#include "systick.h"
#include "usart.h"
#include "iic.h"
#include "oled.h"
#include "MAX30102.h"
#include "algorithm.h"
#include "blood.h"
#include "PWM.h"
#include "string.h"
#include "delay.h"
	

int main(void)
{	
	USART_CONFIG();
	//printf("这是串口调试助手\r\n");
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	
//	printf("AT\r\n");
//	
//	delay_ms(2000);					//延迟	
// 	
//    //printf("AT+CWMODE=1\r\n");		//第二步		模式  只用一次
//	printf("AT+GMR\r\n");
//	delay_ms(3000);					//延迟
//	
//	printf("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n");		//第三步
//	delay_ms(3000);					//延迟
//	
//	printf("AT+CWJAP=\"kad\",\"kad@666@888\"\r\n");		//第四步 wifi热点
//	delay_ms(5000);					//延迟
//	
//	
//	printf("AT+MQTTUSERCFG=0,1,\"NULL\",\"wifi&k0m17RRZzf9\",\"3448fa9818e7731ddd8b2f4cb74294e9f66600f540df02b449209cee56f48a9b\",0,0,\"\"\r\n");		//第五步
//	delay_ms(3000);					//延迟			

//	printf("AT+MQTTCLIENTID=0,\"k0m17RRZzf9.wifi|securemode=2\\,signmethod=hmacsha256\\,timestamp=1704626426691|\"\r\n");		//第六步
//	delay_ms(3000);					//延迟	
//	
//	printf("AT+MQTTCONN=0,\"iot-06z00aw1mgp0q9x.mqtt.iothub.aliyuncs.com\",1883,1\r\n");		//第七步
//	delay_ms(3000);					//延迟
//	
//	
	
	IIC_GPIO_INIT();
	
	PWM_Init();
	
	OLED_Init();	
//	
//	
	printf("OLED_Init\r\n");
//	//fill_picture(0x00);

//	Ds18b20_Init();
//	DS18B20_Read_Temperature();
	
	MAX30102_GPIO();
	
	Max30102_reset();
	
	MAX30102_Config();
//	
	int i;
	for(i = 0;i < 128;i++) 
	{
		while(MAX30102_INTPin_Read()==0)
		{
			//读取FIFO
			max30102_read_fifo();
		}
	}
	


	while(1)
	{	
		//printf("这是串口调试助手\r\n");
		//Delay_ms(1000);
//		deal_Temp();
		blood_Loop();
		//ESP8266_SendData(123);
		//delay_ms(1000);
	}
}
