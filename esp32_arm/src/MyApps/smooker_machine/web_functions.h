//=======================================
//handle function: send webpage to client
//=======================================
void webpage()
{
  server.send(200,"text/html", webpageCode);
}
//=====================================================
//function process event: new data received from client
//=====================================================
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength)
{
  String payloadString = (const char *)payload;
  Serial.print("payloadString= ");
  Serial.println(payloadString);

  if(type == WStype_TEXT) //receive text from client
  {
    byte separator=payloadString.indexOf('=');
    String var = payloadString.substring(0,separator);
    Serial.print("var= ");
    Serial.println(var);
    String val = payloadString.substring(separator+1);
    Serial.print("val= ");
    Serial.println(val);
    Serial.println(" ");

    if(var == "varOnOff"){
      varOnOff = false;
      if(val == "ON") varOnOff = true;
    }
    if(var == "varPause"){
      varPause = false;
      if(val == "Paused") varPause = true;
    }
    if(var == "varReset"){
      var_total_count = 0;
      var_total_volume = 0;
    }
    if(var == "var_volume"){
      var_volume = val.toInt();
    }
    if(var == "var_push_time"){
      var_push_time = val.toInt();
    }
    if(var == "var_sleep_time"){
      var_sleep_time = val.toInt();
    }
    
    var_total_volume+=3;
    var_total_count++;

  }
}
