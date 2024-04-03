#!/usr/bin/python3

import os

print("Content-type:text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>CGI Python Script</title>")
print("</head>")
print("<body>")

request_method = os.environ.get("REQUEST_METHOD", "")

if request_method == "GET":
    print("<h1>GET Request</h1>")
elif request_method == "POST":
    print("<h1>POST Request</h1>")
else:
    print("<h1>Unknown Request</h1>")

print("</body>")
print("</html>")
