/*
 * Created by Asya Syarif 
 * https://asyarif.net
 * 
 * Library used :
 * https://github.com/ArminJo/ServoEasing
 * https://github.com/JChristensen/JC_Button
 * 
 * Arduino Pro Mini Pin https://www.robotistan.com/Data/EditorFiles/%C3%9Cr%C3%BCnler%20A%C3%A7%C4%B1klama/11624-min.png
 * 
 * Servo pin :
 * RED    ----  +5V
 * BROWN  ----  GND
 * ORANGE ----  PWM SIGNAL
 */
#include <Arduino.h>
#include "ServoEasing.h"
#include <JC_Button.h>

ServoEasing servo;
#define BAUDRATE 115200
bool LED_BLINKING =  false;

#define LED_PIN 3          // pwm
#define BUTTON_PIN 9

#define LED_MAX_INTENSITY 1023     // full bright
#define LED_MIN_INTENSITY 0       // off
#define SERVO_PIN 5           // Pin Used for Servo
#define SERVO_SPEED 120        //
#define SERVO_MASK_OPENED 60  // Degrees
#define SERVO_MASK_CLOSED 0   // Degress
int LED_BLINK_PATTERN[8]  = {LED_MIN_INTENSITY, LED_MAX_INTENSITY, LED_MIN_INTENSITY, LED_MAX_INTENSITY, LED_MIN_INTENSITY, LED_MAX_INTENSITY, LED_MIN_INTENSITY, LED_MAX_INTENSITY};
int LED_BLINK_DELAY[8]    = {500, 350, 70, 70, 70, 70, 250};
bool isMaskOpen = false;
ToggleButton btn(BUTTON_PIN, 50, true, true);

void ledOn();
void ledOff();
void maskOpen();
void maskClose();

void setup() {
  
  Serial.begin(BAUDRATE);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Intializing...");
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_SERVO_EASING));
  Serial.print(F("Attach servo at pin "));
  Serial.println(SERVO_PIN);
  btn.begin();
  if (servo.attach(SERVO_PIN) == INVALID_SERVO) {
      Serial.println(F("Error attaching servo"));
  }
  analogWrite(LED_PIN, 0); // set to totally off
  maskClose();
  delay(1000);
  ledOn();
}

void loop() {
  btn.read();
  if(btn.wasReleased()){
    Serial.println("Button pressed!");
    (isMaskOpen == true) ? maskClose() : maskOpen();
  }
}


void maskOpen(){
  Serial.print("Mask is open, Servo angle is: ");
  Serial.println(servo.getCurrentAngle());
  ledOff();
  delay(500);
  servo.setEasingType(EASE_CIRCULAR_OUT);
  servo.easeTo(SERVO_MASK_OPENED, SERVO_SPEED);
  isMaskOpen = true;
}

void maskClose(){
  Serial.print("Mask is close, Servo angle is: ");
  Serial.println(servo.getCurrentAngle());
  
  servo.setEasingType(EASE_CIRCULAR_IN);
  servo.easeTo(SERVO_MASK_CLOSED, SERVO_SPEED);
  delay(1000);
  ledOn();
  isMaskOpen = false;
}

void ledOff(){
  analogWrite(LED_PIN, 0);
}

void ledOn(){
 if(LED_BLINKING){
    for(int i=0; i< 8; i++){
     analogWrite(LED_PIN, LED_BLINK_PATTERN[i]);
     delay(LED_BLINK_DELAY[i]);
    }
 }else{
  analogWrite(LED_PIN, LED_MAX_INTENSITY);    
 }
}
