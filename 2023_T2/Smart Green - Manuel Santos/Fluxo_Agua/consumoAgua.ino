void enviaValores_simples(float valorConsumoAgua, int flowrate)
{
  String cstr = String(valorConsumoAgua);
  cstr+="L | Fluxo: " + String(flowrate);
  char buf[30];
  cstr.toCharArray(buf, 30);

  MQTT.publish(TOPIC_PUBLISH, buf);
  Serial.print("Payload enviado com o valor: ");
  Serial.println(cstr);
}

void enviaMensagem(String mensagem)
{
  char buf[50];
  mensagem.toCharArray(buf, 50);

  MQTT.publish(TOPIC_PUBLISH, buf);
  Serial.print("Payload enviado com o valor: ");
  Serial.println(mensagem);
}
