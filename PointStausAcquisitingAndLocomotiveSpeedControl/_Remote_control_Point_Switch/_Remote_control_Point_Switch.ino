//Edited by Shreesh Kolhatkar and Husain Sakarwala

#include <IRremote.h>
#include <Servo.h>

const byte IR_RECEIVE_PIN = 2;
Servo s1;
#define LED_RED 8
#define LED_GREEN 12

void setup()
{
   Serial.begin(115200);
   Serial.println("IR Receive test");
   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
   pinMode(LED_RED, OUTPUT);
   pinMode (LED_GREEN, OUTPUT);
   s1.attach(10);

   }

void loop()
{  int servoPosition = s1.read();

   if (IrReceiver.decode())
   {
      String ir_code = String(IrReceiver.decodedIRData.command, HEX);
      Serial.println(ir_code);    

       if(ir_code == "c1"){
        s1.write(68);
        if(servoPosition == 68){          
          digitalWrite(LED_RED, HIGH);
        }
       }        
      else{
            digitalWrite(LED_RED, LOW);
          }
            
          
        
      if(ir_code == "99"){
        s1.write(0);
         if(servoPosition == 0){
          digitalWrite(LED_GREEN, HIGH);}
                           }
      else{
            digitalWrite(LED_GREEN, LOW);
      }

      if(ir_code == "ce"){ //Setting up a condition where point & signal aren't synchronised
        s1.write(68);
        if(servoPosition == 68){          
          digitalWrite(LED_GREEN, HIGH);
        }
       }                     
      }
       

      IrReceiver.resume();
   }
   
