/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  zichuan
  * @date    2023-06-08
  * @brief   配置串口
  ******************************************************************************
  */

#include "bsp_usart.h"



/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


/**
    * @brief  配置USART1
    * @pin    UART1_TX:PA9  UART1_RX:PA10
    * @param  无
    * @retval 无
    */
void Usart1_Config(void)
{
    /*********************************配置GPIO*************************************/
    RCC_APB2PeriphClockCmd(USART1_GPIO_CLK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    //PA9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Pin  = USART1_TX_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);//初始化寄存器
    //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin  = USART1_RX_Pin;
    GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);  //初始化寄存器

    /*********************************配置串口*************************************/
    RCC_APB2PeriphClockCmd(USART1_CLK, ENABLE);  //开启串口时钟
    USART_InitTypeDef USART_InitStructure = {0};

    USART_InitStructure.USART_BaudRate = 115200;  //波特率
    USART_InitStructure.USART_HardwareFlowControl = DISABLE;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;  //不使用奇偶校验
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;

    USART_Init(USART1, &USART_InitStructure); //初始化寄存器
    USART_Cmd(USART1, ENABLE); //使能串口
}


/**
    * @brief  配置USART2
    * @pin    UART2_TX:PA2  UART2_RX:PA3
    * @param  无
    * @retval 无
    */
void Usart2_Config(void)
{
    /*********************************配置GPIO*************************************/
    RCC_APB2PeriphClockCmd(USART2_GPIO_CLK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    //PA2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //复用推挽输出
    GPIO_InitStructure.GPIO_Pin  = USART2_TX_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);
    //PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //浮空输入
    GPIO_InitStructure.GPIO_Pin  = USART2_RX_Pin;
    GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);

    /*********************************配置串口*************************************/
    RCC_APB1PeriphClockCmd(USART2_CLK, ENABLE);  //开启串口时钟
    USART_InitTypeDef USART_InitStructure = {0};

    USART_InitStructure.USART_BaudRate = 115200;  //波特率
    USART_InitStructure.USART_HardwareFlowControl = DISABLE;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;  //不使用奇偶校验
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;

    USART_Init(USART2, &USART_InitStructure); //初始化寄存器
    USART_Cmd(USART2, ENABLE); //使能串口
}

/**
    * @brief  配置USART3
    * @pin    UART3_TX:PB10     UART3_RX:PB11
    * @param  无
    * @retval 无
    */
void Usart3_Config(void)
{
    /*********************************** 配置GPIO ***********************************/
    RCC_APB2PeriphClockCmd(USART3_GPIO_CLK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    //PB10  USART3_TX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    //复用推挽输出
    GPIO_InitStructure.GPIO_Pin   = USART3_TX_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART3_TX_PORT, &GPIO_InitStructure);     //初始化寄存器

    //PB11  USART3_RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;  //浮空输入
    GPIO_InitStructure.GPIO_Pin   = USART3_RX_Pin;
    GPIO_Init(USART3_RX_PORT, &GPIO_InitStructure);     //初始化寄存器

    /*********************************** 配置串口 ***********************************/
    RCC_APB1PeriphClockCmd(USART3_CLK, ENABLE);
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate  =   115200;
    USART_InitStructure.USART_HardwareFlowControl   =   DISABLE;
    USART_InitStructure.USART_Mode  =   USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity    =   USART_Parity_No;
    USART_InitStructure.USART_StopBits  =   USART_StopBits_1;
    USART_InitStructure.USART_WordLength    =   USART_WordLength_8b;

    USART_Init(USART3, &USART_InitStructure);   //初始化寄存器
    USART_Cmd(USART3, ENABLE);  //使能串口
}



/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}







