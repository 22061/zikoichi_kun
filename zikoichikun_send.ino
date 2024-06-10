#include <SoftwareSerial.h>
#include<ArduinoJson.h>

StaticJsonDocument<200> doc;
SoftwareSerial IM920Serial(8, 9);    // RX, TX

char data[50];
char input[30];
int i = 0;

void setup() {
  pinMode(A0, INPUT);        
  pinMode(A1, INPUT);        
  //pinMode(5, INPUT_PULLUP);

  Serial.begin(19200);
  IM920Serial.begin(19200);  
  IM920Serial.print("TXDU0002,");
  IM920Serial.println(data);
}

void loop() {
  int x_pos = map(analogRead(A0), 0, 1023, -10, 10);
  int y_pos = map(analogRead(A1), 0, 1023, -10, 10);
  //int sw_pos = !digitalRead(5);
  
  float x = x_pos / 10.0;
  float y = y_pos / 10.0;

  if(x == -0.1)
  {
    x = 0;
  }
  if(y == -0.1)
  {
    y = 0;
  }

  doc["x_axis"] = x;
  doc["y_axis"] = y;
  serializeJson(doc, data, 50);

  if (IM920Serial.available()) 
  {
    input[i] = IM920Serial.read();
    if (input[i] == '\n')
    {
      IM920Serial.print("TXDU0002,");
      IM920Serial.println(data);
      //Serial.println(data);
      i = 0;
    }
    else
    {
        i++;
    }
  }
}