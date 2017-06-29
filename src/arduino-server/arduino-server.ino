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
    
  }
}
