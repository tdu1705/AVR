/*
 * ST7735.c
 *
 *  Created on: 3 lut 2016
 *      Author: GufiM
 */


#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "ST7735.h"

int cursor_x;
int cursor_y;

const uint8_t font[] PROGMEM =
{
    0x00, 0x00, 0x00, 0x00, 0x00,
	0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
	0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
	0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
	0x18, 0x3C, 0x7E, 0x3C, 0x18,
	0x1C, 0x57, 0x7D, 0x57, 0x1C,
	0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
	0x00, 0x18, 0x3C, 0x18, 0x00,
	0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
	0x00, 0x18, 0x24, 0x18, 0x00,
	0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
	0x30, 0x48, 0x3A, 0x06, 0x0E,
	0x26, 0x29, 0x79, 0x29, 0x26,
	0x40, 0x7F, 0x05, 0x05, 0x07,
	0x40, 0x7F, 0x05, 0x25, 0x3F,
	0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
	0x7F, 0x3E, 0x1C, 0x1C, 0x08,
	0x08, 0x1C, 0x1C, 0x3E, 0x7F,
	0x14, 0x22, 0x7F, 0x22, 0x14,
	0x5F, 0x5F, 0x00, 0x5F, 0x5F,
	0x06, 0x09, 0x7F, 0x01, 0x7F,
	0x00, 0x66, 0x89, 0x95, 0x6A,
	0x60, 0x60, 0x60, 0x60, 0x60,
	0x94, 0xA2, 0xFF, 0xA2, 0x94,
	0x08, 0x04, 0x7E, 0x04, 0x08,
	0x10, 0x20, 0x7E, 0x20, 0x10,
	0x08, 0x08, 0x2A, 0x1C, 0x08,
	0x08, 0x1C, 0x2A, 0x08, 0x08,
	0x1E, 0x10, 0x10, 0x10, 0x10,
	0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
	0x30, 0x38, 0x3E, 0x38, 0x30,
	0x06, 0x0E, 0x3E, 0x0E, 0x06,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x5F, 0x00, 0x00,
	0x00, 0x07, 0x00, 0x07, 0x00,
	0x14, 0x7F, 0x14, 0x7F, 0x14,
	0x24, 0x2A, 0x7F, 0x2A, 0x12,
	0x23, 0x13, 0x08, 0x64, 0x62,
	0x36, 0x49, 0x56, 0x20, 0x50,
	0x00, 0x08, 0x07, 0x03, 0x00,
	0x00, 0x1C, 0x22, 0x41, 0x00,
	0x00, 0x41, 0x22, 0x1C, 0x00,
	0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
	0x08, 0x08, 0x3E, 0x08, 0x08,
	0x00, 0x80, 0x70, 0x30, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x60, 0x60, 0x00,
	0x20, 0x10, 0x08, 0x04, 0x02,
	0x3E, 0x51, 0x49, 0x45, 0x3E,
	0x00, 0x42, 0x7F, 0x40, 0x00,
	0x72, 0x49, 0x49, 0x49, 0x46,
	0x21, 0x41, 0x49, 0x4D, 0x33,
	0x18, 0x14, 0x12, 0x7F, 0x10,
	0x27, 0x45, 0x45, 0x45, 0x39,
	0x3C, 0x4A, 0x49, 0x49, 0x31,
	0x41, 0x21, 0x11, 0x09, 0x07,
	0x36, 0x49, 0x49, 0x49, 0x36,
	0x46, 0x49, 0x49, 0x29, 0x1E,
	0x00, 0x00, 0x14, 0x00, 0x00,
	0x00, 0x40, 0x34, 0x00, 0x00,
	0x00, 0x08, 0x14, 0x22, 0x41,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x00, 0x41, 0x22, 0x14, 0x08,
	0x02, 0x01, 0x59, 0x09, 0x06,
	0x3E, 0x41, 0x5D, 0x59, 0x4E,
	0x7C, 0x12, 0x11, 0x12, 0x7C,
	0x7F, 0x49, 0x49, 0x49, 0x36,
	0x3E, 0x41, 0x41, 0x41, 0x22,
	0x7F, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x49, 0x49, 0x49, 0x41,
	0x7F, 0x09, 0x09, 0x09, 0x01,
	0x3E, 0x41, 0x41, 0x51, 0x73,
	0x7F, 0x08, 0x08, 0x08, 0x7F,
	0x00, 0x41, 0x7F, 0x41, 0x00,
	0x20, 0x40, 0x41, 0x3F, 0x01,
	0x7F, 0x08, 0x14, 0x22, 0x41,
	0x7F, 0x40, 0x40, 0x40, 0x40,
	0x7F, 0x02, 0x1C, 0x02, 0x7F,
	0x7F, 0x04, 0x08, 0x10, 0x7F,
	0x3E, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x09, 0x09, 0x09, 0x06,
	0x3E, 0x41, 0x51, 0x21, 0x5E,
	0x7F, 0x09, 0x19, 0x29, 0x46,
	0x26, 0x49, 0x49, 0x49, 0x32,
	0x03, 0x01, 0x7F, 0x01, 0x03,
	0x3F, 0x40, 0x40, 0x40, 0x3F,
	0x1F, 0x20, 0x40, 0x20, 0x1F,
	0x3F, 0x40, 0x38, 0x40, 0x3F,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x03, 0x04, 0x78, 0x04, 0x03,
	0x61, 0x59, 0x49, 0x4D, 0x43,
	0x00, 0x7F, 0x41, 0x41, 0x41,
	0x02, 0x04, 0x08, 0x10, 0x20,
	0x00, 0x41, 0x41, 0x41, 0x7F,
	0x04, 0x02, 0x01, 0x02, 0x04,
	0x40, 0x40, 0x40, 0x40, 0x40,
	0x00, 0x03, 0x07, 0x08, 0x00,
	0x20, 0x54, 0x54, 0x78, 0x40,
	0x7F, 0x28, 0x44, 0x44, 0x38,
	0x38, 0x44, 0x44, 0x44, 0x28,
	0x38, 0x44, 0x44, 0x28, 0x7F,
	0x38, 0x54, 0x54, 0x54, 0x18,
	0x00, 0x08, 0x7E, 0x09, 0x02,
	0x18, 0xA4, 0xA4, 0x9C, 0x78,
	0x7F, 0x08, 0x04, 0x04, 0x78,
	0x00, 0x44, 0x7D, 0x40, 0x00,
	0x20, 0x40, 0x40, 0x3D, 0x00,
	0x7F, 0x10, 0x28, 0x44, 0x00,
	0x00, 0x41, 0x7F, 0x40, 0x00,
	0x7C, 0x04, 0x78, 0x04, 0x78,
	0x7C, 0x08, 0x04, 0x04, 0x78,
	0x38, 0x44, 0x44, 0x44, 0x38,
	0xFC, 0x18, 0x24, 0x24, 0x18,
	0x18, 0x24, 0x24, 0x18, 0xFC,
	0x7C, 0x08, 0x04, 0x04, 0x08,
	0x48, 0x54, 0x54, 0x54, 0x24,
	0x04, 0x04, 0x3F, 0x44, 0x24,
	0x3C, 0x40, 0x40, 0x20, 0x7C,
	0x1C, 0x20, 0x40, 0x20, 0x1C,
	0x3C, 0x40, 0x30, 0x40, 0x3C,
	0x44, 0x28, 0x10, 0x28, 0x44,
	0x4C, 0x90, 0x90, 0x90, 0x7C,
	0x44, 0x64, 0x54, 0x4C, 0x44,
	0x00, 0x08, 0x36, 0x41, 0x00,
	0x00, 0x00, 0x77, 0x00, 0x00,
	0x00, 0x41, 0x36, 0x08, 0x00,
	0x02, 0x01, 0x02, 0x04, 0x02,
	0x3C, 0x26, 0x23, 0x26, 0x3C,
	0x1E, 0xA1, 0xA1, 0x61, 0x12,
	0x3A, 0x40, 0x40, 0x20, 0x7A,
	0x38, 0x54, 0x54, 0x55, 0x59,
	0x21, 0x55, 0x55, 0x79, 0x41,
	0x21, 0x54, 0x54, 0x78, 0x41,
	0x21, 0x55, 0x54, 0x78, 0x40,
	0x20, 0x54, 0x55, 0x79, 0x40,
	0x0C, 0x1E, 0x52, 0x72, 0x12,
	0x39, 0x55, 0x55, 0x55, 0x59,
	0x39, 0x54, 0x54, 0x54, 0x59,
	0x39, 0x55, 0x54, 0x54, 0x58,
	0x00, 0x00, 0x45, 0x7C, 0x41,
	0x00, 0x02, 0x45, 0x7D, 0x42,
	0x00, 0x01, 0x45, 0x7C, 0x40,
	0xF0, 0x29, 0x24, 0x29, 0xF0,
	0xF0, 0x28, 0x25, 0x28, 0xF0,
	0x7C, 0x54, 0x55, 0x45, 0x00,
	0x20, 0x54, 0x54, 0x7C, 0x54,
	0x7C, 0x0A, 0x09, 0x7F, 0x49,
	0x32, 0x49, 0x49, 0x49, 0x32,
	0x32, 0x48, 0x48, 0x48, 0x32,
	0x32, 0x4A, 0x48, 0x48, 0x30,
	0x3A, 0x41, 0x41, 0x21, 0x7A,
	0x3A, 0x42, 0x40, 0x20, 0x78,
	0x00, 0x9D, 0xA0, 0xA0, 0x7D,
	0x39, 0x44, 0x44, 0x44, 0x39,
	0x3D, 0x40, 0x40, 0x40, 0x3D,
	0x3C, 0x24, 0xFF, 0x24, 0x24,
	0x48, 0x7E, 0x49, 0x43, 0x66,
	0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
	0xFF, 0x09, 0x29, 0xF6, 0x20,
	0xC0, 0x88, 0x7E, 0x09, 0x03,
	0x20, 0x54, 0x54, 0x79, 0x41,
	0x00, 0x00, 0x44, 0x7D, 0x41,
	0x30, 0x48, 0x48, 0x4A, 0x32,
	0x38, 0x40, 0x40, 0x22, 0x7A,
	0x00, 0x7A, 0x0A, 0x0A, 0x72,
	0x7D, 0x0D, 0x19, 0x31, 0x7D,
	0x26, 0x29, 0x29, 0x2F, 0x28,
	0x26, 0x29, 0x29, 0x29, 0x26,
	0x30, 0x48, 0x4D, 0x40, 0x20,
	0x38, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x38,
	0x2F, 0x10, 0xC8, 0xAC, 0xBA,
	0x2F, 0x10, 0x28, 0x34, 0xFA,
	0x00, 0x00, 0x7B, 0x00, 0x00,
	0x08, 0x14, 0x2A, 0x14, 0x22,
	0x22, 0x14, 0x2A, 0x14, 0x08,
	0xAA, 0x00, 0x55, 0x00, 0xAA,
	0xAA, 0x55, 0xAA, 0x55, 0xAA,
	0x00, 0x00, 0x00, 0xFF, 0x00,
	0x10, 0x10, 0x10, 0xFF, 0x00,
	0x14, 0x14, 0x14, 0xFF, 0x00,
	0x10, 0x10, 0xFF, 0x00, 0xFF,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x14, 0x14, 0x14, 0xFC, 0x00,
	0x14, 0x14, 0xF7, 0x00, 0xFF,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x14, 0x14, 0xF4, 0x04, 0xFC,
	0x14, 0x14, 0x17, 0x10, 0x1F,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0x1F, 0x00,
	0x10, 0x10, 0x10, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0x1F, 0x10,
	0x10, 0x10, 0x10, 0xF0, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x10,
	0x10, 0x10, 0x10, 0x10, 0x10,
	0x10, 0x10, 0x10, 0xFF, 0x10,
	0x00, 0x00, 0x00, 0xFF, 0x14,
	0x00, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0x00, 0x1F, 0x10, 0x17,
	0x00, 0x00, 0xFC, 0x04, 0xF4,
	0x14, 0x14, 0x17, 0x10, 0x17,
	0x14, 0x14, 0xF4, 0x04, 0xF4,
	0x00, 0x00, 0xFF, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x14, 0x14, 0xF7, 0x00, 0xF7,
	0x14, 0x14, 0x14, 0x17, 0x14,
	0x10, 0x10, 0x1F, 0x10, 0x1F,
	0x14, 0x14, 0x14, 0xF4, 0x14,
	0x10, 0x10, 0xF0, 0x10, 0xF0,
	0x00, 0x00, 0x1F, 0x10, 0x1F,
	0x00, 0x00, 0x00, 0x1F, 0x14,
	0x00, 0x00, 0x00, 0xFC, 0x14,
	0x00, 0x00, 0xF0, 0x10, 0xF0,
	0x10, 0x10, 0xFF, 0x10, 0xFF,
	0x14, 0x14, 0x14, 0xFF, 0x14,
	0x10, 0x10, 0x10, 0x1F, 0x00,
	0x00, 0x00, 0x00, 0xF0, 0x10,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
	0xFF, 0xFF, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFF, 0xFF,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0x38, 0x44, 0x44, 0x38, 0x44,
	0x7C, 0x2A, 0x2A, 0x3E, 0x14,
	0x7E, 0x02, 0x02, 0x06, 0x06,
	0x02, 0x7E, 0x02, 0x7E, 0x02,
	0x63, 0x55, 0x49, 0x41, 0x63,
	0x38, 0x44, 0x44, 0x3C, 0x04,
	0x40, 0x7E, 0x20, 0x1E, 0x20,
	0x06, 0x02, 0x7E, 0x02, 0x02,
	0x99, 0xA5, 0xE7, 0xA5, 0x99,
	0x1C, 0x2A, 0x49, 0x2A, 0x1C,
	0x4C, 0x72, 0x01, 0x72, 0x4C,
	0x30, 0x4A, 0x4D, 0x4D, 0x30,
	0x30, 0x48, 0x78, 0x48, 0x30,
	0xBC, 0x62, 0x5A, 0x46, 0x3D,
	0x3E, 0x49, 0x49, 0x49, 0x00,
	0x7E, 0x01, 0x01, 0x01, 0x7E,
	0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
	0x44, 0x44, 0x5F, 0x44, 0x44,
	0x40, 0x51, 0x4A, 0x44, 0x40,
	0x40, 0x44, 0x4A, 0x51, 0x40,
	0x00, 0x00, 0xFF, 0x01, 0x03,
	0xE0, 0x80, 0xFF, 0x00, 0x00,
	0x08, 0x08, 0x6B, 0x6B, 0x08,
	0x36, 0x12, 0x36, 0x24, 0x36,
	0x06, 0x0F, 0x09, 0x0F, 0x06,
	0x00, 0x00, 0x18, 0x18, 0x00,
	0x00, 0x00, 0x10, 0x10, 0x00,
	0x30, 0x40, 0xFF, 0x01, 0x01,
	0x00, 0x1F, 0x01, 0x01, 0x1E,
	0x00, 0x19, 0x1D, 0x17, 0x12,
	0x00, 0x3C, 0x3C, 0x3C, 0x3C,
	0x00, 0x00, 0x00, 0x00, 0x00
};

