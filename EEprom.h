/* 
 * File:   EEprom.h
 * Author: nandu
 *
 * Created on 4 April, 2023, 5:38 AM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#define SLAVE_WRITE_EEPROM             0b10100000 // 0xA0
#define SLAVE_READ_EEPROM              0b10100001 // 0xA1

unsigned char ext_eeprom_24C02_read(unsigned char addr);
void ext_eeprom_24C02_byte_write(unsigned char addr,char data);
void ext_eeprom_24C02_str_write(unsigned char addr,char *data);

#endif	/* EEPROM_H */

