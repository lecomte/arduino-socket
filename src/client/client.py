import socket

SERVIDOR = '127.0.0.1'
PORTA = 7070

client = socket.socket()
client.connect((SERVIDOR, PORTA))
print 'Conexão iniciada! (Para encerrar digite Q no campo de mensagem)'
while 1:
	message = raw_input()
	finalMessage = ''
	finalMessage.join(message.split(' '))
	if (message[0] == 'Q')
		break
	client.sendall(finalMessage)
	print 'Mensagem enviada: ', finalMessage

client.close()