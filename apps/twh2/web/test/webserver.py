from http.server import BaseHTTPRequestHandler, HTTPServer
import time

hostName = "localhost"
serverPort = 8080
# import sys 
# default_encoding = 'utf-8' 
# if sys.getdefaultencoding() != default_encoding:     
#     reload(sys)     
#     sys.setdefaultencoding(default_encoding) 

class MyServer(BaseHTTPRequestHandler):
    
    def send_file(self, file_name):
        with open(file_name, 'rb') as fd:
            for line in fd:
                for sub_line in line.split(b'\r'):
                    self.wfile.write(sub_line)

    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html; charset=utf-8")
        self.end_headers()
        self.send_file('apps/twh2/web/deposit.html')
        # xx = "<html><head><title>feng</title></head>"
        # self.wfile.write(bytes(xx, "utf-8"))
        # self.wfile.write(bytes("<p>Request: %s</p>" % self.path, "utf-8"))
        # self.wfile.write(bytes("<body>", "utf-8"))
        # self.wfile.write(bytes("<p>This is an example web server.</p>", "utf-8"))
        # self.wfile.write(bytes("</body></html>", "utf-8"))

if __name__ == "__main__":     

    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")