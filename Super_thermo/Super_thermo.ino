

#include "DHT.h"

#define DHTPIN A0     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
#define DHTTYPE DHT11  
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
int ButtonValue = 0;
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
int INBUILT_LED=13;
int LED=10;
int BUTTON=12;
#define RELAY 7
float limit=30.00;  //variable to set the limiting temperature i.e after which circuit must stop.(Here,after limit LED will turn on.< :) > ) 
void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));//An starting message.
  pinMode(INBUILT_LED, OUTPUT);
  pinMode(LED, OUTPUT);  
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON,INPUT_PULLUP);
  dht.begin();
}
void loop() {
  // Wait a few seconds between measurements.
  digitalWrite(LED,HIGH);
  delay(800);
  ButtonValue=digitalRead(BUTTON);  
  if(ButtonValue == 0){
      digitalWrite(INBUILT_LED,HIGH);
      delay(1000);
      float prevtemp=dht.readTemperature();//stores previous temperature value
      float currtemp=0.0;   //store current temperature value i.e.temperature value while current illteration of the loop.
      
      int d=1;//variable to store whether object is hot or cold.
      int f=0; //flag variable:Since temperature sensor is slow .So This checks it temp changes in opposite direction 2 times then breaks the loop.
      int loops=1;//to store number of loops.
      //to check whether new temperature is increasing or decreasing.
      while(true){
       currtemp=dht.readTemperature();
       if(currtemp-prevtemp<0)
       d=-1;
       if(currtemp!=prevtemp)
       break;
       loops++;
       if(loops==20) 
       break;
       prevtemp=currtemp;
       delay(200);
      }
      loops=0;
      while(true){
        float currtemp=dht.readTemperature();
        Serial.println(currtemp);
        if(currtemp-prevtemp==0)
        loops++;
        else
        loops=0;
        if((d*currtemp)+(prevtemp*d*-1)<0)
        f++;
        else
        f=0;
       /*This is equivalent to-->
        if(d==1){
          if(currtemp-prevtemp<0)
          f++;
          }
        else
        { 
          if(currtemp-prevtemp>0)
          f++;
          }
        */
        if(f>1)
        {
          limit=prevtemp;
          break;
        }
        prevtemp=currtemp;
        if(loops==16)
        {limit=prevtemp;
        break;}
        delay(1000);
      }
      Serial.print(F("Limit: "));
      Serial.println((limit));
  }
   
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 // float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
 // float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
 // float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);

  /*Serial.print(F("Humidity: "));
  Serial.print(h);*/
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));  
  digitalWrite(INBUILT_LED,HIGH);
  Serial.println(F(""));
  if(t<=limit)
  {digitalWrite(RELAY,HIGH);digitalWrite(INBUILT_LED,HIGH);}
  else
  {digitalWrite(RELAY,LOW);digitalWrite(INBUILT_LED,LOW);}


  
}


