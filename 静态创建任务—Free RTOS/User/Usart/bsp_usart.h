#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"                  // Device header
#include <stdio.h>

// ´®¿Ú1-USART1
#define DEBUG_USARTx                USART1
#define USART1_GPIO_CLK				RCC_APB2Periph_GPIOA
#define USART1_CLK					RCC_APB2Periph_USART1
#define USART1_TX_PORT				GPIOA
#define USART1_TX_Pin				GPIO_Pin_9
#define USART1_RX_PORT				GPIOA
#define USART1_RX_Pin				GPIO_Pin_10


#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

// ´®¿Ú2-USART2
//#define DEBUG_USARTx                USART2
#define USART2_GPIO_CLK				RCC_APB2Periph_GPIOA
#define USART2_CLK					RCC_APB1Periph_USART2
#define USART2_TX_PORT				GPIOA
#define USART2_TX_Pin				GPIO_Pin_2	
#define USART2_RX_PORT				GPIOA
#define USART2_RX_Pin				GPIO_Pin_3

// ´®¿Ú3-USART3
//#define DEBUG_USARTx                USART3
#define	USART3_GPIO_CLK				RCC_APB2Periph_GPIOB
#define USART3_CLK					RCC_APB1Periph_USART3
#define USART3_TX_PORT				GPIOB
#define USART3_TX_Pin				GPIO_Pin_10
#define USART3_RX_PORT				GPIOB
#define USART3_RX_Pin				GPIO_Pin_11



void Usart1_Config(void);
void Usart2_Config(void);
void Usart3_Config(void);


void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);


#endif /* __BSP_USART_H */

