#define F_CPU 16000000UL // 16 MHz clock speed

#include <avr/io.h>
#include <avr/interrupt.h> //header to enable interrupt function in program

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
			
char digitArray[] = {0,1,2,3,4,5,6,7,8,9};
	
volatile uint8_t pb7Flag = 0;
volatile uint8_t countDown = 10;
volatile uint8_t msDelay = 0;
volatile uint8_t secDelay = 0;
volatile uint8_t a = 0;

uint8_t displayDigit = 0;

void display_waiting(void);
void display_numerals(void);
void generateTimer(void);
void generateTimer0(void);

int main(void)
{
	int x;
	
	DDRD = 0xFF;//PORTB,C,D are set as output
	DDRB = 0b01111111;
	DDRC = 0xFF;

	PORTB |= (1 << PORTB7);  //set pull up	
	
	PCICR |= (1 << PCIE0);  // enable PCMSK0 scan
	PCMSK0 |= (1 << PCINT0); //PCINT0 will trigger on state change
	
	sei();
	
	generateTimer();
	generateTimer0();
	
	while(1)
	{
		if(pb7Flag == 1)
		{
			countDown = 10;
			
			//200 ms delay
			while(x < 200)
			{
				if(msDelay == 1 )
				{
					x++;
					msDelay = 0;
				}
			}
			
			x = 0;	
			
			if(displayDigit == 9)
			{
				displayDigit = 0;		
			}
			else
			{
				displayDigit ++;
			}
			
			pb7Flag = 0;		
			
			display_numerals();	
		}
		
		if(secDelay == 1)
		{
			if(countDown == 1)
			{				
				countDown = 10;
				display_waiting();
			}
			else
			{
				countDown --;
			}
			
			secDelay = 0;
		}	
		
		display_numerals();	
	}
}

/***************************************************************************************

                                          ISRs
									  
***************************************************************************************/


ISR(PCINT0_vect)
{
	
	if(!(PINB & (1<<7)))
	{
		pb7Flag = 1;
	}

}

ISR(TIMER0_COMPA_vect)
{
	msDelay = 1;	
}

ISR(TIMER1_COMPA_vect)
{
	secDelay = 1;
}


/****************************************************************************************

                                       Functions

****************************************************************************************/


void display_numerals(void)
{
	
	int x;
	
	PORTB = 0x00;
	PORTC = 0x00;
	
	for(int j=0; j<50; j++)
	{
		for(int k=0; k<8;k++)
		{
			if(k<4)
			{
				PORTB = ((0b10000000) | (1<<k));
				PORTD = ~numeralArray[displayDigit][k];
				PORTC = 0x00;
			}
			else
			{
				PORTC = ((0x00) | (1<<(k-4)));
				PORTD = ~numeralArray[displayDigit][k];
				PORTB = 0b10000000;
			}
			
		   //1 ms delay
			while(x < 1)
			{
				if(msDelay == 1 )
				{
					x++;
					msDelay = 0;
				}
			}
			
			x = 0;	
			
			PORTB = 0b10000000;
			PORTC = 0x00;
			
		}
	}
}

void display_waiting(void)
{
	int x = 0;
	
	displayDigit = 0;
	
	for(int n=0; n < 8; n++)
	{
		if(n <4)
		{
			PORTB = ((0b10000000) | (1<<n));
			PORTC = 0x00;
		}
		else
		{
			PORTC = ((0b10000000) | (1<<(n-4)));
			PORTB = 0x00;
		}
			
		for(int i = 0; i < 8; i++ )
		{
			PORTD = (0xFF) & (~(1<<i));
			
			//100 ms delay
			while(x < 100)
			{
				if(msDelay == 1 )
				{
					x++;
					msDelay = 0;
				}
			}
			
			x = 0;			
		}
	}
}

void generateTimer(void)
{ 
	TCCR1A |= (1<<WGM12); //Set to CTC Mode
	TCCR1B |= (1<<CS12) | (1<<CS10);
	TIMSK1 |= (1<<OCIE1A);
	OCR1A = 7813;
}

void generateTimer0(void)
{ 
	ADCSRB |= (1<<ADTS0) | (1<<ADTS1); //Set Timer/Counter 0 Compare Match A
	
	TCCR0B |= (1<<CS00) | (1<<CS02); //Set prescaler to 1024
	TCCR0A |= (1<<WGM01);  //Set to CTC Mode
	TIMSK0 |= (1<<OCIE0A); //Compare Match A interrupt enabled
	
	OCR0A = 10;
}