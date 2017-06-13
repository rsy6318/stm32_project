#include<stm32f10x.h>
#include<math.h>
#define uint unsigned int
#define uchar unsigned char
void delay(uint zzz)
{
	uint xxx,yyy;
	for(xxx=5000;xxx>0;xxx--)
	{
		for(yyy=zzz;yyy>0;yyy--);
	}
}
struct point
{
	int x;
	int y;
}start_point;

struct point current_point;

void gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void usart1_init()
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

void y_up()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
	current_point.y++;
	for(char i=0;i<4;i++)
	{
	
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	delay(10);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	delay(10);
	}
}

void y_down()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
	current_point.y--;
	for(char i=10;i>0;i--)
	{
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	delay(10);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	delay(10);
	}
}

void x_left()
{
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	current_point.x--;
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	delay(10);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	delay(10);
}

void x_right()
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	current_point.x++;
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	delay(10);
	GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	delay(10);
}

int main()
{
	start_point.x=0;
	start_point.y=0;
	struct point final_point;
	final_point.x=500;
	final_point.y=500;
	
	current_point.x=0;
	current_point.y=0;
	gpio_init();
	while(current_point.y<final_point.y)
	{
		if(sqrt(current_point.x*current_point.x+(current_point.y-500)*(current_point.y-500))>=500)
		{
			y_up();
			
		}
		if(sqrt(current_point.x*current_point.x+(current_point.y-500)*(current_point.y-500))<500)
		{
			x_right();
		}
	}
	while(current_point.y<2*final_point.y)
	{
		if(sqrt(current_point.x*current_point.x+(current_point.y-500)*(current_point.y-500))>=500)
		{
			x_left();
		}
		if(sqrt(current_point.x*current_point.x+(current_point.y-500)*(current_point.y-500))<500)
		{
			y_up();
		}
	}
	
}

void USART1_IRQHandler(void)
{
	USART_ClearFlag(USART1,USART_FLAG_RXNE);
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==0)
	{
		USART_SendData(USART1,'A');
	}
}
