void bluetooth_emissor(){
  // Envio dos dados via Bluetooth para a Interface de Monitorização

                BTserial.print("Temp:");
                BTserial.print(temperatura);   // Envio do valor da variável da Temperatura  
                BTserial.println(" oC");
                BTserial.print("Humid:");
                BTserial.print(humidade);      // Envio do valor da variável da Humidade
                BTserial.println(" %");
                BTserial.print("Temp_min:");
                BTserial.println(temp_min);   // Envio do valor da variável da temp_min
                BTserial.print("Temp_Max:");  
                BTserial.println(temp_max);   // Envio do valor da variável da temp_max 
                BTserial.print("Humid_min:");
                BTserial.println(hum_min);  // Envio do valor da variável da hum_min
                BTserial.print("Humid_Max:");  
                BTserial.print(hum_max);  // Envio do valor da variável da hum_max     
                BTserial.println(""); 
                delay(100);   
}
