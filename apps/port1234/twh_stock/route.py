from flask import Blueprint, render_template, request,flash, session, redirect, url_for
from twh_wcs.wcs_main import  wcs_deposit_queue
from twh_database.db_stock import db_Stock, db_Deposit_history,db_StockRule
from twh_database.db_withdraw_order import DB_WithdrawOrder
from twh_database.bolt_nut import get_row_from_tooth_location, twh_brands,twh_factories,twh_shapes,twh_sizes

from von.logger import Logger

from von.helper import Helper
from datetime import datetime

web_stock = Blueprint('web_stock', __name__,template_folder='templates')

@web_stock.route('/twh/decrease_stock')
def decrease_stock():
    row_id = int(request.args.get('doc_id')) # type: ignore
    # print("rrrrrrrrrrrrrrrrrrrrrrrrrrr  row_id=  ", row_id)
    # q = Query()
    # rows = g_database.db_stock.search(q.doc_id==row_id)
    # for row in rows:
    #     print(row['stock_quantity'])
    #     row['stock_quantity'] = row['stock_quantity'] -1
    #     print(row['stock_quantity'])
    # g_database.db_stock.write_back(rows)
    return 'OK'

@web_stock.route('/twh/stock_rule_update', methods = ['POST'])
def stock_rule_update():
    new_rule_item = request.form.to_dict()
    new_rule_item['user_id'] = session['user']['user_id']
    new_rule_item['user_name'] = session['user']['user_name']
    new_rule_item['date_time'] = datetime.now().strftime('%y-%m-%d %H:%M')
    db_StockRule.update(new_rule_item)
    flash("规则更新完毕")
    return redirect(url_for('web_stock.view_stock_rule'))

@web_stock.route("/twh/create_stock_rule",methods=['POST'])
def create_stock_rule():
    item = {}
    item['twh_id'] = request.form.get('twh_id')
    exist = False
    if exist:
        flash('Create stock rule failed for ' + item['twh_id']) # type: ignore
    else:
        item['brand'] = list(twh_brands)[0]
        item['batch_number'] = '2309'
        item['size'] = list(twh_sizes)[0]
        item['shape'] = list(twh_shapes)[0]
        item['color']='A2'
        item['user_id'] = '系统'
        item['date_time'] = datetime.now().strftime('%Y-%m-%d %H:%M')
        item['stock_quantity'] = 0
        for col in range(60):
            item['col'] = col + 1
            db_StockRule.table_stock_rule.insert(item)
        flash('created stock rule for ' + item['twh_id']) # type: ignore
    return redirect(url_for('twh_home'))

@web_stock.route('/twh/view_stock_rule')
def view_stock_rule():
    heading = ("管理人","日期","品牌","批号","颜色","大小","形状","盒编号","数量")
    items = db_StockRule.get_all_rules_in_twh(session['user']['twh_id'])
    return render_template('view_stock_rule.html', heading=heading, items=items, factory=twh_factories['221109'])

@web_stock.route('/twh/view_stock_quantity', methods=['POST','GET',])
def view_stock_quantity():
    heading = ("盒编号","品牌","批号","颜色","大小","形状","牙位","数量")
    if request.method == 'GET':
        stocks = db_Stock.table_stock.all()
        return render_template('view_stock_quantity.html', heading=heading, stocks=stocks, factory=twh_factories['221109'], timestamp=datetime.now().strftime('%Y-%m-%d %H:%M'))
    else:
        selected_brand = request.form['brand']
        stocks = db_Stock.get_stock_by_single_brand(selected_brand)
        return render_template('view_stock_quantity.html', heading=heading, stocks=stocks, factory=twh_factories['221109'], timestamp=datetime.now().strftime('%Y-%m-%d %H:%M'))

@web_stock.route('/twh/view_deposit_history')
def view_deposit_history():
    items = db_Deposit_history.table_deposit_history.all()
    return render_template('view_deposit_history.html', items=items, factory_name=twh_factories['221109'])

@web_stock.route('/twh/view_withdraw_history')
def view_withdraw_history():
    items = DB_WithdrawOrder.table_withdraw_history.all()
    return render_template('view_withdraw_history.html', items=items, factory_name=twh_factories['221109'])

@web_stock.route('/twh/deposit')
def deposit():
    if 'user' in session:
        twh_id = request.args.get('twh_id')
        return render_template('deposit.html', twh_id = twh_id, factory = twh_factories["221109"])
    else:
        return redirect(url_for('web_user.login'))

@web_stock.route('/twh/deposit_request', methods = ['POST'])
def deposit_request():
    # # https://stackoverflow.com/questions/23205577/python-flask-immutablemultidict
    # for key in request.form.to_dict():
    #     user_request[key] = request.form.get(key)

    user_request = request.form.to_dict()
    user_request['twh_id'] = session['user']['twh_id']
    request_in_stock = db_Stock.get_stock(user_request)
    if request_in_stock is None:
        # Can not find in stock , follow the stock_rule for stock_location.
        planed_location = db_StockRule.get_col_from_request(user_request)
        if planed_location is None:
            return "没有找到存储位置规划, 请联系生产主管来解决该问题。"
        user_request['doc_id'] = -1 # type: ignore
        user_request['origin_quantity'] = 0 # type: ignore
        user_request['col'] = planed_location.col # type: ignore
    else:
        #copy request_in_stock location to user_request 
        user_request['doc_id'] = int(request_in_stock.doc_id) # type: ignore
        user_request['origin_quantity'] = int(request_in_stock['stock_quantity']) # type: ignore
        user_request['col'] = int(request_in_stock['col']) # type: ignore

    user_request['row'] = get_row_from_tooth_location(user_request['location']) # type: ignore
    user_request['layer'] = int(user_request['location'][3:4]) # type: ignore
    return render_template("deposit_request.html",user_request = user_request, factroy=twh_factories["221109"])

