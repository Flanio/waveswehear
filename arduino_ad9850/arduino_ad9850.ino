//AD9850 DDS test
//AD9850  for Arduino+DDS9850
#define DDS_CLOCK 125000000

#define  CLOCK  8  //pin connections for DDS
#define  LOAD 9 
#define  DATA  10
#define  RESET 11

int sensorPin = A0;
int sensorValue = 0;

int key1=A4;   //4 keys to setup frequency
int key2=A5;
int key3=A6;
int key4=A7;
int keynumber;
int f=400;    //initial f=10;
void setup()
{
  Serial.begin(9600);
  pinMode (DATA,  OUTPUT); 
  pinMode (CLOCK, OUTPUT); 
  pinMode (LOAD,  OUTPUT); 
  pinMode (RESET, OUTPUT); 
  pinMode(key1,INPUT);
  pinMode(key2,INPUT);
  pinMode(key3,INPUT);
  pinMode(key4,INPUT);
  AD9850_init();
  AD9850_reset();

  SetFrequency(f);

}

void loop()
{  
  sensorValue=analogRead(sensorPin);
  Serial.println(sensorValue);
  //delay(1);        // delay in between reads for stability
  if(analogRead(key1)>1000)
  {
    delay(20);
    if(analogRead(key1)>1000){ f=f+10;SetFrequency(f);}
  }
  if(analogRead(key2)>1000)
  {
      delay(20);
    if(analogRead(key2)>1000&&f>10){ f=f-10;SetFrequency(f);}
  }
    if(analogRead(key3)>1000)
  {
      delay(20);
    if(analogRead(key3)>1000){ f=f+1000;SetFrequency(f);}
  }
    if(analogRead(key4)>1000)
  {
      delay(20);
    if(analogRead(key4)>1000&&f>1000){ f=f-1000;SetFrequency(f);}
  }
}

void SetFrequency(unsigned long frequency)
{
  unsigned long tuning_word = (frequency * pow(2, 32)) / DDS_CLOCK;
  digitalWrite (LOAD, LOW); 

  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 8);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 16);
  shiftOut(DATA, CLOCK, LSBFIRST, tuning_word >> 24);
  shiftOut(DATA, CLOCK, LSBFIRST, 0x0);
  digitalWrite (LOAD, HIGH); 
}

void AD9850_init()
{

  digitalWrite(RESET, LOW);
  digitalWrite(CLOCK, LOW);
  digitalWrite(LOAD, LOW);
  digitalWrite(DATA, LOW);
}

void AD9850_reset()
{
  //reset sequence is:
  // CLOCK & LOAD = LOW
  //  Pulse RESET high for a few uS (use 5 uS here)
  //  Pulse CLOCK high for a few uS (use 5 uS here)
  //  Set DATA to ZERO and pulse LOAD for a few uS (use 5 uS here)

  // data sheet diagrams show only RESET and CLOCK being used to reset the device, but I see no output unless I also
  // toggle the LOAD line here.

  digitalWrite(CLOCK, LOW);
  digitalWrite(LOAD, LOW);

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

    digitalWrite(LOAD, LOW);
  delay(5);
  digitalWrite(LOAD, HIGH);  //pulse LOAD
  delay(5);
  digitalWrite(LOAD, LOW);
  // Chip is RESET now
}
