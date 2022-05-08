
#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo

int pValue; //potentiometer value
int outPorts[] = {15, 27, 26, 25}; //stepper motor ports
boolean forward = false;
int posVal = 0;    // variable to store the servo position
int servoPin = 32; // Servo motor pin
int maxDegree = 0; //max degree of servo motion


const int pPin = 33; //potentiometer

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  myservo.setPeriodHertz(50);           // standard 50 hz servo
  myservo.attach(servoPin, 500, 2500);  // attaches the servo on servoPin to the servo object
  myservo.write(posVal);
  
  // set steppe motor pins to output
  for (int i = 0; i < 4; i++) {
    pinMode(outPorts[i], OUTPUT);
  }
}

void loop() {
  //delay(1000); // this speeds up the simulation

  pValue = analogRead(pPin); //ranges from 0 to 4095
  String toPrint = String(pValue);
  Serial.println(toPrint);
  int speedValue = int(pValue/300); //ranges from 0 to 13
  if(speedValue == 0){
    moveAunt(speedValue);
    delay(1000);
  }
  else{
    moveAunt(speedValue);
    moveSteps(false, 256, 13-speedValue+3); //move a 1/8 turn at the given speed
                                            //subtraction is because lower number is faster here
  }
  
}

void moveAunt(int speedValue){
  if(forward){
    posVal=0;
    myservo.write(posVal); 
    forward=false;
  }
  else if(speedValue != 0){
    posVal = 50+speedValue*10;
    myservo.write(posVal);
    forward = true;
  }
}

//Suggestion: the motor turns precisely when the ms range is between 3 and 20
void moveSteps(bool dir, int steps, byte ms) {
  for (unsigned long i = 0; i < steps; i++) {
    moveOneStep(dir); // Rotate a step
    delay(constrain(ms,3,20));        // Control the speed
  }
}

void moveOneStep(bool dir) {
  // Define a variable, use four low bit to indicate the state of port
  static byte out = 0x01;
  // Decide the shift direction according to the rotation direction
  if (dir) {  // ring shift left
    out != 0x08 ? out = out << 1 : out = 0x01;
  }
  else {      // ring shift right
    out != 0x01 ? out = out >> 1 : out = 0x08;
  }
  // Output singal to each port
  for (int i = 0; i < 4; i++) {
    digitalWrite(outPorts[i], (out & (0x01 << i)) ? HIGH : LOW);
  }
}
