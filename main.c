/*
 * GccApplication2.c
 *
 * Created: 5/5/2021 10:14:50 PM
 * Author : nirob
 */
#define LED_OFF PORTB = 0X00
#define F_CPU 1000000UL
#define BIT_IS_SET(byte, bit) (byte & (1 << bit))
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1 << bit)))

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED_OFF PORTB = 0X00
int sec = 0;
int minute = 38;
int hour = 7;
int press = 0;
int BROKEOUT = 0;


void zero()
{
	PORTB = 0XBF;
}
void one()
{
	PORTB = 0X30;
}
void two()
{
	PORTB = 0X5B;
}
void three()
{
	PORTB = 0X4F;
}
void four()
{
	PORTB = 0X66;
}
void five()
{
	PORTB = 0X6D;
}
void six()
{
	PORTB = 0X7D;
}
void seven()
{
	PORTB = 0X07;
}
void eight()
{
	PORTB = 0XFF;
}
void nine()
{
	PORTB = 0X6F;
}
void DISPLAYsingle(int i)
{
	switch(i)
	{
		case 0:
		LED_OFF;
		zero();
		break;
		
		case 1:
		LED_OFF;
		one();
		break;
		
		case 2:
		LED_OFF;
		two();
		break;
		
		case 3:
		LED_OFF;
		three();
		break;
		
		case 4:
		LED_OFF;
		four();
		break;
		
		case 5:
		LED_OFF;
		five();
		break;
		
		case 6:
		LED_OFF;
		six();
		break;
		
		case 7:
		LED_OFF;
		seven();
		break;
		
		case 8:
		LED_OFF;
		eight();
		break;
		
		case 9:
		LED_OFF;
		nine();
		break;
		
		default:
		LED_OFF;
	}
}
void numberposition(int num, int *r1, int *r0)
{
	int pos0 = 0, pos1 = 1;
	*r1 = num / pow(10, pos1);
	*r1 = *r1 % 10;
	
	*r0 = num / pow(10, pos0);
	*r0 = *r0 % 10;
}
void DISPLAY_HOUR(int hour)
{
	int position_one, position_zero;
	numberposition(hour, &position_one, &position_zero);
	PORTC = 0XFF;
	PORTC = ~(1 << PC0);
	DISPLAYsingle(position_one);
	_delay_ms(5);
	LED_OFF;
	PORTC = 0XFF;
	PORTC = ~(1 << PC1);
	DISPLAYsingle(position_zero);
	_delay_ms(5);
	LED_OFF;
}

void DISPLAY_MINUTE(int minutes)
{
	int position_one, position_zero;
	numberposition(minutes, &position_one, &position_zero);
	PORTC = 0XFF;
	PORTC = ~(1 << PC2);
	DISPLAYsingle(position_one);
	_delay_ms(5);
	LED_OFF;
	PORTC = 0XFF;
	PORTC = ~(1 << PC3);
	DISPLAYsingle(position_zero);
	_delay_ms(5);
	LED_OFF;
}
void DISPLAY_SECOND(int seconds)
{
	int position_one, position_zero;
	numberposition(seconds, &position_one, &position_zero);
	PORTC = 0XFF;
	PORTC = ~(1 << PC4);
	DISPLAYsingle(position_one);
	_delay_ms(5);
	LED_OFF;
	PORTC = 0XFF;
	PORTC = ~(1 << PC5);
	DISPLAYsingle(position_zero);
	_delay_ms(5);
	LED_OFF;
}
void INIT_DISP()
{
	DDRB |= 0XFF;
	DDRC |= 0XFF;
}
void INTERUPT_ENABLE()
{
	MCUCR = (1 << ISC11);
	GICR = (1 << INT1);
	DDRD &= ~(1 << PD4);
	PORTD |= (1 << PD4);
	DDRD &= ~(1 << PD5);	
	PORTD |= (1 << PD5);
}


void InitialiseHardwareTimer()
{
	TCCR1A = 0;
	TCCR1B |= 1 << WGM12 | 1 << CS10 | 1 << CS12;  // 1024 prescaler
	OCR1A = 0x03E8;
	TIMSK |= 1 << OCIE1A;
	sei();
}

int main(void)
{
	InitialiseHardwareTimer();
	INIT_DISP();
	INTERUPT_ENABLE();
	sei();
	while (1)
	{
		DISPLAY_SECOND(sec);
		DISPLAY_MINUTE(minute);
		DISPLAY_HOUR(hour);
		while(BROKEOUT == 1)
		{
			if(BIT_IS_CLEAR(PIND, PD4))
			{
				press++;
				_delay_ms(300);
			}
			if(press == 0)
			{
				DISPLAY_HOUR(hour);
			}
			else if(press == 1)
			{
				DISPLAY_MINUTE(minute);
			}
			else
			{
				press = 0;
				break;
			}
			if(BIT_IS_CLEAR(PIND, PD5))
			{
				if(press == 0)
				{
					hour++;
				}
				else
				{
					minute++;
				}
				if(hour > 12)
				{
					hour = 0;
				}
				if(minute > 59)
				{
					minute = 0;
				}
				_delay_ms(300);
			}
		}
		if(BROKEOUT > 1)
		{
			BROKEOUT = 0;
		}
	}
}
ISR(TIMER1_COMPA_vect)
{
	sec++;
	if(sec > 59)
	{
		minute++;
		sec = 0;
	}
	if(minute > 59)
	{
		hour++;
		minute = 0;
	}
	if(hour > 12)
	{
		hour = 1;
	}
}

ISR(INT1_vect)
{
	BROKEOUT++;
	_delay_ms(300);
}