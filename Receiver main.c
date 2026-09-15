#include "Headers/ADC.h"
#include "Headers/DIO.h"
#include "Headers/servo_motor.h"
#include "Headers/USART.h"
#define F_CPU 8000000UL
#include <avr/delay.h>


int main(void)
{
    DIO_voidSetPinDirection(PORTC,PIN0,OUTPUT);
    DIO_voidSetPinValue(PORTC,PIN0,LOW);
	DIO_voidSetPinDirection(PORTA,PIN0,INPUT);//Pin for LDR output
	DIO_voidSetPinDirection(PORTB,PIN0,OUTPUT); // pin for the electronic switch
	ADC_init();
	USART_Init(9600);
	ServoMotor_Init();
	CLR_BIT(DDRA,1);// pin1 in PORTA work as input ->LDR pin
	SET_BIT(DDRD,5);// pin2 in PORTD work as output->servo motor pin
	u32 led_ins; // var store the outdoor lighting instensity
	u8 reciever = '0'; // default state
	u8 new_cmd = 0;
	
	//DDRD &= ~(1 << DDD2);

	//DDRC |= (1 << DDC0);
	//DPORTC &= ~(1 << PIN0);

	
	while (1)
	{
		// Check for UART command without stopping the loop
		if (USART_Receive_NoBlock(&new_cmd))
		{
			reciever = new_cmd; // Updates to '1' when password is correct, or '0' when locked
		}
		
		if (reciever == '1')
		{
			// Continuous Automatic Lighting (LDR -> LED)
			u16 ldr_light = ADC_Read(0);
			if (ldr_light >= 350)
			DIO_voidSetPinValue(PORTB, PIN0, LOW);
			else
			DIO_voidSetPinValue(PORTB, PIN0, HIGH);
			
			// Continuous Curtain Control (LDR -> Servo)
			u16 ldr_curtain = ADC_Read(1);
			if (ldr_curtain > 400)
			ServoMotor_OCA1_Angle(180);
			else
			ServoMotor_OCA1_Angle(0);
		}
		else if(reciever == '2')
		{
			DIO_voidSetPinValue(PORTC,PIN0,HIGH);
		}
		else
		{
			
		}
	}
}
	
  
		
		
		


