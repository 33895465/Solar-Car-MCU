void Code_002 (unsigned int RPM0,unsigned int Mile1,unsigned int Mile2,unsigned int MA0,unsigned int MV0)
{ 
  byte DA[10];
  DA[0]=highByte(RPM0);
  DA[1]=lowByte(RPM0);
  DA[2]=highByte(Mile1);
  DA[3]=lowByte(Mile1);  
  DA[4]=highByte(Mile2);
  DA[5]=lowByte(Mile2);
  DA[6]=highByte(MA0);
  DA[7]=lowByte(MA0);
  DA[8]=highByte(MV0);
  DA[9]=lowByte(MV0); 
  Connection_code(2, DA);
}

void Connection_code(byte ID_W, byte D[10])
{
  mydata.ID = ID_W; 
  mydata.D1 = D[0];
  mydata.D2 = D[1];
  mydata.D3 = D[2];
  mydata.D4 = D[3];
  mydata.D5 = D[4];
  mydata.D6 = D[5];
  mydata.D7 = D[6];
  mydata.D8 = D[7];
  mydata.D9 = D[8];
  mydata.D10 = D[9];
  ET.sendData();
}