// *************************************************
void ST7735_Write(uint8_t Data)
{
	CLR_CS;
	SET_RS;

	SPDR = Data;
	while(!(SPSR & (1<<SPIF)));

	SET_CS;
}

void ST7735_WriteCommand(uint8_t Command)
{
	CLR_CS;
	CLR_RS;

	SPDR = Command;
	while(!(SPSR & (1<<SPIF)));

	SET_CS;
}

// *************************************************
void ST7735_SetAddressWindow(int x0, int y0, int x1, int y1)
{
  ST7735_WriteCommand(CASET);	// Ustawianie adresu kolumn
  ST7735_Write(0x00);
  ST7735_Write(x0);
  ST7735_Write(0x00);
  ST7735_Write(x1);

  ST7735_WriteCommand(RASET);	// Ustawienie adresu wierszy
  ST7735_Write(0x00);
  ST7735_Write(y0);
  ST7735_Write(0x00);
  ST7735_Write(y1);

  ST7735_WriteCommand(RAMWR);	// Zapis do RAM
}

// ************* ST7735 Init **********************************
void ST7735_DispInit(void)
{
	ST7735_WriteCommand(SPLOUT);	// Wybudz wyswietlacz
	_delay_ms(100);

	ST7735_WriteCommand(FRMCTR1);	// Frame Rate Control
	ST7735_Write(0x2C);
	ST7735_Write(0x2D);

	ST7735_WriteCommand(FRMCTR2);
	ST7735_Write(0x2C);
	ST7735_Write(0x2D);
	ST7735_Write(0xB3);
	ST7735_Write(0x01);
	ST7735_Write(0x2C);
	ST7735_Write(0x2D);
	ST7735_Write(0x01);
	ST7735_Write(0x2C);
	ST7735_Write(0x2D);

	ST7735_WriteCommand(INVCTR);	// Display Inversion Control
	ST7735_Write(0x07);

	ST7735_WriteCommand(PWCTR0);	// Power Control 1
	ST7735_Write(0xA2);
	ST7735_Write(0x02);
	ST7735_Write(0x84);

	ST7735_WriteCommand(PWCTR1);	// Power Control 2
	ST7735_Write(0xC5);

	ST7735_WriteCommand(PWCTR2);	// Power Control 3
	ST7735_Write(0x0A);
	ST7735_Write(0x00);

	ST7735_WriteCommand(PWCTR3);	// Power Control 4
	ST7735_Write(0x8A);
	ST7735_Write(0x2A);

	ST7735_WriteCommand(PWCTR4);	// Power Control 5
	ST7735_Write(0x8A);
	ST7735_Write(0xEE);

	ST7735_WriteCommand(VMCTR1);	// VCOM Control
	ST7735_Write(0x0E);

	ST7735_WriteCommand(MADCTL);   // Memory Acces Data Control
#if USE_PION == 1
		ST7735_Write(0x08);		   // PIONOWO
#endif
#if USE_PION == 0
		ST7735_Write(0x68);        // Poziomi
#endif

// ************** Gamma Control *********************
	ST7735_WriteCommand(GMCTRP1);
	ST7735_Write(0x0f);
	ST7735_Write(0x1a);
	ST7735_Write(0x0f);
	ST7735_Write(0x18);
	ST7735_Write(0x2f);
	ST7735_Write(0x28);
	ST7735_Write(0x20);
	ST7735_Write(0x22);
	ST7735_Write(0x1f);
	ST7735_Write(0x1b);
	ST7735_Write(0x23);
	ST7735_Write(0x37);
	ST7735_Write(0x00);
	ST7735_Write(0x07);
	ST7735_Write(0x02);
	ST7735_Write(0x10);

	ST7735_WriteCommand(GMCTRN1);
	ST7735_Write(0x0f);
	ST7735_Write(0x1b);
	ST7735_Write(0x0f);
	ST7735_Write(0x17);
	ST7735_Write(0x33);
	ST7735_Write(0x2c);
	ST7735_Write(0x29);
	ST7735_Write(0x2e);
	ST7735_Write(0x30);
	ST7735_Write(0x30);
	ST7735_Write(0x39);
	ST7735_Write(0x3f);
	ST7735_Write(0x00);
	ST7735_Write(0x07);
	ST7735_Write(0x03);
	ST7735_Write(0x10);

	ST7735_WriteCommand(COLMOD);	// Interface Pixel Format

	ST7735_Write(0x05);	            // 16bit/pixel

	ST7735_WriteCommand(DISPON);	// Wlacz wyswietlacz
}
void ST7735_Init(void)
{
	DDR(ST7735_RS_PORT) |= (1<<ST7735_RS_PIN);
	DDR(ST7735_RES_PORT) |= (1<<ST7735_RES_PIN);

	DDR(ST7735_CS_PORT) |= (1<<ST7735_CS_PIN);
	DDR(ST7735_MOSI_PORT) |= (1<<ST7735_MOSI_PIN);
	DDR(ST7735_SCK_PORT) |= (1<<ST7735_SCK_PIN);

// ********** SPI init ***************************
	SPCR = (1<<SPE)|(1<<MSTR);
	SPSR |=  (1<<SPI2X);


// ********* Reset wyswietlacza ***********
	SET_RES;
	_delay_ms(100);
	CLR_RES;
	_delay_ms(100);
	SET_RES;
	_delay_ms(100);

// **** Init sterownika ****************
	ST7735_DispInit();
}

