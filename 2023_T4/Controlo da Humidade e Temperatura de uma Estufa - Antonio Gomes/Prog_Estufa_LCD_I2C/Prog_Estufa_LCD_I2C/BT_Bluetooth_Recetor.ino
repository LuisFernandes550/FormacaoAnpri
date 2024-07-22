void bluetooth_recetor(){
  // Receção de Dados via Bluetooth  ****************************

if(BTserial.available())
  {
  char daten = BTserial.read();
  Serial.println(daten); 
 
    if(daten=='R')  // Se caracter recebido for "R"
    {
        digitalWrite(resist_aquec, HIGH); 
        BTserial.println("Resistência ON");
    }
    if (daten=='F')  // Se caracter recebido for "F"
    {
        digitalWrite(resist_aquec, LOW);
        BTserial.println("Resistência OFF");
    }
    if (daten=='8')
    {
        menu = menu + 1;   // Decrementa menu
        delay(100);
    }
    
    if (daten=='2')
    {
        menu = menu - 1;   // Decrementa menu
        delay(100);
    }
    
    if (daten=='6')
    {
        if (menu == 1 ){ temp_min = temp_min + 1; }
        if (menu == 2 ) temp_max ++;
        if (menu == 3 ) hum_min ++;
        if (menu == 4 ) hum_max ++;
        delay(100);
    }

     if (daten=='4')
    {
        if (menu == 1 ){ temp_min = temp_min - 1; } 
        if (menu == 2 ) temp_max --;
        if (menu == 3 ) hum_min --;
        if (menu == 4 ) hum_max --;     
        delay(100);
    }
   
}

}
