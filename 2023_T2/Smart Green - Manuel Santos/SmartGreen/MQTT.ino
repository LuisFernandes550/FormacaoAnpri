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

void enviaValores_simples()
{
  String cstrSoil = String((int) asoilmoist);
  String cstrTemp = String(temp);
  String cstrHum = String(hum);
  if (limiteI > 0)
  {
    if (regaAUX < 1)
    {
      if (asoilmoist < limiteRega)
      {
        rega = 1;
      }
      else
      {
        rega = 0;
      }
    }
    else
    {
      rega = 1;
    }
  }
  else
  {
    rega = 0;
  }
  String cstr = cstrSoil + ",," + cstrHum + "," + cstrTemp + "," + limiteS + "," + limiteI + "," + rega;
  char buf[30];
  cstr.toCharArray(buf, 30);

  MQTT.publish(TOPIC_PUBLISH, buf);
  Serial.print("Payload enviado com o valor: ");
  Serial.println(cstr);
}
