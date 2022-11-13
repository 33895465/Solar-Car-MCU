void ConnectionREAD()
{ 
  if(ET.receiveData()){ 
  ID_t = mydata.ID;
  inDATA[0] = mydata.D1;
  inDATA[1] = mydata.D2;
  inDATA[2] = mydata.D3;
  inDATA[3] = mydata.D4;
  inDATA[4] = mydata.D5;
  inDATA[5] = mydata.D6;
  inDATA[6] = mydata.D7;
  inDATA[7] = mydata.D8;
  inDATA[8] = mydata.D9;
  inDATA[9] = mydata.D10;
  Show_re[ID_t] = 1;
   for (byte i = 0 ; i < 10 ; i++) 
    DataALL[ID_t][i] = inDATA[i];
  digitalWrite(ledRX, HIGH);
  }
}