// ********** Czyszczenie LCD ******************
void ST7735_LcdClear(uint16_t Color)
{

	int x;
	int y;

	ST7735_SetAddressWindow(0, 0, LCD_X - 1, LCD_Y - 1);

	for (x=0; x < LCD_X; x++) {
		for (y=0; y < LCD_Y; y++)
		{
			ST7735_Write(Color >> 8);
			ST7735_Write(Color);

		}
	}

}

void ST7735_SetPixel(int x, int y, uint16_t Color)
{
	ST7735_SetAddressWindow(x,y,x+1,y+1);

	ST7735_Write(Color >> 8);
	ST7735_Write(Color);

}


void ST7735_drawChar( int x , int y , char c , uint16_t color , uint16_t bg , uint8_t size )
{
	if(( x >= LCD_X ) || ( y >= LCD_Y ) || (( x + 6 * size - 1 ) < 0 ) || (( y + 8 * size - 1 ) < 0 )) return;

	uint8_t line;

	for( uint8_t i = 0 ; i < 6 ; i++ )
	{
		if( i == 5 )
		{
			line = 0x0;
		}
		else
		{
			line = pgm_read_byte( font + ( c * 5 ) + i );
		}
		for( uint8_t j = 0 ; j < 8 ; j++ )
		{
			if( line & 0x1 )
			{
				if( size == 1 )
				{
					ST7735_SetPixel( x + i , y + j , color );
				}
				else
				{
					ST7735_fillRect( x + ( i * size ) , y + ( j * size ) , size , size , color );
				}
			}
			else if( bg != color )
			{
				if( size == 1 )
				{
					ST7735_SetPixel( x + i , y + j , bg );
				}
		 		else
				{
		 			ST7735_fillRect( x + ( i * size ) , y + ( j * size ) , size , size , bg );
				}
			}
			line >>= 1;
		}
	}
}


