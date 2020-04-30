#include <avr/io.h>
#include <util/delay.h>
#include "spi.h"

void SPI_init(){
	/*  Configure SS pin of master as output pin on HIGH */
	DDRB |= (1 << PB4);
	PORTB |= (1 << PB4);
	_delay_ms(1);
	
	/* Configure MOSI pin and SCK pin as output pin  */
	DDRB |= (1 << SPI_MOSI) | (1 << SPI_SCK);
	
	/* TODO 1: Enable SPI  */
	SPCR0 |= _BV(SPE0);
	
	/* TODO 1: Set master mode */
	SPCR0 |= _BV(MSTR0);
	
	/* set prescaler 16 */
	SPCR0  |=  (1 << SPR00);
}

uint8_t SPI_exchange(uint8_t data){
	/* TODO 1: Start transmission */
	SPDR0 = data;
	
	/* TODO 1: Wait for transmission / reception complete */
	while(!(SPSR0 & _BV(SPIF0)));
	
	/* TODO 1: Return Data Register */
	return SPDR0;
}
