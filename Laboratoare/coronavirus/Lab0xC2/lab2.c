#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"

ISR(TIMER1_COMPA_vect)
{
	/* Set pin high */
	PORTD |= _BV(PD5);
}

ISR(TIMER1_COMPB_vect)
{
	/* Set pin low */
	PORTD &= ~_BV(PD5);
}

void timer1_init()
{
    TIMSK1 |= _BV(OCIE1A);
	/* Activate interrupt compare with OCR1B */
	TIMSK1 |= _BV(OCIE1B);
	// Fast PWM with top at OCR1A
	TCCR1B |= _BV(WGM12);
	// Fast PWM
	TCCR1A |= _BV(WGM10) | _BV(COM1A1); 
	// Prescaler 8
	TCCR1B |= _BV(CS11);

	/* Init counter register */
	TCNT1 = 0;
	/* Init top register OCR1A 
	 * for an interrupt frequency
	 * of 10k HZ
	 */
	OCR1A = 3;
}

//we will modify this variable when PB2 is pressed
volatile char pb2_pressed = 0;

ISR(INT2_vect)
{
	pb2_pressed = 1;
	PORTD ^= _BV(PD4);
}

ISR(INT0_vect)
{
	if ((PIND & _BV(PD3)))
	{
		USART0_print("Clockwise rotation\r\n");
	} else
	{
		USART0_print("Trigonomteric rotation\r\n");
	}
}

void int2_init()
{
	EICRA|=_BV(ISC21); //the interrupt will be generated on a falling edge of PB2
	EIMSK|=_BV(INT2);
}

void int0_init()
{
	EICRA |= _BV(ISC01) | _BV(ISC00);
	EIMSK |= _BV(INT0);
}

int main(void)
{
	timer1_init();	//initialize the timer
	int2_init();	//initialize interrupt for int2
	int0_init();
	sei();
	USART0_init();
	DDRB  &= ~_BV(PB2);
	PORTB |= _BV(PB2);
	
	/* Set PD4 as output */
	DDRD = _BV(PD4) | _BV(PD5);
	PORTD &= ~_BV(PD4);

	while (1) 
	{
		if (pb2_pressed)
		{
			USART0_print("Hello there\r\n");
			USART0_print("Motor changed direction\r\n");

			pb2_pressed=0;
		}

		_delay_ms(100);
		OCR1A += 25;
		OCR1A = (OCR1A > 255) ? 3 : OCR1A;
	}

	return 0;
}
