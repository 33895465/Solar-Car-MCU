void ConnectionREAD(){ 
  if(ET.receiveData()){  
    byte inDATA[10];
    byte ID_t = mydata.ID;
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
    Show_re[ID_t]=1;
    
    for (byte i=0 ; i< 10 ;i++)
      DataALL[ID_t][i]=inDATA[i];
      
    digitalWrite(ledRX, HIGH); 
    decoding(ID_t);//代碼轉換
  }
}

void decoding(byte x){  
  pf("ID:%d ",x);  
  for (byte i=0 ; i< 10 ;i++) 
    pf(",%d",DataALL[x][i]);
  //Serial.println("");
  switch (x)
  {
    case 1:     //MCU1_PV
      for (byte i=0 ; i< 5 ;i++)  
        PV_A[i] = word(DataALL[1][i*2],DataALL[1][i*2+1]);
      break;
      
    case 2:     //MCU3_MOTOR
      for (byte i=0 ; i< 5 ;i++)
        MOT[i] = word(DataALL[2][i*2],DataALL[2][i*2+1]);
      break;
      
    case 3:      //MCU3_PV
      MOT_PV = word(DataALL[3][8],DataALL[3][9]);
      break;
 
    case 10:     //MCU4_BMS
      Vol_P = DataALL[10][9];
      BMS_A = DataALL[10][8];
      break;
       
    default:
      break;
  }
}
