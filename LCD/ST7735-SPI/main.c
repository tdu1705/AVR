#include <avr/io.h>
#include <util/delay.h>
#include "ST7735.c"
#include "ST7735.h"

int main()
{

	ST7735_Init();
	ST7735_LcdClear(BLACK);

	_delay_ms(500);
	ST7735_pionowo();
	ST7735_text( 10 , 10 , "Witaj" , 1 , BLACK , WHITE );

}
