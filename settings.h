unsigned char eeprom_val __attribute__((section(".eeprom")));

void save(uint8_t pozycja, unsigned char wartosc ) {
  eeprom_busy_wait();
  eeprom_write_byte(&eeprom_val[pozycja],wartosc);
};

char read(uint8_t pozycja) {
  eeprom_busy_wait();
  return eeprom_read_byte(&eeprom_val[pozycja]);
};
