#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "i2c_master.h"

#define BUFSIZE	256
char buffer[BUFSIZE];

void step1( void){
	uint8_t ack;

	USART0_print("Step 1 - First operation\r\n");

  	// TODO - send start
	I2C_start();

	// TODO - send the header and the transmit bit (0) - return the status in the ack variable
	ack = I2C_write(0xA0);

	// TODO - send stop 
	I2C_stop();

	snprintf(buffer, sizeof(buffer), "    The response from memory is: %d\r\n", ack);
	USART0_print(buffer);

	USART0_print("\r\n");
}

void step2( void){
	int i;

	USART0_print("Step 2 - Check the slaves:\r\n");

	for (i = 0; i < 8; i++)
	{
	  	// TODO - send start
		I2C_start();
		
		if (I2C_write(0xA0 | (i << 1)))
		{
			snprintf(buffer, sizeof(buffer), "    Found slave at address 0x%2X\r\n", i);
			USART0_print(buffer);
		}

		// TODO - send stop
		I2C_stop();
	}

	USART0_print("\r\n");
}

void step3( void){

	USART0_print("Step 3 - Write in the memory\r\n");

	// Write in the slave 0, at location 0 the 0x55
  	// TODO - send start;
	I2C_start();
	
	// TODO - Send the header + slave address + transmit bit
	I2C_write(0xA0);

	// TODO - Send the memory array address
	I2C_write(0x00);
	// TODO - Send the data
	I2C_write(0x55);
  	
	// TODO - send stop;
	I2C_stop();

	_delay_ms(10); // Wait for the memory chip to write the data

	// Write in the slave 0, at location 1 the 0xAA
  	
	// TODO - send start;
	I2C_start();

	// TODO - Send the header + slave address + transmit bit
	I2C_write(0xA0);

	// TODO - Send the memory array address
	I2C_write(0x01);
	// TODO - Send the data
	I2C_write(0xAA);

	// TODO - send stop;
	I2C_stop();

	_delay_ms(10); // Wait for the memory chip to write the data

	USART0_print("\r\n");
}

void step4( void){
	uint8_t data0, data1, data2, data3;

	USART0_print("Step 4 - Read from the memory\r\n");

  	// TODO - send start;
	I2C_start();

	// TODO - Send the header + slave address + transmit bit
	I2C_write(0xA0);

	// TODO - Send the memory array address
	I2C_write(0x00);

	// TODO - send start;
	I2C_start();
	// TODO - Send the header + slave address + receive bit
	I2C_write(0xA1);

	// TODO - Read with ack in data0
	data0 = I2C_read_ack();
	// TODO - Read with ack in data1
	data1 = I2C_read_ack();
	// TODO - Read with ack in data2
	data2 = I2C_read_ack();
	// TODO - Read with no ack in data3
	data3 = I2C_read_nack();

	// TODO - send stop
	I2C_stop();

	snprintf(buffer, sizeof(buffer), "    Read from the memory 0x%2X 0x%2X 0x%2X 0x%2X \r\n", data0, data1, data2, data3);
	USART0_print(buffer);

	USART0_print("\r\n");
}

int main(void)
{
	USART0_init();
	I2C_init();

	USART0_print("Lab 6 - Started!\r\n");
	USART0_print("\r\n");

	step1();
	step2();
	step3();
	step4();

	while (1);

	return 0;
}