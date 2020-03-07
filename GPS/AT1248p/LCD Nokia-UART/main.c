/*
 * at1284p_gps_uart2.c
 *
 * Author : Tomasz Durnik
 *      ATmega1284P
 *      GPS Neo 6m
 *      Lcd Nokia 5110
 *
 *      ATmega,Lcd zasilane z 3.3V
 *      Neo6m zasilany z 5V
 *      Uart w Neo6m na poziomie 3.3V
 *
 *
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "pcd8544.h"
#include "nokia.h"

#include "uart.c"

uint8_t dec2bcd(uint8_t dec);
uint8_t bcd2dec(uint8_t bcd);
void WriteInt(int variable);
void ParseGPS (char c);

// Example: $GPRMC,194509.000,A,4042.6142,N,07400.4168,W,2.03,5.84,160412,,,A*77
char fmt[]="$GPRMC,dddtdd.ds,A,eeee.eeeee,l,eeeee.eeeee,o,jdk,c,dddydx";

int state = 0;

unsigned int temp;
long ltmp;
volatile bool mode;

// Zmienne GPS
volatile unsigned int Time, Msecs,Csecs, Knots, Course, Date, Year;
volatile long Lat, Long;
volatile bool Fix;
uint32_t predkosc;
int godz,min,sek,mies,dzien;
char dlug,szer;	//N,S,W,E

int main (void)
{
	unsigned char c;
	uint8_t i=0;
    char buffer [100];
	
	DDRD |= (1<<PD7); //w³¹czenie pinu PD7 jako wyjœcie

	LcdInit();
	LcdClear();
	PORTD |= (1<<PD7); //wysterowanie 1 na wyjœcie PD7
	
	LcdGotoXYFont(3,2);
	LcdFStr(FONT_2X,(unsigned char*)PSTR("Dzien"));
	LcdGotoXYFont(3,5);
	LcdFStr(FONT_2X,(unsigned char*)PSTR("Dobry"));
	LcdUpdate();
	_delay_ms(2000);
	
	PORTD &= ~(1<<PD7); //wysterowanie 0 na wyjœcie PD7
	_delay_ms(2000);
	
	LcdClear();
	PORTD |= (1<<PD7);//wysterowanie 1 na wyjœcie PD7
	LcdGotoXYFont(3,2);
	LcdFStr(FONT_1X,(unsigned char*)PSTR("Zaczynamy!"));
	LcdUpdate();
	_delay_ms(2000);
	
	PORTD &= ~(1<<PD7); //wysterowanie 0 na wyjœcie PD7
	LcdClear();
	LcdUpdate();

	//inicjalizacja UART
	uart_init0(16);
	uart_init1(16);

    while(1){				//inicjalizacja GPS
		i=0;
		do{
			c = uart_receive0();
			if(c!='*'){
			ParseGPS(c);
				i++;

			}
		}

		while(c!='*');
		{

		if(mode==0)
		{
			PORTD ^= (1<<PD7);
		godz=Time/100;
		//Pobierany jest czas UTC
		//+1 czas zimowy
		//+2 czas letni
		godz=godz+2;

		min=Time%100;

		sek=Csecs/100;

		LcdClear();
		LcdGotoXYFont(1,1);
		LcdFStr(1,(unsigned char*)PSTR("Godz "));


			if(godz<10)
			{
				LcdGotoXYFont(6,1);
				LcdFStr(1,(unsigned char*)PSTR("0"));
				LcdStr(FONT_1X,(unsigned char*)(ltoa(godz,buffer,10)));
			}
			if(godz>=10)
			{
				LcdGotoXYFont(6,1);
				LcdStr(FONT_1X,(unsigned char*)(ltoa(godz,buffer,10)));
			}
			if(godz==24)
			{
				LcdGotoXYFont(6,1);
		//		LcdStr(FONT_1X,(unsigned char*)(ltoa(godz,buffer,10)));
				LcdFStr(1,(unsigned char*)PSTR("00"));
			}
				LcdGotoXYFont(8,1);
				LcdFStr(1,(unsigned char*)PSTR(":"));

			if(min<10)
			{
				LcdGotoXYFont(9,1);
				LcdFStr(1,(unsigned char*)PSTR("0"));
				LcdStr(FONT_1X,(unsigned char*)(ltoa(min,buffer,10)));
			}
			else
			{
				LcdGotoXYFont(9,1);
				LcdStr(FONT_1X,(unsigned char*)(ltoa(min,buffer,10)));
			}

			LcdGotoXYFont(11,1);
			LcdFStr(1,(unsigned char*)PSTR(":"));

			if(sek<10)
			{
				LcdGotoXYFont(12,1);
				LcdFStr(1,(unsigned char*)PSTR("0"));
				LcdStr(FONT_1X,(unsigned char*)(ltoa(sek,buffer,10)));
			}
			else
			{
				LcdGotoXYFont(12,1);
				LcdStr(FONT_1X,(unsigned char*)(ltoa(sek,buffer,10)));
			}

		LcdGotoXYFont(1,2);
		LcdFStr(1,(unsigned char*)PSTR("Data "));
		//ustawienie daty na wyswietlaczu przy godzinie 00:00, bo GPS pobiera czas UTC

		mies=Date%100;
		dzien=Date/100;

		if(godz==24)
		{
			Date=Date+100;
			LcdGotoXYFont(5,2);
			LcdStr(FONT_1X,(unsigned char*)(ltoa(Date/100,buffer,10)));

		}

		if(dzien<10)
		{
			LcdGotoXYFont(5,2);
			LcdFStr(1,(unsigned char*)PSTR("0"));
			LcdStr(FONT_1X,(unsigned char*)(ltoa(dzien,buffer,10)));
		}
		else
		{
		LcdGotoXYFont(5,2);
		LcdStr(FONT_1X,(unsigned char*)(ltoa(Date/100,buffer,10)));
		}
		LcdGotoXYFont(7,2);
		LcdFStr(1,(unsigned char*)PSTR("/"));
		
		if(mies<10)
		{
			LcdGotoXYFont(8,2);
			LcdFStr(1,(unsigned char*)PSTR("0"));
			LcdStr(FONT_1X,(unsigned char*)(ltoa(Date%100,buffer,10)));
		}
		else
		{
		LcdGotoXYFont(8,2);
		LcdStr(FONT_1X,(unsigned char*)(ltoa(Date%100,buffer,10)));
		}
		LcdGotoXYFont(10,2);
		LcdFStr(1,(unsigned char*)PSTR("/"));
		LcdGotoXYFont(11,2);
		LcdFStr(1,(unsigned char*)PSTR("20"));
		LcdGotoXYFont(13,2);
		LcdStr(FONT_1X,(unsigned char*)(ltoa(Year,buffer,10)));

		LcdGotoXYFont(4,3);
		LcdFStr(1,(unsigned char*)PSTR("Aktualne"));

		LcdGotoXYFont(2,4);
		LcdFStr(1,(unsigned char*)PSTR("Wspolrzedne:"));

		//Dlugosc geograficzna
		LcdGotoXYFont(2,5);
		LcdStr(FONT_1X,(unsigned char*)(ltoa(Lat/10000000,buffer,10)));
		LcdGotoXYFont(5,5);
		LcdStr(FONT_1X,(unsigned char*)(ltoa(Lat/100000%100,buffer,10)));
		LcdGotoXYFont(7,5);
		LcdFStr(1,(unsigned char*)PSTR("."));
		LcdGotoXYFont(8,5);
		LcdStr(FONT_1X,(unsigned char*)(ltoa(Lat%100000,buffer,10)));
		if(Lat>0)
		{
			LcdGotoXYFont(14,5);
			LcdFStr(1,(unsigned char*)PSTR("N"));
			dlug='N';
		}
		if(Lat<0)
		{
			LcdGotoXYFont(14,5);
			LcdFStr(1,(unsigned char*)PSTR("S"));
			dlug='S';
		}
		if(Lat==0)
		{
			LcdGotoXYFont(14,5);
			LcdFStr(1,(unsigned char*)PSTR("-"));
			dlug='-';
		}

		//Szerokosc geograficzna
		LcdGotoXYFont(2,6);
		LcdStr(FONT_1X,(unsigned char*)(ltoa(Long/10000000,buffer,10)));
		LcdGotoXYFont(5,6);
		LcdStr(FONT_1X,(unsigned char*)(ltoa(Long/100000%100,buffer,10)));
		LcdGotoXYFont(7,6);
		LcdFStr(1,(unsigned char*)PSTR("."));
		LcdGotoXYFont(8,6);
		LcdStr(FONT_1X,(unsigned char*)(ltoa(Long%100000,buffer,10)));
		if(Long>0)
		{
			LcdGotoXYFont(14,6);
			LcdFStr(1,(unsigned char*)PSTR("E"));
			szer='E';
		}
		if(Long<0)
		{
			LcdGotoXYFont(14,6);
			LcdFStr(1,(unsigned char*)PSTR("W"));
			szer='W';
		}
		if(Long==0)
		{
			LcdGotoXYFont(14,6);
			LcdFStr(1,(unsigned char*)PSTR("-"));
			szer='-';
		}

		/*LcdClear();
						LcdGotoXYFont(2,4);
						//LcdStr(FONT_1X,(unsigned char*)(ltoa(Course,buffer,10)));

						//LcdGotoXYFont(2,5);
						LcdStr(FONT_1X,(unsigned char*)(ltoa(Fix,buffer,10)));
						//LcdChr(FONT_1X,Fix);


						//predkosc wezly na kmh
						predkosc=Knots;
						predkosc=predkosc*1.852;
						LcdGotoXYFont(2,5);
						LcdStr(FONT_1X,(unsigned char*)(ltoa(Knots,buffer,10)));
						LcdGotoXYFont(2,6);
						LcdStr(FONT_1X,(unsigned char*)(itoa(predkosc,buffer,10)));*/

		uart_transmit1('\r');
		uart_transmit_s1("Czas:   ");
		if(godz<10)
		{
			uart_transmit_s1("0");
			uart_sendl(godz);
		}
		else
		{

			uart_sendl(godz);
		}
		uart_transmit1(':');
		if(min<10)
		{
			uart_transmit_s1("0");
			uart_sendl(min);
		}
		else
		{
			uart_sendl(min);
		}
		uart_transmit1(':');
		if(sek<10)
		{
			uart_transmit_s1("0");
			uart_sendl(sek);
		}
		else
		{
			uart_sendl(sek);
		}


		uart_transmit1('\n');

		uart_transmit_s1("\rData:   ");
		if(dzien<10)
		{
			uart_transmit_s1("0");
			uart_sendl(dzien);
		}
		else
		{

			uart_sendl(dzien);
		}
		uart_transmit1('/');
		if(mies<10)
		{
			uart_transmit_s1("0");
			uart_sendl(mies);
		}
		else
		{
			uart_sendl(mies);
		}
		uart_transmit1('/');
		uart_transmit_s1("20");
		uart_sendl(Year);
		uart_transmit1('\n');




		uart_transmit_s1("\rDlugosc geograficzna:    ");
		//uart_transmit1('\n');
		uart_sendl(Lat/10000000);
		uart_transmit_s1("  ");
		uart_sendl(Lat/100000%100);
		uart_transmit1('.');
		uart_sendl(Lat%100000);
		uart_transmit_s1("   ");
		uart_transmit1(dlug);
		uart_transmit1('\n');

		uart_transmit_s1("\rSzerokosc geograficzna:  ");
		//uart_transmit1('\n');
		uart_sendl(Long/10000000);
		uart_transmit_s1("  ");
		uart_sendl(Long/100000%100);
		uart_transmit1('.');
		uart_sendl(Long%100000);
		uart_transmit_s1("   ");
		uart_transmit1(szer);
		uart_transmit1('\n');

		uart_transmit_s1("\rFix:    ");
		//uart_transmit1('\n');
		uart_sendl(Fix);
		uart_transmit1('\n');

		uart_transmit_s1("\rKnots:    ");
		//uart_transmit1('\n');
		uart_sendl(Knots);
		uart_transmit1('\n');

		uart_transmit_s1("\rCourse:    ");
		//uart_transmit1('\n');
		uart_sendl(Course);
		uart_transmit1('\n');

		uart_transmit1('\n');

		LcdUpdate();
		_delay_ms(500);
		}
		
		//Je¿eli brak sygna³u GPS:
		else
		{
			LcdClear();
			LcdGotoXYFont(2,2);
			LcdFStr(1,(unsigned char*)PSTR("Czekam na"));
			LcdGotoXYFont(2,3);
			LcdFStr(1,(unsigned char*)PSTR("sygna³ GPS"));

		}

		}
    }
}

