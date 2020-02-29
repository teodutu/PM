#include "lcd.h"

// Initializare LCD considerand o interfatare cu 4 pini de date.
// Trebuie apelata inainte de a face orice operatie cu LCD-ul.
void LCD_init(void)
{
    // setam pinii de date ca pini de iesire
    LcdDATA_DDR |= LCD_DATA_MASK;
    // setam pinii de comenzi ca pini de iesire
    LcdCTRL_DDR |= LCD_CTRL_MASK;

    // intram in modul comenzi: RS - low, RW - low
    LCD_RS_LOW();
    LCD_RW_LOW();

    // asteptam timpul de set-up pentru RS si RW
    LCD_DELAY();

    // initializarea pentru 4 fire de date necesita transferarea lui 0100 -> D4-D7

    // E pe high incepe transferul
    LCD_ENABLE();

    // scriem cei mai semnificativi 4 biti pe firele de date, fara a modifica alti biti
    LcdDATA_PORT = (LcdDATA_PORT & ~LCD_DATA_MASK) |
                   (0 << LcdD7) |
                   (0 << LcdD6) |
                   (1 << LcdD5) |
                   (0 << LcdD4);

    // asteptam timpul minim de set-up
    LCD_DELAY();
    // E pe low termina transferul
    LCD_DISABLE();
    // asteptam timpul minim pana la urmatorul E (contine timpul de hold)
    LCD_DELAY();

    // transmitem configuratia initiala
    // 4 biti de date, 2 linii, font 8x5
    LCD_writeInstr(LCD_INSTR_4wire);
    // display pornit, cursor afisat, blink activat
    LCD_writeInstr(LCD_INSTR_displayConfig);
    // auto-increment, fara shiftare
    LCD_writeInstr(LCD_INSTR_incNoShift);
    // sterge display
    LCD_writeInstr(LCD_INSTR_clearDisplay);
}

// Executa secventa de citire a unui octet de date de la LCD.
// Pentru interfatarea cu 4 pini de date sunt necesare 2 transferuri.
uint8_t LCD_read(void)
{
    // setam pinii de date ca intrari pentru a putea primit date de la LCD
    LcdDATA_DDR &= ~LCD_DATA_MASK;
    // dezactivam rezistentele de pull-up pentru pinii de date
    LcdDATA_PORT &= ~LCD_DATA_MASK;

    // stocheaza valoarea care va fi citita
    uint8_t data = 0;

    // transferul primilor 4 biti de la LCD

    LCD_ENABLE();
    // asteptam timpul minim de set-up
    LCD_DELAY();
    // citim de pe pinii de date cei mai semnificativi 4 biti
    data |= (((LcdDATA_PIN >> LcdD7) & 0x01) << 7) |
            (((LcdDATA_PIN >> LcdD6) & 0x01) << 6) |
            (((LcdDATA_PIN >> LcdD5) & 0x01) << 5) |
            (((LcdDATA_PIN >> LcdD4) & 0x01) << 4);
    LCD_DISABLE();
    // asteptam timpul minim pana la urmatorul E
    LCD_DELAY();

    // transferul ultimilor 4 biti de la LCD

    LCD_ENABLE();
    // asteptam timpul minim de set-up
    LCD_DELAY();
    // citim de pe pinii de date cei mai putin semnificativi 4 biti
    data |= (((LcdDATA_PIN >> LcdD7) & 0x01) << 3) |
            (((LcdDATA_PIN >> LcdD6) & 0x01) << 2) |
            (((LcdDATA_PIN >> LcdD5) & 0x01) << 1) |
            (((LcdDATA_PIN >> LcdD4) & 0x01) << 0);
    LCD_DISABLE();
    // asteptam timpul minim pana la urmatorul E
    LCD_DELAY();

    // setam pinii de date inapoi ca iesiri
    LcdDATA_DDR |= LCD_DATA_MASK;

    // returnam octetul citit
    return data;
}

// Citeste starea LCD-ului (contine busy flag).
uint8_t LCD_readStatus(void)
{
    // intram in modul citire stare (busy flag + addresa curenta)
    LCD_RS_LOW();
    LCD_RW_HIGH();

    // asteptam timpul de set-up pentru RS si RW
    LCD_DELAY();

    // citim si returnam datele
    return LCD_read();
}

