void ConnectionWRITE()
{    
  bitWrite( Data03[1], 1, CANonline);
  for (byte x = 0 ; x < QuaID ; x++)
  {   
    if(Show_re[x])
    { 
      Show_re[x] =0; 
      byte data[10]; 
      for(byte t=0 ; t< 10 ;t++)  
        data[t]=DataALL[x][t];
      if(x==3)
      {
        bitWrite(data[1],0,CANfail);    
        bitWrite(data[1],1,CANonline);  
        bitWrite(data[1],2,HIGH);
      }
      Connection_code(x,data); 
    }
    if(x==3 & !Show_re[x])
      Connection_code( 3, Data03);
  } 
}
