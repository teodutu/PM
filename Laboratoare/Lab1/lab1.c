/*
 * lab1.c
 */

#include "lcd.h"
#include "usart.h"

#include <avr/io.h>
#include <util/delay.h>


#define MORSE_LUNG  3
#define MORSE_SCURT 1

/* Literele alfabetului in cod Morse */
char* morse_alphabet[] = {
    (char*)"13",    // a
    (char*)"3111",  // b
    (char*)"3131",  // c
    (char*)"311",   // d
    (char*)"1",     // e
    (char*)"1131",  // f
    (char*)"331",	// g
    (char*)"1111",	// h
    (char*)"11",	// i
    (char*)"1333",	// j
    (char*)"313",	// k
    (char*)"1311",	// l
    (char*)"33",	// m
    (char*)"31",	// n
    (char*)"333",	// o
    (char*)"1331",	// p
    (char*)"3313",	// q
    (char*)"131",	// r
    (char*)"111",	// s
    (char*)"3",	    // t
    (char*)"113",	// u
    (char*)"1113",	// v
    (char*)"133",	// w
    (char*)"3113",	// x
    (char*)"3133",	// y
    (char*)"3311"	// z
};


void exemplu_usart()
{
    for(;;)
    {
        USART0_print("Salut!\n\r");
        _delay_ms(10);
    }
}

void task1_usart()
{
    /* TODO task 1 USART */

    // configurare porturi folosite
    DDRD &= ~_BV(PD6);
    DDRB &= ~_BV(PB2);

    PORTD |= _BV(PD6);
    PORTB |= _BV(PB2);

    int8_t pressed_B = 0;
    int8_t pressed_D = 0;

    // main loop
    for(;;)
    {
        // cand PB2 este apasat trimite un mesaj pe USART o singura data
        if (!(PINB & _BV(PB2)) && !pressed_B)
        {
            USART0_print("PB2 apasat\n");
            pressed_B = 1;
        }

        // cand PB2 este lasat trimite un mesaj pe USART o singura data
        if ((PINB & _BV(PB2)) && pressed_B)
        {
            USART0_print("PB2 lasat\n");
            pressed_B = 0;
        }

        // cand PD6 este apasat trimite un mesaj pe USART o singura data
        if (!(PIND & _BV(PD6)) && !pressed_D)
        {
            USART0_print("PD6 apasat\n");
            pressed_D = 1;
        }

        // cand PD6 este lasat trimite un mesaj pe USART o singura data
        if ((PIND & _BV(PD6)) && pressed_D)
        {
            USART0_print("PD6 lasat\n");
            pressed_D = 0;
        }

        // much switch, so debouncing, such delay
        _delay_ms(5);
    }
}


/* apelati functia pentru a obtine un ton lung/scurt,
 * folosind macro-urile MORSE_LUNG si MORSE_SCURT
 */
void speaker_morse(int tip)
{
    // !!
    uint8_t delay = 1;
    uint8_t i;

    for (i = 0; i < tip * 50 / delay; i++)
    {
        // initial era PIND fara xor
        PORTD ^= _BV(PD4);  // alternam starea iesirii
        _delay_ms(1);
    }
}

void task2_usart()
{
    /* TODO task 2 USART */
    char c;
    char* msg;
    int8_t i;

    // configurare porturi folosite
    DDRD |= _BV(PD4);

    // main loop
    for(;;)
    {
        // primeste un caracter
        c = USART0_receive();

        // daca este caracter litera mica il reda in codul Morse
        // folosing  speaker_morse
        if (c >= 'a' && c <= 'z')
        {
            msg = morse_alphabet[c - 'a'];

            for (i = 0; msg[i] != '\0'; ++i)
            {
                speaker_morse(msg[i] - '0');
                _delay_ms(50);
            }
            _delay_ms(50);
        }
    }
}

void exemplu_lcd()
{
    LCD_print("Hello World!");

    for(;;);
}

