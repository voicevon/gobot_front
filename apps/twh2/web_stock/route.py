from flask import Blueprint, render_template, request,flash, session, redirect, url_for
from web_stock.db_api import db_Stock, DbShipout, db_Withdraw, db_Deposit_history,db_StockRule
from bolt_nut import get_row_from_tooth_location
from wcs_robots.twh_wcs import  wcs_deposit_queue, packer_cells_state, reset_packer_cell_queue
from logger import Logger
from datetime import datetime

web_stock = Blueprint('web_stock', __name__,template_folder='templates')

twh_factory = {'221109':'山东雅乐福义齿公司'}


@web_stock.route('/decrease_stock')
def decrease_stock():
    row_id = int(request.args.get('doc_id'))
    # print("rrrrrrrrrrrrrrrrrrrrrrrrrrr  row_id=  ", row_id)
    # q = Query()
    # rows = g_database.db_stock.search(q.doc_id==row_id)
    # for row in rows:
    #     print(row['stock_quantity'])
    #     row['stock_quantity'] = row['stock_quantity'] -1
    #     print(row['stock_quantity'])
    # g_database.db_stock.write_back(rows)
    return 'OK'

@web_stock.route('/stock_rule_update', methods = ['POST'])
def stock_rule_update():
    new_rule_item = request.form.to_dict()
    new_rule_item['user_id'] = session['user']['user_id']
    new_rule_item['user_name'] = session['user']['user_name']
    new_rule_item['date_time'] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    db_StockRule.update(new_rule_item)
    flash("规则更新完毕")
    return redirect(url_for('web_stock.view_stock_rule'))

@web_stock.route("/stock_rule_creator")
def stock_rule_creator():
    item = {}
    item['twh_id'] = '221109'
    exist = False
    if exist:
        flash('Create stock rule failed for ' + item['twh_id'])
    else:
        item['brand'] = '定远'
        item['batch_number'] = '2309'
        item['size'] = '中'
        item['shape'] = '圆'
        item['color']='A2'
        item['user_id'] = '系统'
        item['date_time'] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        item['stock_quantity'] = 0
        for col in range(60):
            item['col'] = col + 1
            db_StockRule.table_stock_rule.insert(item)
        flash('created stock rule for ' + item['twh_id'])
    return redirect(url_for('home'))

@web_stock.route('/view_stock_rule')
def view_stock_rule():
    items = db_StockRule.get_all_rules_in_twh(session['user']['twh_id'])
    return render_template('view_stock_rule.html', items=items)

@web_stock.route('/view_stock_quantity')
def view_stock_quantity():
    stocks = db_Stock.table_stock.all()
    return render_template('view_stock_quantity.html', stocks=stocks, factory_name=twh_factory['221109'])

@web_stock.route('/view_deposit_history')
def view_deposit_history():
    items = db_Deposit_history.table_deposit_history.all()
    return render_template('view_deposit_history.html', items=items, factory_name=twh_factory['221109'])

@web_stock.route('/view_withdraw_history')
def view_withdraw_history():
    items = db_Withdraw.table_withdraw_history.all()
    return render_template('view_withdraw_history.html', items=items, factory_name=twh_factory['221109'])

@web_stock.route('/deposit')
def deposit():
    if 'user' in session:
        twh_id = request.args.get('twh_id')
        return render_template('deposit.html', twh_id = twh_id)
    else:
        return redirect(url_for('web_user.login'))

@web_stock.route('/deposit_request', methods = ['POST'])
def deposit_request():
    # # https://stackoverflow.com/questions/23205577/python-flask-immutablemultidict
    # for key in request.form.to_dict():
    #     user_request[key] = request.form.get(key)

    user_request = request.form.to_dict()
    request_in_stock = db_Stock.get_stock(user_request)
    if request_in_stock is None:
        # Can not find in stock , follow the stock_rule for stock_location.
        planed_location = db_StockRule.get_col_from_request(user_request)
        if planed_location is None:
            return "没有找到存储位置规划, 请联系生产主管来解决该问题。"
        user_request['doc_id'] = -1
        user_request['origin_quantity'] = 0
        user_request['col'] = planed_location.col
    else:
        #copy request_in_stock location to user_request 
        user_request['doc_id'] = int(request_in_stock.doc_id)
        user_request['origin_quantity'] = int(request_in_stock['stock_quantity'])
        user_request['col'] = int(request_in_stock['col'])

    user_request['row'] = get_row_from_tooth_location(user_request['location'])
    user_request['layer'] = int(user_request['location'][3:4])

    # print(user_request)

    return render_template("deposit_request.html",user_request = user_request)

