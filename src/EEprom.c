
#include "main.h"

//function to read external eeprom
unsigned char ext_eeprom_24C02_read(unsigned char addr)
{
    unsigned char data;
    
    i2c_start();
    
    i2c_write(SLAVE_WRITE_EEPROM );
    i2c_write(addr);
    i2c_rep_start();
    i2c_write(SLAVE_READ_EEPROM );
    
    data = i2c_read(0);
    i2c_stop();
    
    return data;
}

void ext_eeprom_24C02_byte_write(unsigned char addr,char data) // SEc_ADDR, data
{
    i2c_start();
    int ack=i2c_write(SLAVE_WRITE_EEPROM);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
}

void ext_eeprom_24C02_str_write(unsigned char addr,char *data) // SEc_ADDR, data
{
    while(*data != 0)
    {
        ext_eeprom_24C02_byte_write(addr,*data);
        data++;
        addr++;
    }
}