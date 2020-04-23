#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "usart.h"

#define BUFSIZE	256
#define ADC_AREF_VOLTAGE	5.0
#define ADC_MAX_LEVEL	1024.0
#define LM35_RESOLUTION	0.01

volatile uint16_t ADC_value;

int count;

ISR(ADC_vect)
{
	char buffer[BUFSIZE];
	double result, temperature;
	ADC_value = ADC;
	
	result = ADC_value * ADC_AREF_VOLTAGE / ADC_MAX_LEVEL;
	temperature = result / LM35_RESOLUTION;
	
	if (temperature > 40)
	{
		count++;
	} else
	{
		/*
		 * Altfel o sa se afiseze "ALARM!" mereu dupa ce se ajunge la 30*C
		 * timp de 5 secunde (sau la overflow).
		 */
		count = 0;
	}

	if (count >= 10)
	{
		/* Ca sa se evite overflow-ul */
		count = 10;
		sprintf(buffer, "ALARM!\r\n");
		USART0_print(buffer);
	}

	_delay_ms(500);
	/* start another conversion */
	ADCSRA |= (1 << ADSC);
}


void ADC_init()
{
	ADMUX = 0;
	ADMUX |= (1 << REFS0);
	
	ADCSRA = 0;
	ADCSRA |= (7 << ADPS0);

	ADCSRB = 0;

	/* enable ADC */
	ADCSRA |= (1 << ADEN);
	ADCSRA |= _BV(ADIE);
	ADCSRA |= (1 << ADSC);
}


int main(void)
{

	ADC_init();
	USART0_init();

	sei();
	
	while (1)
	{
	}

	return 0;
}
