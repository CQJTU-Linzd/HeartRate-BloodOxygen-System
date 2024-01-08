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









   //GPIO�˿�����
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //��ʱ��
// 
//	
//	/*  ����GPIO��ģʽ��IO�� */
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //�������PA9
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
//	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //��������PA10
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
//	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */
//	

//   //USART1 ��ʼ������
//	USART_InitStructure.USART_BaudRate = 115200;//����������
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
//	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
//	
//	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
//	
//	USART_ClearFlag(USART1, USART_FLAG_TC);
//		
//	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

//	//Usart1 NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
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

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART_PORT, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART_PORT, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART_PORT, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART_PORT);
}


