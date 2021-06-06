#ifndef __I2C_H__
#define __I2C_H__

void I2C1_init(void);
int I2C_wait_till_done(void);
char I2C1_byteWrite(int , char , char );

#endif