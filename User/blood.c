#include "blood.h"
#include "usart.h"
#include "oled.h"
#include "PWM.h"
uint16_t g_fft_index = 0;         	 	//fft输入输出下标

struct compx s1[FFT_N+16];           	//FFT输入和输出：从S[1]开始存放，根据大小自己定义
struct compx s2[FFT_N+16];           	//FFT输入和输出：从S[1]开始存放，根据大小自己定义

struct
{
	float 	Hp	;			//血红蛋白	
	float 	HpO2;			//氧合血红蛋白
	
}g_BloodWave;//血液波形数据

BloodData g_blooddata = {0};					//血液数据存储

void dis_DrawCurve(struct compx* data, u8 x);

#define CORRECTED_VALUE			47   			//标定血液氧气含量

/*funcation start ------------------------------------------------------------*/
//血液检测信息更新
void blood_data_update(void)
{
	//标志位被使能时 读取FIFO
	g_fft_index=0;
	while(g_fft_index < FFT_N)
	{
		while(MAX30102_INTPin_Read()==0)
		{
			//读取FIFO
			max30102_read_fifo();  //read from MAX30102 FIFO2
			//将数据写入fft输入并清除输出
			if(g_fft_index < FFT_N)
			{
				//将数据写入fft输入并清除输出
				s1[g_fft_index].real = fifo_red;
				s1[g_fft_index].imag= 0;
				s2[g_fft_index].real = fifo_ir;
				s2[g_fft_index].imag= 0;
				g_fft_index++;
			}
		}
	}
}

//血液信息转换
void blood_data_translate(void)
{	
	float n_denom;
	uint16_t i;
	// 4 pt Moving Average
//	printf("****************red******************************************************************\r\n");
//	for(i = 0;i < FFT_N;i++) 
//	{
//		printf("%f\r\n",s1[i].real);	
//	}
//	printf("***************** ir*****************************************************************\r\n");
//	for(i = 0;i < FFT_N;i++) 
//	{
//		printf("%f\r\n",s2[i].real);	
//	}
	//直流滤波
	float dc_red =0; 
	float dc_ir =0;
	float ac_red =0; 
	float ac_ir =0;
	
	for (i=0 ; i<FFT_N ; i++ ) 
	{
		dc_red += s1[i].real ;
		dc_ir +=  s2[i].real ;
	}
		dc_red =dc_red/FFT_N ;
		dc_ir =dc_ir/FFT_N ;
	for (i=0 ; i<FFT_N ; i++ )  
	{
		s1[i].real =  s1[i].real - dc_red ; 
		s2[i].real =  s2[i].real - dc_ir ; 
	}
	
	//移动平均滤波
	printf("***********8 pt Moving Average red******************************************************\r\n");
	for(i = 1;i < FFT_N-1;i++) 
	{
			n_denom= ( s1[i-1].real + 2*s1[i].real + s1[i+1].real);
			s1[i].real=  n_denom/4.00; 
			
			n_denom= ( s2[i-1].real + 2*s2[i].real + s2[i+1].real);
			s2[i].real=  n_denom/4.00; 			
	}
	//八点平均滤波
	for(i = 0;i < FFT_N-8;i++) 
	{
			n_denom= ( s1[i].real+s1[i+1].real+ s1[i+2].real+ s1[i+3].real+ s1[i+4].real+ s1[i+5].real+ s1[i+6].real+ s1[i+7].real);
			s1[i].real=  n_denom/8.00; 
			
			n_denom= ( s2[i].real+s2[i+1].real+ s2[i+2].real+ s2[i+3].real+ s2[i+4].real+ s2[i+5].real+ s2[i+6].real+ s2[i+7].real);
			s2[i].real=  n_denom/8.00; 
		
			printf("%f\r\n",s1[i].real);		
	}
	printf("************8 pt Moving Average ir*************************************************************\r\n");
	for(i = 0;i < FFT_N;i++) 
	{
		printf("%f\r\n",s2[i].real);	
	}
	printf("**************************************************************************************************\r\n");
	//开始变换显示	
	g_fft_index = 0;	
	//快速傅里叶变换
	FFT(s1);
	FFT(s2);
	
	//解平方
	printf("开始FFT算法****************************************************************************************************\r\n");
	for(i = 0;i < FFT_N;i++) 
	{
		s1[i].real=sqrtf(s1[i].real*s1[i].real+s1[i].imag*s1[i].imag);
		s1[i].real=sqrtf(s2[i].real*s2[i].real+s2[i].imag*s2[i].imag);
	}
	//计算交流分量
	for (i=1 ; i<FFT_N ; i++ ) 
	{
		ac_red += s1[i].real ;
		ac_ir +=  s2[i].real ;
	}
	
	for(i = 0;i < FFT_N/2;i++) 
	{
		printf("%f\r\n",s1[i].real);
	}
	printf("****************************************************************************************\r\n");
	for(i = 0;i < FFT_N/2;i++) 
	{
		printf("%f\r\n",s2[i].real);
	}
	
	printf("结束FFT算法***************************************************************************************************************\r\n");
//	for(i = 0;i < 50;i++) 
//	{
//		if(s1[i].real<=10)
//			break;
//	}
	
	//printf("%d\r\n",(int)i);
	//读取峰值点的横坐标  结果的物理意义为 
	int s1_max_index = find_max_num_index(s1, 30);
	int s2_max_index = find_max_num_index(s2, 30);
	printf("%d\r\n",s1_max_index);
	printf("%d\r\n",s2_max_index);
	//检查HbO2和Hb的变化频率是否一致
//	if(i>=45)
//	{
		//心率计算
//		uint16_t Heart_Rate = 60.00 * SAMPLES_PER_SECOND * 
//													s1_max_index / FFT_N;
		
		float Heart_Rate = 60.00 * ((100.0 * s1_max_index )/ 512.00);
		
		g_blooddata.heart = Heart_Rate;
		
		//血氧含量计算
		//double R = (s2[average].real * s1[0].real)/(s1[s1_max_index].real * s2[0].real);		
		//sp02_num = (1 - sp02_num) * SAMPLES_PER_SECOND + CORRECTED_VALUE;
		
		//double R = (s1[s1_max_index].real/s1[0].real)/(s2[s2_max_index].real/s2[0].real);
		
		//double sp02_num =-16.666666*R*R+ 8.333333 *R + 100;
		//g_blooddata.SpO2 = sp02_num;
		//血氧含量计算
	//float sp02_num = (s2[s1_max_index].real * s1[0].real)/(s1[s1_max_index].real * s2[0].real);
			
			//sp02_num = (1 - sp02_num) * SAMPLES_PER_SECOND + CORRECTED_VALUE;
			//( n_y_ac *n_x_dc_max) / ( n_x_ac *n_y_dc_max)
			
			float R = (ac_ir*dc_red)/(ac_red*dc_ir);
			float sp02_num =-45.060*R*R+ 30.354 *R + 94.845;
			g_blooddata.SpO2 = sp02_num;
			
		//状态正常
//	}
//	else //数据发生异常
//	{
//		g_blooddata.heart = 0;
//		g_blooddata.SpO2 	= 0;
//	}
	//结束变换显示
}


