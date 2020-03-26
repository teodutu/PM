
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

// digit pattern for a 7-segment display
const uint8_t digit_pattern[16] =
	{
		0b00111111, // 0
		0b00000110, // 1
		0b01011011, // 2
		0b01001111, // 3
		0b01100110, // 4
		0b01101101, // 5
		0b01111101, // 6
		0b00000111, // 7
		0b01111111, // 8
		0b01101111, // 9
		0b01110111, // A
		0b01111100, // b
		0b00111001, // C
		0b01011110, // d
		0b01111001, // E
		0b01110001  // F
};

volatile uint8_t counter;
volatile uint8_t increase;

/* Timer1 Output Compare Interrupt A */
ISR(TIMER1_COMPA_vect)
{
	PORTD ^= _BV(PD2);

	if (increase)
	{
		counter = (counter + 1) % 100;
	}
}

/* pin change interrupt on PORTD */
ISR(PCINT3_vect)
{
	if (!(PIND & _BV(PD3)))
    {
        PORTD ^= _BV(PD4);
		increase ^= 1;

		if (increase == 0)
		{
			counter = 0;
		}
    }
}

/* initialize GPIO inputs and outputs */
void IO_init()
{
	DDRD |= _BV(PD4);
	PORTD &= ~_BV(PD4);

	DDRD |= _BV(PD2);
	PORTD &= ~_BV(PD2);

	DDRA = 0xFF;

	DDRC |= _BV(PC2);

	PCICR |= _BV(PCIE3);
	PCMSK3 |= _BV(PCINT27);
}

/* initializeaza timer-ul 1 */
void timer1_init()
{
	// initialize the timer counter
	TCNT1 = 0;
	// configure the threshold
	OCR1A = 12000000 / 256;
	// enable compare interrupt A
	TIMSK1 = _BV(OCIE1A);
	// configure mode of operation e.g. CTC with OCR1A
	TCCR1B = _BV(WGM12);
	// set the prescaler e.g 256
	TCCR1B |= _BV(CS12);
}

int main(void)
{
	// init peripherals
	IO_init();
	_delay_ms(1000);
	timer1_init();
	sei();

	// power on test
	PORTD |= (1 << PD4);
	_delay_ms(500);
	PORTD &= ~(1 << PD4);
	_delay_ms(500);

	counter = 0;

	// main loop
	while (1)
	{
		// convert the number to digits
		// drive the 7-segment display one digit at a time with a delay in between
		PORTA = _BV(PA7) | digit_pattern[counter / 10];
		PORTC = 0;

		_delay_ms(100);

		PORTC |= _BV(PC2);
		PORTA = digit_pattern[counter % 10];

		_delay_ms(100);
	}

	return 0;
}
