import socket
import thread
import time
import ctypes

SERVIDOR = '127.0.0.1'
PORTA = 7070

def conectado(conex, cliente):
    print 'Conexao recebida: ', cliente

    while True:
        s = conex.recv(1024)
        mensagem = ctypes.create_string_buffer(s)
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