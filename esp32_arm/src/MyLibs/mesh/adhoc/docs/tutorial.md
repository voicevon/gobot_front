
# Related Projects
* https://rntlab.com/question/esp-now-gateway-wifi_mode_sta-with-a-wifi-router/
* https://github.com/m1cr0lab-esp32/esp-now-network-and-wifi-gateway/blob/master/src/receiver.cpp


* https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/
```
int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
              return WiFi.channel(i);
          }
      }
  }
  return 0;
}
```