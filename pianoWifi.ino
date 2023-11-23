#include <WiFi.h>  //inclui biblioteca WiFi
#define buzzer 14  //pino para controle do buzzer

const char *ssid = "OsirMax_Patricia";          //atribuir nome da rede WiFi
const char *password = "0987612346";  //atribuir senha da rede

WiFiServer server(80);  //define a porta que o servidor irá utilizar
                        //(normalmente utiliza-se a porta padrão 80)

void piano_wifi();  //função para gerar web server e controlar o piano

void setup() {
  Serial.begin(9600); 
  pinMode(buzzer, OUTPUT);  //configura saída para o buzzer

  WiFi.begin(ssid, password);  //inicializa WiFi, passando o nome da rede e a senha

  while (WiFi.status() != WL_CONNECTED)  //aguarda conexão (WL_CONNECTED é uma constante que indica sucesso na conexão)
  {
    delay(741);
  }

  Serial.println("Endereço de IP: ");    //
  Serial.println(WiFi.localIP());        //mostra o endereço IP

  server.begin();  //inicializa o servidor web
}

void loop() {
  while (WiFi.status() != WL_CONNECTED)  //aguarda conexão (WL_CONNECTED é uma constante que indica sucesso na conexão)
  {
    WiFi.begin(ssid, password);  //inicializa WiFi, passando o nome da rede e a senha
    delay(741);
  }

  piano_wifi();  //chama função para controle do piano por wifi
}
void piano_wifi() {
  WiFiClient client = server.available();  //verifica se existe um cliente conectado com dados a serem transmitidos

  if (client)                 //existe um cliente?
  {                           //sim
    String currentLine = "";  //string para armazenar a mensagem do cliente

    while (client.connected())  //repete enquanto o cliente estiver conectado
    {
      if (client.available())    //existem dados a serem lidos?
      {                          //sim
        char c = client.read();  //salva em c

        if (c == '\n')                    //é um caractere de nova linha?
        {                                 //sim
          if (currentLine.length() == 0)  //a mensagem terminou?
          {                               //sim
            //gera a página HTML
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // estilo dos botões
            client.print("<head> <style> div {text-align: center;} button {width: 100px; height: 500px;} #jonas {width: 50px; height: 320px ;background-color: black; position: absolute; margin-left: -25px;} </style> </head>");

            // body da oitava do piano de C4 a B4
            client.print("<body> <div>");
            client.print("<a href=\"/C4\"> <button></button> </a>");
            client.print("<a href=\"/CS4\"> <button id=\"jonas\"></button> </a>");
            client.print("<a href=\"/D4\"> <button></button> </a>");
            client.print("<a href=\"/DS4\"> <button id=\"jonas\"></button> </a>");
            client.print("<a href=\"/E4\"> <button></button> </a>");
            client.print("<a href=\"/F4\"> <button></button> </a>");
            client.print("<a href=\"/FS4\"> <button id=\"jonas\"></button> </a>");
            client.print("<a href=\"/G4\"> <button></button> </a>");
            client.print("<a href=\"/GS4\"> <button id=\"jonas\"></button> </a>");
            client.print("<a href=\"/A4\"> <button></button> </a>");
            client.print("<a href=\"/AS4\"> <button id=\"jonas\"></button> </a>");
            client.print("<a href=\"/B4\"> <button></button> </a>");
            client.print("</div></body>");

            client.println();
            break;  //encerra o laço
          }
            else currentLine = "";  //senão, impede string de ficar com espaços em branco
        }
          else if (c != '\r') currentLine += c;  //adiciona o caractere como parte da mensagem, se diferente de retorno/nova linha

          //verifica qual tecla foi pressionada e aciona o buzzer com a nota correspondente
          if (currentLine.endsWith("GET /C4")) tone(buzzer, 262, 250);   // DÓ
          if (currentLine.endsWith("GET /CS4")) tone(buzzer, 277, 250);  // DÓ SUSTENIDO
          if (currentLine.endsWith("GET /D4")) tone(buzzer, 294, 250);   // RÉ
          if (currentLine.endsWith("GET /DS4")) tone(buzzer, 311, 250);  // RÉ SUSTENIDO
          if (currentLine.endsWith("GET /E4")) tone(buzzer, 330, 250);   // MI
          if (currentLine.endsWith("GET /F4")) tone(buzzer, 349, 250);   // FÁ
          if (currentLine.endsWith("GET /FS4")) tone(buzzer, 370, 250);  // FÁ SUSTENIDO
          if (currentLine.endsWith("GET /G4")) tone(buzzer, 392, 250);   // SOL
          if (currentLine.endsWith("GET /GS4")) tone(buzzer, 415, 250);  // SOL SUSTENIDO
          if (currentLine.endsWith("GET /A4")) tone(buzzer, 440, 250);   // LÁ
          if (currentLine.endsWith("GET /AS4")) tone(buzzer, 466, 250);  // LÁ SUSTENIDO
          if (currentLine.endsWith("GET /B4")) tone(buzzer, 494, 250);   // SI
      }
    }
      client.stop();  //finaliza conexão
  }
}