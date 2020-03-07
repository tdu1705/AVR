/*
 * ST7735.h
 *
 *  Created on: 3 lut 2016
 *      Author: GufiM
 */

#ifndef ST7735_H_
#define ST7735_H_

#include <inttypes.h>

// ******* Colory *******************************************************
#define BLACK         0x0000
#define BLUE          0x001F
#define RED           0xF800
#define GREEN         0x07E0
#define CYAN          0x07FF
#define MAGENTA       0xF81F
#define YELLOW        0xFFE0
#define WHITE         0xFFFF

// **********************************************************************
#define LCD_X 128
#define LCD_Y 128

// PIONOWO-1   POZIMO-0
#define USE_PION 1

// ******** Podlaczenie LCD **************************************

#define ST7735_RES_PIN		1   // Reset
#define ST7735_RES_PORT		B

#define ST7735_RS_PIN		0   // A0
#define ST7735_RS_PORT		B

#define ST7735_CS_PIN		2   // CS
#define ST7735_CS_PORT		B

#define ST7735_MOSI_PIN		5   // SDA
#define ST7735_MOSI_PORT	B

#define ST7735_SCK_PIN		7   // SCK
#define ST7735_SCK_PORT		B

// ******** Makra dla latwiejszej obslugi LCD ******************
#define CLR_RS      PORT(ST7735_RS_PORT) &= ~(1<<ST7735_RS_PIN);
#define SET_RS      PORT(ST7735_RS_PORT) |= (1<<ST7735_RS_PIN);

#define CLR_CS      PORT(ST7735_CS_PORT) &= ~(1<<ST7735_CS_PIN);
#define SET_CS      PORT(ST7735_CS_PORT) |= (1<<ST7735_CS_PIN);

#define CLR_RES     PORT(ST7735_RES_PORT) &= ~(1<<ST7735_RES_PIN);
#define SET_RES     PORT(ST7735_RES_PORT) |= (1<<ST7735_RES_PIN);

// *********** PORT ***********
#define PORT(x) SPORT(x)
#define SPORT(x) (PORT##x)
// ********** DDR *************
#define DDR(x) SDDR(x)
#define SDDR(x) (DDR##x)

#define FRMCTR1 0xB1
#define FRMCTR2 0xB2
#define INVCTR	0xB4
#define PWCTR0	0xC0
#define PWCTR1	0xC1
#define PWCTR2	0xC2
#define PWCTR3	0xC3
#define PWCTR4	0xC4
#define VMCTR1	0xC5
#define MADCTL	0x36
#define GMCTRP1	0xE0
#define GMCTRN1	0xE1
#define COLMOD	0x3A
#define DISPON	0x29
#define SPLOUT	0x11
#define CASET	0x2A
#define RASET	0x2B
#define RAMWR	0x2C
#define RESET	0x01

// *************** wlacz wylacz z kompilacji **************************************************
#define USE_ST7735_drawRect 	   0			//
#define USE_ST7735_drawRoundRect   0			//
#define USE_ST7735_fillRoundRect   0			//
#define USE_ST7735_drawTriangle    0			//
#define USE_ST7735_circle      	   1			//
#define USE_ST7735_fillcircle      0			//
#define USE_ST7735_ulint           1            //

// **************** deklaracje funkcji *************************************************************
void ST7735_Init(void);
void ST7735_Write(uint8_t Data);
void ST7735_WriteCommand(uint8_t Command);

void ST7735_LcdClear(uint16_t Color);
void ST7735_SetPixel(int x, int y, uint16_t Color);
void ST7735_pionowo(void);
void ST7735_poziomo(void);

void ST7735_drawChar( int x , int y , char c , uint16_t color , uint16_t bg , uint8_t size );
void ST7735_fillRect( int x , int y , int w , int h , uint16_t color);
void ST7735_fastLine( int x , int y , int h , uint16_t color);
void ST7735_fastLineH( int x , int y , int w , uint16_t color);
void ST7735_drawLine( int x0 , int y0 , int x1 , int y1 , uint16_t color );
void ST7735_text( int x , int y , char * str , uint8_t txt_size , uint16_t color , uint16_t bg );

void ST7735_drawInt( int x , int y , int data , uint8_t txt_size , uint16_t color , uint16_t bg );
void ST7735_ulint( int x , int y , uint32_t data , uint8_t txt_size , uint16_t color , uint8_t bg );

void ST7735_drawLine( int x0 , int y0 , int x1 , int y1 , uint16_t color );
void ST7735_fastLine( int x , int y , int h , uint16_t color);
void ST7735_fastLineH( int x , int y , int w , uint16_t color);
void ST7735_fillRect( int x , int y , int w , int h , uint16_t color);

void ST7735_drawRect( int x , int y , int w , int h , uint16_t color );
void ST7735_drawRoundRect( int x , int y , int w , int h , uint8_t r , uint16_t color );
void ST7735_fillRoundRect( int x , int y , int w , int h , uint8_t r , uint16_t color );
void ST7735_drawTriangle( int x0 , int y0 , int x1 , int y1 , int x2 , int y2 , uint16_t color );
void ST7735_circle( int x , int y , uint8_t r , uint16_t color );
void ST7735_fillcircle( int x , int y , uint8_t r , uint16_t color );

void ST7735_drawcirclehelper( int x0 , int y0 , int r , uint8_t cornername , uint16_t color );
void ST7735_fillcirclehelper( int x0 , int y0 , int r , uint8_t cornername , int delta , uint16_t color );

void ST7735SetAddressWindow(int x0, int y0, int x1, int y1);

#endif /* ST7735_H_ */
