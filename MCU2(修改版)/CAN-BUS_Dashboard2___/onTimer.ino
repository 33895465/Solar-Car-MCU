void onTimer() //計時中斷
{      
  if(MOT[3]>offset_A)
    COM_Get_BRAKE_V_Indicator() = 0;
  else
    COM_Get_BRAKE_V_Indicator() = 1;
  
  unsigned int cur = abs(MOT[3] - offset_A) / 10;
  unsigned int AV = cur * MOT[4] / 10;

/*--------------------上方數字計算區--------------------------------*/
  if(SB_Top_tmp == 1){    //AVG
    if(MtoI == true)
      Top_tmp = (MOT[2] - ODO_sec) * 36 / AV;
    else
      Top_tmp = (MOT[2] - ODO_sec) * 2.2369 / AV;
   }
   else{                  //Power
    Top_tmp = Vol_P;
   }
   ODO_sec = MOT[2];
}
