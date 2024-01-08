#include "usart.h"

void USART_CONFIG(void)
{	
	RCC_APB2PeriphClockCmd(USART_TX_CLK|USART_RX_CLK,ENABLE);
	USART_CLKCMD(USART_CLK,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin		=	USART_TX_GPIO_Pin;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin		=	USART_RX_GPIO_Pin;
	GPIO_Init(USART_RX_PORT,&GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitTypeDef;
	USART_InitTypeDef.USART_BaudRate						= USART_BAD;
	USART_InitTypeDef.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	USART_InitTypeDef.USART_Mode								= USART_Mode_Rx|USART_Mode_Tx;
	USART_InitTypeDef.USART_Parity							= USART_Parity_No;
	USART_InitTypeDef.USART_StopBits						= USART_StopBits_1;
	USART_InitTypeDef.USART_WordLength					= USART_WordLength_8b;
	USART_Init(USART_PORT,&USART_InitTypeDef);
	
//	NVIC_InitTypeDef NVIC_InitStruct;
//	NVIC_InitStruct.NVIC_IRQChannel 									= USART_IRQChannel;
//	NVIC_InitStruct.NVIC_IRQChannelCmd 								= ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority 				= 1;
//	NVIC_Init(&NVIC_InitStruct);
//	
//	USART_ITConfig(USART_PORT,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART_PORT,ENABLE);









   //GPIO端口设置
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //打开时钟
// 
//	
//	/*  配置GPIO的模式和IO口 */
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //串口输出PA9
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
//	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //串口输入PA10
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
//	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
//	

//   //USART1 初始化设置
//	USART_InitStructure.USART_BaudRate = 115200;//波特率设置
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//	USART_Init(USART1, &USART_InitStructure); //初始化串口1
//	
//	USART_Cmd(USART1, ENABLE);  //使能串口1 
//	
//	USART_ClearFlag(USART1, USART_FLAG_TC);
//		
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

//	//Usart1 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}


void Usart_SendByte(USART_TypeDef* USARTx,uint8_t data)
{
	USART_SendData(USARTx,data);
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
}

void Usart_SendString(USART_TypeDef* USARTx,uint8_t *str)
{
	while(*str)
	{	
		Usart_SendByte(USARTx,*str++);
	}
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(USART_PORT, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART_PORT, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART_PORT, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART_PORT);
}


