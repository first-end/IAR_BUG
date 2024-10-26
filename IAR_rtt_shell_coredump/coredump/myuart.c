#include "myuart.h"

#define USART1_BASE           (0x40011000)

typedef unsigned int uint32_t;
typedef struct
{
  volatile uint32_t SR;    /*!< USART Status register, Address offset: 0x00 */
  volatile uint32_t DR;    /*!< USART Data register,   Address offset: 0x04 */
  volatile uint32_t BRR;   /*!< USART Baud rate register, Address offset: 0x08 */
  volatile uint32_t CR1;   /*!< USART Control register 1, Address offset: 0x0C */
  volatile uint32_t CR2;   /*!< USART Control register 2, Address offset: 0x10 */
  volatile uint32_t CR3;   /*!< USART Control register 3, Address offset: 0x14 */
  volatile uint32_t GTPR;  /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_TypeDef;
	
int mygetchar(void)
{
	USART_TypeDef *usart1 = (USART_TypeDef *)USART1_BASE;
	while ((usart1->SR & (1<<5)) == 0);
	return usart1->DR;
}

int myputchar(char c)
{
	USART_TypeDef *usart1 = (USART_TypeDef *)USART1_BASE;
	while ((usart1->SR & (1<<7)) == 0);
	usart1->DR = c;
	
	return c;
}

