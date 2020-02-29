#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <stdint.h>

/*****************************************************************************\
 * Define-uri pentru parametrizarea bibliotecii LCD                          *
\*****************************************************************************/

// Portul pe care conectam pinii de date ai LCD-ului
#define LcdDATA_DDR             DDRC
#define LcdDATA_PORT            PORTC
#define LcdDATA_PIN             PINC

// Pinii de date ai LCD-ului
#define LcdD4                   PC3
#define LcdD5                   PC4
#define LcdD6                   PC5
#define LcdD7                   PC6

// Portul pe care conectam pinii de control ai LCD-ului
#define LcdCTRL_DDR             DDRC
#define LcdCTRL_PORT            PORTC
#define LcdCTRL_PIN             PINC

// Pinii de control ai LCD-ului
#define LcdRS                   PC0
#define LcdRW                   PC1
#define LcdE                    PC2

/*****************************************************************************\
 * Comnezi utile                                                             *
\*****************************************************************************/

#define LCD_INSTR_4wire         0x28    // 4 biti de date, 2 linii, font 8x5
#define LCD_INSTR_displayConfig 0x0f    // display pornit, cursor afisat, blink activat
#define LCD_INSTR_incNoShift    0x06    // auto-increment, fara shiftare
#define LCD_INSTR_clearDisplay  0x01    // sterge display
#define LCD_INSTR_returnHome    0x02    // reseteaza cursorul si shiftarea
#define LCD_INSTR_nextLine      0xC0    // adresa celei de-a doua linii
#define LCD_INSTR_DDRAM         0x80    // prima adresa din DDRAM
#define LCD_INSTR_CGRAM         0x40    // prima adresa din CGRAM

/*****************************************************************************\
 * Macro-uri utile                                                           *
\*****************************************************************************/

// Mask pentru bitii de date
#define LCD_DATA_MASK           (_BV(LcdD4)|_BV(LcdD5)|_BV(LcdD6)|_BV(LcdD7))

// Mask pentru bitii de control
#define LCD_CTRL_MASK           (_BV(LcdRS)|_BV(LcdRW)|_BV(LcdE))

// Controleaza pinul RS
#define LCD_RS_HIGH()           do { LcdCTRL_PORT |=  _BV(LcdRS); } while(0)
#define LCD_RS_LOW()            do { LcdCTRL_PORT &= ~_BV(LcdRS); } while(0)

// Controleaza pinul RW
#define LCD_RW_HIGH()           do { LcdCTRL_PORT |=  _BV(LcdRW); } while(0)
#define LCD_RW_LOW()            do { LcdCTRL_PORT &= ~_BV(LcdRW); } while(0)

// Controleaza pinul E
#define LCD_ENABLE()            do { LcdCTRL_PORT |=  _BV(LcdE); } while(0)
#define LCD_DISABLE()           do { LcdCTRL_PORT &= ~_BV(LcdE); } while(0)

// Delay-ul minim necesar pentru operatiile cu semnalul E -> 250ns
// Acopera toate celelalte delay-uri si este jumatate din Enable cycle time (T_cycE)
#define LCD_DELAY()             __builtin_avr_delay_cycles(250 / (1000000000/F_CPU))

/*****************************************************************************\
 * API LCD                                                                   *
\*****************************************************************************/

// Initializare LCD considerand o interfatare cu 4 pini de date.
// Trebuie apelata inainte de a face orice operatie cu LCD-ul.
void LCD_init(void);

// Executa secventa de citire a unui octet de date de la LCD.
// Pentru interfatarea cu 4 pini de date sunt necesare 2 transferuri.
uint8_t LCD_read(void);

// Citeste starea LCD-ului (contine busy flag).
uint8_t LCD_readStatus(void);

// Citeste un octet din ultima memorie folosita (DDRAM sau CGRAM).
uint8_t LCD_readData(void);

// Returneaza starea LCD-ului: 1 - busy, 0 - available
uint8_t LCD_isBusy(void);

// Asteapta pana cand LCD-ul devine disponibil pentru o noua comanda.
void LCD_waitNotBusy(void);

// Executa secventa de trimitere a unui octet de date catre LCD.
// Pentru interfatarea cu 4 pini de date sunt necesare 2 transferuri.
void LCD_write(uint8_t data);

// Trimite o instructiune de control catre LCD.
void LCD_writeInstr(uint8_t instr);

// Trimite o instructiune de scriere date catre LCD.
void LCD_writeData(uint8_t data);


void LCD_putChar(char c);                           // Afiseaza caracterul pe LCD la adresa curenta.
void LCD_putCharAt(uint8_t addr, char c);           // Afiseaza caracterul pe LCD la adresa primita.

void LCD_print(const char* msg);                    // Afiseaza string-ul pe LCD incepand de la adresa curenta.
void LCD_printAt(uint8_t addr, const char* msg);    // Afiseaza string-ul pe LCD incepand de la adresa primita.

#endif // LCD_H_

