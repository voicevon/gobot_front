# Start

[windows command line]
#set FLASK_DEBUG=1      #set environment var
set FLASK_DEBUG=0      #set environment var

cd gobot_front/apps/twh2
python -m flask --app main_wms run --host=0.0.0.0



# dependency
pip install flask
pip install flask-wtf
pip install tinydb
