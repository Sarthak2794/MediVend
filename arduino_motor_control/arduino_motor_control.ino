
#include <Vector.h>
#include <AccelStepper.h>
#include <Servo.h>

#define limitSwitch 8
Servo servo;
AccelStepper stepper(1, 10, 11); 

bool flag = false;
bool flagg = false;
bool servoFlag = false;
bool nextFlag = false;
bool stackFlag = false;
// bool limitSwitch = false
// int daata[3][6] = {{1, 0, 2, 0, 0, 1}, {0, 2, 1, 0, 1, 1}, {1, 2, 0, 0, 0, 0}};
String daata = "";
int data[6] = {0};
int data1;
int num[6] = {0};
int st, st2;
int j = 0;
int k = 0;
int n = 0;

bool moveStepper(int stackNo, bool flagg){
  int steps[6] = {0, 8000, 16000};
  if(flagg && k < j){
    if((stackNo == 0) || (stackNo == 1)){
      stepper.moveTo(steps[0]);
      // Serial.println("step 0");
    }
    if((stackNo == 2) || (stackNo == 3)){
      stepper.moveTo(steps[1]);
      // Serial.println("step 1");
    }
    if((stackNo == 4) || (stackNo == 5)){
      stepper.moveTo(steps[2]);
      // Serial.println("step 2");
    }
    // if((data[num][0] == 7) || (data[num][0] == 8))
    //   stepper.moveTo(steps[3]);
    // if((data[num][0] == 9) || (data[num][0] == 10))
    //   stepper.moveTo(steps[4]);
    // if((data[num][0] == 11) || (data[num][0] == 12))
    //   stepper.moveTo(steps[5]);
    // Serial.println(String("k value ") + k);
    // Serial.println(String("stackNo: ") + stackNo);
    // Serial.println("nextFlag false");
    flag = true;
    return false;
  }
  return false;
}

void moveServo(int var){
  int deflectAngle = 60;
  static int angle = 90;
  int leftAngle = 168;
  int rightAngle = 10;

  if(data[var] > 0){
    if(var % 2 == 0){
      while(data[var] > 0){
        servo.write(leftAngle);
        Serial.println("left angle");
        delay(1000);
        servo.write(leftAngle - 60);
        Serial.println("left back angle");
        delay(1000);
        data[var]--;
      }
    }
    else{
      while(data[var] > 0){
        servo.write(rightAngle);
        Serial.println("right angle");
        delay(1000);
        servo.write(rightAngle + 60);
        Serial.println("right back angle");
        delay(1000);
        data[var]--;
      }
    }
    k++;
    flagg = true;
    Serial.println("servo");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

  pinMode(13, OUTPUT);
  pinMode(limitSwitch, INPUT_PULLUP);
  stepper.setMaxSpeed(2000);	
  stepper.setAcceleration(700);
  servo.attach(9, 405, 2550);
  servo.write(90);
  while (digitalRead(limitSwitch) == 1){
    stepper.setSpeed(-800);
    stepper.runSpeed();
  }
    stepper.setSpeed(0);
    stepper.runSpeed();
    stepper.setCurrentPosition(0);
  // stepper.stop();
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.println("print check");
  if(Serial.available()){
    n = 0;
    daata = Serial.readString();
    for(int i = 0; i < 12; i+=2){
      if(daata[i] > 48){
        data[n] = daata[i] - 48;
      }
      n++;
    }
    nextFlag = true;
    stackFlag = true;
    flag = true;
    // servoFlag = true;
  }

  // if(Serial.available()){
  //   n = 0;
  //   int inp = Serial.parseInt();
  //   Serial.println(String(" inp: ") + inp);
  //   for(int i = 0; i < 6; i++){
  //     // if(daata[inp][i] > 48){
  //       data[n] = daata[inp][i];
  //       Serial.println(data[n]);
  //     // }
  //     n++;
  //   }
  //   nextFlag = true;
  //   stackFlag = true;
  //   flag = true;
  //   // servoFlag = true;
  // }

  if(stackFlag){
    k = 0;
    j = 0;
    for(int i = 0; i < 6; i++){
      // Serial.println("Num:");
      // Serial.print(data[0]);
      if(data[i] > 0){
        num[j] = i;
        Serial.println(String("  Num ") + num[j]);
        j++;
      }
      // Serial.println();
    }
    stackFlag = false;
  }
  // if(Serial.available()){
  //   num = 1;
  //   data[num][0] = Serial.read();
  //   nextFlag = true;
  // }

  if((flagg) && ((millis() - st2) > 1000)){
    nextFlag = true;
    // Serial.println("nextFlag true");
    flagg = false;
  }

  nextFlag = moveStepper(num[k], nextFlag);

  if(stepper.distanceToGo() != 0){
    stepper.run();
    // flag = true;
    // Serial.println("yess");
  }
  else if(flag){
    // Serial.println("ServoFlag is true");
    moveServo(num[k]);
    servoFlag = true;
    flag = false;
  }
  // else if(stepper.distanceToGo() == 0)
  //   servoFlag = true;
  
}