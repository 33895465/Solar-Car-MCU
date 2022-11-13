#define max_V 100                                //120.00
#define min_V 0                                  //100.00
#define offset_A 4386                            // =2.5/0.0057*10
#define interval_500 500
unsigned long previousMillis_500, currentMillis;
bool Flash, MtoI;                                //Flash:0.5sec MtoI:公制英制
unsigned char data[8] = {0,0,0,0,0,0,0,0};
/********************************************************************************************************/
bool SW_Filter1, SW_Filter2;
byte SB_mil, SB_Top_tmp;
unsigned int F1, F2;
bool Trip_state;
/********************************************************************************************************/
#include <df_can.h>
#include <SPI.h>
#define SPI_CS_PIN 10
MCPCAN CAN(SPI_CS_PIN);
/********************************************************************************************************/
bool ERROR_COLD[16];
/********************************************************************************************************/
#include <EasyTransfer.h>
EasyTransfer ET;  
struct data_STRUCTURE { 
  byte ID; 
  byte D1;
  byte D2;
  byte D3;
  byte D4;
  byte D5;
  byte D6;
  byte D7;
  byte D8;
  byte D9;
  byte D10;
};  data_STRUCTURE mydata;

bool ConR; 
byte ID_t; 
byte inDATA[10];
  
#define QuaID 16
byte DataALL[QuaID][10];
bool Show_re[QuaID];        //Show_record 
unsigned int PV_A[5];
unsigned int MOT[5];        // 0:RPM ; 1:里程(km) ; 2:里程(m) ; 3:總電流 (60.00A) ; 4:總電壓 (120.00V) 
unsigned int TRIP_EEP;
unsigned int MOT_PV;
byte Vol_P,BMS_A;           //Vol_P:BMS電壓 BMS_A:BMS電流
/********************************************************************************************************/
#include <EEPROM.h>
#include <MsTimer2.h>       //定時器庫的標頭檔案
unsigned int ODO_sec;       //1 sec before
/********************************************************************************************************/
#define SW_Throttle   14     //A0X 油門切換     /test 14
#define RL_IN         15     //A1X 右方向燈
#define LL_IN         16     //A2X 左方向燈
#define TRIP_IN3      17     //A3X 短里程符號歸零
#define SW_ECO        4      //D4X 節能符號
#define SW_mil        19     //A5X 里程切換
#define SW_MtoI       18     //A4X 公英制切換
#define SW_GEAR       2      //D2X 倒車符號
#define SW_Top_tmp    3      //D3X 上方數字切換
#define ledRX         9      //Connection 有讀取
#define led_LL        7
#define led_RL        6
#define SW_HWL           5      //D5X 警示燈
/********************************************************************************************************/
struct CAN_DIGITS_Out_1_MESSAGE{
  union{
    byte Bytes[8];
    struct{
      uint8_t TOP_Digits_H1:8;      //上方數字
      uint8_t TOP_Digits_L0:8;      //上方數字
      uint8_t Mind_Digits:8;        //中央數字
      uint8_t Down_Digits_Bytes3:8; //下方數字
      uint8_t Down_Digits_Bytes2:8; //下方數字
      uint8_t Down_Digits_Bytes1:8; //下方數字
      uint8_t Down_Digits_Bytes0:8; //下方數字
      uint8_t Bar_LV:8;             //電量等級
    }Values;
  }Data;
};

