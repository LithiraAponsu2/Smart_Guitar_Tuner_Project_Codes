// Encoder outputs A B trigger check


// #define ENCA 20 // pin2 of the Arduino
// #define ENCB 21 // Pin3 of the Arduino

// int ENCA_DATA;
// int ENCB_DATA;
 
// void setup() {
//   Serial.begin(9600); // Activates Serial communication
//   pinMode(ENCA,INPUT); // sets pin2 as the input
//   pinMode(ENCB,INPUT); // sets pin3 as the input
// }
 
// void loop() {
//   ENCA_DATA = digitalRead(ENCA); 
// // We simply read Pin2 of the Arduino and store the result in variable ENCA_DATA
//   ENCB_DATA = digitalRead(ENCB); 
// // We simply read Pin3 of the Arduino and store the result in variable b
//   Serial.print(ENCA_DATA*5); 
//   Serial.print(" ");
//   Serial.print(ENCB_DATA*5);
//   Serial.println();
// }

// position of encoder -> - values and + values for two sides

#define Encoder_output_A 20 // pin2 of the Arduino
#define Encoder_output_B 21 // pin 3 of the Arduino
// these two pins has the hardware interrupts as well. 
 
int Count_pulses = 0;
void setup() {
Serial.begin(9600); // activates the serial communication
pinMode(Encoder_output_A,INPUT); // sets the Encoder_output_A pin as the input
pinMode(Encoder_output_B,INPUT); // sets the Encoder_output_B pin as the input
attachInterrupt(digitalPinToInterrupt(Encoder_output_A),DC_Motor_Encoder,RISING);
}
 
void loop() {
  Serial.println("Result: ");
  Serial.println(Count_pulses); 
}
 
void DC_Motor_Encoder(){
  int b = digitalRead(Encoder_output_B);
  if(b > 0){
    Count_pulses++;
  }
  else{
    Count_pulses--;
  }
}


