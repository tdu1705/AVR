/*
*
*	Author: Tomasz Durnik
*	Atmega32
*	LCD Nokia 5110
*	kwarc 11.0592MHz
*
*/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


#include "pcd8544.h"
#include "main.h"

#define LCD PORTC

int main()
{
	DDRC=0xFF;
	PORTC|=(1<<PC0);
	LcdInit();


    //display image
//    LcdContrast(0xFF);
    LcdClear();
//    LcdImage(waitImage);
    LcdUpdate();


    _delay_ms(2500);
//    LcdContrast(0x00);
//    LcdUpdate();
//    _delay_ms(2500);

    //display string
    LcdClear();
    LcdGotoXYFont(1,1);
    LcdFStr(FONT_1X,(unsigned char*)PSTR("Test"));
    LcdGotoXYFont(1,3);
    LcdFStr(FONT_1X,(unsigned char*)PSTR("Goto Test"));
    LcdUpdate();

    _delay_ms(5000);


    return 0;
}