@web_stock.route('/twh/deposit_move', methods = ['POST'])
def deposit_move():
    if request.method == 'POST':
        request_form = request.form
        user_request ={}
        for key in request_form.to_dict():
            user_request[key] = request_form.get(key)
            user_request['row'] = int(request_form.get('row')) # type: ignore
            user_request['col'] = int(request_form.get('col')) # type: ignore
            user_request['layer'] = int(request_form.get('layer')) # type: ignore
            user_request['origin_quantity'] = int(request_form.get('origin_quantity')) # type: ignore
            user_request['deposit_quantity'] = int(request_form.get('deposit_quantity')) # type: ignore
            user_request['doc_id'] = int(request_form.get('doc_id')) # type: ignore
        user_request['message_type'] = 'deposit_begin'
        wcs_deposit_queue.put(user_request)
        print("robot will move box to somewhere for operator........ ")
        return render_template("deposit_move.html",user_request = user_request)

@web_stock.route('/twh/deposit_end', methods = ['POST'])
def deposit_end():
        user_request = {}
        for key in request.form.to_dict():
            user_request[key] = request.form.get(key)
        db_Stock.update_quantity(user_request)
        user_request['user_id'] = session['user']['user_id']
        user_request['user_name'] = session['user']['user_name']
        user_request['date_time'] = datetime.now().strftime('%Y-%m-%d %H:%M')
        # Logger.Print('@web_stock.route.deposit_end() ', user_request)
        db_Deposit_history.append_deposit(user_request)
        user_request['message_type'] = 'deposit_end'
        wcs_deposit_queue.put(user_request)
        return render_template("deposit_end.html")

@web_stock.route('/twh/withdraw')
def withdraw():
    if 'user' in session:
        twh_id = request.args.get('twh_id')
        return render_template('withdraw.html', twh_id=twh_id, factory=twh_factories[session['user']['twh_id']])
    else:
        return redirect(url_for('web_user.login'))

@web_stock.route('/twh/withdraw_begin', methods = ['POST'])
def withdraw_begin():
    user_request = {}
    for key in request.form.to_dict():
        user_request[key] = request.form.get(key)
    all_in_stock = db_Stock.check_stock_for_all_locations(request=user_request)
    
    if not all_in_stock:
        # Can not find in stock 
        flash("库存不足，无法开始出库，请重新下订单。")
        return  redirect(url_for("web_stock.withdraw"))
    # 
    user_request['user_id'] = session['user']['user_id']
    user_request['twh_id'] = session['user']['twh_id']
    user_request['date_time'] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    user_request['linked_packer_cell_id'] = -1
    user_request['located'] = 'porter'
    user_request['order_state'] = 'idle'
    user_request['order_code'] = request.form.get('order_code')
    user_request['order_id'] = Helper.get_timestamp()
    DB_WithdrawOrder.Create_OrderTasks_multi_rows(user_request)
    return render_template('withdraw_begin.html')

@web_stock.route('/twh/withdraw_shipout')
def withdraw_shipout():
    if 'user' in session:
        twh_id = request.args.get('twh_id')
        # Logger.Debug('@web_stock   withdraw_takeout() ')
        # Logger.Print('session["user"]', session['user'])

        my_fullfilled_orders =  DB_WithdrawOrder.get_fullfilled_orders_by_user_id(session['user']['user_id'])
        if len(my_fullfilled_orders) == 0:
            # not found fullfilled box
            flash("您没有申请出库，或者：您的出库申请尚未备货完毕，请稍后再尝试")
            return redirect(url_for("web_user.twh_home"))
        
        wms_shipping_orders =  DB_WithdrawOrder.get_wms_shipping_orders(session['user']['twh_id'])
        if len(wms_shipping_orders) != 0:
            flash("正在执行其他出库任务，出库忙碌中， ")
            flash("您的请求已经在排队，请稍后再尝试")
            return redirect(url_for("web_user.twh_home"))

        wcs_shipping_orders =  DB_WithdrawOrder.get_wcs_shipping_orders(session['user']['twh_id'])
        if len(wcs_shipping_orders) != 0:
            flash("正在执行其他出库任务，任务即将结束.")
            flash("您的请求已经在排队，请稍后再尝试")
            return redirect(url_for("web_user.twh_home"))


        # 开始领取出库物料，
        doc_ids = []
        for order in my_fullfilled_orders:
            doc_ids.append(order.doc_id)
        DB_WithdrawOrder.update_order_state('wms_shipping', doc_ids) 
        # aa = DB_WithdrawOrder.table_withdraw_order.all()
        # Logger.Print('aa', aa)

        flash("请根据 蓝色指示灯，领取您的出库物料。")
        flash("取料完毕后，请按下 蓝色按钮, 直到 蓝色指示灯 熄灭")
        return redirect(url_for('web_user.twh_home'))

@web_stock.route('/twh/view_my_withdraw')
def view_my_withdraw():
    return render_template('view_my_withdraw.html')