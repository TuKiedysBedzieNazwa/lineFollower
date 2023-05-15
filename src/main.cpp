#include <Arduino.h>

void setup(){
  pinMode(8, OUTPUT); //tył prawe koło
  pinMode(7, OUTPUT); //przód prawe koło (10)
  pinMode(6, OUTPUT); //tył lewe koło
  pinMode(5, OUTPUT); //przód lewe koło (9)

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

bool sensors[3] = {false, false, false};
bool lastSensors[3] = {false, false};
bool sensorsCheck[3];

bool sensorDetect(){

  for(int i=0; i<3; i++){
    sensorsCheck[i] = sensors[i];
  }

  if(analogRead(0) > 100){
    sensors[0] = false;
    digitalWrite(4, HIGH);
  }
  else{
    sensors[0] = true;
    digitalWrite(4, LOW);
  }
  if(analogRead(1) > 100){
    sensors[1] = false;
    digitalWrite(3, HIGH);
  }
  else{
    sensors[1] = true;
    digitalWrite(3, LOW);
  }
  if(analogRead(2) > 100){
    sensors[2] = false;
    digitalWrite(2, HIGH);
  }
  else{
    sensors[2] = true;
    digitalWrite(2, LOW);
  }

  for(int i=0; i<3; i++){

  }

  for(int i=0; i<3; i++){
    if(sensorsCheck[i] != sensors[i])
      return true;
  }

  return false;
}

void clear(){
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);

  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
}

bool seekLines(bool lastSensors[3]){

  analogWrite(9, 100);
  analogWrite(10, 100);
  clear();
  int i=0;

  if(lastSensors[0]){
    while(analogRead(1) > 100 && analogRead(2) > 100){

      digitalWrite(5, HIGH);

      i++;
      if(i > 3000) return false;
    }
  }
  else if(lastSensors[1]){
    while(analogRead(0) > 100 && analogRead(1) > 100){

      digitalWrite(7, HIGH);

      i++;
      if(i > 3000) return false;
    }
  }

  return true;
}



int leftEngine = 120;
int rightEngine = 120;
float makeItLonger = 0;

void loop(){

  analogWrite(9, leftEngine);
  analogWrite(10, rightEngine);

  if(sensors[0] || sensors[2]){
    lastSensors[1] = sensors[2];
    lastSensors[0] = sensors[0];
  }

  if(sensorDetect()) clear();

  if(
    !sensors[0] && sensors[1] && !sensors[2] ||
    sensors[0] && sensors[1] && sensors[2]
  ){

    if(leftEngine != 120 || rightEngine != 120){
      leftEngine = 120;
      rightEngine = 120;
    }
    if(makeItLonger != 0) makeItLonger = 0;


    digitalWrite(5, HIGH);
    digitalWrite(7, HIGH);
  }
  else if(
    sensors[0] && !sensors[1] && !sensors[2] ||
    sensors[0] && sensors[1] && !sensors[2]
  ){

    if(makeItLonger < 1){
      makeItLonger += 0.5;
    }
    else{
      if(rightEngine != 120) rightEngine = 120;
      if(leftEngine > 0) leftEngine--;
      
      makeItLonger = 0;
    }

    digitalWrite(5, HIGH);
  }
  else if(
    !sensors[0] && !sensors[1] && sensors[2] ||
    !sensors[0] && sensors[1] && sensors[2]
  ){

    if(makeItLonger < 1){
      makeItLonger += 0.5;
    }
    else{
      if(leftEngine != 120) leftEngine = 120;
      if(rightEngine > 0) rightEngine--;

      makeItLonger = 0;
    }

    digitalWrite(7, HIGH);
  }
  else if(!sensors[0] && !sensors[1] && !sensors[2] && !seekLines(lastSensors)){
    digitalWrite(5, LOW);
    digitalWrite(7, LOW);
    exit(0);
  }
}