struct CAN_Indicator_Out_1_MESSAGE {   //指標
  union {
    byte Bytes[8];
    struct {
      /*--Byte 0--*/
      uint8_t GEAR_R:1;       //倒車符號
      uint8_t BRAKE_V:1;      //煞車回充符號
      uint8_t TRIP:1;         //短里程符號
      uint8_t ODO:1;          //總里程符號
      uint8_t RANGE:1;        //範圍符號
      uint8_t Recharg:1;      //充電符號
      uint8_t ECO:1;          //節能符號
      uint8_t kmh:1;          //公里/小時符號
      /*--Byte 1--*/
      uint8_t MPH:1;          //英里/小時符號
      uint8_t Tripode:1;      //腳架符號 X
      uint8_t Prevent:1;      //防暴衝符號 X
      uint8_t km:1;           //公里符號
      uint8_t mile:1;         //英里符號
      uint8_t HiSpeed:1;      //高速符號
      uint8_t POWER:1;        //能量符號
      uint8_t AVG:1;          //平均符號
      /*--Byte 2--*/
      uint8_t Percentage:1;   //百分比符號
      uint8_t kmhkwh:1;       //平均公里電耗符號
      uint8_t milekwh:1;      //平均英里電耗符號
      uint8_t LED1:1;         //定速
      uint8_t LED2:1;
      uint8_t LED3:1;
      uint8_t LED4:1;
      uint8_t LED5:1;         //BT過壓1
      /*--Byte 3--*/
      uint8_t LED6:1;         //BT過壓2
      uint8_t LED7:1;         //左方向燈
      uint8_t LED8:1;         //BT過壓3
      uint8_t LED9:1;
      uint8_t LED10:1;        //BT過溫1
      uint8_t LED11:1;        //BT過溫2
      uint8_t LED12:1;        //右方向燈
      uint8_t LED13:1;        //BT過溫3
      /*--Byte 4--*/
      uint8_t LED14:1;        //馬達過溫 (紅色)
      uint8_t LED15:1;
      uint8_t LED16:1;
      uint8_t LED17:1;
      uint8_t LED18:1;
      uint8_t LED19:1;
      uint8_t Resv46:2;
      /*--Byte 5--*/
      uint8_t Resv50:8;
      /*--Byte 6--*/
      uint8_t Resv60:8;
      /*--Byte 7--*/
      uint8_t Resv70:8;
    }Values;
  }Data;
};  

 
struct CAN_DIGITS_Out_1_MESSAGE  _CAN_DIGITS_Out_1_msg;
struct CAN_Indicator_Out_1_MESSAGE  _CAN_Indicator_Out_1_msg;
  
#define COM_Get_TOP_Digits_H1()      (_CAN_DIGITS_Out_1_msg.Data.Values.TOP_Digits_H1)
#define COM_Get_TOP_Digits_L0()      (_CAN_DIGITS_Out_1_msg.Data.Values.TOP_Digits_L0)
  
#define COM_Get_Mind_Digits()     (_CAN_DIGITS_Out_1_msg.Data.Values.Mind_Digits)
  
#define COM_Get_Down_Digits_Bytes0()     (_CAN_DIGITS_Out_1_msg.Data.Values.Down_Digits_Bytes0)
#define COM_Get_Down_Digits_Bytes1()     (_CAN_DIGITS_Out_1_msg.Data.Values.Down_Digits_Bytes1)
#define COM_Get_Down_Digits_Bytes2()     (_CAN_DIGITS_Out_1_msg.Data.Values.Down_Digits_Bytes2)
#define COM_Get_Down_Digits_Bytes3()     (_CAN_DIGITS_Out_1_msg.Data.Values.Down_Digits_Bytes3)
  
#define COM_Get_Bar_LV()      (_CAN_DIGITS_Out_1_msg.Data.Values.Bar_LV)
  
#define COM_Get_GEAR_R_Indicator()      (_CAN_Indicator_Out_1_msg.Data.Values.GEAR_R)
#define COM_Get_BRAKE_V_Indicator()     (_CAN_Indicator_Out_1_msg.Data.Values.BRAKE_V)
#define COM_Get_TRIP_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.TRIP)
#define COM_Get_ODO_Indicator()         (_CAN_Indicator_Out_1_msg.Data.Values.ODO)
#define COM_Get_RANGE_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.RANGE)
#define COM_Get_Recharg_Indicator()     (_CAN_Indicator_Out_1_msg.Data.Values.Recharg)
#define COM_Get_ECO_Indicator()         (_CAN_Indicator_Out_1_msg.Data.Values.ECO)
#define COM_Get_kmh_Indicator()         (_CAN_Indicator_Out_1_msg.Data.Values.kmh)
#define COM_Get_MPH_Indicator()         (_CAN_Indicator_Out_1_msg.Data.Values.MPH)
#define COM_Get_Tripode_Indicator()     (_CAN_Indicator_Out_1_msg.Data.Values.Tripode)
#define COM_Get_Prevent_Indicator()     (_CAN_Indicator_Out_1_msg.Data.Values.Prevent)
#define COM_Get_km_Indicator()          (_CAN_Indicator_Out_1_msg.Data.Values.km)
#define COM_Get_mile_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.mile)
#define COM_Get_HiSpeed_Indicator()     (_CAN_Indicator_Out_1_msg.Data.Values.HiSpeed)
#define COM_Get_POWER_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.POWER)
#define COM_Get_AVG_Indicator()         (_CAN_Indicator_Out_1_msg.Data.Values.AVG)
#define COM_Get_Percentage_Indicator()  (_CAN_Indicator_Out_1_msg.Data.Values.Percentage)
#define COM_Get_kmhkwh_Indicator()      (_CAN_Indicator_Out_1_msg.Data.Values.kmhkwh)
#define COM_Get_milekwh_Indicator()     (_CAN_Indicator_Out_1_msg.Data.Values.milekwh)
#define COM_Get_LED1_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.LED1)
#define COM_Get_LED2_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.LED2)
#define COM_Get_LED3_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.LED3)
#define COM_Get_LED4_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.LED4)
#define COM_Get_LED5_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.LED5)
#define COM_Get_LED6_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.LED6)
#define COM_Get_LED7_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.LED7)
#define COM_Get_LED8_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.LED8)
#define COM_Get_LED9_Indicator()        (_CAN_Indicator_Out_1_msg.Data.Values.LED9)
#define COM_Get_LED10_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.LED10)
#define COM_Get_LED11_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.LED11)
#define COM_Get_LED12_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.LED12)
#define COM_Get_LED13_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.LED13)
#define COM_Get_LED14_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.LED14) 
#define COM_Get_LED15_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.LED15)
#define COM_Get_LED16_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.LED16)
#define COM_Get_LED17_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.LED17)
#define COM_Get_LED18_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.LED18)
#define COM_Get_LED19_Indicator()       (_CAN_Indicator_Out_1_msg.Data.Values.LED19)
/********************************************************************************************************/
#define makeLong(hi,low) (((long)hi) << 16 | (low))
/********************************************************************************************************/
void pf(const char *fmt, ... )        // 簡便的工具函式，格式化輸出字串
{   
  char tmp[128];                       // resulting string limited to 128 chars
  va_list args;    
  va_start (args, fmt );    
  vsnprintf(tmp, 128, fmt, args);  
  va_end (args);   
  Serial.print(tmp);  
}
/********************************************************************************************************/
unsigned char Can_TX_count;
uint16_t Top_tmp;
uint32_t Down_tmp;
uint8_t LV_tmp;

