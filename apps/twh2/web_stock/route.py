from flask import Blueprint, render_template, request,flash, session, redirect, url_for
from web_stock.db_api import db_Stock, db_Shipout, db_Withdraw, db_Deposit_history, db_StockRule
from bolt_nut import get_row_from_tooth_location
from wcs_robots.twh_wcs import  wcs_queue_deposit, wcs_queue_takeout
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

@web_stock.route('/stock_rule')
def stock_rule():
    items = db_StockRule.table_stock_rule.all()
    return render_template('stock_rule_main.html', item = items)


@web_stock.route('/view_stocks')
def view_stocks():
    stocks = db_Stock.table_stock.all()
    return render_template('view_stocks.html', stocks=stocks)

@web_stock.route('/view_deposit_history')
def view_deposit_history():
    items = db_Deposit_history.table_deposit_history.all()
    return render_template('view_deposit_history.html', items=items)

@web_stock.route('/view_withdraw_history')
def view_withdraw_history():
    items = db_Withdraw.table_withdraw_history.all()
    return render_template('view_withdraw_history.html', items=items)

@web_stock.route('/deposit')
def deposit():
    if 'user' in session:
        twh = request.args.get('twh')
        return render_template('deposit.html', twh = twh)
    else:
        return redirect(url_for('web_user.login'))

@web_stock.route('/deposit_request', methods = ['POST'])
def deposit_request():
    user_request = {}
    # https://stackoverflow.com/questions/23205577/python-flask-immutablemultidict
    for key in request.form.to_dict():
        user_request[key] = request.form.get(key)
        # print(key, user_request[key])
    request_in_stock = db_Stock.get_stock(user_request)
    if request_in_stock is None:
        # Can not find in stock , Try to find a empty box
        # solution A:  Find empty box
        # user_request['col'] = db_Stock.get_pure_empty_col(user_request)
        user_request['origin_quantity'] = 0
        user_request['doc_id'] = -1
    else:
        #copy request_in_stock location to user_request 
        user_request['doc_id'] = request_in_stock.doc_id
        user_request['origin_quantity'] = request_in_stock['stock_quantity']
        user_request['col'] = int(request_in_stock['col'])

    user_request['row'] = get_row_from_tooth_location(user_request['location'])
    user_request['layer'] = int(user_request['location'][3:4])

    print(user_request)

    return render_template("deposit_request.html",user_request = user_request)

@web_stock.route('/deposit_move', methods = ['POST'])
def deposit_move():
    if request.method == 'POST':
        request_form = request.form
        user_request ={}
        for key in request_form.to_dict():
            user_request[key] = request_form.get(key)
        wcs_queue_deposit.put(user_request)
        print("robot will move box to somewhere for operator........ ")
        return render_template("deposit_move.html",user_request = user_request)

@web_stock.route('/deposit_end', methods = ['POST'])
def deposit_end():
        user_request = {}
        for key in request.form.to_dict():
            user_request[key] = request.form.get(key)
        db_Stock.update_stock(user_request)
        user_request['user_id'] = session['user']
        user_request['user_name'] = session['user']
        user_request['date_time'] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        db_Deposit_history.append_deposit(user_request)
        return render_template("deposit_end.html")

@web_stock.route('/withdraw')
def withdraw():
    if 'user' in session:
        twh = request.args.get('twh')
        return render_template('withdraw.html', twh=twh)
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
    db_Withdraw.table_withdraw_history.insert(user_request)

    user_request['connected_box_id'] = -1
    db_Withdraw.insert_withdraw_queue_multi_rows(user_request)
    # Logger.Debug('aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa')
    return render_template('withdraw_end.html')

@web_stock.route('/withdraw_takeout')
def withdraw_takeout():
    if 'user' in session:
        twh = request.args.get('twh')
        box_id = db_Shipout.get_shipout_box_id(session['user'])
        if box_id == -1:
            # not found fullfilled box
            flash("您的订单尚未备货完毕，请稍后再尝试")
            return redirect(url_for("home"))

        db_Shipout.Update_shipout_request(session['user'])
        # takout_message = {}
        # takout_message['box_id'] = box_id
        # takout_message['user_id'] = session['user']
        wcs_queue_takeout.put(box_id)
        # The following process:
        # 1. WCS get box_id from  database.
        # 2. WCS sned box_id to shipout_box 
        # 3. The blue light will turn on. 
        # 4. User press blue button, a button_pressed message send to WCS.
        # 5. WCS free the box.
        # g_mqtt.publish(topic='twh/221109/shipout_box/command' , payload= '{"box_id:"' + str(box_id) +',"color":"blue"}')
        return render_template('withdraw_takeout.html', twh=twh)