void ST7735_fillRect( int x , int y , int w , int h , uint16_t color)
{
	for( uint16_t i = x ; i < x + w ; i++ )
	{
		ST7735_fastLine( i , y , h , color);
	}
}


void ST7735_fastLine( int x , int y , int h , uint16_t color)
{
	ST7735_drawLine( x , y , x , y + h - 1 , color );
}


void ST7735_fastLineH( int x , int y , int w , uint16_t color)
{
	ST7735_drawLine( x , y , x + w - 1 , y  , color );
}


void ST7735_drawLine( int x0 , int y0 , int x1 , int y1 , uint16_t color )
{
	  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
	  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
	  int err = (dx>dy ? dx : -dy)/2, e2;

	  for(;;){
		  ST7735_SetPixel(x0,y0,color);
	    if (x0==x1 && y0==y1) break;
	    e2 = err;
	    if (e2 >-dx) { err -= dy; x0 += sx; }
	    if (e2 < dy) { err += dx; y0 += sy; }
	  }
}

void ST7735_text( int x , int y , char * str , uint8_t txt_size , uint16_t color , uint16_t bg )
{
	cursor_x = x;
	cursor_y = y;

	while( *str)
	{
		ST7735_drawChar( cursor_x , cursor_y , *str++ , color , bg , txt_size );
		cursor_x += txt_size * 6;
	}
}

