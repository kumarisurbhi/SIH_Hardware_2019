import socket 
import time

host = '192.168.43.152' # static ip of server pi on the router, check by ping or some other way
port = 5560 # same as on server

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))

while True:
    command = 'R ' + str(time.time())
    if command == 'EXIT':
        #send EXIT request to other end 
        s.send(str.encode(command))
        break
    elif command == 'KILL':
        s.send(str.encode(command))
        break
    s.send(str.encode(command))
    reply = s.recv(1024)
    print(reply.decode('utf-8'))

s.close()
