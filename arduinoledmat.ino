//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <SPI.h>

#define PxMATRIX_COLOR_DEPTH 1
#define PxMATRIX_MAX_HEIGHT 32
#define PxMATRIX_MAX_WIDTH 64

#include <PxMatrix.h>
#include <avr/pgmspace.h>
// Pins for LED MATRIX
//#define P_LAT 16
//#define P_A 5
//#define P_B 4
//#define P_C 15
//#define P_D 12
//#define P_E 0
//#define P_OE 2
#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
PxMATRIX display(A, B, C,D, CLK, LAT, OE, true,64);
//PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B,P_C);
//PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
//PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);
// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(25, 205, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);
unsigned long start_time=0;
uint8_t icon_index=0;
#define SSID "wifi name"
#define PASS "password"  
String form =
  "<p>"
  "<center>"
  "<h1>WiFi Control MAX7219 DISPLAY Panel with ESP8266</h1>"
  "<form action='msg'><p>Message <input type='text' name='msg' size=80 autofocus> <input type='submit' value='Submit'></form>"
  "</center>";
//ESP8266WebServer server(80);                          
long period;
int offset=1,refresh=0;
int pinCS = 0;
int numberOfHorizontalDisplays = 8;
int numberOfVerticalDisplays = 1;
String decodedMsg;
String tape = "Arduino";
int wait = 40;
int spacer = 2;
int width = 5 + spacer; // The font width is 5 pixels
void handle_msg() {                      
 display.fillScreen(LOW);
//  server.send(200, "text/html", form);  
  refresh=1;
//  String msg = server.arg("msg");
//  Serial.println(msg);
//  decodedMsg = msg;
  decodedMsg.replace("+", " ");      
  decodedMsg.replace("%21", "!");  
  decodedMsg.replace("%22", "");  
  decodedMsg.replace("%23", "#");
  decodedMsg.replace("%24", "$");
  decodedMsg.replace("%25", "%");  
  decodedMsg.replace("%26", "&");
  decodedMsg.replace("%27", "'");  
  decodedMsg.replace("%28", "(");
  decodedMsg.replace("%29", ")");
  decodedMsg.replace("%2A", "*");
  decodedMsg.replace("%2B", "+");  
  decodedMsg.replace("%2C", ",");  
  decodedMsg.replace("%2F", "/");  
  decodedMsg.replace("%3A", ":");    
  decodedMsg.replace("%3B", ";");  
  decodedMsg.replace("%3C", "<");  
  decodedMsg.replace("%3D", "=");  
  decodedMsg.replace("%3E", ">");
  decodedMsg.replace("%3F", "?");  
  decodedMsg.replace("%40", "@");
}
void setup() {
 Serial.begin(115200);
  // Define your display layout here, e.g. 1/8 step, and optional SPI pins begin(row_pattern, CLK, MOSI, MISO, SS)
  display.begin(16);
Serial.begin(115200);                           // full speed to monitor                            
//  WiFi.begin(SSID, PASS);                         // Connect to WiFi network
//  while (WiFi.status() != WL_CONNECTED) {         // Wait for connection
    delay(500);
    Serial.print(".");
  }
  // Set up the endpoints for HTTP server,  Endpoints can be written as inline functions:
//  server.on("/", []() {
//    server.send(200, "text/html", form);
//  });
//  server.on("/msg", handle_msg);                  // And as regular external functions:
//  server.begin();                                 // Start the server
//  Serial.print("SSID : ");                        // prints SSID in monitor
//  Serial.println(SSID);                           // to monitor            
  char result[16];
//  sprintf(result, "%3d.%3d.%1d.%3d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
//  Serial.println();
//  Serial.println(result);
//  decodedMsg = result;
//  Serial.println("WebServer ready!   ");
///  Serial.println(WiFi.localIP());                 // Serial monitor prints localIP
//  Serial.print(analogRead(A0));
//}
union single_double{
  uint8_t two[2];
  uint16_t one;
} this_single_double;

// This draws the weather icons
void draw_weather_icon (uint8_t icon)
{
  if (icon>10)
  icon=10;
  for (int yy=0; yy<10;yy++)
  {
    for (int xx=0; xx<10;xx++)
    {
      uint16_t byte_pos=(xx+icon*10)*2+yy*220;

      display.drawPixel(1+xx,yy,this_single_double.one);
    }
  }
}
void scroll_text()
{
    display.setTextWrap(false);  // we don't wrap text so it scrolls nicely
    display.setTextSize(1);
    display.setRotation(0);

    for (int xpos=64; xpos>-15000; xpos--)
    {
      display.setTextColor(myWHITE);
      display.clearDisplay();
         display.setTextSize(1);
      display.setCursor(0,9);
        display.setTextColor(myCYAN);
    
  display.println(decodedMsg);
              display.setCursor(0,0);
                              display.setTextSize(1);
         display.println("*yawot*");
           display.setCursor(0,1);
                              display.setTextSize(1);
         display.println("*yawot*");
                   display.setTextSize(2);
      display.setCursor(xpos,16);
      display.println("your message scroll text here please visit www.yawot.com.......................");
      start_time = millis();
      while((millis()-start_time)<15)
           display.display(5);
    } 

       
}

void loop() {

  scroll_text();
scroll_text();
  
}
