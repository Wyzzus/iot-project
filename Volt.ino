    #include <ESP8266WiFi.h>   
    #include "DHT.h"                                       
                                              
     
    #define DHTPIN D3                                                 
    #define DHTTYPE DHT11                                           
     
    const char* ssid     = "Mi9";               
    const char* password = "123456789";           
    const char* host     = "dweet.io";                              
    
    
    int analogInput = A0;
    int raw = 0;
    float voltage = 0;

    DHT dht(DHTPIN, DHTTYPE); 
 
    void setup() {
      Serial.begin(115200);  
      pinMode(analogInput, INPUT); 
      dht.begin();                                      
      delay(10);
     
     
      Serial.println();
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println(ssid);
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
     
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
     
    void loop() {
      delay(100);                                                
     
      Serial.print("Connecting to ");
      Serial.println(host);
     
      
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }

      
      raw = analogRead(analogInput);
      voltage = (raw * 5.0 ) / 1024.0;
      float h = dht.readHumidity();                              
      float t = dht.readTemperature();
      Serial.println(h);
      Serial.println(t);
      Serial.println(voltage);
      
     
      
      client.print(String("GET /dweet/for/Satty42?Voltage=") + String(voltage) + "&humidity=" + String(h) + "&temperature=" + String(t) + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
      delay(10);
     
      // Получаем все строки ответа сервера и отправляем их в последовательный порт
      while (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
     
      Serial.println();
      Serial.println("closing connection");
    }
