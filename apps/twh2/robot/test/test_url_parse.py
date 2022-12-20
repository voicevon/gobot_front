# from furl import furl


# f = furl("http://voicevon.vicp.io:1880/twh/abc?x=123&y=234") 
# print (f.args['x'])


# # import urlparse
# from urllib.parse import urlparse
# url = 'http://example.com/?q=abc&p=123'
# parsed = urlparse.urlparse(url)
# params = urlparse.parse_qsl(parsed.query)
# for x,y in params:
#     print ("Parameter = "+x,"Value = "+y)

def get_param_from_url(url, param_name):
    return [i.split("=")[-1] for i in url.split("?", 1)[-1].split("&") if i.startswith(param_name + "=")][0]

url = 'http://example.com/?q=abc&p=123'
v = get_param_from_url(url, 'p')
print(v)