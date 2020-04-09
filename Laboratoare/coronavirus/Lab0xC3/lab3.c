#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usart.h"

#define BUFSIZE				256
#define ADC_AREF_VOLTAGE	5.0
#define ADC_MAX_LEVEL		1024.0
#define LM35_RESOLUTION		0.01

volatile uint16_t ADC_value;
char buffer[BUFSIZE];
uint8_t task_num = 3;

ISR(ADC_vect)
{
	/* TODO 2: Read ADC value */
	ADC_value = ADC;
	
	/* TODO 2: Start another conversion */
	ADCSRA |= _BV(ADSC);

	/* TODO 3: Clear output compare match flag A */
	/* ??? */
}

void ADC_init()
{
	/* TODO: Configure channel */
	if (task_num == 1)
	{
		ADMUX = _BV(MUX0);
	} else if (task_num == 2)
	{
		ADMUX = 0;
	} else
	{
		ADMUX = _BV(MUX2) | _BV(MUX1);
	}

	/* AVCC with external capacitor at AREF pin */
	ADMUX |= (1 << REFS0);
	
	ADCSRA = 0;
	/* set prescaler at 128 */
	ADCSRA |= (7 << ADPS0);

	/* TODO 3: Enable auto trigger */
	ADCSRA |= _BV(ADATE);
	
	/* TODO 3: Timer0 compare match A */
	ADCSRB = _BV(ADTS0) | _BV(ADTS1);

	/* TODO2: Enable conversion complete interrupt */
	if (task_num != 1)
	{
		ADCSRA |= _BV(ADIE);
	}

	/* enable ADC */
	ADCSRA |= (1 << ADEN);

	/* start conversion */
	ADCSRA |= (1 << ADSC);
}

void TIMER0_init(void)
{
	/*
	 * f_timer = f_CPU / (2 * prescaler * (OCR0A + 1))
	 * OCR0A + 1 = 234
	 * f_CPU = 12MHz
	 * prescaler = 256
	 * => f_timer = 12 * 10^6 / (2 * 256 * 234) ~= 100Hz
	 * => T ~= 0.01s = 10ms
	 */
	OCR0A = 233;
	TCCR0A = _BV(WGM02);  /* CTC */
	TCCR0B = _BV(CS02);  /* prescaler 1/256 */

	TCNT0 = 0;
}

void task1()
{
	double volts;

	while (!(ADCSRA & _BV(ADIF)));

	volts = ADC * ADC_AREF_VOLTAGE / ADC_MAX_LEVEL;

	sprintf(buffer, "Read %.3fV from PA1\r\n", volts);
	USART0_print(buffer);

	ADCSRA |= _BV(ADSC);
}

void task2()
{
	double volts;
	double degrees;

	if (ADCSRA & _BV(ADSC))
	{
		volts = ADC_value * ADC_AREF_VOLTAGE / ADC_MAX_LEVEL;
		degrees = volts / LM35_RESOLUTION;

		sprintf(buffer, "Read %.3fV i.e. %.3f*C from PA0\r\n", volts, degrees);
		USART0_print(buffer);
	}
}

void task3()
{
	double volts;

	if (ADCSRA & _BV(ADSC))
	{
		volts = ADC_value * ADC_AREF_VOLTAGE / ADC_MAX_LEVEL;

		sprintf(buffer, "PA6 potentiometer voltage is %.3fV\r\n", volts);
		USART0_print(buffer);
	}
}

int main(void)
{
	ADC_init();
	USART0_init();
	TIMER0_init();

	/* TODO2: Enable global interrupts */
	sei();
	
	while (1)
	{
		if (task_num == 1)
		{
			task1();
		} else if (task_num == 2)
		{
			task2();
		} else
		{
			task3();
		}
		
	}

	return 0;
}
