void Code_010 ( unsigned int D0, unsigned int D1, unsigned int D2)
{ 
  byte DA[10];
  DA[0] = highByte(D0); 
  DA[1] = lowByte(D0);  
  DA[2] = highByte(D1); 
  DA[3] = lowByte(D1);  
  DA[4] = highByte(D2); 
  DA[5] = lowByte(D2);
  DA[6] = 0; 
  DA[7] = 0;  
  DA[8] = 0; 
  DA[9] = 0;
  Connection_code(10, DA);
}

void Connection_code( byte ID_W, byte D[10])
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
