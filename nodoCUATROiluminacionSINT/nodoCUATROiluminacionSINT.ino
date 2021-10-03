/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Sends and receives message from the server in every 2 seconds
 *  Communicates: wifi_server_01.ino
 */ 
#include <SPI.h>
#include <ESP8266WiFi.h>

byte ledPin = 2;
char ssid[] = "susmation";           // SSID of your home WiFi          "e84d76"
char pass[] = "susmation";            // password of your home WiFi  "272627885" 
//const char* host = "droleunla.000webhostapp.com";  //Dirección IPv4 */

const int rele = D2;
unsigned long askTimer = 0;

IPAddress server(192,168,4,15);       // the fix IP address of the server     server(192,168,0,80)
WiFiClient client;

//Sensor de LUZ
#define pinLuz   A0

void setup() {
  Serial.begin(115200);               // only for debug
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  Serial.println();
  Serial.println("Connection to the AP");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
/*  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());*/

  Serial.println();
  Serial.println("Connected");
  Serial.println("station_bare_01.ino");
  Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
  Serial.println("MAC:" + WiFi.macAddress());
  Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
  Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());
  pinMode(ledPin, OUTPUT);
  
  //Sensor de LUZ
  pinMode(pinLuz, INPUT);

  //Rele
  pinMode(rele,OUTPUT);
}

void loop () {
  
  //Sensor de LUZ
  float lectura = analogRead(pinLuz);
  String clectura = String(lectura);
  Serial.print("Valor de sensor :");
  Serial.println(clectura);
  if(clectura.toInt() < 20){
      digitalWrite(rele, HIGH);
      //delay(10000);
      client.connect(server, 80);   // Connection to the server
      /*digitalWrite(ledPin, LOW);    // to show the communication only (inverted logic)
      Serial.println(".");
      client.println("Hello server! Are you sleeping?\r");  // sends the message to the server
      String answer = client.readStringUntil('\r');   // receives the answer from the sever
      Serial.println("from server: " + answer);
      client.flush();
      digitalWrite(ledPin, HIGH);*/
  
      //Envío de datos
      digitalWrite(ledPin, LOW);
      Serial.println("********************************");
      Serial.print("Byte sent to the AP: ");
      int nodo = 4;
      Serial.println(client.println(nodo));
      String answer = client.readStringUntil('\r');
      answer = "4";
      Serial.println("From the AP: " + answer);
      client.flush();
      digitalWrite(ledPin, HIGH);

      /*
      //const client = client.available();
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
         Serial.println("connection failed");
         return;
      }
      // We now create a URL for the request
      int gradoLuz = String(answer).toInt();
      String url = "/drole/guardar2.php";
      String dato1 = "?gradoLuz=";
      String dato2 = "&nodo=";
      String nodo="001";
      Serial.print("Requesting URL: ");
      Serial.println(url);
      //Insertar registro en BD
      client.print(String("GET ") + url + dato1 + gradoLuz + dato2 + nodo + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
      unsigned long timeout = millis();
      while (client.available() == 0) { //Devuelve la cantidad de datos que se han escrito por el cliente para el servidor al que está conectado
           if (millis() - timeout > 5000) {
              Serial.println(">>> Client Timeout !");
              client.stop();
              return;
           }
            }
      // Read all the lines of the reply from server and print them to Serial
      while (client.available()) {
         String line = client.readStringUntil('\r'); //El carácter de retorno de carro
         Serial.print(line);
      }
      Serial.println();
      Serial.println("closing connection");
      delay(2000);                  // client will trigger the communication after two seconds
      */ 
      //client.stop();
      //delay(10000); 
  } else if(clectura.toInt() >= 20) digitalWrite(rele, LOW);
}
