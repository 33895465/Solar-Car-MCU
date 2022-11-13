void Store_mileage()
{
   turn_off = digitalRead(PoworLOW);
   bool mile_RST = digitalRead(mileageRST);
   if(turn_off & !Storage & (mil_m != mileageEEPR) ){
    eepromWrite( 0, mil_m); 
    Storage = 1; 
    StorageOld = 0;
   }   //斷電儲存
   if (!turn_off & Storage){ 
    StorageOld ++; 
    if (StorageOld > 100){
      Storage = 0;
      StorageOld = 0;
    }
   }
   //pf("%d %d %d\n", turn_off, Storage, StorageOld);
   if((currentMillis - previousEEPR) >= 10000 & (mil_m != mileageEEPR))  //10sec儲存
   { 
    previousEEPR = currentMillis; 
    eepromWrite(0,mil_m); 
    mileageEEPR = mil_m;
    //Serial.println("store ontime");
   } 
   //digitalWrite(ledRX, Storage); //set LED OFF
   if (!mile_RST){ 
    SW_Q++;
    if (SW_Q > 2000){
      eepromWrite(0,0); 
      count=0; 
      mileageEEPR=0;SW_Q=0; 
    } 
   }
   //reset Serial.println("reset");
   else
    SW_Q=0;
   //pf("%d %d\n",mile_RST,SW_Q);
}