void ST7735_drawInt( int x , int y , int data , uint8_t txt_size , uint16_t color , uint16_t bg )
{
	char buf[7];
	ST7735_text( x , y , itoa( data , buf , 10 ) , txt_size , color , bg );
}

void ST7735_pionowo(void)
{
	ST7735_WriteCommand(MADCTL);
	ST7735_Write(0x08);
}

void ST7735_poziomo(void)
{
	ST7735_WriteCommand(MADCTL);
	ST7735_Write(0x68);
}


#if USE_ST7735_ulint == 1
void ST7735_ulint( int x , int y , uint32_t data , uint8_t txt_size , uint16_t color , uint8_t bg )
{
	char buf[16];
	ST7735_text( x , y , ultoa( data , buf , 10 ) , txt_size , color , bg );
}
#endif

#if USE_ST7735_circle == 1
void ST7735_circle( int x , int y , uint8_t r , uint16_t color )
{
	int f = 1 - r;
	int ddf_x = 1;
	int ddf_y = -2 * r;
	int xa = 0;
	int ya = r;

	ST7735_SetPixel( x , y + r , color );
	ST7735_SetPixel( x , y - r , color );
	ST7735_SetPixel( x + r , y , color );
	ST7735_SetPixel( x - r , y , color );

	while( xa < ya )
	{
		if( f >= 0 )
		{
			ya--;
			ddf_y += 2;
			f += ddf_y;
		}
		xa++;
		ddf_x += 2;
		f += ddf_x;

		ST7735_SetPixel( x + xa , y + ya , color );
		ST7735_SetPixel( x - xa , y + ya , color );
		ST7735_SetPixel( x + xa , y - ya , color );
		ST7735_SetPixel( x - xa , y - ya , color );
		ST7735_SetPixel( x + ya , y + xa , color );
		ST7735_SetPixel( x - ya , y + xa , color );
		ST7735_SetPixel( x + ya , y - xa , color );
		ST7735_SetPixel( x - ya , y - xa , color );
	}

}
#endif

