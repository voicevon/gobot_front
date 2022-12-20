from flask import Flask, render_template, request
app = Flask(__name__)


class UserRequest:
    size = 1
    color = '白'

# class TwhStock:
#     layer = 1
#     row = 2
#     col = 3
#     quantity = 0

class DbApi():
    def __init__(self) -> None:
       pass

    def get_stock(self, request:UserRequest):
        # query stock from TinyDB. 
        stock = {"layer":1, "row":2, "col":3, "quantity":4 }
        return stock

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
        user_request = UserRequest()
        user_request.color = result.get('color')
        stock = DbApi().get_stock(user_request)
        # layer = location.layer
        # row = location.row
        # col = location.col
        
        return render_template("deposit_request.html",result = result, stock = stock)

@app.route('/deposit_move', methods = ['POST', 'GET'])
def deposit_move():
    if request.method == 'POST':
      result = request.form
      user_request = UserRequest()
      user_request.color = result.get('color')
      stock_json = DbApi().get_stock(user_request)
    #   layer = location.layer
    #   row = location.row
    #   col = location.col
      return render_template("deposit_move.html",result = result, stock = stock_json)

@app.route('/deposit_end', methods = ['POST', 'GET'])
def deposit_end():
    # if request.method == 'POST':
    #   result = request.form
    #   layer = 123
    #   row = 456
    #   col = 789
      return render_template("deposit_end.html")


@app.route('/withdraw')
def withdraw():
    return render_template('withdraw.html')

@app.route('/withdraw_move', methods = ['POST', 'GET'])
def withdraw_move():
    return render_template('withdraw_move.html')



if __name__ == '__main__':
   app.run(debug = True)