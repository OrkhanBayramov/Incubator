#include <LiquidCrystal.h>

#include "DHT.h"

#define DHTPIN 38
#define DHTTYPE DHT21
DHT dht(DHTPIN, DHTTYPE); 


const int rs = 10, en = 9, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


long THpreviusMillis = 0;
long THinterval = 2000;
long SPpreviusMillis = 0;
long SPinterval = 5000;
long MRpreviusMillis = 0;
long MRinterval = /*14400000*/ 15000;

int h;
int t;
int selected_temp;
int selected_hum = 45;
int door = 0;
int auto_control = 0;
int up_mikrik_state = 0;
int down_mikrik_state = 0;
int manual_control_up = 0;
int manual_control_down = 0;
int bt1 = 0;
int bt2 = 0;
int bt3 = 0;
int bt4 = 0;
int bt5 = 0;

const int circular_fan_relay = 22;
const int cooler_fan_relay = 24;
const int motor_up_relay1 = 26;
const int motor_up_relay2 = 28;
const int motor_down_relay1 = 30;
const int motor_down_relay2 = 32;
const int heater_light_relay = 34;
const int hummidier_relay = 36;

const int mikrik_up_state = 23;
const int mikrik_down_state = 25;
const int mikrik_door_state = 27;
const int m_up_control_button = 29;
const int m_down_control_button = 31;
const int control_button = 33;

const int b1 = 52;
const int b2 = 50;
const int b3 = 48;
const int b4 = 46;



boolean door_mikrik=LOW, up_mikrik=LOW, down_mikrik=LOW, water_sensor=LOW, 
heater_light=LOW, hummidier= HIGH, circular_fan=LOW, cooler_fan=LOW , manual_control=LOW;

void setup() {
  pinMode(circular_fan_relay, OUTPUT);
  pinMode(cooler_fan_relay, OUTPUT);
  pinMode(motor_up_relay1, OUTPUT);
  pinMode(motor_up_relay2, OUTPUT);
  pinMode(motor_down_relay1, OUTPUT);
  pinMode(motor_down_relay2, OUTPUT);
  pinMode(heater_light_relay, OUTPUT);
  pinMode(hummidier_relay, OUTPUT);
  pinMode(mikrik_up_state, INPUT);
  pinMode(mikrik_down_state, INPUT);
  pinMode(mikrik_door_state, INPUT);
  pinMode(m_down_control_button, INPUT);
  pinMode(m_up_control_button, INPUT);
  pinMode(control_button, INPUT);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b4, INPUT);


  digitalWrite(circular_fan_relay, HIGH);
  digitalWrite(cooler_fan_relay, HIGH);
  digitalWrite(motor_up_relay1, HIGH);
  digitalWrite(motor_up_relay2, HIGH);
  digitalWrite(motor_down_relay1, HIGH);
  digitalWrite(motor_down_relay2, HIGH);
  digitalWrite(heater_light_relay, HIGH);
  digitalWrite(hummidier_relay, HIGH);
  
  Serial.begin(9600); 
  
  dht.begin();

  lcd.begin(20, 4);

}


void reading_sensors(){

    bt5 = digitalRead(control_button);

    manual_control_up = digitalRead(m_up_control_button);
    manual_control_down = digitalRead(m_down_control_button);
    up_mikrik_state = digitalRead(mikrik_up_state);
    down_mikrik_state = digitalRead(mikrik_down_state);
    door = digitalRead(mikrik_door_state);
    
    bt1 = digitalRead(b1);
    delay(60);
    bt2 = digitalRead(b2);
    delay(60);
    bt3 = digitalRead(b3);
    delay(60);
    bt4 = digitalRead(b4);
    delay(60);

  

  unsigned long currentMillis = millis();
  if (currentMillis - THpreviusMillis > THinterval) {
    THpreviusMillis = currentMillis;
    h = dht.readHumidity();
    t = dht.readTemperature();
  }

     if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
  }
  
}

void print_lcd(){

//----------------------------- TEMPERATURE MONITORING --------------------------//
  unsigned long SPcurrentMillis = millis();
     if (SPcurrentMillis - SPpreviusMillis > SPinterval) {
      SPpreviusMillis = SPcurrentMillis;
      Serial.print("Rütubət: ");
      Serial.print(h);
      Serial.print(" %\t");
      Serial.print("Temperatur: ");
      Serial.print(t);
      Serial.println(" *C ");
    //----------------------------------------
     }
      lcd.setCursor(0, 0);
      lcd.print("Faktiki- ");
      
      lcd.setCursor(9, 0);
      lcd.print("R: ");
      lcd.setCursor(12, 0);
      lcd.print(h);
      
      lcd.setCursor(15, 0);
      lcd.print("T: ");
      lcd.setCursor(18, 0);
      lcd.print(t);
    //---------------------------------------
      
      lcd.setCursor(0, 2);
      lcd.print("Verilen- ");
      
      lcd.setCursor(9, 2);
      lcd.print("R: ");
      lcd.setCursor(12, 2);
      lcd.print(selected_hum);
      
      lcd.setCursor(15, 2);
      lcd.print("T: ");
      lcd.setCursor(18, 2);
      lcd.print(selected_temp);
}

