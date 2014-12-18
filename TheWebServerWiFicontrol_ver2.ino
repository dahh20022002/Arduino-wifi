/*
  WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 9.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 9

 created 25 Nov 2012
 by Tom Igoe
 */
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>


char ssid[] = "home10-2";      //  your network SSID (name)
char pass[] = "0932456342";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

// 右輪馬達-A
const int IN1 = 3;		// 馬達A 的正反轉接腳編號
const int IN2 = 4;
const int ENA = 5;		// 馬達A 的 PWM 轉速控制
 
// 左輪馬達-B
const int IN3 = 12;		// 馬達B 的正反轉接腳編號
const int IN4 = 8;
const int ENB = 6;		// 馬達B 的 PWM 轉速控制

int speed;			// 定義速度變數，PWM 輸出範圍為 0～255

int status = WL_IDLE_STATUS;
WiFiServer server(80);

void setup() {
  Serial.begin(9600);      // initialize serial communication
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  speed = 80;
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }

  String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  analogWrite(ENA, speed);		//輸出 PWM 脈衝到 ENA
  analogWrite(ENB, speed);		//輸出 PWM 脈衝到 ENB

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            client.println("<!doctype html>");
            client.println("<html>");
            
            client.println("<head>");
            client.println("<meta charset=utf-8>");
            //client.println("<meta name=homerobot content=width=device-width initial-scale=1.0 maximum-scale=1.0 user-scalable=0");
            client.print("<meta name=homerobot ");
            client.print("content=width=device-width; "); //內容寬度設定為裝置寬度
            client.print("initial-scale=1.0; "); //初始縮放比率1:1
            client.print("maximum-scale=1.0; "); //最大縮放比率1:1
            client.println("user-scalable=0;>"); //禁止用戶縮放畫面
            
            client.println("<style type=text/css>");
            client.println("body{");
            client.println("background-color:#CCC;");
            client.println("font-family:'微軟正黑體', '黑體-繁', sans-serif;");
            client.println("}");
            
            client.println(".sw{");
            client.println("margin:0 0.5em;");
            client.println("font-weight:bolder;");
            client.println("}");
            
            client.println(".on{");
            client.println("color:#F00;"); //on按鈕顏色設定紅色
            client.println("}");
            
            client.println(".off{");
            client.println("color:#090;"); //off按鈕顏色設定為綠色
            client.println("}");
            
            client.println("#robot{");
            client.println("border-radius:10px;moz-border-radius:10px;webkit-border-radius:10px;");
            client.println("background-color:#FFF;");
            client.println("padding:1px;");
            client.println("}");
            
            client.println("#robot ul{");
            client.println("list-style-type:none;");
            client.println("padding-left:0px;");
            client.println("font-size:24pt;");
            client.println("}");
            
            client.println("#robot ul li{");
            client.println("margin:10px 0;");
            client.println("border-bottom:1px dotted #CCC;");
            client.println("padding-bottom:5px;");
            client.println("}");
            
            client.println("#robot ul li a{");
            client.println("display:block;");
            client.println("text-decoration:none;");
            client.println("color:#000;");
            client.println("}");
            
            client.println("</style>");
            client.println("</head>");
            
            client.println("<body>");
            //css
            
            //
            client.println("<h1>居家智能機器人</h1>");
            client.println("<div id=robot>");
            //機器人控制
            client.println("<ul>");
            client.println("<li><a href=/up><span class='sw on'>ON</span>向前</a></li>");
            client.println("<li><a href=/down><span class='sw on'>ON</span>向後</a></li>");
            client.println("<li><a href=/left><span class='sw on'>ON</span>向左</a></li>");
            client.println("<li><a href=/right><span class='sw on'>ON</span>向右</a></li>");
            client.println("<li><a href=/stop><span class='sw off'>OFF</span>停止</a></li>");
            client.println("</ul>");
            //
            client.println("</div>");
            
            client.print("<iframe width=\"640\" height=\"480\" frameborder=\"0\" scrolling=\"YES\"");
            client.print("src=\"http://therobot.dlinkddns.com/lphone.htm\"");
            client.println("</iframe>");
            
            client.println();
            client.println("</body>");
            client.println("</html>");
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /up")) {
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
        }
        if (currentLine.endsWith("GET /down")) {
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, HIGH);
        }
        if (currentLine.endsWith("GET /left")) {
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
        }
        if (currentLine.endsWith("GET /right")) {
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
        }
        if (currentLine.endsWith("GET /stop")) {
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, LOW);
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
