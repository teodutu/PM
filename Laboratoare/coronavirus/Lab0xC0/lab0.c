
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD &= ~_BV(PD2);
	PORTD |= _BV(PD2);

	DDRC = 0xFF;
	PORTC = 0;

	while (1)
	{
		if (!(PIND & _BV(PD2)))
		{
			PORTC |= _BV(PC0);
		} else
		{
			PORTC &= ~_BV(PC0);
		}
	}

	return 0;
}
