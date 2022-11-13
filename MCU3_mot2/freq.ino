void freq()//硬體中斷
{
  ledState =! ledState; 
  count++;              //用於里程數計算
  tick++;
}

void onTimer() //計時中斷
{ 
  cycle_mic = tick;  
  tick = 0; 
}