void task1_lcd()
{
    /* TODO task 1 LCD */

    // configurare porturi folosite
    DDRB &= ~_BV(PB2);
    DDRD &= ~_BV(PD6);

    PORTB |= _BV(PB2);
    PORTD |= _BV(PD6);

    int8_t pos = 0;
    int8_t pressed_B = 0;
    int8_t pressed_D = 0;

    // initializari
    char str[] = "Hello World";
    LCD_print(str);

    // main loop
    for(;;)
    {
        // in functie de butoanele apasate, reafiseaza mesajul cu o pozitie
        // la stanga sau la dreapta fata de pozitia anterioara
        if (!(PINB & _BV(PB2)))
        {
            if (!pressed_B)
            {
                pressed_B = 1;
                ++pos;

                if (pos + sizeof(str) - 1 > 16)
                {
                    pos = 16 - sizeof(str) + 1;
                }
            }
        } else
        {
            pressed_B = 0;
        }

        if (!(PIND & _BV(PD6)))
        {
            if (!pressed_D)
            {
                pressed_D = 1;
                --pos;

                if (pos < 0)
                {
                    pos = 0;
                }
            }
        } else
        {
            pressed_D = 0;
        }

        if (pressed_B || pressed_D)
        {
            LCD_writeInstr(LCD_INSTR_clearDisplay);
            LCD_printAt(pos, str);
        }

        _delay_ms(5);
    }
}

void task2_lcd()
{
    /* TODO task 2 LCD */

    // main loop
    for(;;)
    {
            // primeste un caracter pe seriala,
            // il afiseaza pe ecran la pozitia curenta
            // daca noua pozitie a cursorului nu este intr-o zona vizibila,
            // trecem la cealalta linie
        }
    }

void bonus_lcd()
{
    /* TODO bonus LCD */

    // configurare porturi folosite

    // definire caracter custom
    uint8_t chuck[] = {
        0x00,   // .....
        0x0e,   // .***.
        0x0e,   // .***.
        0x04,   // ..*..
        0x0e,   // .***.
        0x04,   // ..*..
        0x0e,   // .***.
        0xa     // .*.*.
    };
    chuck[0] = chuck[0];
    // Scrieti caracterul custom in memoria CGRAM, linie cu linie

    // main loop
    for(;;)
    {
        _delay_ms(5);
    }
}

void bonus_usart()
{
    /* TODO bonus USART */

    // configurari porturi folosite
    DDRD &= ~_BV(PD6);
    DDRB &= ~_BV(PB2);

    PORTD |= _BV(PD6);
    PORTB |= _BV(PB2);

    // configurarea stdout
    FILE LCD_stdout;
    
    LCD_stdout.put = USART0_printf;
    LCD_stdout.flags = __SWR;

    stdout = &LCD_stdout;

    // main loop
    int8_t PB2_flag = 0, PD6_flag = 0;
    for(;;)
    {
        // cand PB2 este apasat trimite un mesaj pe USART
        if(!(PINB & (1 << PB2)) && !PB2_flag)
        {
            PB2_flag = 1;
            printf("PB2 apasat\n");
        }

        // cand PB2 este lasat trimite un mesaj pe USART
        if((PINB & (1 << PB2)) && PB2_flag)
        {
            PB2_flag = 0;
            printf("PB2 lasat\n");
        }

        // cand PD6 este apasat trimite un mesaj pe USART
        if(!(PIND & (1 << PD6)) && !PD6_flag)
        {
            PD6_flag = 1;
            printf("PD6 apasat\n");
        }

        // cand PD6 este lasat trimite un mesaj pe USART
        if((PIND & (1 << PD6)) && PD6_flag)
        {
            PD6_flag = 0;
            printf("PD6 lasat\n");
        }

        // much switch, so debouncing, such delay
        _delay_ms(5);
    }
}

int main()
{
    USART0_init();
    LCD_init();

    switch(0)
    {
        //USART
        case 0: exemplu_usart();    break;
        case 1: task1_usart();      break;
        case 2: task2_usart();      break;
        
        //LCD
        case 4: exemplu_lcd();      break;
        case 5: task1_lcd();        break;
        case 6: task2_lcd();        break;
        
        //bonus
        case 3: bonus_usart();      break;
        case 7: bonus_lcd();        break;
    }

    return 0;
}

