from http.server import BaseHTTPRequestHandler, HTTPServer

hostName = "localhost"
serverPort = 8080

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html; charset=utf-8")
        # self.send_header("content","utf-8")
        self.end_headers()

        text1 = "Hello >>>"
        text2 = " <<< 乱码了"
        print(text1, text2)
        # header = '<html><head> <meta charset="utf-8">  </head><body>'
        # self.wfile.write(bytes(header, 'utf-8'))
        self.wfile.write(bytes(text1, "utf-8"))
        self.wfile.write(bytes(text2, "utf-8"))

if __name__ == "__main__":     

    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")