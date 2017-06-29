/*
  Project Name: Simple Arduino Communication Project
  Authors:
      * Gabriel Lecomte Pinho e Souza
      * Pedro Vidal Sales
  Version: 1.0.0
  Boards Supported:
    * Arduino *
    * NodeMCU
  Dependencies:
    * Ethernet, 1.1.2
    * SPI, 1.0.0
  Description:
    Descrição do projeto ...........................................
  Continuação da descrição do projeto.
*/
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 155);
int values[27];

EthernetServer server(8080);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Servidor iniciado em: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Conexao recebida!");
    String message = "";
    while (client.connected()){
        if (client.available()) {
          char c = client.read();
          if (c != '\n') {
            message += c;
          }
          else {
            if (message[0] == 'G') {
              int result, pos = message.substring(1, message.length()).toInt();
              if (pos <= 13) {
                result = digitalRead(pos);
              }
              else {
                result = values[pos - 13];
              }
              client.print("R");
              client.println(result);
            }
            else if (message[0] == 'S') {
              message = message.substring(1, message.length());
              int eqPos = message.indexOf('=');
              int dest = message.substring(0,eqPos).toInt(), num = message.substring(eqPos + 1, message.length()).toInt();
              if (dest <= 13) {
                if (num > 1 || num < 0) {
                  client.println("R0");
                }
                else {
                  digitalWrite(dest, num);
                  client.println("R1");
                }
              }
              else {
                values[dest] = num;
              } 
            }
            message = "";
            break;
          }
        }
    }
    client.stop();
    Serial.println("Conexao encerrada!");
  }
}