void setup()
{   
  Serial.begin(9600);   ET.begin(details(mydata), &Serial);
  pinMode(ledRX, OUTPUT);  digitalWrite(ledRX, LOW);      //set LED OFF
  pinMode(led_LL, OUTPUT); digitalWrite(led_LL, LOW);    //set LED OFF
  pinMode(led_RL, OUTPUT); digitalWrite(led_RL, LOW);    //set LED OFF
      
  pinMode(RL_IN, INPUT_PULLUP);        pinMode(LL_IN, INPUT_PULLUP);
  pinMode(SW_mil, INPUT_PULLUP);       pinMode(SW_Throttle, INPUT_PULLUP);
  pinMode(SW_MtoI, INPUT_PULLUP);      pinMode(SW_Top_tmp, INPUT_PULLUP);
  pinMode(SW_GEAR, INPUT_PULLUP);      pinMode(SW_ECO, INPUT_PULLUP);
  pinMode(SW_HWL, INPUT_PULLUP);       pinMode(TRIP_IN3, INPUT_PULLUP);
      
  COM_Get_LED19_Indicator() = HIGH;

  TRIP_EEP = word(EEPROM.read(0), EEPROM.read(1));   //Km
  MsTimer2::set(1000, onTimer); 
  MsTimer2::start();          //設定中斷，每1000ms進入一次中斷服務程式 onTimer()
  int count = 50;
  do{ 
    CAN.init(); 
    if(CAN_OK == CAN.begin(CAN_1000KBPS)){ 
      ERROR_COLD[0] = 0; 
      break; 
    }   //Serial.println("CAN_OK");
    else 
      delay(100); 

    if (count <= 1) {
      ERROR_COLD[0] = 1;  
      break; 
    }
  }while(count--);
}

