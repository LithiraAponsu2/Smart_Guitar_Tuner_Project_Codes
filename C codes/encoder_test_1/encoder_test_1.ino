// dc motor
const int A1A = 3; 
const int A1B = 5;
const int ENCA = 2;  // Yellow
const int ENCB = 7;  // White

int pos = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // pinMode(A1A, OUTPUT);
  // pinMode(A1B, OUTPUT);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);
}

void loop() {
  // Motor Control
  // digitalWrite(A1B, HIGH);
  // digitalWrite(A1A, LOW);
  // delay(50);  // ~ 45 degrees
  // digitalWrite(A1A, LOW);
  // digitalWrite(A1B, LOW);
  // delay(50);

  // encoder A B trigger test
  // int a = digitalRead(ENCA);
  // int b = digitalRead(ENCB);
  // Serial.print(a*5);
  // Serial.print(" ");
  // Serial.print(b*5);
  // Serial.println();

  // encoder position get
  Serial.println(pos);
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
