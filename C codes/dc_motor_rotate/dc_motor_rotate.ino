// rotete both side

// const int A1A = 7;
// const int A1B = 6;

// void setup(){
//   pinMode(A1A, OUTPUT);
//   pinMode(A1B, OUTPUT);
// }

// void loop(){
//   digitalWrite(A1A, LOW);
//   digitalWrite(A1B, HIGH);
//   delay(4000);
  
//   digitalWrite(A1A, HIGH);
//   digitalWrite(A1B, LOW);
//   delay(4000);
// }

// rotate with speed control 0-255

const int A1A = 7;
const int A1B = 6;
int speed = 255;

void setup(){
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
}

void loop(){
  analogWrite(A1B, speed);
  digitalWrite(A1A, 0);
  delay(50);
  digitalWrite(A1B, LOW);
  digitalWrite(A1A, LOW);
  // digitalWrite(A1A, HIGH);
  // digitalWrite(A1B, LOW);
  delay(4000);
}