void loop()
{  
  delayMicroseconds(9800) ; //10ms  參考時脈

  ConnectionREAD(); //接收
/*======數值控制區========================================================================= */
  currentMillis = millis();
  if(currentMillis - previousMillis_500 >= interval_500)
  { 
    previousMillis_500 = currentMillis;
    Flash ^= 1;                         /*若要閃爍，把^加上去就行了(^=)*/
  }
  Monitor05();
  Serial.println("qwe");

/****** Button *******************************************************************************/

  if(!digitalRead(SW_mil) && !SW_Filter1){     // 里程切換控制
    F1 = millis();
    SW_Filter1 = 1;  
    SB_mil ++; 
    if(SB_mil > 2)
      SB_mil = 0;   
  }      //0 1 2
  else if ((millis() - F1 > 500) && SW_Filter1)
    SW_Filter1=0;
       
  if(!digitalRead(SW_Top_tmp) && !SW_Filter2){     //上方數字切換控制
    F2 = millis();
    SW_Filter2 = 1;  
    SB_Top_tmp ++; 
    if(SB_Top_tmp >= 2)
      SB_Top_tmp = 0;   
  }/* 0 1 */  
  else if ((millis() - F2 > 500) && SW_Filter2)
    SW_Filter2 = 0;

/*--------------------------------------------------------------------------------------------------------------------------*/
  MtoI = digitalRead(SW_MtoI);                 // 1=公制 0=英制 //---公英制
  bool b_ECO =! digitalRead(SW_ECO);                           // POWER/ECO        
  bool b_RL = digitalRead(RL_IN), b_LL = digitalRead(LL_IN); //方向燈
  bool b_HWL =! digitalRead(SW_HWL);                           //危險警示燈
  digitalWrite(led_RL, ((b_RL | b_HWL) & Flash) );      
  digitalWrite(led_LL, ((b_LL | b_HWL) & Flash) );
/*--------------------------------------------------------------------------------------------------------------------------*/
  COM_Get_LED12_Indicator() = ((b_RL | b_HWL) & Flash);  
  COM_Get_LED7_Indicator()  = ((b_LL | b_HWL) & Flash);
          
  COM_Get_ECO_Indicator() = b_ECO;  
  COM_Get_HiSpeed_Indicator() =! b_ECO;

  /*unit SW*/
  COM_Get_km_Indicator() = MtoI;
  COM_Get_kmh_Indicator() = MtoI;
  COM_Get_kmhkwh_Indicator() = MtoI;
  COM_Get_mile_Indicator() =! MtoI;
  COM_Get_MPH_Indicator() =! MtoI;
  COM_Get_milekwh_Indicator() =! MtoI;
        
  COM_Get_GEAR_R_Indicator() = digitalRead(SW_GEAR);        //倒車符號
  COM_Get_LED1_Indicator()   =! digitalRead(SW_Throttle);    //定速
  COM_Get_LED14_Indicator()  = bitRead(DataALL[3][1], 0);    //馬達過溫
       
  COM_Get_LED5_Indicator()  = bitRead(DataALL[10][7], 0);    //過溫1
  COM_Get_LED6_Indicator()  = bitRead(DataALL[10][7], 1);    //過溫2
  COM_Get_LED8_Indicator()  = bitRead(DataALL[10][7], 2);    //過溫3
  COM_Get_LED10_Indicator() = bitRead(DataALL[10][7], 3);    //低壓1
  COM_Get_LED11_Indicator() = bitRead(DataALL[10][7], 4);    //低壓2
  COM_Get_LED13_Indicator() = bitRead(DataALL[10][7], 5);    //低壓3
/************儀表********************************************************************/
  Show_Digits();   //面板數字轉換
  Send_area();     //CAN BUS傳送
}
/*
void  Monitor05()
{ 
  if(!digitalRead(TRIP_IN3)){
    EEPROM.write(highByte(MOT[1]), 0);
    EEPROM.write(lowByte(MOT[1]), 1); 
    TRIP_EEP = MOT[1];
  } //儲存現在里程
    
  //pf("%d %d\n",SB_mil,SB_Top_tmp);
  //pf("%d %d %d %d / %d %d\n",b_RL,b_LL,MtoI,b_ECO,SB_mil,SB_Top_tmp);
  digitalWrite(ledRX, LOW); 
  SerialMonitor();
}
*/
void  Monitor05()
{
  if(!digitalRead(TRIP_IN3)){  //若要歸零 請按壓歸零鍵
    EEPROM.write(highByte(MOT[1]), 0);
    EEPROM.write(lowByte(MOT[1]), 0); 
    TRIP_EEP = MOT[1];
  } //儲存現在里程

  digitalWrite(ledRX, LOW); 
  SerialMonitor();
}
 
void  SerialMonitor()
{    
  for (byte x=0 ; x< QuaID ;x++)
  {  
    if(Show_re[x])
    { 
      Show_re[x] =0;
      switch (x)
      {  
        case 1: pf("-PV- %d,%d,%d,%d,%d\n", PV_A[0], PV_A[1], PV_A[2], PV_A[3], PV_A[4]);            
          break;
        case 2: pf("-MOT- %d,%d,%d,%d,%d\n", MOT[0], MOT[1], MOT[2], MOT[3], MOT[4]);                
          break;
        case 3: pf("-MOT2- PV:%03dA err:%d\n",MOT_PV, DataALL[3][0]);                               
          break;
        case 10:      //MCU4_TIME
          pf("-TIME- %d/%d/%d %02d:%02d:%02d \n", DataALL[10][0], DataALL[10][1], DataALL[10][2], DataALL[10][3], DataALL[10][4], DataALL[10][5]);       
          pf("-BMS-  Al:03d% BT:%03dA/%03dV\n", DataALL[10][7],BMS_A,Vol_P);
          break; 
        default:    
          Serial.println("ERR");           
          break;
      }
    }
  }
}
