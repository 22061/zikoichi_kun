#include <ArduinoJson.h>
#include<SoftwareSerial.h>

#ifndef ZIKOICHI_KUN_API_HPP_
#define ZIKOICHI_KUN_API_HPP_

namespace zk_api {
  inline void setup(void){
    pinMode(2,OUTPUT); // CW.CCW-1 FR
    pinMode(4,OUTPUT); // CW.CCW-2 FL
    pinMode(7,OUTPUT); // CW.CCW-3 RL
    pinMode(12,OUTPUT); // CW.CCW-4 RR
  }

  inline void front_right_pwm(int value){
    analogWrite(3,value);
  }
  inline void front_left_pwm(int value){
    analogWrite(5,value);
  }
  inline void rear_left_pwm(int value){
    analogWrite(6,value);
  }
  inline void rear_right_pwm(int value){
    analogWrite(10,value);
  }

  inline void front_right_cw_ccw(int status){
    digitalWrite(2,status);
  }
  inline void front_left_cw_ccw(int status){
    digitalWrite(4,status);
  }
  inline void rear_left_cw_ccw(int status){
    digitalWrite(7,status);
  }
  inline void rear_right_cw_ccw(int status){
    digitalWrite(12,status);
  }

  inline void control_front_right(int power){
    if(power > 255){power = 255;}
    if(power < -255){power = -255;}

    front_right_pwm(abs(power));
    
    if(power > 0){
      front_right_cw_ccw(HIGH);
    }else{
      front_right_cw_ccw(LOW);
    }
  }
  inline void control_front_left(int power){
    if(power > 255){power = 255;}
    if(power < -255){power = -255;}

    front_left_pwm(abs(power));
    
    if(power > 0){
      front_left_cw_ccw(HIGH);
    }else{
      front_left_cw_ccw(LOW);
    }
  }
  inline void control_rear_left(int power){
    if(power > 255){power = 255;}
    if(power < -255){power = -255;}

    rear_left_pwm(abs(power));
    
    if(power > 0){
      rear_left_cw_ccw(HIGH);
    }else{
      rear_left_cw_ccw(LOW);
    }
  }
  inline void control_rear_right(int power){
    if(power > 255){power = 255;}
    if(power < -255){power = -255;}

    rear_right_pwm(abs(power));
    
    if(power > 0){
      rear_right_cw_ccw(HIGH);
    }else{
      rear_right_cw_ccw(LOW);
    }
  }

  inline void xyz_control(float x, float y, float z, float power){
    int vec_fr =  (int)(power * ( 0.707106781 * x - 0.707106781 * y + 0.5 * z));
    int vec_fl =  (int)(power * (-0.707106781 * x - 0.707106781 * y + 0.5 * z));
    int vec_rl =  (int)(power * (-0.707106781 * x + 0.707106781 * y + 0.5 * z));
    int vec_rr =  (int)(power * ( 0.707106781 * x + 0.707106781 * y + 0.5 * z));

    vec_fr *= power;
    vec_fl *= power;
    vec_rl *= power;
    vec_rr *= power;

    if(vec_fr > 255){vec_fr = 255;}
    if(vec_fl > 255){vec_fl = 255;}
    if(vec_rl > 255){vec_rl = 255;}
    if(vec_rr > 255){vec_rr = 255;}

    if(vec_fr < -255){vec_fr = -255;}
    if(vec_fl < -255){vec_fl = -255;}
    if(vec_rl < -255){vec_rl = -255;}
    if(vec_rr < -255){vec_rr = -255;}

    control_front_right(vec_fr);
    control_front_left(vec_fl);
    control_rear_left(vec_rl);
    control_rear_right(vec_rr);
  }
}

StaticJsonDocument<200> doc;
SoftwareSerial IM920Serial(8, 9);  //RX, TX

char input[50];
int i = 0;  
String data;
float x1 = 0;
float y1 = 0;

void setup() {
  Serial.begin(19200);
  IM920Serial.begin(19200);
  zk_api::setup();
}

void loop()
{
  if (IM920Serial.available()) 
  {
    input[i] = IM920Serial.read();
    if (input[i] == '\n') 
    {
      input[i] = '\n';
      //Serial.print(input);
      i = 0;
    }
    else
    {
        i++;
    }

    data = input;
    data = data.substring(11);

    DeserializationError error = deserializeJson(doc, data);

    x1=doc["x_axis"];
    y1=doc["y_axis"];
    // Serial.print(x1);
    // Serial.print(",");
    // Serial.println(y1);
    zk_api::xyz_control(x1, y1, 0.0, 50);
  }
}
#endif