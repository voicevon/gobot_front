sudo apt-get update 
time sudo -y apt upgrade 

# https://www.youtube.com/watch?v=QzVYnG-WaM4
time sudo apt -y install python3-pip

time sudo apt install -y build-essential cmake pkg-config libjpeg-dev libtiff5-dev libpng-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libfontconfig1-dev libcairo2-dev libgdk-pixbuf2.0-dev libpango1.0-dev libgtk2.0-dev libgtk-3-dev libatlas-base-dev gfortran libhdf5-dev libhdf5-serial-dev libhdf5-103 libqt5gui5 libqt5webkit5 libqt5test5 python3-pyqt5 python3-dev


time sudo pip install opencv-python

# test opencv
python
import cv2
cv2.__version__


sudo apt install htop
sudo apt install git -y
git clone https://github.com/voicevon/gobot_front.git


sudo pip install numpy
sudo pip install VonPylib 
sudo pip install paho-mqtt
sudo apt install -y tesseract-ocr
sudo pip install mss
sudo pip install pytesseract


# test main.py
sudo python /home/xm/gobot_front/apps/kvm_ocr_cloud/main.py

# Add to rc.local
sudo nano /etc/rc.local
// Add below to editor. and save it, then: sodu reboot
sudo python /home/xm/gobot_front/apps/kvm_ocr_cloud/main.py &


# Turtorial:
* https://raspberrypi-guide.github.io/programming/install-opencv
* https://www.dexterindustries.com/howto/run-a-program-on-your-raspberry-pi-at-startup/