void climate_control(){
//--------------------------- SET CLIMAT-------------------------------//
    
      if (bt1 == HIGH){
          selected_temp++;        
      }
     
      if (bt2 == HIGH){
          selected_temp--;        
      }

      if (bt3 == HIGH){
          selected_hum++;        
      }
     
      if (bt4 == HIGH){
          selected_hum--;        
      }

//------------------------------ TEMPERATURE CONTROL --------------------------//

     if (door == HIGH){

          if (selected_temp > t) {
              digitalWrite(heater_light_relay, LOW);
              digitalWrite(circular_fan_relay, LOW);
          }

          if (selected_temp < t){
              digitalWrite(heater_light_relay, HIGH);
              digitalWrite(circular_fan_relay, HIGH);
          }

//------------------------------ HUMMIDITY CONTROL  --------------------------//

          if (selected_hum > h){
              digitalWrite(hummidier_relay, LOW);
          }

          if (selected_hum < h){
              digitalWrite(hummidier_relay, HIGH);
          }
     }

//------------------------------ DOOR STATE CONTROL --------------------------//

    if (door == LOW && door_mikrik == LOW){
        digitalWrite(heater_light_relay, LOW);
        digitalWrite(hummidier_relay, LOW);
        digitalWrite(circular_fan_relay, LOW);
        digitalWrite(cooler_fan_relay, LOW);
        door_mikrik = HIGH;
    }
    if (door == HIGH && door_mikrik == HIGH){
        digitalWrite(heater_light_relay, HIGH);
        digitalWrite(hummidier_relay, HIGH);
        digitalWrite(circular_fan_relay, HIGH);
        digitalWrite(cooler_fan_relay, HIGH);
        door_mikrik = LOW;
    }

    if (door == HIGH && t == selected_temp + 1 && circular_fan == LOW){
        digitalWrite(cooler_fan_relay, LOW);
        circular_fan == HIGH;
    }

    if (door == HIGH && t > selected_temp && circular_fan == HIGH){
        digitalWrite(cooler_fan_relay, HIGH);
        circular_fan == LOW;
    }
  
}

void motor_controller(){

 //---------------------------  Up/Down state Control -------------------//
     

//---------------------------- Manual Control --------------------//

      if (bt5 == LOW){


      if (up_mikrik_state == HIGH || manual_control_up == LOW){
             digitalWrite(motor_up_relay1, HIGH);
             digitalWrite(motor_up_relay2, HIGH);       
      }

      if (down_mikrik_state == HIGH || manual_control_down == LOW){
             digitalWrite(motor_down_relay1, HIGH);
             digitalWrite(motor_down_relay2, HIGH);       
      } 
        
            if (manual_control_up == HIGH && up_mikrik_state == LOW){
                  digitalWrite(motor_up_relay1, LOW);
                  digitalWrite(motor_up_relay2, LOW);
             }

             if (manual_control_down == HIGH && down_mikrik_state == LOW){
                  digitalWrite(motor_down_relay1, LOW);
                  digitalWrite(motor_down_relay2, LOW);
             }
      }

//---------------------------- Auto Control --------------------//

      if (bt5 == HIGH){
        
             unsigned long MRcurrentMillis = millis();

             if (MRcurrentMillis - MRpreviusMillis > MRinterval && down_mikrik_state == HIGH && up_mikrik_state == LOW){
                 MRpreviusMillis = MRcurrentMillis;
                 digitalWrite(motor_up_relay1, LOW);
                 digitalWrite(motor_up_relay2, LOW);
             }

             if (up_mikrik_state == HIGH){
                  digitalWrite(motor_up_relay1, HIGH);
                  digitalWrite(motor_up_relay2, HIGH);
             }
    
             if (MRcurrentMillis - MRpreviusMillis > MRinterval && up_mikrik_state == HIGH  && down_mikrik_state == LOW) {
                 MRpreviusMillis = MRcurrentMillis;
                 Serial.print(MRcurrentMillis);
                 digitalWrite(motor_down_relay1, LOW);
                 digitalWrite(motor_down_relay2, LOW);
             }
             
             if (down_mikrik_state == HIGH){
                 digitalWrite(motor_down_relay1, HIGH);
                 digitalWrite(motor_down_relay2, HIGH);
             }
             
      }
  
}


void loop() {

  reading_sensors();
  print_lcd();
  climate_control();
  motor_controller();
  
}
