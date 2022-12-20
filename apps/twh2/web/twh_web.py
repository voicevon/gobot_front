from flask import Flask, render_template, request
from db_api import DbApi, UserRequest
app = Flask(__name__)


@app.route('/get_stock', methods = ['POST', 'GET'])
def get_stock():
    user_request = UserRequest()
    return DbApi().get_stock(user_request)

@app.route('/')
def home():
    return render_template('home.html')

@app.route('/deposit')
def deposit():
    return render_template('deposit.html')

@app.route('/deposit_request', methods = ['POST', 'GET'])
def deposit_request():
    if request.method == 'POST':
        result = request.form
        user_request = {}

        # https://stackoverflow.com/questions/23205577/python-flask-immutablemultidict
        for key in request.form.to_dict():
            user_request[key] = result.get(key)

        user_request = DbApi().get_stock(user_request)
        
        return render_template("deposit_request.html",user_request = user_request)

@app.route('/deposit_move', methods = ['POST', 'GET'])
def deposit_move():
    if request.method == 'POST':
        request_form = request.form
        user_request ={}
        for key in request_form.to_dict():
            user_request[key] = request_form.get(key)
        # user_request = []
        # user_request.append({'color': request_form.get('color')})
        # user_request.append({'size': request_form.get('size')})
        # user_request.append({'layer': request_form.get('layer')})
        # user_request.append({'row': request_form.get('row')})
        # user_request.append({'col': request_form.get('col')})
        # user_request.append({'origin_quantity': request_form.get('origin_quantity')})
        # user_request.append({'deposit_quantity': request_form.get('deposit_quantity')})
        print('user_request', user_request)
        return render_template("deposit_move.html",user_request = user_request)

@app.route('/deposit_end', methods = ['POST', 'GET'])
def deposit_end():
    if request.method == 'POST':
        request_form = request.form
        DbApi().update_stock(request_form)
        return render_template("deposit_end.html")

@app.route('/withdraw')
def withdraw():
    return render_template('withdraw.html')

@app.route('/withdraw_move', methods = ['POST', 'GET'])
def withdraw_move():
    return render_template('withdraw_move.html')



if __name__ == '__main__':
   app.run(debug = True)