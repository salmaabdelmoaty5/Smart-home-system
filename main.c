#include <avr/io.h>
#include "Headers/Bit_Math.h"
#include "Headers/Std_types.h"
#include "Headers/DIO.h"
#include "Headers/LCD.H"
#include "Headers/keypad.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include "Headers/USART.h"


int main(void)
{
	LCD_init();
	KEYPAD_init();
	USART_Init(9600);
	const u8 correctpass[3] = {'1', '2', '3'};
	u8 attempts=0;
	
	for ( attempts =0 ; attempts <3 ; attempts++){
		LCD_SendCommand(0b00000001);
		LCD_SendString((u8 *)"Enter door pass:");
		GoToXY(0, 1);
		u8 entered[3];
		
		for(u8 enterednum =0 ; enterednum <3 ; enterednum++) {
			u8 storing = KEYPAD_keypressed();
			while (storing == '\0') {
				storing = KEYPAD_keypressed();
			}
			if (storing == 'C' ) {
				_delay_ms(150); 
				LCD_SendCommand(0b00000001);
				GoToXY(0, 0);
				LCD_SendString((u8 *)"Enter door pass:");
				GoToXY(0, 1);
				enterednum = 0;
				_delay_ms(100);
				continue;
			}
			
			entered[enterednum] = storing;
			LCD_sendChar(storing);
			_delay_ms(50);
		}
		LCD_SendCommand(0b00000001);
		GoToXY(0, 0);
		if (entered[0] == correctpass[0] && entered[1] == correctpass[1] && entered[2] == correctpass[2]) {
			LCD_SendString((u8 *)"door opened");
			GoToXY(0, 1);
			LCD_SendString((u8 *)"Welcome!");
			
			USART_SendChar('1');
			
			break;
		}
		else {
			LCD_SendString((u8 *)"wrong pass");
		}
		_delay_ms(150);
	}
	if (attempts >= 3){
		LCD_SendCommand(0b00000001);
		GoToXY(0, 0);
		LCD_SendString((u8 *)"No more trials!");
		
		USART_SendChar('2');
	}
	while(1)
	{
		
	}
}
