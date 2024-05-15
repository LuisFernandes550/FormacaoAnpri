#define DO 267 
#define RE 294 
#define MI 335 
#define FA 349 
#define SOL 392 
#define LA 430 
#define SI 500 
#define C 525
#define D 590

int notas[9] = {DO,RE,MI,FA,SOL,LA,SI, C, D}; 

void setup() { 
  
for (int i=2; i<9; i++)  {
pinMode(i, INPUT); 

}

} 

void loop() {  
  

for (int i=2; i<11; i++)  

{     
  
while (!digitalRead(i))    
{      
tone (12, notas [i-2]); 

}
    
noTone (12);   
} 

}
