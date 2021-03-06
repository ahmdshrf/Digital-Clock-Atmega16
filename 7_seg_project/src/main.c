#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define NUMBER_OF_OVERFLOWS_PER_SECOND 4

unsigned volatile char num[6]={5,9,5,9,5,0};
void display();


unsigned volatile char tick = 0;
unsigned char flag1 = 0;
unsigned char flag2 = 0;

void Timer0_init_Normal_Mode(void)
{
	TCNT0 = 0;
	TIMSK = (1<<TOIE0);
	TCCR0 = (1<<FOC0) |(1<<CS00)| (1<<CS02);
	SREG |= (1<<7);
}
void int0_reset(void)
{
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);
	MCUCR |= (1<<ISC00) | (1<<ISC01);
	GICR |= (1<<INT0);
	SREG |= (1<<7);
}

void int1_reset(void)
{
	DDRD &= ~(1<<PD3);
	PORTD |= (1<<PD3);
	MCUCR |= (1<<ISC00) | (1<<ISC01);
	GICR |= (1<<INT1);
	SREG |= (1<<7);
}


ISR(INT0_vect)
{
	flag1 = 1;
}

ISR(INT1_vect)
{
	flag2 = 1;
}

ISR(TIMER0_OVF_vect)
{
	tick++;
}

void display(void)
{
	for(int j=0;j<6;j++)
	{
		PORTB |=(1<<j);
		PORTA = num[j];
		_delay_us(10);
		PORTB &=~(1<<j);
	}
}


int main(void)
{
	DDRB = 0xFF;
	DDRA  = 0xFF;
	PORTA = 0x00;
	int0_reset();
	int1_reset();
	Timer0_init_Normal_Mode();
    while(1)
    {
    	if (flag2 == 1)
    	{
    		flag2 = 0;
    		TIMSK ^= (1<<TOIE0);
    	}
    	if(flag1 == 1){
    		flag1 = 0;
    		for(int i = 0 ; i<6 ;i++ )
    		{
    			num[i] = 0;
    		}
    	}
    	if(tick >= NUMBER_OF_OVERFLOWS_PER_SECOND){
    		tick=0;
    		num[5]++;
    		if(num[5] > 9)
    		{
    			num[5]=0;
    			num[4]++;
    		}
    		if(num[4] > 5){
    			num[5]=0;
    			num[4]=0;
    			num[3]++;
    		}
    		if(num[3] > 9){
    			num[3]=0;
    			num[2]++;
    		}
    		if(num[2] > 5){
    			num[3]=0;
    			num[2]=0;
    			num[1]++;
    		}
    		if(num[1] > 9){
    			num[1]=0;
    			num[0]++;
    		}
    		if(num[0] > 5){
    			num[0] = 0;
    		}
    	}
    	display();
    }
}
