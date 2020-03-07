/*
*
*	Author : Tomasz Durnik
*	
*	ATmega32
*	DHT11
*	UART TTL USB
*
*	Zasilanie 3.3V
*
*/


#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>


#define BAUD 9600                                   // define baud
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#include <util/setbaud.h>

//dioda LED sygnalizuj¹ca pracê czujnika
#define LCD PORTB
#define LED_DIR_SET_OUT DDRB|=(1<<PB1)
#define LED_ON PORTB|=(1<<PB1)
#define LED_OFF PORTB&=~(1<<PB1)
#define LED_TOGGLE PORTB^=(1<<PB)

//pod³¹czenie czujnika DHT11 do odpowiedniego wejœcia w Atmega32
#define DHT_PORT PORTB
#define DHT_PIN PB0
#define DHT_DIR DDRB
#define DHT_PIN_STATE PINB

void init_dht(void);
uint8_t fetchData(uint8_t* arr);

//tablice char do konwersji z danych czujnika na wyœwietlacz LCD
char humidity_init[10];
char humidity_deci[10];
char temp_init[10];
char temp_deci[10];

volatile uint16_t liczba;

const uint8_t data[] PROGMEM = {
	0b00000100,
	0b01101110,
	0b00011000,
	0b01001000,
	0b01100010,
	0b01000001,
	0b00000001,
	0b01101100,
	0b00000000,
	0b01000000,
};


