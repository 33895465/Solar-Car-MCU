/* ============CAN BUS 傳送區=============================================*/ 
void  Send_area()
{
  CAN.sendMsgBuf(0x123, 0, 8, _CAN_DIGITS_Out_1_msg.Data.Bytes);       //此段為須滿足儀表設定之數值 0x123區的掃描時間(Cycle)為10ms
  Can_TX_count ++;
  
  if(Can_TX_count >= 5)     //傳送更新速度，可以不用動 5 * 10ms = 50ms
    Can_TX_count = 0;

  if(Can_TX_count== 0)
    CAN.sendMsgBuf(0x234, 0, 8, _CAN_Indicator_Out_1_msg.Data.Bytes);  //此段為須滿足儀表設定之數值 0x234區的掃描時間(Cycle)為50ms
}
/* =======================================================================*/ 