// konwersja liczby dziesietnej na BCD
uint8_t dec2bcd(uint8_t dec) {
	return ((dec / 10)<<4) | (dec % 10);
}

// konwersja liczby BCD na dziesietnej
uint8_t bcd2dec(uint8_t bcd) {
	return ((((bcd) >> 4) & 0x0F) * 10) + ((bcd) & 0x0F);
}

void WriteInt(int variable)
{
	int i, n;
	if(variable == 0)
	{
		LcdFStr(FONT_1X,(unsigned char*)("0"));
	}
	if(variable < 0)
	{
		variable = (-1)*variable;
		LcdFStr(FONT_1X,(unsigned char*)("-"));
	}
	temp = variable;
	n = 0;
	while(temp>0)
	{
		n++;
		temp = temp/10;
	}
	char txt[n],buffer[100];
	for(i=n-1; i>=0; i--)
	{
		txt[i] = variable%10 + 0x30;
		variable = variable/10;
	}
	for(i=0; i<n; i++)
	{

		LcdStr(FONT_1X,(unsigned char*)((ltoa(txt[i],buffer,10))));
	}
}

void ParseGPS (char c) {
  if (c == '$') { state = 0; temp = 0; ltmp = 0; }
  char mode = fmt[state++];
  // If received character matches format string, return
  if (mode == c) return;
  char d = c - '0';
  // Ignore extra digits of precision
  if (mode == ',') state--;
  // d=decimal digit; j=decimal digits before decimal point
  else if (mode == 'd') temp = temp*10 + d;
  else if (mode == 'j') { if (c != '.') { temp = temp*10 + d; state--; } }
  // e=long decimal digit
  else if (mode == 'e') ltmp = (ltmp<<3) + (ltmp<<1) + d; // ltmp = ltmp*10 + d;
  // a=angular measure
  else if (mode == 'a') ltmp = (ltmp<<2) + (ltmp<<1) + d; // ltmp = ltmp*6 + d;
  // t=Time - hhmm
  else if (mode == 't') { Time = temp*10 + d; temp = 0; }
  // s=Centisecs
  else if (mode == 's') { Csecs = temp*10 + d; temp = 0; }
  // l=Latitude - in minutes*1000
  else if (mode == 'l') { if (c == 'N') Lat = ltmp; else Lat = -ltmp; ltmp = 0; }
  // o=Longitude - in minutes*1000
  else if (mode == 'o') { if (c == 'E') Long = ltmp; else Long = -ltmp; ltmp = 0; }
   // k=Speed - in knots*100
  else if (mode == 'k') { Knots = temp*10 + d; temp = 0; }
  // c=Course (Track) - in degrees*100. Allow for empty field.
  else if (mode == 'c')
  {
    if (c == ',') { Course = temp; temp = 0; state++; }
    else if (c == '.') state--;
    else { temp = temp*10 + d; state--; }
  }
 /* // y=Date - ddmm
  else if (mode == 'y') { Date = temp*10 + d ; Fix = 1; state = 0; }
  else if (mode == 'x') { Year = temp*10 +d ;}

  else state = 0;*/

	else if (mode == 'y') { Date = temp*10 + d ; Fix = 1; temp = 0; }
	else if (mode == 'x') { Year = temp*10 +d ;}
	else state = 0;


}
