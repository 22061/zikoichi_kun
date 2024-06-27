#include <SoftwareSerial.h>
#include<ArduinoJson.h>

StaticJsonDocument<200> doc;
SoftwareSerial IM920Serial(8, 9);    // RX, TX

char data[50];

void setup() 
{
  Serial.begin(19200);

  pinMode(A0, INPUT);        
  pinMode(A1, INPUT); 
  pinMode(10, INPUT_PULLUP);       
}

void loop() 
{
  int x_pos = map(analogRead(A0), 0, 1023, -10, 10);
  int y_pos = map(analogRead(A1), 0, 1023, -10, 10);
  
  float x = x_pos / 10.0;
  float y = y_pos / 10.0;

  // if(x == -0.1)
  // {
  //   x = 0;
  // }
  // if(y == -0.1)
  // {
  //   y = 0;
  // }

  doc["x_axis"] = x;
  doc["y_axis"] = y;
  serializeJson(doc, data, 50);

  if(digitalRead(10) == LOW)
  {
    IM920Serial.print("TXDU0002,");
    IM920Serial.println(data);
    //Serial.println(data);
  }
}
