// Modified File, info is given in file I2CTestMain.c

#include <stdint.h>

void I2C_Init(void);
void writeByte(uint8_t dataByte, uint8_t conditions);
void setRW(uint8_t mode);
void setSlaveAddress(uint8_t slave);

