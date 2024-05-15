// Código projeto de sensor com igação de dados via porta série
// Existem duas opções, para obtenção dosficheiros com dados em dadosSerieTL.xlsx:
// 1- Opção via Aplicação Excel, destinado a alunos do 3.º ciclo
// 2 - Opção via aplicação realizada em Python e controlo com Software OpenSource destinado
// aalunos do ensino secundárioou profissional
// Notas:
// 1.Para ligar ao excel o ultimo campo tem de ter  Serial.println(luminosidade); e o campo anterior tem de ter uma virgula
// 2. A opção do"Seralmonitor" do IDEdo Arduino tem de estar desligadapara queseja possível aceder à porta COM
// 3. Na configuração foi usado tanto no Excel como no PyThon a porta COM3
// 4. A versão do Excel não pode ser o licenciamento de Educação A1 (i.e. versão atualmente gratuita)
// 5 Para visualizar dos dados e na versão do PyThon são usadas livrarias para abertura do ficheiro Excel
// 6 No caso do PyThon o ficheiro dadosSerieTL.xlsx , o mesmo lê os mesmos, mas tem de ser atualizado na pasta onde se encontrar a versão main.py
// 7 Deve no caso do Python alterar a variável num_registros, dado que o programa recolhe esse número de registos apenas antes de fechar as 
// comunicações com o arduino e  escrever o pandas dataframe no ficheiro excel.

const int ldrPin = A2;
const int tempPin = A0;
//const float tempAmb = 17.0;
int sensor;
float tensao;
float temperatura;
float luminosidade;

void setup() {
  // put your setup code here, to run once:
  pinMode(ldrPin, INPUT);
  pinMode(tempPin, INPUT);
  Serial.begin(9600);
}

void loop() {

  unsigned long timestamp = millis();
  luminosidade = analogRead(ldrPin);
  delay(100);
  sensor = analogRead(tempPin);
  tensao = (sensor / 1024.0) * 5;
  temperatura = tensao * 100;

  // Enviar os dados pela porta serial
  // Serial.print("Timestamp: ");
  Serial.print(timestamp + String(","));
  // Serial.print(", Temperatura: ");
  Serial.print(tensao + String(","));

  Serial.print(temperatura + String(','));

  //  Serial.print(luminosidade);
  Serial.println(luminosidade);



  delay(900);
}
