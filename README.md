# Hardware Platform
- Raspberry Pi zero w
- With Pi camera

# OS setup and Dependency installation 
- Enable Pi camera ?
- Python 3.x (preinstalled with Pi OS)
- OpenCV Version 
```
sudo apt install python3-opencv -y
```
- Paho-mqtt Ver 1.5.1
```
pip3 install paho-mqtt
```

# Involved repos
- [pylib](https://github.com/voicevon/pylib)
- [gogame_bot](https://github.com/voicevon/gogame_bot)
- [imaGo](https://github.com/tomasmcz/imago)
- [gbr](https://github.com/skolchin/gbr)
- [kifu](https://www.remi-coulom.fr/kifu-snap/)
- [SGF](https://www.red-bean.com/sgf/user_guide/index.html)

# Tasks and Functions
- [] Recognize go game chessboard and stone position.
- [] Send layout to MQTT broker.
- [] Simple GPIO to turn on/off some LED lights to indicate status or progress
