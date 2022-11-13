#include <DS3231.h>
DS3231  rtc(SDA, SCL);
Time  t;
unsigned int time_YM, time_DT, time_ms;
//********************************************************************************************************
#define interval  100
unsigned long previousMillis, currentMillis;
//********************************************************************************************************
#include <EasyTransfer.h>
EasyTransfer ET;  
struct data_STRUCTURE 
{ 
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
};
data_STRUCTURE mydata;
bool ConR; 
byte ID_t;
byte inDATA[10];
#define ledRX 9 //Connection 有讀取
/********************************************************************************************************/
#include <SPI.h>
#include "df_can.h"
const int SPI_CS_PIN = 10;
MCPCAN CAN(SPI_CS_PIN);     // Set CS pin
/********************************************************************************************************/
void pf(const char *fmt, ... ) // 簡便的工具函式，格式化輸出字串
{
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;    
  va_start (args, fmt );    
  vsnprintf(tmp, 128, fmt, args);  
  va_end (args);    
  Serial.print(tmp);  
}
/********************************************************************************************************/
bool CANfail, CANonline;
byte Alert, BT_curr, BTpercen; 
#define QuaID 16
byte DataALL[QuaID][10];
bool Show_re[QuaID];      //Show_record
byte Data03[10];

void setup()
{ 
  Serial.begin(9600); 
  rtc.begin(); //while (!Serial) {}
  pinMode(ledRX, OUTPUT);  
  digitalWrite(ledRX, LOW); //set LED OFF
  ET.begin(details(mydata), &Serial);
  int count = 50;  
  do { 
    CAN.init();  
    if(CAN_OK == CAN.begin(CAN_500KBPS))
    {   
      CANfail = 0; 
      break;
    }
    else    
      delay(100); 
    if (count <= 1)
    {
      CANfail = 1; 
      break;
    }
  }while(count--);
  //rtc.setDOW(WEDNESDAY);       // Set Day-of-Week to SUNDAY
  //rtc.setTime(21, 19, 00);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(30, 5, 2021);    // Set the date to January 1st, 2014
  bitWrite( Data03[1], 0, CANfail);
}

void loop()
{ 
  BMS_READ();
  ConnectionREAD();
  
  currentMillis = millis();
  t = rtc.getTime(); //time_YM= ((t.year)%100)*100+ t.mon;  time_DT= (t.date)*100+ t.hour;  time_ms= (t.min)*100+ t.sec;Serial.println(time_YM); Serial.println(time_DT); Serial.println(time_ms);  
  if(currentMillis - previousMillis >= interval) 
  { 
    previousMillis = currentMillis;  
    CANonline = 0; 
    digitalWrite(ledRX, LOW);  
    byte DA[10]; 
    DA[0] = (t.year) % 100; 
    DA[1] = t.mon;
    DA[2] = t.date; 
    DA[3] = t.hour; 
    DA[4] = t.min; 
    DA[5] = t.sec; 
    DA[6] = 0; 
    DA[7] = Alert; 
    DA[8] = BT_curr; 
    DA[9] = BTpercen;
    //pf("%02d/%02d/%02d %02d:%02d:%02d \n",DA[0],DA[1],DA[2],DA[3],DA[4],DA[5]);
    Connection_code (10, DA); 
    delay(5);
    ConnectionWRITE();
  }
  delay (5);
}
