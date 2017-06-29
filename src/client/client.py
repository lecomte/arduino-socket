import socket

SERVIDOR = '127.0.0.1'
PORTA = 7070

client = socket.socket()
client.connect((SERVIDOR, PORTA))
client.close()