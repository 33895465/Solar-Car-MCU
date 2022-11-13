void eepromWrite(unsigned int current_addressW, unsigned long data32)
{
  byte data_to_send[4];
  data_to_send[0] = highByte(highWord(data32));
  data_to_send[1] = lowByte(highWord(data32));
  data_to_send[2] = highByte(lowWord(data32));
  data_to_send[3] = lowByte(lowWord(data32));
  for (byte i = 0 ; i < 4 ; i++) {
    i2c_eeprom.write(current_address+i, data_to_send[i]); 
    delay(2);
  }
}
unsigned long eepromRead(unsigned int current_addressR)
{ 
  byte data_to_Read[4];
  for (byte i=0 ; i< 4 ;i++)
  { 
    data_to_Read[i] = i2c_eeprom.read(current_addressR+i);  
    delay(2); 
  }
  return makeLong(word(data_to_Read[0], data_to_Read[1]), word(data_to_Read[2], data_to_Read[3]));
}