void blood_Loop(void)
{

	//血液信息获取
	blood_data_update();
	
	OLED_Fill(0,23,127,63,0);
	
	dis_DrawCurve(s1, 20);
	dis_DrawCurve(s2, 0);
	//OLED_Refresh_Gram();//更新显示到OLED
	
	//血液信息转换
	blood_data_translate();

	//显示血液状态信息
	//OLED_Printf_EN(2,0,"heart:%3d/min  ",g_blooddata.heart);
	
	
	if (g_blooddata.heart == 46) {
		PWM_Off();
		return;
	}
	
	
	g_blooddata.SpO2 = (g_blooddata.SpO2 > 99.99) ? 99.99:g_blooddata.SpO2;
	//OLED_Printf_EN(4,0,"SpO2:%2.2f%%  ",g_blooddata.SpO2);
	
	u8 str[100];
	sprintf((char *)str,"HR:%d SpO2:%d ",g_blooddata.heart, (int)g_blooddata.SpO2);//**HR:%3d SpO2:%3d 
	OLED_ShowString(0, 0, str, 16);
	OLED_Refresh_Gram();//更新显示到OLED
	
	if ((int)g_blooddata.heart > 80) {
		PWM_On();
		int i;
		for (i = 0; i <= 100; i++)
		{
			PWM_SetCompare1(i);
			Delay_ms(10);
		}
		for (i = 0; i <=100; i++)
		{
			PWM_SetCompare1( 100- i);
			Delay_ms(10);
		}
		Delay_ms(1000);
	} 
	else {
		PWM_Off();
	}
	
	
	
	printf("指令心率%3d",g_blooddata.heart);
	Delay_ms(10);
	printf("指令血氧%0.2f",g_blooddata.SpO2);
	//tft显示刷新
	//LED 蜂鸣器信息更新
}


void dis_DrawCurve(struct compx* data, u8 x)
{
	u16 i;
	u32 max=0,min=262144;
	u32 temp;
	u32 compress;
	
	for(i=0;i<128*2;i++)
	{
		if(data[i].real>max)
		{
			max = data[i].real;
		}
		if(data[i].real<min)
		{
			min = data[i].real;
		}
	}
	
	compress = (max-min)/20;
	
	for(i=0;i<128;i++)
	{
		temp = data[i*2].real + data[i*2+1].real;
		temp/=2;
		temp -= min;
		temp/=compress;
		if(temp>20)temp=20;
		OLED_DrawPoint(i,63-x-temp,1);
	}
}
