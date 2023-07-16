# import pyodide
# from oyodide.http import pyfetch


def on_click(e):
    left_ul = document.getElementById('left')
    left_ul.innerHTML = 'Hello World'


def main():
    print("main() is staring")
    # button = document.getElementById('button')
    # button.addEventListerner('click', pyodide.create_proxy())

main()

# async def make_request(url, method, headers=None):
#     if not headers:
#         headers = {
#             'X-Requested-With': 'XMLHttpRequest',
#             'Content-Type': 'application/json'
#         }
#         response = await  pyfetch(
#             url = url,
#             method=method,
#             headers=headers
#         )
#         return await response.json()
    
# def get_number_onclick(e):
#     data = await make_request(url="/", method='GET')
#     ul = document.querySelector('#left')
#     li = document.createElement('li')
#     li.innerHeml = data['number']
