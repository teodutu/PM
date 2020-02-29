PORT ?= COM12

all: lab1.hex

program: upload

lab1.elf: lab1.c lcd.c usart.c
	avr-g++ -mmcu=atmega324p -DF_CPU=16000000 -Wall -Os -o $@ $^

lab1.hex: lab1.elf
	avr-objcopy -j .text -j .data -O ihex lab1.elf lab1.hex
	avr-size lab1.elf

upload: lab1.hex
	avrdude -c arduino -P $(PORT) -b 57600 -p atmega324p -U flash:w:$<:a

clean:
	rm -rf lab1.elf lab1.hex

.PHONY: all clean program upload