#if USE_ST7735_fillcircle == 1
void ST7735_fillcircle( int x , int y , uint8_t r , uint16_t color )
{
	ST7735_fastLine( x , y - r , 2 * r + 1 , color);
	ST7735_fillcirclehelper( x , y , r , 3 , 0 , color );
}
#endif

void ST7735_fillcirclehelper( int x0 , int y0 , int r , uint8_t cornername , int delta , uint16_t color )
{
	int f = 1 - r;
	int ddf_x = 1;
	int ddf_y = -2 * r;
	int x = 0;
	int y = r;

	while( x < y )
	{
		if( f >= 0 )
		{
			y--;
			ddf_y += 2;
			f += ddf_y;
		}
		x++;
		ddf_x += 2;
		f += ddf_x;

		if( cornername & 0x1 )
		{
			ST7735_fastLine( x0 + x , y0 - y , 2 * y + 1 + delta , color);
			ST7735_fastLine( x0 + y , y0 - x , 2 * x + 1 + delta , color);
		}
		if( cornername & 0x2 )
		{
			ST7735_fastLine( x0 - x , y0 - y , 2 * y + 1 + delta , color);
			ST7735_fastLine( x0 - y , y0 - x , 2 * x + 1 + delta , color);
		}

	}
}

void ST7735_drawcirclehelper( int x0 , int y0 , int r , uint8_t cornername , uint16_t color )
{
	int f = 1 - r;
	int ddf_x = 1;
	int ddf_y = -2 * r;
	int x = 0;
	int y = r;

	while( x < y )
	{
		if( f >= 0 )
		{
			y--;
			ddf_y += 2;
			f += ddf_y;
		}
		x++;
		ddf_x += 2;
		f += ddf_x;

		if( cornername & 0x4 )
		{
			ST7735_SetPixel( x0 + x , y0 + y , color);
			ST7735_SetPixel( x0 + y , y0 + x , color);
		}
		if( cornername & 0x2 )
		{
			ST7735_SetPixel( x0 + x , y0 - y , color);
			ST7735_SetPixel( x0 + y , y0 - x , color);
		}
		if( cornername & 0x8 )
		{
			ST7735_SetPixel( x0 - x , y0 + y , color);
			ST7735_SetPixel( x0 - y , y0 + x , color);
		}
		if( cornername & 0x1 )
		{
			ST7735_SetPixel( x0 - x , y0 - y , color);
			ST7735_SetPixel( x0 - y , y0 - x , color);
		}

	}
}


