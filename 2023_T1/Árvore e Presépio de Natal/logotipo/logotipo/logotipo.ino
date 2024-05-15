const int ledPin[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13}; 
int i;
void setup() {
  // put your setup code here, to run once:
  for(i=0;i<14;i++){
    pinMode(ledPin[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(i=0;i<12;i++){
    digitalWrite(ledPin[i], HIGH);
    digitalWrite(ledPin[i+1], HIGH);
    digitalWrite(ledPin[i+2], HIGH);
    digitalWrite(ledPin[i-1], LOW); 
    delay(100);
  }
  for(i=12;i>1;i--){
    digitalWrite(ledPin[i], HIGH);
    digitalWrite(ledPin[i-1], HIGH);
    digitalWrite(ledPin[i-2], HIGH);
    digitalWrite(ledPin[i+1], LOW); 
    delay(100);
  }
}