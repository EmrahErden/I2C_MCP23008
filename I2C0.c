// Modified File, info is given in file I2CTestMain.c

#include <stdint.h>
#include "inc/tm4c123gh6pm.h"

#define I2C_MCS_ACK             0x00000008  // Data Acknowledge Enable
#define I2C_MCS_DATACK          0x00000008  // Acknowledge Data
#define I2C_MCS_ADRACK          0x00000004  // Acknowledge Address
#define I2C_MCS_STOP            0x00000004  // Generate STOP
#define I2C_MCS_START           0x00000002  // Generate START
#define I2C_MCS_ERROR           0x00000002  // Error
#define I2C_MCS_RUN             0x00000001  // I2C Master Enable
#define I2C_MCS_BUSY            0x00000001  // I2C Busy
#define I2C_MCR_MFE             0x00000010  // I2C Master Function Enable

#define I2C_MCS_ACK							0x00000008	// Data Acknowledge Address

#define TPR 24
void I2C_Init(void){
	volatile unsigned long delay;
  SYSCTL_RCGCI2C_R |= 0x0001;           // activate I2C0
  SYSCTL_RCGCGPIO_R |= 0x0002;          // activate port B
	delay = SYSCTL_RCGC2_R;								// allow some time to finish

  GPIO_PORTB_AFSEL_R |= 0x0C;           // 3) enable alt funct on PB2,3
  GPIO_PORTB_ODR_R |= 0x08;             // 4) enable open drain on PB3 only ??
  GPIO_PORTB_DEN_R |= 0x0C;             // 5) enable digital I/O on PB2,3
                                        // 6) configure PB2,3 as I2C
  GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFF00FF)+0x00003300;
  GPIO_PORTB_AMSEL_R &= ~0x0C;          // 7) disable analog functionality on PB2,3
  I2C0_MCR_R = 0x00000010;      				// master function enable, setting bit 4 of the register to 1
																				// I2C Master Configuration (I2CMCR)
  I2C0_MTPR_R = TPR;             // configure for 100 kbps clock

  // 20*(TPR+1)*20ns = 10us, with TPR=24
}

void writeByte(uint8_t dataByte, uint8_t conditions)
{
  I2C0_MDR_R = dataByte&0xFF;         // write data byte
	I2C0_MCS_R = conditions;
	while(I2C0_MCS_R&I2C_MCS_BUSY){};		// wait for I2C ready, by checking the busy bit
	if((I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0)
	{
		// error in transmission
	}else
	{
		//send stop
		I2C0_MCS_R |= 0x04;
		while((I2C0_MCS_R&0x01)!=0){};
	}	
}

void setRW(uint8_t mode)
{
	if (mode==0)
	{
		//clearing bit, trasmitting data
		I2C0_MCS_R &= ~0x01;
	}else
	{
		// reading data
		I2C0_MCS_R |= 0x01;
	}
}

void setSlaveAddress(uint8_t slave)
{
	I2C0_MSA_R = (slave<<1)&0xFE;    // MSA[7:1] is slave address
}


