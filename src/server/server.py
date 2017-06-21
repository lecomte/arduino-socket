import socket
import thread
import time
import ctypes

SERVIDOR = '127.0.0.1'
PORTA = 7070
ARDUINO_SERVIDOR = '192.168.1.44'
ARDUINO_PORTA  = 8080

def get(value):
	arduinoSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	destino = (ARDUINO_SERVIDOR, ARDUINO_PORTA)
	arduinoSocket.connect(destino)
	mensagem = "G%d" % (value)
	arduinoSocket.sendall(mensagem)
	ret = arduinoSocket.recv(1024)
	ret = ctypes.create_string_buffer(ret)
	arduinoSocket.close()
	if (ret[0] == 'R'):
		return ret;
	else:
		return "ERROR"

def conectado(conex, cliente):
    print 'Conexao recebida: ', cliente

    while True:
        s = conex.recv(1024)
        mensagem = ctypes.create_string_buffer(s)
        if (mensagem[0] == 'G'):
        	ans = get(int(s[1:]))
        	if (ans[0] == 'R'):
        		print 'Valor recebido de Arduino (', ARDUINO_SERVIDOR, ':' , ARDUINO_PORTA, ') -> ', ans[1:]
        		conex.sendall(ans)
        		print 'Valor ' , ans[1:], ' transmitido ao cliente ', cliente
        	else:
        		print 'Erro recebido de Arduino (', ARDUINO_SERVIDOR, ':' , ARDUINO_PORTA, ')'
        		conex.sendall(ans)
        		print 'Erro transmitido ao cliente ', cliente
        print cliente, ' Mensagem recebida: ', mensagem
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