#ifndef _I2C_J_h_
#define _I2C_J_h_

#define I2C_DEV_DRIVER "/dev/i2c-0"

int i2c_open();
int i2c_close();
int i2c_setSlave(int dev_addr);
int i2c_write(int dev_addr, unsigned char *data, int size, int offsetAddr);//return size+1;
int i2c_read(int dev_addr, unsigned char *data, int size, int offsetAddr);//return size;
int i2c_simpleWrite(int fd_sensor, uint8_t address, uint8_t value);
int i2c_one_Write(int fd_sensor, uint8_t value);

#endif
