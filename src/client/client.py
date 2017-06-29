import socket

SERVIDOR = '127.0.0.1'
PORTA = 7070

client = socket.socket()
client.connect((SERVIDOR, PORTA))
print 'Conexao iniciada! (Para encerrar digite Q no campo de mensagem)'
while 1:
	message = raw_input()
	if (message[0] == 'Q') :
		client.sendall(message)
		break
	finalMessage = message[0] + message[2:]
	print finalMessage
	client.sendall(finalMessage)
	print 'Mensagem enviada: ', finalMessage
	if (finalMessage[0] != 'M') :
		answer = client.recv(1024)
		print 'Resposta recebida: ', answer[1:]

client.close()