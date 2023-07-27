# https://github.com/abalarin/Flask-on-Linode

from flask import Flask,  render_template, redirect,flash, url_for, request, send_file
from twh_wcs.wcs_main import Start_TwhWcs_Process
from twh_user.route import web_user
from twh_stock.route import web_stock
from von.mqtt.mqtt_agent import g_mqtt,g_mqtt_broker_config
# from von.ocr.ocr_factory import OcrFactory
# from kvm_ocr_cloud.ocr_node_factory import OcrNodeFactory
from tinydb import TinyDB, Query, where

app = Flask(__name__)
app.config['SECRET_KEY'] = '20221220'
app.secret_key = '20221221'   # for WTForm

app.register_blueprint(web_user)
app.register_blueprint(web_stock)

# app.debug=False
new_message = {}
new_message['test'] = {'id':2, 'payload': 'hello, dynamic message.'}


@app.route('/lua_post', methods = ['POST'])
def lua_post():
    ide_request = request.form.to_dict()
    print(ide_request["editor"])
    g_mqtt.publish("lua/test", ide_request["editor"])
    # flash("规则更新完毕")
    return redirect(url_for('lua_ide'))


@app.route('/')
def index():
    return render_template('index.html')

@app.route('/hire')
def hire():
    return render_template('hire.html')

@app.route('/hire_cpp')
def hire_cpp():
    return render_template('hire/cpp.html')



@app.route('/contact')
def contact():
    return render_template('contact.html')        

@app.route('/product_wcs')
def product_wcs():
    return render_template('wcs/product_wcs.html')

@app.route('/product_twh_m1')
def product_twh_m1():
    return render_template('wcs/twh/product_twh_m1.html')

@app.route('/product_twh_m2')
def product_twh_m2():
    return render_template('wcs/twh/product_twh_m2.html')

@app.route('/product_twh_m3')
def product_twh_m3():
    return render_template('wcs/twh/product_twh_m3.html')

@app.route('/product_twh_m4')
def product_twh_m4():
    return render_template('wcs/twh/product_twh_m4.html')

@app.route('/product_twh_rental_price')
def product_twh_rental_price():
    return render_template('wcs/twh/rental_price.html')

@app.route('/product_acupuncture')
def product_acupuncture():
    return render_template('/acupuncture/product_acupuncture.html')

@app.route('/product_iot')
def product_iot():
    return render_template('product_iot.html')

@app.route('/ocr')
def ocr():
    # kvm_nodes = OcrNodeFactory.GetKnown_KvmNodeList()
    # app_windows = OcrNodeFactory.GetKnown_AppWindowList()
    kvm_nodes = {"yalefu", 'yalefu_viewer'}
    app_windows = {}
    return render_template('ocr/index.html', kvm_nodes=kvm_nodes,app_windows=app_windows)
    # return render_template('ocr/index.html')

@app.route('/read_config')
def read_config():
    key = request.args.get('k', default='none', type=str)
    return key




@app.route('/lua_ide')
def lua_ide():
    return render_template('lua_ide/index.html')

@app.route('/lua_ide/save', methods=['POST','GET'])
def lua_ide_save():
    # print("insert method is", request.method)
    content_type = request.headers.get('Content-Type')
    # print("conetent_type", content_type)
    if content_type == 'application/json':
        if request.is_json:
            # print("Yes, request is json")
            lua_node = request.json
            table_lua_ide = TinyDB('lua_ide.json')
            items = table_lua_ide.search(Query().mac_addr == lua_node['mac_addr'])
            if len(items)>0:
                # do update
                doc_ids = []
                doc_ids.append(items[0].doc_id)
                table_lua_ide.update(lua_node, doc_ids=doc_ids)
                # topic,payload=("intergral/mcu/reset", lua_node['mac_addr'])
                topic="intergral/mcu/reset"
                payload = lua_node['reset_level']
                payload = payload + ","
                payload = payload + lua_node['mac_addr']
                g_mqtt.publish(topic, payload, retain=False)              
                return {"result":"Updated to lua_ide.json"}
            else:
                # do insert
                print("do insert")
                new_doc_id = table_lua_ide.insert(lua_node)
                return {"result":"Inserted to lua_ide.json"}

        else:
            return ("request is not json")
    else:
        print("Content_type is not json.")
        return "insert failed"

@app.route('/lua_ide/delete', methods=['POST'])
def lua_ide_delete():
    content_type = request.headers.get('Content-Type')
    if content_type == 'application/json':
        if request.is_json:
            print("Yes, request is json")
            key = request.json['mac_addr']
            table_lua_ide = TinyDB('lua_ide.json')
            items = table_lua_ide.remove(Query().mac_addr == key)
            return items
        else:
            return ("request is not json")
    else:
        print("Content_type is not json.")
        return "insert failed"  

