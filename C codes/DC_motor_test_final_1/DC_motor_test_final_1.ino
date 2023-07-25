// dc motor
const int A1A = 3; 
const int A1B = 5;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
}

void loop() {
  digitalWrite(A1B, HIGH); // B high CW
  digitalWrite(A1A, LOW);
  delay(50);  // ~ 45 degrees
  digitalWrite(A1A, LOW);
  digitalWrite(A1B, LOW);
  delay(50);

}