#if USE_ST7735_drawRect == 1
void ssd1306_drawRect( int x , int y , int w , int h , uint16_t color )
{
	ST7735_fastLineH( x , y , w , color);
	ST7735_fastLineH( x , y + h - 1 , w , color);
	ST7735_fastLine( x , y , h , color);
	ST7735_fastLine( x + w - 1 , y , h , color);
}
#endif


#if USE_ST7735_drawRoundRect == 1
void ST7735_drawRoundRect( int x , int y , int w , int h , uint8_t r , uint16_t color )
{
	ST7735_fastLineH( x + r , y , w - 2 * r , color);
	ST7735_fastLineH( x + r , y + h - 1 , w - 2 * r , color);
	ST7735_fastLine( x , y + r , h - 2 * r , color);
	ST7735_fastLine( x + w - 1 , y + r , h - 2 * r , color);

	ST7735_drawcirclehelper( x + r , y + r , r , 1 , color );
	ST7735_drawcirclehelper( x + w - r - 1 , y + r , r , 2 , color );
	ST7735_drawcirclehelper( x + w - r - 1 , y + h - r - 1 , r , 4 , color );
	ST7735_drawcirclehelper( x + r , y + h - r - 1 , r , 8 , color );
}
#endif


#if USE_ST7735_fillRoundRect == 1
void ST7735_fillRoundRect( int x , int y , int w , int h , uint8_t r , uint16_t color )
{
	ST7735_fillRect( x + r , y , w - 2 * r , h , color);

	ST7735_fillcirclehelper( x + w - r - 1 , y + r , r , 1 , h - 2 * r - 1 , color );
	ST7735_fillcirclehelper( x + r , y + r , r , 2 , h - 2 * r - 1 , color );
}
#endif


#if USE_ST7735_drawTriangle == 1
void ST7735_drawTriangle( int x0 , int y0 , int x1 , int y1 , int x2 , int y2 , uint16_t color )
{
	ST7735_drawLine( x0 , y0 , x1 , y1 , color );
	ST7735_drawLine( x1 , y1 , x2 , y2 , color );
	ST7735_drawLine( x2 , y2 , x0 , y0 , color );
}
#endif




