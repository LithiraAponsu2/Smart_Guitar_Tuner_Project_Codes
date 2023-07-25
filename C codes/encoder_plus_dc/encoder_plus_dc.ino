// dc motor
const int A1A = 3; 
const int A1B = 5;
const int ENCA = 2;  // Yellow
const int ENCB = 7;  // White

int pos = 0;
int speed_pwm = 200;
long prevT = 0;
float eprev = 0;
float eintegral = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
}

void loop() {
  // // set target position
  // int target = 1200;

  // // PID constants
  // float kp = 1;
  // float kd = 0.025;
  // float ki = 0;

  // // time difference
  // long currT = micros();

  // float deltaT = ((float) (currT-prevT)/1.0e6);
  // prevT = currT;

  // // error
  // int e = -pos + target;

  // // derivative
  // float dedt = (e-eprev)/(deltaT);

  // // integral
  // eintegral = eintegral + e*deltaT;

  // // control signal
  // float u = kp*e + kd*dedt + ki*eintegral;

  // // motor power
  // float pwr = fabs(u);
  
  // if(pwr>255){
  //   pwr = 255;
  // }

  // if(pwr<70){
  //   pwr = 70;
  // }

  // // motor direction
  // int dir = 1;
  // if(u<0){
  //   dir = -1;
  // }

  // // signal the motor
  // setMotor(dir, pwr);

  // // store prev error
  // eprev = e;

  // Serial.print(target);
  // Serial.print(" ");
  // Serial.print(pos);
  // Serial.println();
  // Serial.println(pwr);
  Serial.println(1);
  PID_motor(1200);
}

void readEncoder() {
  int b = digitalRead(ENCB);
  if (b>0) {
    pos++;
  }
  else {
    pos--;
  }
}

void setMotor(int dir, int pwmVal) {
  if (dir == 1){
    analogWrite(A1A, pwmVal);
    digitalWrite(A1B, LOW);
  }
  else if (dir == -1) {
    analogWrite(A1B, pwmVal);
    digitalWrite(A1A, LOW);
  }
  else {
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, LOW);
  }
}

// loop to function
void PID_motor(int target){

  // PID constants
  float kp = 1;
  float kd = 0.025;
  float ki = 0;

  
  // time difference
  long currT = micros();

  float deltaT = ((float) (currT-prevT)/1.0e6);
  prevT = currT;

  // error
  int e = -pos + target;

  // derivative
  float dedt = (e-eprev)/(deltaT);

  // integral
  eintegral = eintegral + e*deltaT;

  // control signal
  float u = kp*e + kd*dedt + ki*eintegral;

  // motor power
  float pwr = fabs(u);
  
  if(pwr>255){
    pwr = 255;
  }

  if(pwr<70){
    pwr = 70;
  }

  // motor direction
  int dir = 1;
  if(u<0){
    dir = -1;
  }

  // signal the motor
  setMotor(dir, pwr);

  // store prev error
  eprev = e;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(pos);
  Serial.println();
  Serial.println(pwr);
  

  
}
