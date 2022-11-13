void BMS_READ()
{
  unsigned char len = 0;
  byte bufbyte[8];
  // check if data coming
  if(CAN_MSGAVAIL == CAN.checkReceive())
  {   
    CANonline=1;
    // read data,  len: data length, buf: data buf
    CAN.readMsgBuf(&len, bufbyte);
    unsigned long canId = CAN.getCanId();
    /*
    Serial.print(canId, HEX); 
    Serial.print(" ");
    for(int i = 0; i<len; i++)
    {
      Serial.print(bufbyte[i]); 
      Serial.print(" ");  
    }
    Serial.println();
    */
    //402653185(ODC) = 18000001(HEX)
    //if want more data from BMS, can add if with bufbyte 
    if(canId == 402653185)
    {
      Alert = bufbyte[0],
      BT_curr = bufbyte[1],
      BTpercen = bufbyte[2];
    }
  }
}
