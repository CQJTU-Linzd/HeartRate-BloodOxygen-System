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
	//printf("���Ǵ��ڵ�������\r\n");
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	
//	printf("AT\r\n");
//	
//	delay_ms(2000);					//�ӳ�	
// 	
//    //printf("AT+CWMODE=1\r\n");		//�ڶ���		ģʽ  ֻ��һ��
//	printf("AT+GMR\r\n");
//	delay_ms(3000);					//�ӳ�
//	
//	printf("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n");		//������
//	delay_ms(3000);					//�ӳ�
//	
//	printf("AT+CWJAP=\"kad\",\"kad@666@888\"\r\n");		//���Ĳ� wifi�ȵ�
//	delay_ms(5000);					//�ӳ�
//	
//	
//	printf("AT+MQTTUSERCFG=0,1,\"NULL\",\"wifi&k0m17RRZzf9\",\"3448fa9818e7731ddd8b2f4cb74294e9f66600f540df02b449209cee56f48a9b\",0,0,\"\"\r\n");		//���岽
//	delay_ms(3000);					//�ӳ�			

//	printf("AT+MQTTCLIENTID=0,\"k0m17RRZzf9.wifi|securemode=2\\,signmethod=hmacsha256\\,timestamp=1704626426691|\"\r\n");		//������
//	delay_ms(3000);					//�ӳ�	
//	
//	printf("AT+MQTTCONN=0,\"iot-06z00aw1mgp0q9x.mqtt.iothub.aliyuncs.com\",1883,1\r\n");		//���߲�
//	delay_ms(3000);					//�ӳ�
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
			//��ȡFIFO
			max30102_read_fifo();
		}
	}
	


	while(1)
	{	
		//printf("���Ǵ��ڵ�������\r\n");
		//Delay_ms(1000);
//		deal_Temp();
		blood_Loop();
		//ESP8266_SendData(123);
		//delay_ms(1000);
	}
}
