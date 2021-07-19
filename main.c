/*
 * XYPlotter5.c
 *
 * Created: 28-Nov-19 5:29:50 PM
 * Author : chama
 */ 
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define forward 0
#define backward 1
#define stop 2

//function prototypes
void Set_parameter(int* motor, int position);
void RESET(void);
void Update_Parameters(int* motor);
void Go_to(int x, int y);
void motorFunc(int* motor);

//global variables
int motor1[]={0,stop,0,0,0,0x05,0x06,0x0a,0x09}; //position(0),direction(1),destination(2),count(3),status(4),pin1,pin2,pin3,pin4
int motor2[]={0,stop,0,0,0,0x50,0x60,0xa0,0x90}; //position,direction,destination,count,status,pin1,pin2,pin3,pin4
int toggle=0;

//ISR
ISR(TIMER0_OVF_vect){
	switch(toggle){
		case 0:
		PORTB &= 0xf0;
		motorFunc(motor1);
		Update_Parameters(motor1);
		toggle = 1;
		break;
		case 1:
		PORTB &= 0x0f;
		motorFunc(motor2);
		Update_Parameters(motor2);
		toggle = 0;
		break;
	}
}
	int a=0;
	int b = 0 ;
int main(void)
{
    RESET();

	Go_to(50,50);
	
	
    while (1) 
    {
		if ((motor1[1] == (int)stop) && (motor2[1] ==(int)stop) )
		{
			if(a == 0){
				Go_to(25,10);
				a = 1;
				}else if(a == 1){
				Go_to(75,50);
			}
			
			}else{
			b++;
		}
    }
	
	
	
}


//subroutines
void RESET(){
	DDRB	=	0xFF;
	TIMSK	=	(1<<TOIE0);
	TCCR0B	=	0x02;
	sei();
}

void Go_to(int x, int y){
	Set_parameter(motor1,x);
	Set_parameter(motor2,y);
}

 void Set_parameter(int* motor, int position){
	 
	 if (position > motor[0])
	 {
		 motor[2] = position - motor[0];
		 motor[1] = forward;
	 }
	 else if (position < motor[0])
	 {
		 motor[2] = motor[0] - position;
		 motor[1] = backward;
	 }
	 else
	 {
		 motor[1] = stop;
		 motor[2] = 0;
	 }
	 
 }
 
 void Update_Parameters(int* motor){
	 
	 if (motor[1] != stop)
	 {
		 if (motor[1]==forward)
		 {
			 (motor[0])++;
		 }
		 else
		 {
			 (motor[0])--;
		 }
	 }
	 
	 if (motor[3] == motor[2])
	 {
		 motor[1] = stop;
		 motor[2] = 0;
		 motor[3] = 0;
	 }
	 
 }

void motorFunc(int* motor){
	(motor[3])++;
	switch(motor[1]){
		case forward:
		
		switch(motor[4]){
			case 0:
			PORTB |= motor[5];
			motor[4] = 1;
			break;
			case 1:
			PORTB |= motor[6];
			motor[4] = 2;
			break;
			case 2:
			PORTB |= motor[7];
			motor[4] = 3;
			break;
			case 3:
			PORTB |= motor[8];
			motor[4] = 0;
			break;
		}
		break;
		
		case backward:
		
		switch(motor[4]){
			case 0:
			PORTB |= motor[8];
			motor[4] = 1;
			break;
			case 1:
			PORTB |= motor[7];
			motor[4] = 2;
			break;
			case 2:
			PORTB |= motor[6];
			motor[4] = 3;
			break;
			case 3:
			PORTB |= motor[5];
			motor[4] = 0;
			break;
		}
		break;
		
		case stop:
		motor[3] = 0;
		break;
	}
	
}
