// DC motor
const int A1A = 3; 
const int A1B = 5;

int motorSpeed = 700; // Default speed, range: 0 (off) to 255 (full speed), 65 limit

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
}

void loop() {
  // Rotate the motor anti clockwise at the specified speed
  analogWrite(A1A, motorSpeed);
  digitalWrite(A1B, LOW);
  // delay(2000); // Motor rotates clockwise at the specified speed for 2 seconds
  
  // Rotate the motor clockwise at the specified speed
  // analogWrite(A1B, motorSpeed);
  // digitalWrite(A1A, LOW);
  // delay(2000); // Motor rotates counterclockwise at the specified speed for 2 seconds
}
