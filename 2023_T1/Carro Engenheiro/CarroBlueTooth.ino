//Definir os pinos de saída para os dois motores
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7
 
//Definir pinos para o controlo da velocidade dos motores
#define ENA 10
#define ENB 11
 
void setup(){
pinMode(IN1,OUTPUT);
pinMode(IN2,OUTPUT);
pinMode(IN3,OUTPUT);
pinMode(IN4,OUTPUT);
pinMode(ENA,OUTPUT);
pinMode(ENB,OUTPUT);
analogWrite(ENA,110);
analogWrite(ENB,110);
}
 
char an;
 
void loop() {
if (Serial.available() > 0) {
an = Serial.read();
delay(2);
 
// frente
if(an == 'F'){
digitalWrite(IN1,HIGH);
digitalWrite(IN2,LOW);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,LOW);
}
 
//trás
if(an == 'B'){
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);
}
 
//direita
if(an == 'R'){
digitalWrite(IN1,LOW);
digitalWrite(IN2,HIGH);
digitalWrite(IN3,HIGH);
digitalWrite(IN4,LOW);
}
 
//esquerda
if(an == 'L'){
digitalWrite(IN1,HIGH);
digitalWrite(IN2,LOW);
digitalWrite(IN3,LOW);
digitalWrite(IN4,HIGH);
}
 
//parado
if(an == '0'){
digitalWrite(IN1,LOW);
digitalWrite(IN2,LOW);
digitalWrite(IN3,LOW);
digitalWrite(IN4,LOW);
}
}
}