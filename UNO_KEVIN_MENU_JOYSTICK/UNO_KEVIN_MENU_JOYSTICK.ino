/*UNO-module (VIVES): ontvangen van data van andere arduino via softwarematig aangelegd kanaal op digitale pinnen 10, 11
                      om menu te wijzigen en deze data te splitsen van de seriële data op de USB.
   BELANGRIJK: DIGpin 10 = RX
               DIGpin 11 = TX
               Aansluiting --> pin 10 (uno1) naar pin 11 (uno2)
                               pin 11 (uno1) naar pin 10 (uno2)
               !! Beide uno's op gelijke massa plaatsen !!
  JOYSTICK: aansluiting --> GND - GND
                            5V - 5V
                            VRx - pin 0
  LCD : aansluiting --> GND - GND
  |||I2C communicatie|||Vcc - 5V
                        SDA - SDA
                        SCL - SCL              
*/               
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define I2C_ADDR 0x27
#include <SoftwareSerial.h>

//                         I2C,RS,RW,EN,D4,D5,D6,D7,BL,ACTIVE)
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

char heartRateMenu [1] = "A";
char tempMenu [1] = "B";
char xMenu [1] = "C";
char yMenu [1] = "D";
char zMenu [1] = "E";
int menuPosition = 0;

SoftwareSerial menuSerial(10, 11); //(RX, TX)

const int X_pin = 0; // analog pin connected to X output

//int xValue = analogRead(X_pin);

void setup()
{ 
 lcd.begin(16,2);
 //lcd.clear();
 Serial.begin(9600);
 menuSerial.begin(4800);
}

void loop()
{
  int xValue = analogRead(X_pin);

  if (xValue <100)
  {
     menuPosition -= 1;
     if (menuPosition < 0)
     {
        menuPosition = 4; 
     }
  } else if (xValue >900)
  {
     menuPosition += 1;
     if (menuPosition > 4)
     {
        menuPosition = 0;
      }
  }
  
  switch (menuPosition)
  {
    case 0:
      heartrate();
      menuSerial.write(heartRateMenu, 1);      
    break;
    case 1:  
      temperature();
      menuSerial.write(tempMenu, 1);
    break;
    case 2:
      xAxis();
      menuSerial.write(xMenu, 1);
    break;
    case 3:
      yAxis();
      menuSerial.write(yMenu, 1);
    break;
    default:
      zAxis();
      menuSerial.write(zMenu, 1); 
    break;
  }
}

void heartrate()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("< Z-Axis");
  lcd.setCursor(3, 1);
  lcd.print("Temperature");
  lcd.setCursor(15, 1);
  lcd.print(">");
  delay(400);
}
void temperature()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("< Heartrate");
  lcd.setCursor(3, 1);
  lcd.print("X-Axis");
  lcd.setCursor(15, 1);
  lcd.print(">");
  delay(400);
}
void xAxis()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("< Temperature");
  lcd.setCursor(3, 1);
  lcd.print("Y-Axis");
  lcd.setCursor(15, 1);
  lcd.print(">");
  delay(400);
}
void yAxis()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("< X-Axis");
  lcd.setCursor(3, 1);
  lcd.print("Z-Axis");
  lcd.setCursor(15, 1);
  lcd.print(">");
  delay(400);
}
void zAxis()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("< Y-Axis");
  lcd.setCursor(3, 1);
  lcd.print("Heartrate");
  lcd.setCursor(15, 1);
  lcd.print(">");
  delay(400);
}
