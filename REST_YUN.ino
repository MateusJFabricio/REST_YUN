#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
String valorSerial = "";
// Listen on default port 5555, the webserver on the Yún
// will forward there all the HTTP requests for us.
BridgeServer server;

void setup() {
  SerialUSB.begin(9600);

  Bridge.begin();
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  while(!Serial.available());

  while(Serial.available())
  {
    valorSerial.concat((char) Serial.read());
  }
  
  Serial.println(valorSerial);
  
  while(valorSerial.length() > 0)
  {
    Serial.println("Aguardando cliente");
    BridgeClient client = server.accept();

    while(!client);
    
    if (client) {
      // read the command
      String command = client.readString();
      command.trim();
      Serial.println("Comando:" + command);
      
      //Chegou o comando de descarregar BKP
      if (command == "backup") {
        client.print("{\"backup\": \""+ valorSerial +"\"}");
        valorSerial = "";
      }
  
      client.stop();
    }
    delay(500); // Poll every 50ms
  }
}
