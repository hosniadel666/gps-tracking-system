#ifndef __I2C_H__
#define __I2C_H__

void i2c1_init(void);
int i2c1_wait_till_done(void);
char i2c1_byteWrite(int , char , char );
char i2c1_read(int , char , int , char* );

#endif // __I2C__