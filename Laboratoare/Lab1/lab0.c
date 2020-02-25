/*
 * PM - 2020
 * Laboratorul 0: Aplicatii introductive
 * https://ocw.cs.pub.ro/courses/pm/lab/lab0
 * lab0.c
 */

#include <avr/io.h>
#include <util/delay.h>

#define DELAY_SECVENTA  (250)
#define NUM_SECVENTE    (6)

#define MORSE_LUNG      (150)
#define MORSE_SCURT     (50)
#define MORSE_PAUZA     (50)

#define NUM_NOTE        (8)

#define SHIFT_RIGHT     (1)
#define SHIFT_LEFT      (0)

int frecventa_nota[NUM_NOTE] =
{
    262, 294, 330, 349, 392, 440, 494, 523
};

/* Aprinde led-ul PC0 la apasarea butonului PB2. */
void exemplu(void)
{
    /* Setez pinul PB2 ca intrare. */
    DDRB &= ~(1 << PB2);

    /* Activez rezistenta de pull-up pentru pinul PB2. */
    PORTB |= (1 << PB2);

    /* Setez pinul PC0 ca iesire. */
    DDRC |= (1 << PC0);

    /* Sting LED-ul. */
    PORTC &= ~(1 << PC0);

    while (1)
    {
        /* Daca butonul este apasat. */
        if ((PINB & (1 << PB2)) == 0)
            /* Aprind LED-ul. */
            PORTC |= (1 << PC0);
        else
            /* Sting LED-ul. */
            PORTC &= ~(1 << PC0);
    }
}

/*
 * Aprinde LED-urile legate la portul C in functie de secventele alese.
 * Secventa va fi selectata la apasarea butonului PB2 sau PD6.
 */
void task1(void)
{
    /* TODO1: Setare directii porturi folosite. */
    DDRB &=         ~_BV(PB2);
    DDRD &=         ~_BV(PD6);
    DDRA |=         _BV(PA0);

    PORTB |=        _BV(PB2);
    PORTD |=        _BV(PD6);
    PORTC &=        ~_BV(PC0);

    int8_t pin      = 0;
    int8_t old_pin  = 1;
    int8_t state    = SHIFT_LEFT;

    while(1)
    {
        /* TODO1: Citire butoane PB2 si PD6. */
        /* TODO1: Generare secvente. */
        PORTC &= ~_BV(old_pin);
        PORTC |= _BV(pin);

        old_pin = pin;

        if (!(PINB & _BV(PB2)))
        {
            if (!(PIND & _BV(PD6)))
            {
                PORTC = 0xFF;
            } else
            {
                pin <<= 1;

                if (pin == 0)
                {
                    pin = 1;
                }
            }            
        } else if (!(PIND & _BV(PD6)))
        {   
            if (state == SHIFT_LEFT)
            {
                pin <<= 1;
            } else
            {
                pin >>= 1;
            }

            if (pin == 0)
            {
                if (state == SHIFT_LEFT)
                {
                    pin = 0x80;
                } else
                {
                    pin = 1;
                }

                state ^= 1;
            }
        }

        _delay_ms(DELAY_SECVENTA);
    }
}

/* Genereaza sunetul pentru tipul de ton folosit: lung (150ms) sau scurt (50ms). */
void speaker_morse(int tip_morse)
{
    /*
     * TODO2: Alternarea starii pinului PD4 la interval de o milisecunda.
     * Atentie la tipul de ton folosit (tip_morse).
     */
    PORTD |= _BV(PD4);

    int8_t i;

    for (i = 0; i != tip_morse; ++i)
    {
        _delay_ms(1);
    }

    PORTD &= ~_BV(PD4);
}

/*
 * Genereaza tonuri pentru codul Morse folosind speaker-ul. Tonul generat este
 * selectat folosind butoanele PB2 si PD6.
 */
void task2(void)
{
    /* TODO2: Setare directii porturi folosite. */
    DDRD |= _BV(PD4);
    DDRD &= ~_BV(PD6);
    DDRB &= ~_BV(PB2);

    PORTD &= ~_BV(PD4);
    PORTD |= _BV(PD6);
    PORTB |= _BV(PB2);

    while (1)
    {
        /* TODO2: Generare cod morse pentru PB2 si PD6. */
        if(!(PINB & _BV(PB2)))
        {
            speaker_morse(MORSE_SCURT);
        } else if (!(PIND & _BV(PD6)))
        {
            speaker_morse(MORSE_LUNG);
        }

        _delay_ms(MORSE_PAUZA);
    }
}

/* Genereaza o nota folosind speaker-ul. */
void speaker_reda(int nota)
{
    int i;

    /*
     * TODO3: Calculati perioada notei in functie de frecventa, in
     * microsecunde.
     */

    /*
     * TODO3: Generati un ton corespunzator notei.
     *
     * Pentru a genera un ton cu o anumita perioada trebuie ca jumatate din
     * perioada speaker-ul sa fie pornit, iar jumatate din perioada sa fie
     * oprit.
     */
}

/* Aprinde LED-ul corespunzator notei alese. */
void led_aprinde(int nota)
{
    /* TODO3: Aprinde LED-ul corespunzator. */
}

/* Stinge LED-urile. */
void led_stinge(void)
{
    /* TODO3: Stinge LED-urile. */
}

/*
 * Genereaza note folosind speaker-ul. Nota generata este aleasa folosind
 * butoanele PB2 si PD6.
 */
void task3(void)
{
    int nota_curenta = -1;

    /* TODO3: Setare directii porturi folosite. */
    while (1)
    {
        /* TODO3: Verificare butoane. */
        if (nota_curenta != -1)
        {
            /* Aprindem LED-ul corespunzator. */
            led_aprinde(nota_curenta);

            /* Redam nota. */
            speaker_reda(nota_curenta);
        } else
        {
            /* Stingem LED-urile. */
            led_stinge();
        }
    }
}

int main(void)
{
    exemplu();
    /*task1();*/
    /*task2();*/
    /*task3();*/

    return 0;
}
