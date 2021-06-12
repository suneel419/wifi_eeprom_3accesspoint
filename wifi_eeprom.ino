#include <EEPROM.h>
//#include <Praudyogi32.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WiFiClientSecure.h>
//#include <ESPmDNS.h>
#include <Update.h>
int c;
String esid;
String epass="";
String esid1;
String epass1="";
String esid2;
String epass2="";

//Praudyogi Dev;
WebServer server(80);
const char* serverIndex = "<form method='POST' action='/status' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
int i = 0, statusCode, value;
uint8_t variable;
String content, st;

// mcu accesspoint credentials
char* AP_NAME = "device";// accesspoint ssid
char* AP_PASS = "device";// accesspoint pass



void handleNotFound() { // page not found err for webserver
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void wifi_check()
{
  int n = WiFi.scanNetworks();
  //Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
     for (int i = 0; i < n; ++i)
     {
       //Serial.println("  ");
       //Serial.print(i + 1);
      //Serial.print(": ");
      //Serial.print(WiFi.SSID(i));
      if((WiFi.SSID(i)==esid.c_str()))
      {
      WiFi.disconnect();
       WiFi.begin(esid.c_str(), epass.c_str());
       while (WiFi.status() != WL_CONNECTED) {
       }
       //k=i;
       Serial.println();
       Serial.print("Connected to: ");Serial.println(WiFi.SSID(i));
       Serial.println("");
       Serial.println("WiFi connected!");
       Serial.print("IP address: ");
       Serial.println(WiFi.localIP());
       Serial.print("ESP Mac Address: ");
       Serial.println(WiFi.macAddress());
       Serial.print("Subnet Mask: ");
       Serial.println(WiFi.subnetMask());
       Serial.print("Gateway IP: ");
       Serial.println(WiFi.gatewayIP());
       esid="";epass="";esid1="";epass1="";esid2="";epass2="";   

            } 

      else if((WiFi.SSID(i)==esid1.c_str()))
      {
      WiFi.disconnect();
      WiFi.begin(esid1.c_str(), epass1.c_str()); 
      while (WiFi.status() != WL_CONNECTED) {
       }

      Serial.println();
      Serial.print("Connected to: ");Serial.println(WiFi.SSID(i));
      Serial.println("WiFi connected!");
       Serial.print("IP address: ");
       Serial.println(WiFi.localIP());
       Serial.print("ESP Mac Address: ");
       Serial.println(WiFi.macAddress());
       Serial.print("Subnet Mask: ");
       Serial.println(WiFi.subnetMask());
       Serial.print("Gateway IP: ");
       Serial.println(WiFi.gatewayIP()); 
       esid="";epass="";esid1="";epass1="";esid2="";epass2="";   
      } 

      else if((WiFi.SSID(i)==esid2.c_str()))
      {
      WiFi.disconnect();
      WiFi.begin(esid2.c_str(), epass2.c_str());
       while (WiFi.status() != WL_CONNECTED) {
       }
      Serial.println();
      Serial.print("Connected to: ");Serial.println(WiFi.SSID(i));
      Serial.println("WiFi connected!");
       Serial.print("IP address: ");
       Serial.println(WiFi.localIP());
       Serial.print("ESP Mac Address: ");
       Serial.println(WiFi.macAddress());
       Serial.print("Subnet Mask: ");
       Serial.println(WiFi.subnetMask());
       Serial.print("Gateway IP: ");
       Serial.println(WiFi.gatewayIP());
      esid="";epass="";esid1="";epass1="";esid2="";epass2="";
      } 
    } 
}
}


// wifi connection test
bool testWifi(void)
{
  int c = 0;
  while ( c < 20 ) 
  {
    Serial.print(c);
    wifi_check();
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(50);
    c++;
  }
  return false;
}

