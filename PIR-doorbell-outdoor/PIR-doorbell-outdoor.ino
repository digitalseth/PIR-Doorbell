int PIR = A0;
int count = 1;

void setup() {
 pinMode(PIR, INPUT);
 Serial.begin(9600);
}

void loop() {
 if (analogRead(PIR) > 600) {
 Serial.print('D');
 delay(2000);
 count++;
 if(count>2){
  delay(10000);
  count = 1;
 }
 
 }
}

