#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>


const char* ssid = "Manish";
const char* password = "Manish@123";

WiFiServer server(80);


int relayone =5;
String output5State = "off"; 
String webpage = "";
String header;



void setup(){

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println(".....");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(">>>>");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("HTTP server started");
}

void loop(){
  WiFiClient client = server.available(); 
  if(client){
  Serial.println("New Client");
  String currentLine = "";
  while (client.connected()){
    if(client.available()){
      char c = client.read();
      Serial.write(c);
      webpage +=c;
      if(c == '\n'){
        if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO 5 on");
              output5State = "on";
              digitalWrite(relayone, LOW);
              
              
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO 5 off");
              output5State = "off";
              digitalWrite(relayone, LOW);
            
              
            } 
          

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
              
            // Web Page Heading
            client.println("<body><h1>Home Automation Using IOT</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
          //  client.println("<p>GPIO 5 - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button  

              client.println("<p>GPIO 5 - State " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
               client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
               output5State="on";
               HTTPClient http; 
                   // client.println("<p><a href=\"/5/on\"><button class=\"button\">OFF</button></a></p>");
                    String url = "http://cubersindia.com/";
                    http.begin(url+ "manish/test.php?data=6");
                     // http.begin("http://cubersindia.com/manish/test.php?data=6");
                      const size_t bufferSize = JSON_OBJECT_SIZE(1) + 10;
                      DynamicJsonBuffer jsonBuffer(bufferSize);
                      const char* json = "{\"res\":\"6\"}";
                      JsonObject& root = jsonBuffer.parseObject(json);
                      const char* res = root["res"]; // "6"
                      Serial.println(res);
                      Serial.println(output5State);
                    
                      digitalWrite(relayone, LOW);
                     
                      
              
            } else if(output5State=="on"){
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
               output5State="off";
                     
                      digitalWrite(relayone, LOW);
                   
            } 
                        
                      client.println("<img src=\"https://cdn.dribbble.com/users/110995/screenshots/2092634/ikonica_600_wide_dribble_animation-duze-trajanje.gif\">");
            
               
            
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
   
    }
    
}
    
 
