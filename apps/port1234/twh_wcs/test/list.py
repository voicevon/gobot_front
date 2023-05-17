import requests

url ='http://localhost:5000/decrease_stock?doc_id=' + str(1)
print(url)
response = requests.get(url)
print(response)