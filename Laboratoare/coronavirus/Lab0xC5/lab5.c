#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "spi.h"
#define BUFF_LEN	5
#define TASK_NUM	3

const unsigned char digits_pattern[25]= {
/*  A     C     E     F     G     */
    0x77, 0x39, 0x79, 0x71, 0x3D, 
/*  H     I     J     L     O     */
    0x76, 0x30,	0x1E, 0x38,	0x3F,
/* P     S     U     a     b    */
   0x73, 0x6D, 0x3E, 0x5F, 0x7C,
/* c     d     h     n     o    */
   0x58, 0x5E, 0x74, 0x54, 0x5C,
/* q     r      t    u     y */
   0x67, 0x50, 0x78, 0x1C, 0x6E    
};

/*								  S     A     L     A     H */
unsigned char buffer[BUFF_LEN] = {0x6D, 0x77, 0x38, 0x77, 0x76} ;
uint8_t task_num = TASK_NUM;

void leds_init()
{
	DDRA |= (1 << PA0);
	DDRA |= (1 << PA1);
	DDRA |= (1 << PA2);	
	DDRA |= (1 << PA3);
	DDRA |= (1 << PA4);
	DDRA |= (1 << PA5);
	DDRA |= (1 << PA6);
	DDRA |= (1 << PA7);
	
	PORTA = 0;
}

void task1()
{
	int i;
	for (i = 0; i < BUFF_LEN; i++) {
		/* TODO 1: Use SPI_exchange to transmit the message SALAH. */
		/*Each letter is encoded in the buffer array. */
		SPI_exchange(buffer[i]);
		
		_delay_ms(1);
		PORTB |= (1 << PB0);
		_delay_ms(1);
		PORTB &= ~(1 << PB0);
		_delay_ms(1000);
	}
}

void task2()
{
	uint8_t button;

	PORTB |= (1 << PB1);
	_delay_ms(1);
	PORTB &= ~(1 << PB1);

	/* TODO 2: Use SPI_exchange to receive the state of the buttons. */
	button = SPI_exchange(0);
	
	/* TODO 2: Turn on the LEDs associated with the pressed buttons. */
	PORTA = button;
}

void task3()
{	
	/*TODO 3: Read the state of the buttons and  display the letter on the 7-segment display. */
	uint8_t button;

	PORTB |= _BV(PB1);
	_delay_ms(1);
	PORTB &= ~_BV(PB1);

	button = SPI_exchange(0);

	/*TODO 3: Turn on the LEDs associated with the pressed buttons. */
	SPI_exchange(digits_pattern[button % sizeof(digits_pattern)]);

	_delay_ms(1);
	PORTB |= _BV(PB0);
	_delay_ms(1);
	PORTB &= ~_BV(PB0);
}

int main(void)
{
	leds_init();
	SPI_init();

	/* TODO 1: Set the Slave Select for the shift register. Set PB0 pin as output pin.*/
	DDRB |= _BV(PB0);

	/* TODO 2: Set the Slave Select for the shift register. Set PB1 pin as output pin.*/
	DDRB |= _BV(PB1);

	while (1)
	{
		switch (task_num)
		{
		case 1:
			task1();
			break;
		
		case 2:
			task2();
			break;

		case 3:
			task3();
			break;

		default:
			break;
		}
	}

	return 0;
}