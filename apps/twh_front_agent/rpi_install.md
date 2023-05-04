sudo apt-get update 
time sudo apt-get upgrade -y

# https://www.youtube.com/watch?v=QzVYnG-WaM4
sudo apt install python3-pip

time sudo apt install -y build-essential cmake pkg-config libjpeg-dev libtiff5-dev libpng-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libfontconfig1-dev libcairo2-dev libgdk-pixbuf2.0-dev libpango1.0-dev libgtk2.0-dev libgtk-3-dev libatlas-base-dev gfortran libhdf5-dev libhdf5-serial-dev libhdf5-103 libqt5gui5 libqt5webkit5 libqt5test5 python3-pyqt5 python3-dev


time pip install opencv-python

# test opencv
python
import cv2
cv2.__version__


sudo apt install htop

sudo apt install git -y
git clone https://github.com/voicevon/gobot_front.git


pip install numpy VonPylib 
pip install paho-mqtt

<!-- pip install falsk -->
pip install fastapi

pip install pillow mss



cd gobot_front/apps/twh_front_agent/
python ocr_window.py




# Turtorial:
* https://raspberrypi-guide.github.io/programming/install-opencv