#include<stm32f10x.h>
#define uint unsigned int
#define uchar unsigned char
#define x_left 1
#define x_stop 2
#define x_right 0
#define y_up 1
#define y_stop 2
#define y_down 0
uchar data[6];

void direction(char xxx,char yyy)
{
	switch(xxx)
	{
		case 1:
			GPIO_SetBits(GPIOC,GPIO_Pin_0);
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			break;
		case 0:
			GPIO_ResetBits(GPIOC,GPIO_Pin_0);
			GPIO_ResetBits(GPIOC,GPIO_Pin_4);
			break;
		case 2:
			GPIO_SetBits(GPIOC,GPIO_Pin_4);
		default:
			break;
	}
	switch(yyy)
	{
		case 1:
			GPIO_SetBits(GPIOC,GPIO_Pin_2);
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
			break;
		case 0:
			GPIO_ResetBits(GPIOC,GPIO_Pin_2);
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);
			break;
		case 2:
			GPIO_SetBits(GPIOC,GPIO_Pin_5);
		default:
			break;
	}
}

char receive_number;
uint x,y;
uint delay_x,delay_y;

void delay(uint j)
{
	uint k,l;
	for(k=j;k>0;k--)
	{
		for(l=50000;l>0;l--);
	}
}

void USART1_INIT()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn  ;      
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1, ENABLE);
	
	
}

void GPIO_INIT()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void TIM2_INIT()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(TIM2);
	TIM_DeInit(TIM3);
	TIM_InternalClockConfig(TIM2);
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseStructure.TIM_Prescaler = 2;      //‘§∑÷∆µ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // ±÷”∑÷∏Ó
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 2400 - 1;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;      
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	NVIC_Init(&NVIC_InitStructure);
	
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;      
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM2, DISABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ARRPreloadConfig(TIM3, DISABLE);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

int main()
{
	delay_x=0;
	delay_y=0;
	GPIO_INIT();
	TIM2_INIT();
	USART1_INIT();
	
//	direction(x_right,y_up);
//	delay_x=100;
//	delay_y=50;
//	delay(500);
//	direction(x_left,y_down);
//	delay(250);
//	direction(x_left,y_down);
//	delay_x=0;
//	delay_y=50;
//	delay(1000);
	
//	direction(x_left,y_down);
//	delay_x=100;delay_y=0;
//	delay(1000);
//	direction(x_right,y_down);
//	delay(500);
//	direction(x_right,y_down);
//	delay_x=0;delay_y=100;
//	delay(5000);

	direction(x_stop,y_up);  //T
	delay_x=0;delay_y=50;
	delay(2500);
	direction(x_left,y_up);
	delay_x=50;delay_y=0;
	delay(250);
	direction(x_right,y_up);
	delay(500);
	
	direction(x_right,y_stop);   //I
	delay_x=50;delay_y=0;
	delay(200);
	direction(x_left,y_stop);   
	delay_x=50;delay_y=0;
	delay(100);
	direction(x_stop,y_down);
	delay_x=0;delay_y=50;
	delay(2500);
	direction(x_left,y_stop);
	delay_x=50;delay_y=0;
	delay(100);
	direction(x_right,y_stop);
	delay(200);
	
	
	direction(x_right,y_up);   //M
	delay_x=250;
	delay_y=15;
	delay(750);
	direction(x_right,y_down);
	delay(750);
	direction(x_right,y_up);
	delay(750);
	direction(x_right,y_down);
	delay(750);
	
//	direction(x_right,y_down);
//	delay_x=0;
//	delay_y=20;
	
	direction(x_stop,y_stop);
	while(1);
}

void TIM2_IRQHandler(void)
{
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	x++;
	if(delay_x==0)
	{
		x=0;
	}
	if(x==delay_x)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
	}
	if(x==delay_x+delay_x)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		x=0;
	}
}

void TIM3_IRQHandler(void)
{
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	y++;
	if(delay_y==0)
	{
		y=0;
	}
	if(y==delay_y)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_3);
	}
	if(y==delay_y+delay_y)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);
		y=0;
	}
}

void USART1_IRQHandler(void)
{
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==0)
	{
		data[receive_number]=USART_ReceiveData(USART1);
		receive_number++;
		if(receive_number==6)
		{
			receive_number=0;
			switch(data[0])
			{
				case 0:
					GPIO_SetBits(GPIOC,GPIO_Pin_4);
				case 1:
					GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					GPIO_ResetBits(GPIOC,GPIO_Pin_0);
					break;
				case 2:
					GPIO_ResetBits(GPIOC,GPIO_Pin_4);
					GPIO_SetBits(GPIOC,GPIO_Pin_0);
					break;
				default:
					break;
			}
				switch(data[3])
			{
				case 0:
					GPIO_SetBits(GPIOC,GPIO_Pin_5);		
				case 1:
					GPIO_ResetBits(GPIOC,GPIO_Pin_5);
					GPIO_ResetBits(GPIOC,GPIO_Pin_2);
					break;
				case 2:
					GPIO_ResetBits(GPIOC,GPIO_Pin_5);
					GPIO_SetBits(GPIOC,GPIO_Pin_2);
					break;
				default:
					break;
			}
			delay_x=data[1]*256+data[2];
			delay_y=data[4]*256+data[5];
		}
	}
}

