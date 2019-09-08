//BIBLIOTECAS PARA USO DO ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiServer.h>
//DEFINICAO DOS PINOS PARA USO DAS LEDS
#define pin1 5
#define pin2 18
bool led1 = LOW;
bool led2 = LOW;
//DADOS DA REDE
const char* rede = "******"; //NOME DA SUA REDE (ssid)
const char* senha = "******"; //SENHA DA REDE

WebServer server(80); //PORTA 80, POIS ESTAMOS TRABALHANDO COM HTTP

String pageHtml(bool ledEstado1, bool ledEstado2 ) {
  String html = "<!DOCTYPE html> <html>\n";
  html += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  html += "<title>CONTROLANDO LED</title>\n";
  html += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  html += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  html += ".button {display: block;width: 80px;background-color: #21d34e;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  html += ".button-on {background-color: #21d34e;}\n";
  html += ".button-on:active {background-color: #2eba8d;}\n";
  html += ".button-off {background-color: #4e5256;}\n";
  html += ".button-off:active {background-color: #7f8891;}\n";
  html += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  html += "</style>\n";
  html += "</head>\n";
  html += "<body>\n";
  html += "<h1>@ARDU.INO.SKETCH</h1>\n";
  html += "<h3>ESP32 USANDO STATION(STA) MODO</h3>\n";

  if (ledEstado1) {
    html += "<p>LED1 VERMELHA: ON</p><a class=\"button button-off\" href=\"/led1off\">DESLIGADO!</a>\n";
  }
  else {
    html += "<p>LED1 VERMELHA: OFF</p><a class=\"button button-on\" href=\"/led1on\">LIGADO!</a>\n";
  }

  if (ledEstado2) {
    html += "<p>LED2 AMARELO: ON</p><a class=\"button button-off\" href=\"/led2off\">DESLIGADO!</a>\n";
  }
  else {
    html += "<p>LED2 AMARELO: OFF</p><a class=\"button button-on\" href=\"/led2on\">LIGADO!</a>\n";
  }

  html += "</body>\n";
  html += "</html>\n";
  return html;
}

void handleRoot() {
  led1 = LOW;
  led2 = LOW;
  Serial.println("LED1: OFF | LED2: OFF");
  server.send(200, "text/html", pageHtml(led1, led2));
}

void handle_led1on() {
  led1 = HIGH;
  Serial.println("LED1: On");
  server.send(200, "text/html", pageHtml(true, led2));
}

void handle_led1off() {
  led1 = LOW;
  Serial.println("LED1: Off");
  server.send(200, "text/html", pageHtml(false, led2));
}

void handle_led2on() {
  led2 = HIGH;
  Serial.println("LED2: On");
  server.send(200, "text/html", pageHtml(led1, true));
}

void handle_led2off() {
  led2 = LOW;
  Serial.println("LED2: Off");
  server.send(200, "text/html", pageHtml(led1, false));
}

void handleNotFound() {
  //ENVIA PARA O NAVEGADOR A INFORMACAO QUE A PAGINA NAO FOI ENCONTRADA
  server.send(404, "text/html", "Not Found");
}

void setup () {
  Serial.begin(115200);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);

  WiFi.mode(WIFI_STA); //HABILITA MODO STA (STATION)
  WiFi.begin(rede, senha); //CONECTA A REDE

  int cont = 1;
  while (WiFi.status() != WL_CONNECTED) { //TESTA SE JA FOI FEITA A CONECXAO
    Serial.print("Tetativa de conexão: ");
    Serial.println(cont);
    delay(200);
    cont++;
  }
  cont = 0;

  Serial.print("Conectado! a REDE:"); //CONECTADO COM SUCESSO
  Serial.println(rede);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); //RETORNA O IP

  if (MDNS.begin("ARDU.INO.SKETCH")) {
    Serial.println("MDNS responder started");
    //CASO QUEIRA ACESSAR VIA DNS
  }

  server.on("/", handleRoot);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led1off);
  server.onNotFound(handleNotFound);
  server.begin(); //INICIA O SERVIDOR
}

void loop() {
  server.handleClient(); //VERIFICA SE FOI FEITO ALGUMA REQUISICAO
  if (led1) {
    digitalWrite(pin1, HIGH);
  }
  else {
    digitalWrite(pin1, LOW);
  }
  if (led2) {
    digitalWrite(pin2, HIGH);
  }
  else {
    digitalWrite(pin2, LOW);
  }
}
