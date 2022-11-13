/*    用於霍爾型油門踏板    */
#include <X9C.h>  //匯入數位電位器函式庫

#define accelerator_1  A0 // 訊號1
#define accelerator_2  A1 // 訊號2
#define DP_feedback    A2 // 回授電位器訊號做確認
#define UD  3
#define INC  4
#define CS   5

X9C pot;

void setup() {
  pot.begin(CS, INC, UD);
  Serial.begin(9600);
  pinMode(accelerator, INPUT);
  pinMode(accelerator_2, INPUT);

  pot.setPotMin(true);    // 電位器歸零
}

void loop() {
  int accelerator_value_1 = analogRead(accelerator_1);
  int accelerator_value_2 = analogRead(accelerator_2);
  int DP = analogRead(DP_feedback);

  accelerator_value -= 148;   // 去除油門踏板的初始電壓

  int DP_value = accelerator_value / 6; // 將油門訊號切割成電位器的可變電阻數量

  //DP_value = constrain(DP_value, 0, 100);

  pot.setPot(DP_value, false);
  
  Serial.print(accelerator_value);
  Serial.print("\t");
  Serial.print(DP_value);
  Serial.print("\t");
  Serial.println(DP);
  
  delayMicroseconds(100);
}