@app.route('/lua_ide/search', methods=['POST','GET'])
def lua_ide_search():
    content_type = request.headers.get('Content-Type')
    if content_type == 'application/json':
        if request.is_json:
            print("Yes, request is json")
            sub_mac_addr = request.json['mac_addr']
            table_lua_ide = TinyDB('lua_ide.json')
            q = Query()
            test_func = lambda value, search: search in value
            items = table_lua_ide.search(Query().mac_addr.test(test_func, sub_mac_addr))
            return items

        else:
            return ("request is not json")
    else:
        print("Content_type is not json.")
        return "insert failed"

@app.route("/lua_ide/save_net_config", methods=['POST'])
def lua_ide_save_net_config():
    mac_addr = request.json['mac_addr']
    net_config = request.json["net_config"]
    
    table_lua_ide = TinyDB('lua_ide.json')
    table_lua_ide.update({'net_config':net_config}, Query().mac_addr==mac_addr)
    return "OK"


@app.route("/lua_ide/down/<mac_addr>")
def node_script(mac_addr):
    table_lua_ide = TinyDB('lua_ide.json')
    items = table_lua_ide.search(Query().mac_addr == mac_addr)
    if len(items) > 0:
        print("found lua_script")
        file_content = items[0]['lua_script']
        return file_content
    else:
        print("trying to get default lua_script")
        items = table_lua_ide.search(Query().mac_addr == 'default_for_blank_mcu')
        file_content = items[0]['lua_script']
        print (file_content)
        return file_content

from flask_socketio import SocketIO, emit
socketio = SocketIO()
socketio.init_app(app)

@socketio.on("connect")
def handle_connect():
    print("Client connected!")


@app.route('/lua_log', methods=['POST'])
def lua_log():
    mcu_log = request.get_data()
    # print(mcu_log)
    # send mcu_log to web browser.
    socketio.emit("log", {"mcu_mac_addr": "1234", "mcu_log": mcu_log})
    return 'OK'


@app.route('/node_config')
def node_config():
    key = request.args.get('key', default='', type=str)
    # read from database
    values={}
    if key=='':
        values = {"ocr","twh"}
    return render_template('node_config/index.html', values = values)
    # return render_template('node_config/test.html', values = values)

@app.route('/node_config/search', methods=['POST','GET'])
def node_config_search():
    content_type = request.headers.get('Content-Type')
    if content_type == 'application/json':
        if request.is_json:
            print("Yes, request is json")
            sub_mqtt_topic = request.json['mqtt_topic']
            table_config = TinyDB('node_configs.json')
            q = Query()
            test_func = lambda value, search: search in value
            items = table_config.search(Query().mqtt_topic.test(test_func, sub_mqtt_topic))
            # items = table_config.search(where('mqtt_topic').matches('*p*'))
            return items

        else:
            return ("request is not json")
    else:
        print("Content_type is not json.")
        return "insert failed"
    
@app.route('/node_config/delete', methods=['POST'])
def node_config_delete():
    content_type = request.headers.get('Content-Type')
    if content_type == 'application/json':
        if request.is_json:
            print("Yes, request is json")
            mqtt_topic = request.json['mqtt_topic']
            table_config = TinyDB('node_configs.json')
            items = table_config.remove(Query().mqtt_topic == mqtt_topic)
            return items
        else:
            return ("request is not json")
    else:
        print("Content_type is not json.")
        return "insert failed"    

@app.route('/node_config/save', methods=['POST','GET'])
def node_config_save():
    # print("insert method is", request.method)
    content_type = request.headers.get('Content-Type')
    # print("conetent_type", content_type)
    if content_type == 'application/json':
        if request.is_json:
            print("Yes, request is json")
            config_item = request.json
            table_config = TinyDB('node_configs.json')
            q = Query()
            items = table_config.search(q.mqtt_topic == config_item['mqtt_topic'])
            if len(items)>0:
                # do update
                print("do update")
                print(items[0].doc_id)
                doc_ids = []
                doc_ids.append(items[0].doc_id)
                table_config.update(config_item, doc_ids=doc_ids)
                topic,payload=(config_item['mqtt_topic'], config_item['config'])
                g_mqtt.publish(topic, payload)              
                return {"result":"Updated to node_configs.json"}
            else:
                # do insert
                print("do insert")
                new_doc_id = table_config.insert(config_item)
                return {"result":"Inserted to node_configs.json"}

        else:
            return ("request is not json")
    else:
        print("Content_type is not json.")
        return "insert failed"



g_mqtt_broker_config.client_id = "230604"
g_mqtt.connect_to_broker(g_mqtt_broker_config, blocked_connection=True)
Start_TwhWcs_Process()

if __name__ == '__main__':
    # reloader or debug must be false.  
    # https://stackoverflow.com/questions/31264826/start-a-flask-application-in-separate-thread
    #  web.run(debug=True)
    app.run(host='0.0.0.0', debug=False)
