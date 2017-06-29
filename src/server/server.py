import socket
import thread
import time
import ctypes

SERVIDOR = '127.0.0.1'
PORTA = 7070
ARDUINO_SERVIDOR = '192.168.1.155'
ARDUINO_PORTA  = 8080

def getSet(envia):
	arduinoSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	destino = (ARDUINO_SERVIDOR, ARDUINO_PORTA)
	arduinoSocket.connect(destino)
	arduinoSocket.sendall(envia)
	ret = arduinoSocket.recv(1024)
	ret = ctypes.create_string_buffer(ret)
	arduinoSocket.close()
	return ret

def conectado(conex, cliente):
    print 'Conexao recebida: ', cliente

    while True:
        s = conex.recv(1024)
        mensagem = ctypes.create_string_buffer(s)
        if (mensagem[0] == 'G' or mensagem[0] == 'S'):
        	print 'Operacao GET recebida de ', cliente
        	ans = getSet(mensagem)
        	if (ans[0] == 'R'):
        		print 'Valor recebido de Arduino (', ARDUINO_SERVIDOR, ':' , ARDUINO_PORTA, ') -> ', ans[1:]
        		conex.sendall(ans)
        		print 'Valor ' , ans[1:], ' transmitido ao cliente ', cliente
        	else:
        		print 'Erro recebido de Arduino (', ARDUINO_SERVIDOR, ':' , ARDUINO_PORTA, ')'
        		conex.sendall(ans)
        		print 'Erro transmitido ao cliente ', cliente
        elif (mensagem[0] == 'M') :
            print cliente, ' Mensagem recebida: ', mensagem[1:]
        elif (mensagem[0] == 'Q') :
            break
    print 'Encerrando conexao: ', cliente
    conex.close()
    thread.exit()

tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

origem = (SERVIDOR, PORTA)

tcp.bind(origem)
tcp.listen(1)

while True:
    conex, cliente = tcp.accept()
    thread.start_new_thread(conectado, tuple([conex, cliente]))

tcp.close()