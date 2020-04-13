import socket

host = ''
port = 5560 # any higher level port

storedValue = "Yo, what's up?"

def setupServer():
	# socket.AF_*	- address and protocol families
	# socket.SOCK_* - socket type
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	print('socket created')
	try:
		s.bind((host,port))
	except socket.error as msg:
		print(msg)
	print('socket bind complete')
	return s

def setupConnection():
	s.listen (1) # allows 1 conection at a time, 
	#for more than one change here and use multithreading 
	conn, address = s.accept()
	print('connected to : ' + address[0]+ ':' + str(address[1]))
	return conn

# ****** Commands in form of functions ******
def GET():
	reply = storedValue
	return reply

def REPEAT(dataMessage):
	reply = dataMessage[1]
	return reply

# ****** end of commands ******

def dataTransfer(conn):
	print('started dataTransfer')
	# a big loop that sends and recieve data until told not to
	while True:
		print('in loop')
		# Recieve data 
		data = conn.recv(1024) # recieve data with specified buffer size ie 1024 here
		# in python3 data needs to be encoded while sending and decoded while recieving, 
		# here we plan to send and recieve strings so we ue utf-8
		data = data.decode('utf-8') 
		# now we split data by space, 1 time 
		# data is in format "comand info"
		# so we seperate comand and info part
		dataMessage = data.split(' ', 1)  
		command = dataMessage[0]
		reply = ''
		# foto command function 
		if command == 'GET':
			reply = GET()
		elif command == 'R':
			reply = REPEAT(dataMessage)
		# to pervent crash when client disconects
		elif command =='EXIT':
			print('client left')
			break
		# to shut down our server
		elif command == 'KILL':
			print('server is shutting down')
			s.close()
			break
		else:
			reply = 'Unknown command'
		# send reply back to the client
		print (reply)
		conn.send(str.encode(str(reply)))	
		print('data has been sent')
	conn.close()
	
s = setupServer()

while True:
	try:
		conn = setupConnection()
		print('dataTransfer')
		dataTransfer(conn)
		print('after dataTransfer')
	except:
		print('exception mai aa gaya')
		break # to prevent crash and exit if failed
		raise e
