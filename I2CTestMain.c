// I2CTestMain.c
// Runs on LM4F120/TM4C123
// Test the functions provided in I2C0.c by periodically sampling
// the temperature, parsing the result, and sending it to the UART.
// Daniel Valvano
// July 2, 2014

// Modified Valvano's Code according to the video:
// https://www.youtube.com/watch?v=A7PUM9wPUAw

// I2C0SCL connected to PB2 and to pin 2 of MCP23008 (I2C type shift register / port extender)
// I2C0SDA connected to PB3 and to pin 1 of MCP23008
// SCL and SDA lines pulled to +3.3 V with 3.8K resistors 

#include <stdint.h>
#include "I2C0.h"
#include "PLL.h"
#include "UART.h"

uint8_t leds=0x00;

int main(void)
{

  PLL_Init();
  UART_Init();
  I2C_Init();
  
	setSlaveAddress(0x20);	// remember to watch the youtube video, linked at the top of the page
	setRW(0);								// writing
	writeByte(0x00, 0x03);	// sending the address, START & RUN
	writeByte(0x00, 0x05);	// sending the address, STOP & RUN	
	while(1)
	{
		writeByte(0x09, 0x03);		// Writing to GPIO of MCP23008, refering to data sheet
		writeByte(leds++, 0x05);	// incrementing the leds value
		for(int i=0; i<1000000; i++) {}
	}
	
  	
}

