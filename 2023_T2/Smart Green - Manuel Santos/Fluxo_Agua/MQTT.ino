void mantemConexoes()
{
  if (!MQTT.connected()) {
    conectaMQTT();
  }
}

void conectaMQTT() 
{
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  while (!MQTT.connected()) {
    Serial.print("Conectando ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao Broker com sucesso!");
    }
    else {
      Serial.println("Nao foi possivel se conectar ao broker.");
      Serial.println("Nova tentatica de conexao em 10s");
      delay(10000);
    }
  }
}
