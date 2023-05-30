//=======================================
//handle function: send webpage to client
//=======================================
void webpage()
{
  server_smooke_bot.send(200,"text/html", webpageCode);
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
    Serial.print("value= ");
    Serial.println(val);
    Serial.println(" ");

    if(var == "varOnOff"){
      varOnOff = false;
      if(val == "ON") varOnOff = true;
    }
    if(var == "done_count"){
      var_done_count = val.toInt();
    }
    if(var == "total_count"){
      var_total_count = val.toInt();
    }
    if(var == "per_volume"){
      var_per_volume = val.toInt();
    }
    if(var == "per_pull_in_second"){
      var_pull_in_second = val.toInt();
    }
    if(var == "per_sleep_in_second"){
      var_sleep_in_second = val.toInt();
    }
  }
}