void uart_init(unsigned int baud){
//void uart_init0(unsigned int BAUDRATE){
    UBRRH = (BAUDRATE>>8);
    UBRRL = BAUDRATE;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	//UCSR0C |= 0b00000011;
    //UCSRC |= (1<<USBS)|(1<<UCSZ);
	//UCSRC = (1<<UCSZ0)|(1<<UCSZ1);
	UCSRC |= (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}

//8 BIT
void uart_transmit( unsigned char data ){
	/* Wait for empty transmit buffer */
	while (!( UCSRA & (1<<UDRE)));
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char uart_receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	/* Get and return received data from buffer */
	return UDR;
}

void uart_transmit_s(char * str)
{
	while(*str) uart_transmit(*str++);
	_delay_ms(50);
}

void uart_sendl(uint32_t liczba)
{
	char buf[17];
	ltoa(liczba,buf,10);
	uart_transmit_s(buf);

}


int main()
{
	uart_init(16);
	init_dht();
	uint8_t data[4],max_temp,min_temp,max_hum,min_hum;
	uint8_t i;//licznik "czasu"
	uint8_t m;//licznik 24h
	uint8_t temp_max[290];
	uint8_t temp_min[290];
	uint8_t hum_max[290];
	uint8_t hum_min[290];


	//TCCR1B |= (1<<WGM12);				/* tryb CTC */
	//TCCR1B |= (1<<CS12)|(1<<CS10);		/* preskaler = 1024 */
	TCCR1B = (1<<WGM12)|(1<<CS12)|(1<<CS10);
	OCR1A = 20;//0.25s dla 16MHz
	TIMSK = (1<<OCIE1A);

	sei();

	PORTD |=(1<<PD0)|(1<<PD2);

	//zmienna s³u¿¹ce do archiwizacji wyników
	//uint8_t dane_temp1[10],dane_temp2[10];
	//uint8_t dane_humidity[10];
	//uint8_t i=0;


//	PORTA = (1<<PA0)|(1<<PA1)|(1<<PA2); //przyciski

	DDRC = 0xFF;//segmenty
	PORTC = 0xFF;//wygas segmenty
	DDRA = 0xFF;
	PORTD |= (1<<PD0)|(1<<PD2)|(1<<PD4)|(1<<PD6);//przyciski
	//PORTA &=~(1<<PD3);//dioda
	//PORTA &=~(1<<PD7);//dioda

	max_temp=0;
	min_temp=100;
	max_hum=0;
	min_hum=100;

	temp_max[0]=99;
	temp_min[0]=01;
	hum_max[0]=99;
	hum_min[0]=01;
	i=0;
	m=0;

for(;;)
{
      LED_OFF;
      _delay_ms(500);
      LED_ON;
      	  if (fetchData(data))
      	  {

      		  itoa(data[0],humidity_deci,10);
      		  itoa(data[1],humidity_deci,10);//po kropce
      		  itoa(data[2],temp_deci,10);
      		  itoa(data[3],temp_deci,10);//po kropce


    			if(max_temp<data[2])
    			      	 	 {
    			      			  max_temp=data[2];
    			      	 	}
    			      		if(min_temp>data[2])
    			      		{
    			      			min_temp=data[2];
    			      		}

    			      		if(max_hum<data[0])
    			    	    {
    			      			max_hum=data[0];
    			    		}

    			    		if(min_hum>data[0])
    			    		{
    			    			min_hum=data[0];
    			    		}



      		  //sprawdzenie co 15 sekund
      		if(i==20)
      		{
          		if(m==288)
          		{
          			m=0;
          		}
          		else


      		temp_max[m]=max_temp;
      		temp_min[m]=min_temp;
      		hum_max[m]=max_hum;
      		hum_min[m]=min_hum;

      		m++;
      		i=0;

      		}





//wybor co ma byc wyswietlane
      		if (!(PIND & (1<<PD0)))
      		{//temperatura
      		liczba=data[2];
      		}

      		if (!(PIND & (1<<PD2)))
      		{//wilgotnosc
      		liczba=data[0];
      		}

      		if (!(PIND & (1<<PD4)))
      		{//max
      		//liczba=hum_max[m];
      			liczba=hum_max[m];
      		}

      		if (!(PIND & (1<<PD6)))
      		{//min
      		//liczba=hum_min[m];
      			liczba=hum_min[m];
      		}




//UART
           	uart_transmit('\r');
           	uart_transmit_s("Temp: ");
           	uart_transmit_s("    ");
          	//uart_transmit('\r');
          	uart_sendl(data[2]);

     /*      	uart_transmit('\n');
           	uart_transmit('\r');
           	uart_transmit_s("Max Temp: ");
           	uart_sendl(temp_max[m]);

           	uart_transmit('\n');
           	uart_transmit('\r');
           	uart_transmit_s("Min Temp: ");
           	uart_sendl(temp_min[m]);*/

          	uart_transmit('\n');
          	uart_transmit('\n');
          	uart_transmit('\r');
          	uart_transmit_s("Hum: ");
          	uart_transmit_s("    ");
          	//uart_transmit('\r');
        	uart_sendl(data[0]);

            uart_transmit('\n');
            uart_transmit('\r');
            uart_transmit_s("Max Hum: ");
            uart_sendl(max_hum);
            uart_transmit('\n');
            uart_transmit('\r');
            uart_transmit_s("Max Hum zap: ");
            uart_sendl(hum_max[m]);

            uart_transmit('\n');
            uart_transmit('\r');
            uart_transmit_s("Min Hum: ");
            uart_sendl(min_hum);
            uart_transmit('\n');
            uart_transmit('\r');
            uart_transmit_s("Min Hum zap: ");
            uart_sendl(hum_min[m]);

          	uart_transmit('\n');
        	uart_transmit('\n');
        	uart_transmit('\n');



      		  }

//ikrementacja i
i++;
}

return 0;
}

/*
 * inicjalizacja czujnika DHT poprzez odczekanie 2s
 * po w³¹czeniu zasilania dla osi¹gniêcia stanu stabilnego
*/

void init_dht(void)
{
 //ustawienie diodu LED jako wyjœcie
 LED_DIR_SET_OUT;
 _delay_ms(2000);
 LED_ON;
}


uint8_t fetchData(uint8_t* arr)
{
 uint8_t data[5];
 uint8_t cnt, check;
 int8_t i, j;

 //Inicjalizacja czujnika DHT11

 // Ustaw pin danych jako wyjœcie
 DHT_DIR |= (1 << DHT_PIN);

 //ustawiamy preskaler clk/2014, przy clk(F_CPU)=11.0592MHz
 TCCR0 = 0;
 //prescaler 1024
 TCCR0 |= (1 << CS02) | (1 << CS00);
 //licz do 215 dla 20ms
 TCNT0 = 0;


 //ustawiamy stan niksi na 20ms
 DHT_PORT &= ~(1 << DHT_PIN);

 //czekaj oko³o 20 ms
 while (TCNT0 < 217)
  ;

 //ustawienie Timer0 dla przeskalera clk/8
 TCCR0=0;
 // clk/8
 TCCR0 |= (1<<CS01);
 TCNT0 = 0;

 //ustawiamy stan wysoki
 DHT_PORT ^= (1 << DHT_PIN);

 //ustawiamy pin danych jako wejœcie
 DHT_DIR &= ~(1 << DHT_PIN);

 //czekamy na odpowiedŸ czujnika 20-40us
 while (DHT_PIN_STATE & (1 << DHT_PIN))
 {
  if (TCNT0 >= 82)
   // odczekujemy 60us
   return 0;
 }

 	 	 	 //Rozpoczêcie transmisji

 TCNT0 = 0;

 /*
  * czekamy na odpowiedŸ sensora
  * najpierw stan niski
 */
 while (!(DHT_PIN_STATE & (1 << DHT_PIN)))
 {
  if (TCNT0 >= 137)

   return 0;
 }

 TCNT0 = 0;

 //czekamy na odpowiedŸ sensora teraz stan wysoki
 while (DHT_PIN_STATE & (1 << DHT_PIN))
 {
  if (TCNT0 >= 137)
   //jeœli wiêcej ni¿ 100us - b³¹d
   return 0;
 }

 	 	 	 	 //Przesy³ danych

 for (i = 0; i < 5; ++i)
 {
  for (j = 7; j >= 0; --j)
  {
   TCNT0 = 0;

//czekamy na odpowiedŸ sensora najpierw stan niski przez 50us
while (!(DHT_PIN_STATE & (1 << DHT_PIN)))
   {
    if (TCNT0 >= 96)
     //je¿eli wiêcej ni¿ 70u-b³¹d
     return 0;
   }

   TCNT0 = 0;

   /*Teraz wysy³ane s¹ dane
    * 26 do 28 us to stan wysoki wskazuje zero
    * oko³o 70us to wysy³ana jedynka
    */

   while (DHT_PIN_STATE & (1 << DHT_PIN))
   {
    if (TCNT0 >= 123)
     //je¿eli wiêcej ni¿ 90us-b³¹d
     return 0;
   }

   cnt = TCNT0;

   if (cnt >= 27 && cnt <= 47)
   {
	//pomiêdzy 20 i 35us ustawiamy zero
    data[i] &= ~(1 << j);
   }
   else if (cnt >= 82 && cnt <= 110)
   {
	//pomiêdzy 60 and 80us ustawiamy jedynke
    data[i] |= (1 << j);
   }

   else
    return 0;
  }
 }

 /*
  * zakoñczenie komunikacji
  * sprawdzenie poprawnoœci danych
 */
 check = (data[0] + data[1] + data[2] + data[3]) & 0xFF;

 if (check != data[4])
  return 0;

 for (i = 0; i < 4; ++i)
 {
  arr[i] = data[i];

 }

 return 1;
}

ISR(TIMER1_COMPA_vect){
	static uint8_t led;

	uint8_t cyfra;

	PORTA = 0xFF;

	switch(led){
		case 0: cyfra = liczba%10;

				PORTC = pgm_read_byte(&data[cyfra]);
				PORTA &= ~(1<<PA6);
				break;
		case 1: cyfra = (liczba/10)%10;

				PORTC = pgm_read_byte(&data[cyfra]);
				PORTA &= ~(1<<PA5);
				break;
	}

	led++;
	led &= ~4;
}
