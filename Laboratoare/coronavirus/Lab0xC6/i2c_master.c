#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/twi.h>

#include "i2c_master.h"

#define F_SCL 100000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)

void I2C_init(void)
{
	TWBR = (uint8_t)TWBR_val;
}

uint8_t I2C_start(void)
{
	// reset TWI control register
	TWCR = 0;

	// transmit START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait for end of transmission
	while (!(TWCR & (1<<TWINT)));

	// check if the start condition was successfully transmitted
	if ((TWSR & 0xF8) != TW_START)
	{
		return 1;
	}

	return 0;
}

uint8_t I2C_write(uint8_t data)
{
	// load data into data register
	TWDR = data;

	// start transmission of data
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait for end of transmission
	while (!(TWCR & (1<<TWINT)));

	if (TW_STATUS & (TW_MT_DATA_ACK | TW_MT_SLA_ACK))
	{
		return 1;
	}

	return 0;
}

uint8_t I2C_read_ack(void)
{
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);

	// wait for end of transmission
	while (!(TWCR & (1<<TWINT)));

	// return received data from TWDR
	return TWDR;
}

uint8_t I2C_read_nack(void)
{
	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait for end of transmission
	while (!(TWCR & (1<<TWINT)));

	// return received data from TWDR
	return TWDR;
}

void I2C_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);    // transmit STOP condition
}