// save wifi cred in eeprom
void wifi()
{
  content = "<!DOCTYPE HTML>\r\n<html><head><meta name='viewport' content='width=device-width, initial-scale=1'></head><body style='color:blue;text-align:center;'><h1>SET YOUR WIFI HERE</h1><br><br>";
  content += "<form method='post' action='setting'><label>SSID: </label><input name='ssid' length=32 ><br><br><label>PASS: </label><input name='pass' length=64><br><br><label>SSID1: </label><input name='ssid1' length=32 ><br><br><label>PASS1: </label><input name='pass1' length=64><br><br><label>SSID2: </label><input name='ssid2' length=32 ><br><br><label>PASS2: </label><input name='pass2' length=64><br><br><input type='submit'></form>";
  content += "</body></html>";
  server.send(200, "text/html", content);


  server.on("/setting", []() {
    String qsid = server.arg("ssid");
    String qpass = server.arg("pass");
    String qsid1 = server.arg("ssid1");
    String qpass1 = server.arg("pass1");
    String qsid2 = server.arg("ssid2");
    String qpass2 = server.arg("pass2");
    if (qsid.length() > 0 && qpass.length() > 0) {
      for (int i = 0; i < 96; ++i) {
        EEPROM.write(i, 0);
      } for (int i = 0; i < qsid.length(); ++i)
      {
        EEPROM.write(i, qsid[i]);
      }
      for (int i = 0; i < qpass.length(); ++i)
      {
        EEPROM.write(32 + i, qpass[i]);
      }

      for (int i = 96; i < 192; ++i) {
        EEPROM.write(i, 0);
      } for (int i = 0; i < qsid1.length(); ++i)
      {
        EEPROM.write(96+i, qsid1[i]);
      }
      for (int i = 0; i < qpass1.length(); ++i)
      {
        EEPROM.write(128 + i, qpass1[i]);
      }

      for (int i = 192; i < 288; ++i) {
        EEPROM.write(i, 0);
      } for (int i = 0; i < qsid2.length(); ++i)
      {
        EEPROM.write(192+i, qsid2[i]);
      }
      for (int i = 0; i < qpass2.length(); ++i)
      {
        EEPROM.write(224 + i, qpass2[i]);
      }

      EEPROM.commit();
      content += "{\"Success\":\" Please wait, This device will restart\"}";
      statusCode = 200;
      ESP.restart();
    } else {
      content = "{\"Error\":\"404 not found\"}";
      statusCode = 404;
    }
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(statusCode, "application/json", content);
  });  
}

// pin initial values

void SERV()
{
  long rssi = WiFi.RSSI();
  //Serial.println(rssi);;
  
}

void wifi_start() // read from eeprom
{
  EEPROM.begin(512);
  delay(10);
  //String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  
  Serial.print("SSID: ");
  Serial.println(esid);
  //esid="";

  //String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);
  //epass="";

  
  //String esid1;
  for (int i = 96; i < 128; ++i)
  {
    esid1 += char(EEPROM.read(i));
  }
  
  Serial.print("SSID1: ");
  Serial.println(esid1);
  //esid1="";

  //String epass1 = "";
  for (int i = 128; i < 192; ++i)
  {
    epass1 += char(EEPROM.read(i));
  }
  Serial.print("PASS1: ");
  Serial.println(epass1);
  //epass1="";
  //String esid2 = "";
  for (int i = 192; i < 224; ++i)
  {
    esid2 += char(EEPROM.read(i));
  }
  
  Serial.print("SSID2: ");
  Serial.println(esid2);
  //esid2="";

  //String epass2 = "";
  for (int i = 224; i < 288; ++i)
  {
    epass2 += char(EEPROM.read(i));
  }
  Serial.print("PASS2: ");
  Serial.println(epass2);
  
}

bool wif(void)
{
  int c = 0;
  while ( c < 50 ) {
    Serial.print(c);
    wifi_check();
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(50);
    c++;
  }
  return false;
}
void restart(void) // wifi restart
{
  Serial.println("wifi service restart");
  wifi_start();
  delay(500);
  if (wif())
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.status());
  } else
  {
    return;
    Serial.println("software service reset");
  }
}

void setup() {
  Serial.begin(115200);
  wifi_start();// start connection to wifi
  if (testWifi())// test if wifi is connected
  {
    WiFi.softAPdisconnect(true);
//    Dev.Connect(Token, MyCode);
    SERV();
        return;
  }
  else // if wifi not found, open accesspoint and host the webpages @ 192.16.4.1 address // hosted pages:1- wifi cred page, 2: software update ota, 3: not found
  {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    Serial.println("HotSpot On");
    WiFi.softAP(AP_NAME, AP_PASS);
    Serial.println(AP_NAME);
    Serial.println(AP_PASS);
    Serial.println(WiFi.softAPIP());
    server.on("/wifi", wifi);// wifi cred page
    server.on("/update", HTTP_GET, []() { //ota update page
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/status", HTTP_POST, []() {
      server.sendHeader("Connection", "close");
      server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    }, []() {
      HTTPUpload& upload = server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin()) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      } else {
        Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
      }
    });
    server.onNotFound(handleNotFound);
    const char * headerkeys[] = {"User-Agent", "Cookie"} ;
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
    server.collectHeaders(headerkeys, headerkeyssize);
    server.begin();
  }
  while ((WiFi.status() != WL_CONNECTED))
  {
    server.handleClient();
  }
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    SERV();
    //    Serial.println(ESP.getFreeHeap());
  }
  else
  {
    Serial.println("Disconnected from wifi, re-checking connection status");
    for (int i = 0; i < 10; ++i) {
      Serial.print(i);
      if (WiFi.status() != WL_CONNECTED) {}
      delay(1000);
    } restart();
  }
}