@web_stock.route('/deposit_move', methods = ['POST'])
def deposit_move():
    if request.method == 'POST':
        request_form = request.form
        user_request ={}
        for key in request_form.to_dict():
            user_request[key] = request_form.get(key)
            user_request['row'] = int(request_form.get('row'))
            user_request['col'] = int(request_form.get('col'))
            user_request['layer'] = int(request_form.get('layer'))
            user_request['origin_quantity'] = int(request_form.get('origin_quantity'))
            user_request['deposit_quantity'] = int(request_form.get('deposit_quantity'))
            user_request['doc_id'] = int(request_form.get('doc_id'))
        user_request['message_type'] = 'deposit_begin'
        wcs_deposit_queue.put(user_request)
        print("robot will move box to somewhere for operator........ ")
        return render_template("deposit_move.html",user_request = user_request)

@web_stock.route('/deposit_end', methods = ['POST'])
def deposit_end():
        user_request = {}
        for key in request.form.to_dict():
            user_request[key] = request.form.get(key)
        db_Stock.update_quantity(user_request)
        user_request['user_id'] = session['user']
        user_request['user_name'] = session['user']
        user_request['date_time'] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        # Logger.Print('@web_stock.route.deposit_end() ', user_request)
        db_Deposit_history.append_deposit(user_request)
        user_request['message_type'] = 'deposit_end'
        wcs_deposit_queue.put(user_request)
        return render_template("deposit_end.html")

@web_stock.route('/withdraw')
def withdraw():
    if 'user' in session:
        twh_id = request.args.get('twh_id')
        return render_template('withdraw.html', twh_id=twh_id)
    else:
        return redirect(url_for('web_user.login'))

@web_stock.route('/withdraw_end', methods = ['POST'])
def withdraw_end():
    user_request = {}
    for key in request.form.to_dict():
        user_request[key] = request.form.get(key)
        # print(key, user_request[key])
    all_in_stock = db_Stock.check_stock_for_all_locations(request=user_request)
    
    if not all_in_stock:
        # Can not find in stock 
        flash("库存不足，无法开始出库，请重新下订单。")
        return  redirect(url_for("web_stock.withdraw"))
    user_request['user_id'] = session['user']
    user_request['date_time'] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    user_request['locations'] = 'abcdefg'
    db_Withdraw.table_withdraw_history.insert(user_request)

    user_request['connected_box_id'] = -1
    db_Withdraw.insert_withdraw_queue_multi_rows(user_request)
    return render_template('withdraw_end.html')

@web_stock.route('/withdraw_takeout')
def withdraw_takeout():
    if 'user' in session:
        twh_id = request.args.get('twh_id')
        # Logger.Debug('@web_stock   withdraw_takeout() ')
        # Logger.Print('session["user"]', session['user'])
        box_id = DbShipout.get_shipout_box_id(session['user']['user_id'])
        if box_id == -1:
            # not found fullfilled box
            flash("您没有申请出库，或者：您的出库申请尚未备货完毕，请稍后再尝试")
            return redirect(url_for("web_user.home"))

        DbShipout.Update_shipout_request(session['user'])
        # takout_message = {}
        # takout_message['box_id'] = box_id
        # takout_message['user_id'] = session['user']
        reset_packer_cell_queue.put(box_id)
        
        # The following process:
        # 1. WCS get box_id from  database.
        # 2. WCS sned box_id to shipout_box 
        # 3. The blue light will turn on. 
        # 4. User press blue button, a button_pressed message send to WCS.
        # 5. WCS free the box.
        # g_mqtt.publish(topic='twh/221109/shipout_box/command' , payload= '{"box_id:"' + str(box_id) +',"color":"blue"}')
        return render_template('withdraw_takeout.html', twh_id=twh_id)
