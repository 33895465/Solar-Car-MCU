void Show_Digits(){ 
  COM_Get_TRIP_Indicator()  = 0;  COM_Get_ODO_Indicator() = 0;  COM_Get_RANGE_Indicator() = 0;
  COM_Get_POWER_Indicator() = 0;  COM_Get_AVG_Indicator() = 0; 

/*************************************電壓********************************************************************/
  COM_Get_Bar_LV() = map( Vol_P, max_V, min_V, 0, 5);     //轉換比例
/*************************************里程********************************************************************/
  switch (SB_mil) //下方數字
  {
    case 0:                                      //短里程符號 
      if ( MOT[1] > TRIP_EEP )
        Down_tmp = (MOT[1] - TRIP_EEP) * 10 + MOT[2] / 100;
      else   
        Down_tmp = TRIP_EEP;
      COM_Get_TRIP_Indicator() = 1;
      Trip_state = 1; 
      break;
    case 1:                                      //總里程
      Down_tmp = MOT[1] * 10 + MOT[2] / 100;   
      COM_Get_ODO_Indicator() = 1;
      Trip_state = 0;
      break;                                       
    case 2:                                      //剩餘電量可行駛距離
      Down_tmp = 0;                      //公式
      COM_Get_RANGE_Indicator() = 1;
      Trip_state = 0; 
      break;                                     
    default:
      break;    
  }
  if(MtoI)
    Down_tmp = Down_tmp;
  else
    Down_tmp = Down_tmp * 0.6214;
/************************************車速*************************************************************************/
  if(MtoI)    //公英制切換
    COM_Get_Mind_Digits() = ((double(MOT[0])/6000) * (31*3.14) * 3.6 ); //(公制) 車速公式 車速=(轉速*60*輪胎周長)/1000  
  else
    COM_Get_Mind_Digits() = ((double(MOT[0])/6000) * (31*3.14) * 3.6 ) * 0.6213712;  //(英制) 車速公式 車速=((轉速*60*輪胎周長)/1000)*0.62137
  //MOT[0]:rpm  
/************************************功率*************************************************************************/

/*---------------------上方符號---------------------------------*/
  if(SB_Top_tmp == 0)           //Power
  {
    /*--title--*/
    COM_Get_POWER_Indicator() = 1;          
    COM_Get_Percentage_Indicator() = 1;
    /*--unit--*/
    COM_Get_kmhkwh_Indicator() = 0;
    COM_Get_milekwh_Indicator() = 0;
  }
  else if(SB_Top_tmp == 1){                    //AVG
    COM_Get_POWER_Indicator() = 0;
    COM_Get_Percentage_Indicator() = 0;
    COM_Get_AVG_Indicator() = 1;
        
    if(MtoI == true)
      COM_Get_kmhkwh_Indicator() = 1;
    else if(MtoI != true)
      COM_Get_milekwh_Indicator() = 1;
  }
/*---------------------上方數字輸送資料寫入---------------------------------*/
  COM_Get_TOP_Digits_H1() = Top_tmp>>8;
  COM_Get_TOP_Digits_L0() = Top_tmp&0x00FF;
/*---------------------下方數字輸送資料寫入---------------------------------*/
  COM_Get_Down_Digits_Bytes0() = Down_tmp&0x000000FF;
  COM_Get_Down_Digits_Bytes1() = Down_tmp>>8;
  COM_Get_Down_Digits_Bytes2() = Down_tmp>>16;
  COM_Get_Down_Digits_Bytes3() = Down_tmp>>24;
}
