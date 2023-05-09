# Start

[windows command line]
#set FLASK_DEBUG=1      #set environment var
set FLASK_DEBUG=0      #set environment var

cd gobot_front/apps/port1234
python -m flask --app main_wms run --host=0.0.0.0


# version 6 May
python -m flask --app routes run --host=0.0.0.0



# dependency
pip install flask
pip install flask-wtf
pip install tinydb


#deploy 
https://www.rosehosting.com/blog/how-to-deploy-flask-application-with-nginx-and-gunicorn-on-ubuntu-20-04/