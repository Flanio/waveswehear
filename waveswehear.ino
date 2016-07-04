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
  // 初始化串口通信及连接SR04的引脚
        Serial.begin(9600); 
        pinMode(TrigPin, OUTPUT); 
    // 要检测引脚上输入的脉冲宽度，需要先设置为输入状态
        pinMode(EchoPin, INPUT); 
       Serial.println("Ultrasonic sensor:");
}

void loop() {
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
