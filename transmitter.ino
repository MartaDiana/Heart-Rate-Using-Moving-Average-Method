#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
unsigned long HR[10] = {0,0,0,0,0,0,0,0,0,0};

float beatsPerMinute;
int beatAvg;

void setup()
{
  Serial.begin(9600);
  lcd.init();  
  lcd.backlight();
//  Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
//  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

void loop()
{
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute =  60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 60)
    {
     
     rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
     rateSpot %= RATE_SIZE; //Wrap variable

     //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
      beatAvg = (HR[0]+HR[1]+HR[2]+HR[3]+HR[4]+HR[5]+HR[6]+HR[7]+HR[8]+HR[9])/10.0;
        HR[9] = HR [8];
        HR[8] = HR [7];
        HR[7] = HR [6];
        HR[6] = HR [5];
        HR[5] = HR [4];
        HR[4] = HR [3];
        HR[3] = HR [2];
        HR[2] = HR [1];
        HR[1] = HR [0];
        HR[0] = beatsPerMinute;
       
    }
    
//  lcd.setCursor(0,0);
//  lcd.print("Denyut Jantung");
//  lcd.scrollDisplayLeft();
//  delay(100);
//
//  lcd.setCursor(0,1);
//  lcd.print(beatAvg);
//  lcd.scrollDisplayLeft();
//  delay(100);
  
//  Serial.print(irValue);
//  Serial.print("BPM=");
//  Serial.println(beatsPerMinute);
//  Serial.print(",beatAvg");
  Serial.print(beatAvg);
//  Serial.println("");
  }
}
