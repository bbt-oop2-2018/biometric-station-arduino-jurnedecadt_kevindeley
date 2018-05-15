/*UNO-module (VIVES): verwerken en serieel doorsturen van sensor data naar USB
                      ontvangen van data van andere arduino via softwarematig aangelegd kanaal op digitale pinnen 10, 11
                      om menu te wijzigen en deze data te splitsen van de seriële data op de USB.
   BELANGRIJK: DIGpin 10 = RX
               DIGpin 11 = TX
               Aansluiting --> pin 10 (uno1) naar pin 11 (uno2)
                               pin 11 (uno1) naar pin 10 (uno2)
               !! Beide uno's op gelijke massa plaatsen !! 
   TEMPERATUURSENSOR: aansluiting --> 3,3V - 3,3V  !!!GEEN 5V!!!
                                      GND - GND
                                      SDA - pin A4
                                      SCL - pin A5
   ACCELEROMETER: aansluiting --> 3.3V - 3.3V !!!GEEN 5V!!!
                                  GND - GND
                                  SDA - pin A4
                                  SCL - pin A5
   HEARTRATE: aansluiting -->  Vcc - 5V
                               GND - GND
                               S(signal) - pin A0                                  
*/               

#include <LiquidCrystal.h>
#include <Wire.h>
#define I2C_ADDR 0x27
#include <SparkFunTMP102.h>
#include <SparkFun_MMA8452Q.h> 
#include <SoftwareSerial.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
TMP102 sensor0(0x48); 
MMA8452Q accel;
int incomingBytes = 0;
const int ALERT_PIN = A3;
int adc_key_in;
int Signal;  
int BeatsPM;
int Threshold = 550;
int PulseSensorPurplePin = 0;

const int HEARTRATE_CODE = 65;
const int TEMPERATURE_CODE = 66;
const int X_AXIS_CODE = 67;
const int Y_AXIS_CODE = 68;
const int Z_AXIS_CODE = 69;

SoftwareSerial menuSerial(10, 11); //(RX, TX)

void setup()
{
  Serial.begin(115200);
  menuSerial.begin(4800);
  
  pinMode(10, INPUT);
  pinMode(11, OUTPUT);  
  pinMode(ALERT_PIN,INPUT);
  
  lcd.begin(16,2);
  accel.init();
    
  sensor0.begin(); 
  sensor0.setConversionRate(3);                     // Detectiesnelheid 3 = 8Hz
  sensor0.setExtendedMode(0); 
}

void loop()
{ 
  incomingBytes = menuSerial.read();
  initiateAccelero(); 
  writeDataToSerial();
  
  if (menuSerial.available()>0) 
  {  
     incomingBytes = menuSerial.read();       
     if (incomingBytes == TEMPERATURE_CODE)
     {
      temperatureData();      
     } else if (incomingBytes == X_AXIS_CODE)
     {
      acceleroX();
     } else if (incomingBytes == Y_AXIS_CODE)
     {
      acceleroY();
     } else if (incomingBytes == Z_AXIS_CODE)
     {
      acceleroZ();
     } else if (incomingBytes == HEARTRATE_CODE)
     {
      heartRate();
     }
  } 
}
void writeDataToSerial()
{
  Signal = analogRead(PulseSensorPurplePin);
  BeatsPM = (1.0/Signal) * 60.0 * 1000;
  float temperature = sensor0.readTempC();
  Serial.print(BeatsPM + String("~"));
  Serial.print(temperature + String("~"));
  Serial.print((accel.cx, 2) + String("~"));
  Serial.print((accel.cy, 2) + String("~"));
  Serial.println(accel.cz, 2);
}

void initiateAccelero()
{
  if(accel.available())
  {
    accel.read();
    delay(50);
  }
}

void heartRate()
{
  Signal = analogRead(PulseSensorPurplePin);
  BeatsPM = (1.0/Signal) * 60.0 * 1000;  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heartrate:");
  lcd.setCursor(0, 1);
  lcd.print("BPM: ");
  lcd.setCursor(5, 1);
  lcd.print(BeatsPM);
}

void temperatureData()
{
  float temperature = sensor0.readTempC();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.setCursor(6, 1);
  lcd.print("degrees C");
}

void acceleroX()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("X-axis:"),
  lcd.setCursor(0, 1);
  lcd.print(accel.cx, 3);
}

void acceleroY()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Y-axis:"),
  lcd.setCursor(0, 1);
  lcd.print(accel.cy, 3);
}

void acceleroZ()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Z-axis:"),
  lcd.setCursor(0, 1);
  lcd.print(accel.cz, 3);
}

