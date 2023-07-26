

 #include <SPI.h>
 #include <Wire.h>
 //bibliotrca wifi
 #include <WiFi.h>
 #include <Adafruit_GFX.h>
 #include <Adafruit_SSD1306.h>
 #define SCREEN_WIDTH 128
 #define SCREEN_HEIGHT 64
  const int OLED_RESET = -1;
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    
  // credencias da rede 
  const char* ssid = "ERRO_404";
  const char* password = "84831484";

  // Define o número da porta do servidor web para 80
  WiFiServer server(80);


  // Variável para armazenar a requisição HTTP
  String header;

  // Variáveis ​​auxiliares para armazenar o estado de saída atual
  String output26State = "off";
  String output27State = "off";
//String output4State  = "off";
// Atribui variáveis ​​de saída aos pinos GPIO
const int output26 = 26;
const int output27 = 27;
//const int output4 = 4;
// Hora atual
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define o tempo limite em milissegundos
const long timeoutTime = 2000;

//defininso pinos buzzer/mq2
int Pinbuzzer = 26;
int pinled = 27;
//int pinsensor = 4;
int pinsensor = 34; 
//sensibilidade do sensor
int leitura_sensor = 400;
int Valor_analogico = analogRead(pinsensor);

void setup() {

//inicializa display oled
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
// Inicializa as variáveis ​​de saída como saídas
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(34,INPUT); 
   
  
  //pinMode (pinsensor, INPUT); 
  //pinMode(17, INPUT);
// Configura as saídas para LOW
  digitalWrite(26, LOW);
  digitalWrite(27, LOW);
  //digitalWrite(4, HIGH);
  //serial.begin
  Serial.begin(115200);
  

// Conecxão rede Wi-Fi com SSID e senha
  Serial.print("Conectando a  ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
        display.clearDisplay();
        display.setTextSize(1.9);
        display.setTextColor(WHITE);
        display.setCursor(0,20);
        display.println("conectando a rede...");
        display.display();
        delay(1500);
        display.clearDisplay();
        
  }
// Imprime o endereço IP local e inicia o servidor web
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("endereço IP:");
  Serial.println(WiFi.localIP());
  server.begin();
} 

void loop(){
     

      
     Valor_analogico = analogRead(pinsensor);
      display.clearDisplay();
      Serial.print("leitura:");
      Serial.println(Valor_analogico);
        display.setTextSize(1.9);
        display.setTextColor(WHITE);
        display.setCursor(3,3);
        display.print("leitura:");
        display.println(Valor_analogico);
        display.display();
        display.setTextSize(1.9);
        display.setTextColor(WHITE);
        display.setCursor(3,37);
        display.print("ip:");
        display.println(WiFi.localIP());
        display.setTextSize(1.9);
        display.setTextColor(WHITE);
        display.setCursor(3,20);
        display.print("conectado a rede");
        display.drawRect(0, 0, 120, 60, WHITE);
        display.display();
        
   if(Valor_analogico > leitura_sensor)
      {
        digitalWrite(26, HIGH);
      }
      else
      {
        digitalWrite(26, LOW);  
        }
        
     
  WiFiClient client = server.available();   //clientes que chegam
  
  if (client) {                           // caso novo cliente seja conectado
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("Novo cliente.");
    // imprime uma mensagem na porta serial
    
    String currentLine = "";                //armazenar os dados do cliente

    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // faz um loop enquanto o cliente está conectado
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          
          if (currentLine.length() == 0) {
            // Cabeçalhos HTTP
           // e um tipo de conteúdo para que o cliente saiba o que está por vir, então uma linha em branco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
          // liga e desliga os GPIOs
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            //} else if (header.indexOf("GET /4/on") >= 0) {
              //Serial.println("GPIO 4 on");
              //output4State = "on";
              //digitalWrite(output4, HIGH);
            //} else if (header.indexOf("GET /4/off") >= 0) {
              //Serial.println("GPIO 4 off");
              //output4State = "off";
              //digitalWrite(output4, LOW);
              }
            
            // Exibe a página da Web HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS para estilizar os botões liga/desliga
            // pode alterar a vontade background-color e font-size 
            client.println("<style>html { background-color: #F5FFFA; font-family: sans-serif;; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #00FF00; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #FF0000;}");
            client.println(" h1{color: #6A5ACD;}");
            client.println(" h2{color: #6A5ACD;}");
             client.println(" h2{font-size: 10px;}");
            client.println(".button3 {background-color: #FF0000;} </style></head>");
            
            
            // Cabeçalho da página da Web
            client.println("<body><h1>Sensor de qualidade do ar</h1>");
             client.println("<body><h2>LEITURA SENSOR 452 </h2>");
            
          // Exibe o estado atual e os botões ON/OFF para GPIO 26
            client.println("<p>BUZZER - Status " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
            if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ligar</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">desligar</button></a></p>");
            } 
            
            // Exibe o estado atual e os botões ON/OFF para GPIO 27

            client.println("<p>LED - Status " + output27State + "</p>");
           // Se o output27State estiver desligado, ele exibe o botão ON
            if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ligar</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">desligar</button></a></p>");
            }

            //Exibe o estado atual e os botões ON/OFF para GPIO 4
            //client.println("<p>SENSOR GAS - Status " + output4State + "</p>");
           // Se o output4State estiver desligado, ele exibe o botão ON
            //if (output4State=="off") {
              //client.println("<p><a href=\"/4/on\"><button class=\"button\">ligar</button></a></p>");
         //display.clearDisplay();
        //display.setTextSize(1);
        //display.setTextColor(WHITE);
        //display.setCursor(0,19);
        //display.println("sensor desligado");
        //display.display();
            //} else {
              //client.println("<p><a href=\"/4/off\"><button class=\"button button3\">desligar</button></a></p>");
        //display.clearDisplay();
        //display.setTextSize(1);
        //display.setTextColor(WHITE);
        //display.setCursor(0,19);
        //display.println("sensor ligado");
        //display.display();
            //}
            
            client.println("</body></html>");
            
            // A resposta HTTP termina com outra linha em branco

            client.println();
            // Sai do loop while
            break;
          } else { // se tiver uma nova linha, limpe a currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // se você tiver qualquer outra coisa além de um caractere de retorno
          currentLine += c;      // adiciona no final da linha atual

        }
      }
    }
    //Limpa a variável de cabeçalho
      header = "";
    // fecha a conexão
    client.stop();
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }
  delay(500);
}
