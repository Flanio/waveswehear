/*
  Melody

 Plays a melody

 circuit:
 * 8-ohm speaker on digital pin 8

 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe

This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Tone

 */
#include "pitches.h"

 int ad9850_w_clk    =0;  //P2.2口接ad9850的w_clk脚/PIN7
 int ad9850_fq_up    =0;  //P2.1口接ad9850的fq_up脚/PIN8
 int ad9850_rest     =0;  //P2.0口接ad9850的rest脚/PIN12
 int ad9850_bit_data =0;  //P1.7口接ad9850的D7脚/PIN25
 const int port_ad9850_w_clk    =9;  //P2.2口接ad9850的w_clk脚/PIN7
 const int port_ad9850_fq_up    =10;  //P2.1口接ad9850的fq_up脚/PIN8
 const int port_ad9850_rest     =11;  //P2.0口接ad9850的rest脚/PIN12
 const int port_ad9850_bit_data =0;  //P1.7口接ad9850的D7脚/PIN25

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 1000, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

//constants for distance sensor
const int TrigPin = 9;
const int EchoPin = 10;
float distance;
float T;

void setup() {
        
        Serial.begin(9600);
	//串行写1000Hz程序
        digitalWrite(port_ad9850_w_clk,LOW);
        digitalWrite(port_ad9850_fq_up,LOW);
        digitalWrite(port_ad9850_rest,LOW);
        digitalWrite(port_ad9850_bit_data,LOW);
        delayMicroseconds(10);
	ad9850_reset_serial();
	ad9850_wr_serial(0x00,500);
        // 初始化串口通信及连接SR04的引脚

        //pinMode(TrigPin, OUTPUT);
        // 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
        //pinMode(EchoPin, INPUT);
  Serial.println("over");
}

//***************************************************//
//              ad9850复位(串口模式)                 //
//---------------------------------------------------//
void ad9850_reset_serial()
{
  //ad9850_w_clk=0;
  digitalWrite(port_ad9850_w_clk,LOW);

  //ad9850_fq_up=0;
  digitalWrite(port_ad9850_fq_up,LOW);

  //rest信号
//ad9850_rest=0;
  digitalWrite(port_ad9850_rest,LOW);
  delayMicroseconds(2);
//ad9850_rest=1;
  digitalWrite(port_ad9850_rest,HIGH);
  delayMicroseconds(8);
//ad9850_rest=0;
  digitalWrite(port_ad9850_rest,LOW);
  delayMicroseconds(2);
  //w_clk信号
//ad9850_w_clk=0;
  digitalWrite(port_ad9850_w_clk,LOW);
//ad9850_w_clk=1;
  digitalWrite(port_ad9850_w_clk,HIGH);
  delayMicroseconds(2);
//ad9850_w_clk=0;
  digitalWrite(port_ad9850_w_clk,LOW);
  //fq_up信号
//ad9850_fq_up=0;
  digitalWrite(port_ad9850_fq_up,LOW);
//ad9850_fq_up=1;
  digitalWrite(port_ad9850_fq_up,HIGH);
  delayMicroseconds(2);
//ad9850_fq_up=0;
  digitalWrite(port_ad9850_fq_up,LOW);
}

//***************************************************//
//          向ad9850中写命令与数据(串口)             //
//---------------------------------------------------//
void ad9850_wr_serial(byte w0,double frequence)
{
byte i,w;
long int y;
double x;
//计算频率的HEX值
x=4294967295/125;//适合125M晶振
//如果时钟频率不为180MHZ，修改该处的频率值，单位MHz  ！！！
frequence=frequence/1000000;
frequence=frequence*x;
y=frequence;
//写w4数据
w=(y>>0);
Serial.println("first loop:");
Serial.println(w);
Serial.println(y);
//first loop
Serial.println("first loop start");
for(i=0;i<8;i++)
{
ad9850_bit_data=((w>>i)&0x01);
write(port_ad9850_bit_data,ad9850_bit_data);
//Serial.println(i);
Serial.println(ad9850_bit_data);
ad9850_w_clk=1;
ad9850_w_clk=0;
}
//写w3数据
w=(y>>8);
Serial.println("second loop:");
Serial.println(w);
Serial.println(y);
//second loop
Serial.println("second loop start");
for(i=0;i<8;i++)
{
ad9850_bit_data=(w>>i)&0x01;
//Serial.println(i);
Serial.println(ad9850_bit_data);
write(port_ad9850_bit_data,ad9850_bit_data);
ad9850_w_clk=1;
ad9850_w_clk=0;
}
//写w2数据
w=(y>>16);
Serial.println("third loop:");
Serial.println(w);
Serial.println(y);
//third loop
Serial.println("third loop start");
for(i=0;i<8;i++)
{
ad9850_bit_data=(w>>i)&0x01;
Serial.println(i);
Serial.println(ad9850_bit_data);
ad9850_w_clk=1;
ad9850_w_clk=0;
}
//写w1数据
w=(y>>24);
Serial.println("fourth loop:");
Serial.println(w);
Serial.println(y);
//fourth loop
Serial.println("fourth loop start");
for(i=0;i<8;i++)
{
ad9850_bit_data=(w>>i)&0x01;
Serial.println(i);
Serial.println(ad9850_bit_data);
ad9850_w_clk=1;
ad9850_w_clk=0;
}
//写w0数据
w=w0;
Serial.println("fifth loop:");
Serial.println(w);
Serial.println("no y!");
Serial.println("fifth loop start");
for(i=0;i<8;i++)
{
ad9850_bit_data=(w>>i)&0x01;
Serial.println(i);
Serial.println(ad9850_bit_data);
ad9850_w_clk=1;
ad9850_w_clk=0;
}
//移入始能 刷新AD9850工作状态 同时复位寄存器指针，准备下一次数据装入
//ad9850_fq_up=1;
digitalWrite(port_ad9850_fq_up,HIGH);
delayMicroseconds(10);
//ad9850_fq_up=0;
digitalWrite(port_ad9850_fq_up,LOW);
}

void write(int port,int val)
{
    if(val == 0)
      digitalWrite(port, LOW);
    else if (val == 1)
      digitalWrite(port,HIGH);
}
//***************************************************//
//                   测试程序1000Hz                  //
//---------------------------------------------------//
void test()
{
//P0=0x00;
//P1=0x00;
//P2=0x00;
//P3=0x00;
//---------------------------------------------------//
//---------------------------------------------------//
//串行写1000Hz程序
ad9850_reset_serial();
ad9850_wr_serial(0x00,1000);
//---------------------------------------------------//
}
void loop()
	{

}

void loop11() {
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
        T = map(distance,5,40,200,450);
        Serial.print("present hz:  ");
        Serial.print(T);
        Serial.print("HZ");
        Serial.println();
        if(T>1200)
        {          }
        else{
        tone(8,T,250);
        delay(2500);
        noTone(8);
        }

}
