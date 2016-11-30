/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#define F_CPU 16000000UL // 16 MHz clock speed

#include <avr/io.h>
#include <util/delay.h>  //header to enable delay function in program

static int numeralArray[10][8]= {
	{0b00111100,0b01100110,0b11000011,0b11000011,0b11000011,0b11000011,0b01100110,0b00111100}, //0
	{0b00010000,0b00011000,0b00011100,0b00011000,0b00011000,0b00011000,0b01111110,0b01111110}, //1
 	{0b00111000,0b01111100,0b01101100,0b00110000,0b00011000,0b00001100,0b01111110,0b01111110}, //2
	{0b01111110,0b11111110,0b11000000,0b11111100,0b11111100,0b11000000,0b11111110,0b01111110}, //3
	{0b01100110,0b01100110,0b01100110,0b01111110,0b01111110,0b01100000,0b01100000,0b01100000}, //4
	{0b01111111,0b01111111,0b00000011,0b00111111,0b01111111,0b01100000,0b01111111,0b00111111}, //5
	{0b01111110,0b01111111,0b00000011,0b00111111,0b01111111,0b01100011,0b01111111,0b00111111}, //6
	{0b11111111,0b11111111,0b01100000,0b00110000,0b00011000,0b00001100,0b00000110,0b00000011}, //7
	{0b01111110,0b11111111,0b11000011,0b11111111,0b11111111,0b11000011,0b11111111,0b01111110}, //8
	{0b01111110,0b11111111,0b11000011,0b11111111,0b11111110,0b11000000,0b11000000,0b11000000}}; //9
			
char digitArray[] ={0,1,2,3,4,5,6,7,8,9};
uint8_t l =0;

void display_waiting(void);
void display_numerals(void);

int main(void)
{
	DDRD = 0xFF;//PORTB,C,D are set as output
	DDRB = 0xFF;
	DDRC = 0xFF;
	
	while(1)
	{
		//display_waiting();
		display_numerals();
	}
}

void display_numerals(void)
{
	for(int i=0; i<sizeof digitArray; i++)
	{
		l = digitArray[i];
		
		for(int j=0; j<50; j++)
		{
			for(int k=0; k<8;k++)
			{
				if(k<4)
				{
					PORTB = ((0x00) | (1<<k));
					PORTD = ~numeralArray[l][k];
					PORTC = 0x00;
				}
				else
				{
					PORTC = ((0x00) | (1<<(k-4)));
					PORTD = ~numeralArray[l][k];
					PORTB = 0x00;
				}
				
				_delay_ms(1.2);
					
				PORTB = 0x00;
				PORTC = 0x00;
			}
		}
	}		
	
}

void display_waiting(void)
{
	for(int n=0; n < 8; n++)
	{
		if(n <4)
		{
			PORTB = ((0x00) | (1<<n));
			PORTC = 0x00;
			
		}
		else
		{
			PORTC = ((0x00) | (1<<(n-4)));
			PORTB = 0x00;
		}
			
		for(int i = 0; i < 8; i++ )
		{
			PORTD = (0xFF) & (~(1<<i));
			_delay_ms(100);			
		}
	}
}

