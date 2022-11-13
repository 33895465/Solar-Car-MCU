#define max_curr 8772     // 8772=5/0.0057*10;//馬達電流
#define maxPV_curr 749    // 74965=5/0.0667*10
#define maxPV_curr2 375   //749/2
#define interval  100
#define sensorPinA A6
#define sensorPinV A7
#define sensorPV_A A0
#define MOT_overheat 6
/******************************************************/
#define ledPin 4          //built-in LED
#define intPin 3
#define ledRX 13
#define PoworLOW 9
#define mileageRST 5      //IO 
/************************************************************************************/
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
};data_STRUCTURE mydata;
bool ConR; 
byte ID_t; 
byte inDATA[10];
/********************************************************************************************************/
unsigned int Mrpm;
unsigned long previousMillis, currentMillis;        // will store last time LED was updated
/********************************************************************************************************/
#include <Wire.h>
#include <I2CEEPROM.h>
#define CHIP_ADDRESS 0x50
#define current_address 0
I2CEEPROM i2c_eeprom(CHIP_ADDRESS);     // Create I2C EEPROM instance
unsigned int data32, StorageOld;
byte data_to_send[4], Bo[10];
unsigned long mileageEEPR, mil_m, SW_Q, previousEEPR;
bool b_ERR[8];
bool Storage = 1, turn_off;
byte no01;
unsigned int mil_meter_km, mil_meter_m ;
/********function**************************************/
#define highWord(w) ((w)>>16)
#define lowWord(w) ((w) & 0xffff)
#define makeLong(hi,low) (((long)hi) << 16|(low))
/*****************************************************/
#include <MsTimer2.h>     //定時器庫的標頭檔案
volatile boolean ledState=LOW;          //initial value of switch pin
volatile unsigned long count, cycle_mic;
volatile unsigned int tick;             //tick (Hz)
/********************************************************************************************************/
void pf(const char *fmt, ... ){   // 簡便的工具函式，格式化輸出字串
  char tmp[128];                  // resulting string limited to 128 chars
  va_list args;    
  va_start (args, fmt );    
  vsnprintf(tmp, 128, fmt, args);  
  va_end (args);    
  // Serial.print(tmp);  
}
/********************************************************************************************************/
void setup() {
  MsTimer2::set(500, onTimer); 
  MsTimer2::start();          //設定中斷，每500ms進入一次中斷服務程式 onTimer()
  
  Serial.begin(9600); ET.begin(details(mydata), &Serial);
  pinMode(ledPin, OUTPUT);  digitalWrite(ledPin, ledState); //set LED OFF
  pinMode(ledRX, OUTPUT);   digitalWrite(ledRX, LOW);        //set LED OFF 
  pinMode(MOT_overheat, INPUT_PULLUP);
  pinMode(intPin, INPUT_PULLUP);   
  pinMode(PoworLOW, INPUT_PULLUP);  
  pinMode(mileageRST, INPUT_PULLUP);
  attachInterrupt(1, freq, FALLING); //assign int0
  mileageEEPR = eepromRead(0);
  //pf("EEPR: %dm\n",mileageEEPR);//公尺

}

void loop() {
  ConnectionREAD();
  //if(count != 0)
    mil_m = count / 28.828 + mileageEEPR;     //公尺  point/48*0.53*PI;//里程數 //0.53是輪胎直徑(unit:m)
  //mil_meter = (count + mileageEEPR);  //mil_meter = currentMillis / 10;
//*****************************************************
  currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {  
    previousMillis = currentMillis; 
    unsigned int motA = map(analogRead(sensorPinA),0,1023,0,max_curr);
    unsigned int MainV = map(analogRead(sensorPinV),0,1023,0,2000);//總電壓  
    unsigned int M_PV = map(analogRead(sensorPV_A),0,1023,0,maxPV_curr) ;//Serial.println(M_PV);
    if (M_PV > maxPV_curr2)
      M_PV = M_PV - maxPV_curr2;
    else
      M_PV = 0; 
    //Serial.println(lowByte(M_PV));
    Bo[8]=highByte(M_PV);     Bo[9]=lowByte(M_PV);
    b_ERR[0] = !digitalRead(MOT_overheat);
    bitWrite(Bo[0],0,b_ERR[0]);
    bitWrite(Bo[0],1,turn_off);
    Mrpm = cycle_mic * 2.5;     //2.5=*2*60/48 (rpm) rpm*0.53*PI*60/1000 (km/hr)      //轉速修訂
    Serial.println(Mrpm);
    
    mil_meter_km = mil_m /1000;
    mil_meter_m = mil_m % 1000;
    //pf("%03d.%03d m\n",mil_meter_km,mil_meter_m);//公尺

    Code_002( Mrpm , mil_meter_km , mil_meter_m , motA , MainV);//Serial.println( mil_meter);
    delay(10);
    /*
    for (byte i=0 ; i< 10 ;i++) { 
      Serial.print(Bo[i]);
      Serial.print(",");
    }
    Serial.println()
    */
    Connection_code(3, Bo);
    delay(10);
    if(ConR){
      ConR =0;  
      Connection_code  (ID_t, inDATA);
    }
    /*
    if(ID_t=1){
      no01=0;
    }
    else if(no01<10){
      no01++;
    }
    */

    /*
    Serial.println(AVGcountN);
    Serial.print(cycle_mic/1000);
    Serial.print("sec ");
    Serial.print("Hz: ");
    Serial.println(1000000/cycle_mic);
    Serial.println(AVGcountN);
    */
  }
//*****************************************************    
  digitalWrite(ledPin, ledState); //set LED OFF
  digitalWrite(ledRX, ConR);      //RS485 READ
  Store_mileage();                //儲存 里程
  /*
  if(!digitalRead(intPin)){
    Serial.println("2");
    digitalWrite(ledPin, 1);
  }
  else
    digitalWrite(ledPin, 0);
  */
  delay(5);
  
}

byte Bo2By(boolean B[8])//Bit to Long
{ 
  byte BY = 0;  
  for(byte i = 0 ; i < 8 ; i++){
    BY = BY << 1;
    BY = BY | B[7 - i];
  }  
  return BY; 
}
