/*
 Project Name:Waves we hear

 Plays a differential sinwave sound

 circuit:
 * 16-ohm speaker on AD9850 PIN ZOUT1
 * AD9850+ARDUINO+SR04
 
 created 27 Oct 2016
 modified 30 Aug 2011
 by Jun Chen in SXKJG

This code is hosted on github.

  git@github:Flanio/waveswehear
*/

/*
DONE:AD9850+ARDUINO+SR04
Arduino控制AD9850输出特定频率正弦波，频率根据SR04传回的值处理后获得，输出电压0-1V之间
TODO:1默认状态下，频率输出值应设置为多少？ 理论上应该关闭端口输出，通过控制控制字节实现
需要查手册，如何使用参考SetFrequency()方法。
     2通过什么样的方法来做用户接口？  怎样开启功能？自动关闭功能时间设定为多少？
     3功率放大器选择？
     4显示？
*/

  //AD9850 DDS test
  //AD9850  for Arduino+DDS9850
  #define DDS_CLOCK 125000000

  #define  CLOCK  8  //pin connections for DDS
  #define  LOAD 9
  #define  DATA  10
  #define  RESET 11

  //constants for distance sensor
  const int TrigPin = 12;
  const int EchoPin = 13;
  float distance;
  float T;

  int f=10; //initial FREQ f=10;    默认状态下频率多少？ 关闭输出？
  int sensorPin = A0;
  int sensorValue = 0;

void setup() {
  // 初始化串口通信及连接SR04的引脚
     Serial.begin(9600);
  // 设置端口模式
  // AD9850 DATA 串口数据输出 CLOCK时钟信号 LOAD FQ_UD 数据锁 RESET 重置位
  pinMode (DATA,  OUTPUT);
  pinMode (CLOCK, OUTPUT);
  pinMode (LOAD,  OUTPUT);
  pinMode (RESET, OUTPUT);
  // SR04
  pinMode(TrigPin, OUTPUT);
  // 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
  pinMode(EchoPin, INPUT);
  //测试数据输出到串口
  Serial.println("Ultrasonic sensor:");
  //初始化AD 9850
  AD9850_init();
  AD9850_reset();

  SetFrequency(f);
}

void loop() {
  sensorValue=analogRead(sensorPin);
  Serial.println(sensorValue);
  //delay(1);        // delay in between reads for stability

  // 产生一个10us的高脉冲去触发TrigPin
        digitalWrite(TrigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(TrigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(TrigPin, LOW);
  // 检测脉冲宽度，并计算出距离
        distance = pulseIn(EchoPin, HIGH) / 58.00;
        Serial.print(distance);
        Serial.print("cm");
        Serial.println();
        T = map(distance,5,15,200,3000);
        Serial.print("present hz:  ");
        Serial.print(T);
        Serial.print("HZ");
        Serial.println();
        if(T>20000)
        {          }
        else if(T<0)
        {          }
        else{
        //tone(8,T,250);  tone(pin, frequency, duration)
	SetFrequency(T);
        delay(100);  //采样时间间隔  ms
        //noTone(8);
        }

}

  //设置频率
void SetFrequency(unsigned long frequency)
{
  //tuning word:频率控制字
  unsigned long tuning_word = (frequency * pow(2, 32)) / DDS_CLOCK;
  digitalWrite (LOAD, LOW);

  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 8);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 16);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 24);
  shiftOut(DATA, CLOCK, LSBFIRST, 0x0);  //状态字节  可关闭输出
  digitalWrite (LOAD, HIGH);
}

  //初始化AD9850
void AD9850_init()
{

  digitalWrite(RESET, LOW);
  digitalWrite(CLOCK, LOW);
  digitalWrite(LOAD, LOW);
  digitalWrite(DATA, LOW);
}

  //AD9850 重置
void AD9850_reset()
{
  //reset sequence is:重设条件是
  // CLOCK & LOAD = LOW  时钟信号和数据锁定装入信号同时为低电平
  //  Pulse RESET high for a few uS (use 5 uS here) RESET高电平保持5uS
  //  Pulse CLOCK high for a few uS (use 5 uS here) CLOCK高电平保持5us
  //  Set DATA to ZERO and pulse LOAD for a few uS (use 5 uS here)
  // 数据输出口置零，并且LOAD FQ_UD 输出高电平保持5uS
  // data sheet diagrams show only RESET and CLOCK being used to reset the device, but I see no output unless I also
  // toggle the LOAD line here.

  digitalWrite(CLOCK, LOW);
  digitalWrite(LOAD, LOW);  //LOAD LOW

  digitalWrite(RESET, LOW);
  delay(5);
  digitalWrite(RESET, HIGH);  //pulse RESET
  delay(5);
  digitalWrite(RESET, LOW);
  delay(5);

  digitalWrite(CLOCK, LOW);
  delay(5);
  digitalWrite(CLOCK, HIGH);  //pulse CLOCK
  delay(5);
  digitalWrite(CLOCK, LOW);
  delay(5);
  digitalWrite(DATA, LOW);    //make sure DATA pin is LOW
  //VERY VERY IMPORTANT !!  CORE STEP!!
  digitalWrite(LOAD, LOW);
  delay(5);
  digitalWrite(LOAD, HIGH);  //pulse LOAD
  delay(5);
  digitalWrite(LOAD, LOW);
  // Chip is RESET now
}