// Citeste un octet din ultima memorie folosita (DDRAM sau CGRAM).
uint8_t LCD_readData(void)
{
    // intram in modul citire memorie
    LCD_RS_HIGH();
    LCD_RW_HIGH();

    // asteptam timpul de set-up pentru RS si RW
    LCD_DELAY();

    // citim si returnam datele
    return LCD_read();
}

// Returneaza starea LCD-ului: 1 - busy, 0 - available
uint8_t LCD_isBusy(void)
{
    // busy flag este bitul 7: 1 -> busy, 0 -> available
    return (LCD_readStatus() & (1 << 7)) != 0;
}

// Asteapta pana cand LCD-ul devine disponibil pentru o noua comanda.
void LCD_waitNotBusy(void)
{
    while(LCD_isBusy());
}

// Executa secventa de trimitere a unui octet de date catre LCD.
// Pentru interfatarea cu 4 pini de date sunt necesare 2 transferuri.
void LCD_write(uint8_t data)
{
    // transferul primilor 4 biti catre LCD

    LCD_ENABLE();
    // scriem pe pinii de date cei mai semnificativi 4 biti, fara a modifica alti biti in registru
    LcdDATA_PORT = (LcdDATA_PORT & ~LCD_DATA_MASK) |
                   (((data >> 7) & 0x01) << LcdD7) |
                   (((data >> 6) & 0x01) << LcdD6) |
                   (((data >> 5) & 0x01) << LcdD5) |
                   (((data >> 4) & 0x01) << LcdD4);
    // asteptam timpul minim de set-up
    LCD_DELAY();
    LCD_DISABLE();
    // asteptam timpul minim pana la urmatorul E (contine timpul de hold)
    LCD_DELAY();

    // transferul ultimilor 4 biti catre LCD

    LCD_ENABLE();
    // scriem pe pinii de date cei mai putin semnificativi 4 biti, fara a modifica alti biti in registru
    LcdDATA_PORT = (LcdDATA_PORT & ~LCD_DATA_MASK) |
                   (((data >> 3) & 0x01) << LcdD7) |
                   (((data >> 2) & 0x01) << LcdD6) |
                   (((data >> 1) & 0x01) << LcdD5) |
                   (((data >> 0) & 0x01) << LcdD4);
    // asteptam timpul minim de set-up
    LCD_DELAY();
    LCD_DISABLE();
    // asteptam timpul minim pana la urmatorul E (contine timpul de hold)
    LCD_DELAY();
}

// Trimite o instructiune de control catre LCD.
void LCD_writeInstr(uint8_t instr)
{
    // asteptam ca LCD-ul sa poata accepta instructiuni
    LCD_waitNotBusy();

    // intram in modul comenzi: RS - low, RW - low
    LCD_RS_LOW();
    LCD_RW_LOW();

    // asteptam timpul de set-up pentru RS si RW
    LCD_DELAY();

    // trimitem datele
    LCD_write(instr);
}

// Trimite o instructiune de scriere date catre LCD.
void LCD_writeData(uint8_t data)
{
    // asteptam ca LCD-ul sa poata accepta instructiuni
    LCD_waitNotBusy();

    // intram in modul scriere date
    LCD_RS_HIGH();
    LCD_RW_LOW();

    // asteptam timpul de set-up pentru RS si RW
    LCD_DELAY();

    // trimitem datele
    LCD_write(data);
}

// Afiseaza caracterul pe LCD la adresa curenta.
void LCD_putChar(char c)
{
    LCD_writeData(c);
}

// Afiseaza un caracter pe LCD la adresa primita.
void LCD_putCharAt(uint8_t addr, char c)
{
    /* TODO task 1 LCD */
    LCD_writeInstr(LCD_INSTR_DDRAM + addr);
    LCD_putChar(c);
}

// Afiseaza string-ul pe LCD incepand de la adresa curenta.
void LCD_print(const char* msg)
{
    while(*msg)
        LCD_putChar(*msg++);
}

// Afiseaza string-ul pe LCD incepand de la adresa primita.
void LCD_printAt(uint8_t addr, const char* msg)
{
    /* TODO task 1 LCD */
    // initial LCD_INSTR_DDRAM | addr !!
    LCD_writeInstr(LCD_INSTR_DDRAM + addr);
    LCD_print(msg);
}

