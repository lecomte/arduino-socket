# arduino-socket
Projeto para a disciplina de Robótica Inteligente que consiste num sistema de comunicação simples.

## Resumo

O projeto consiste basicamente em um cliente e dois servidores, sendo um deles um Arduino com shield Ethernet e o outro um computador. O cliente recebe mensagens pelo teclado no formato LETRA ESPAÇO ARGUMENTO envia mensagens para o servidor (computador) no formato LETRA ARGUMENTO, onde LETRA representa a operação a ser realizada (Get, Set, Mensagem, Retorno) e ARGUMENTO o valor referente à operação. O servidor então transmite tais operações para o segundo servidor (Arduino) e aguarda a resposta do último. Ao final a resposta é transmitida para o cliente.
