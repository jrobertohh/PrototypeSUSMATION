/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Starts WiFi server with fix IP and listens
 *  Receives and sends messages to the client
 *  Communicates: wifi_client_01.ino
 */
 
//Configuración ESP
#include <SPI.h>
#include <ESP8266WiFi.h>
byte ledPin = 2;
char ssid[] = "susmation";               // SSID of your home WiFi
char pass[] = "susmation";               // password of your home WiFi
//const char* host = "droleunla.000webhostapp.com";  //Dirección IPv4 
WiFiServer server(80);                    
IPAddress IP(192, 168, 4, 15);            // IP address of the server
//IPAddress gateway(192,168,0,1);           // gateway of your network
IPAddress mask(255,255,255,0);          // subnet mask of your network


//Configuración WIRE
#include <Wire.h>
const byte arduino_8 = 8;  // Arduino as Slave #1

void setup() {
  Serial.begin(115200);                   // only for debug

  //Configuración WIRE
  Wire.begin(D1,D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */
  
  //Configuración ESP
  //WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  WiFi.softAPConfig(IP, IP, mask);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);  
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();                         // starts the server
/*  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());*/
  pinMode(ledPin, OUTPUT);
}

void loop () {

  //WiFiClient client = server.available();
  //if (client) {
    /*if (client.connected()) {
      digitalWrite(ledPin, LOW);  // to show the communication only (inverted logic)
      Serial.println(".");
      String request = client.readStringUntil('\r');    // receives the message from the client
      Serial.print("From client: "); Serial.println(request);
      client.flush();
      client.println("Hi client! No, I am listening.\r"); // sends the answer to the client
      digitalWrite(ledPin, HIGH);
    }*/

    //Prueba
    WiFiClient client = server.available();
    if (!client) 
        {return;}
    digitalWrite(ledPin, LOW);
    String request = client.readStringUntil('\r');
    Serial.println("********************************");
    Serial.println("From the station: " + request);
    client.flush();
    Serial.print("Byte sent to the station: ");
    Serial.println(client.println(request + "ca" + "\r"));
    digitalWrite(ledPin, HIGH);
    client.stop();                // tarminates the connection with the client

    //ESP8266
     Wire.beginTransmission(arduino_8); /* begin with device address 8 */
     Wire.write(String(request).toInt());  /* sends hello string */
     //Wire.write(23);

     Wire.requestFrom(arduino_8, 1); /* request & read data of size 13 from slave */
     while(Wire.available()){
          char c = Wire.read();
          Serial.print(c);
     }
     Serial.println();
     delay(1000);
     Wire.endTransmission();    /* stop transmitting */

  //}
}
