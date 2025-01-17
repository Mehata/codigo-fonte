#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <RestClient.h>

/*
 * This example serves a "hello world" on a WLAN and a SoftAP at the same time.
 * The SoftAP allow you to configure WLAN parameters at run time. They are not setup in the sketch but saved on EEPROM.
 * 
 * Connect your computer or cell phone to wifi network ESP_ap with password 12345678. A popup may appear and it allow you to go to WLAN config. If it does not then navigate to http://192.168.4.1/wifi and config it there.
 * Then wait for the module to connect to your wifi and take note of the WLAN IP it got. Then you can disconnect from ESP_ap and return to your regular WLAN.
 * 
 * Now the ESP8266 is in your network. You can reach it through http://192.168.x.x/ (the IP you took note of) or maybe at http://esp8266.local too.
 * 
 * This is a captive portal because through the softAP it will redirect any http request to http://192.168.4.1/
 */

/* Set these to your desired softAP credentials. They are not configurable at runtime */
const char *softAP_ssid = "DefuseTheBomb";
const char *softAP_password = "12345678";

/* hostname for mDNS. Should work at least on windows. Try http://remotecontrol */
const char *myHostname = "remotecontrol";

/* Don't set this wifi client credentials. They are configurated at runtime and stored on EEPROM */
char ssid[32] = "";
char password[32] = "";

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

//Soft AP network parameters
IPAddress apIP(192, 168, 99, 1);
IPAddress netMsk(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);
IPAddress gateway(192,168,1,1);

/** Should I connect to WLAN asap? */
boolean connect;

/** Last time I tried to connect to WLAN */
long lastConnectTry = 0;
long bomb_planted_time = 0;

/** Current WLAN status */
int status = WL_IDLE_STATUS;

  //is the user logged
  int logged_in = false;
  int bomb_planted = false;
  int time_extended = 0 ;
  int fase1=false,fase2=false,fase3=false;
  int pontos_atual=0,pontuacao_maxima=1000;
  WiFiClient client;  // Make a HTTP request:

void setup() {
  delay(100); //1000
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  printFlag();
  Serial.println();
  Serial.println("Configuring Access Point: # DEFUSETHEBOMB #");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid, softAP_password);
  delay(200); // Without delay I've seen the IP address blank //500
  Serial.print("AP WIFI address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */  
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  /* Setup web pages: root, wifi config admin pages, SO captive portal detectors and not found. */
  server.on("/", handleRoot );
  server.on("/admin", handleAdmin );
  server.on("/getbombstatus", handleGetBombStatus );
  server.on("/get", handleGet );
  server.on("/bomb", handleBomb );
  server.on("/wifi", handleWifi );
  server.on("/challengerstatus", handleChallengerStatus );
  server.on("/admin/b0mbsetuping", handleBombSetuping );
  server.on("/admin/b0mbdefusing", handleBombDefusing );
  server.on("/admin/timerextend", handleTimerExtend );
  server.on("/admin/somatoria", handleSomatoria );
  server.on("/admin/fibonacci", handleFibonacci );
  server.on("/admin/arquivo.zip", handleArquivoZip );
  server.on("/admin/wifisave", handleWifiSave );
  server.on("/admin/bombwificonnect", handleBombWifiClientConnect);
  server.on("/tips",handleTips );
  server.on("/manual",handleManual );
  server.on("/generate_204", handleRoot );  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot );  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound ( handleNotFound );
  server.begin(); // Web server start
  Serial.println("HTTP server started on port: 80");
  loadCredentials(); // Load WLAN credentials from EEPROM to connect in network wifi APs
  printFlag2();
  connect = strlen(ssid) > 0; // Request WLAN connect if there is a SSID
}

void connectWifi() {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin ( ssid, password );
  int connRes = WiFi.waitForConnectResult();
  Serial.print ( "connResult: " );
  Serial.println ( connRes );
   
}

void loop() {
  if (connect) {
    Serial.println ( "Connect requested" );
    connect = false;
    connectWifi();
    lastConnectTry = millis();
  }
  {
    int s = WiFi.status();
    if (s == 0 && millis() > (lastConnectTry + 2000) ) {
      /* If WLAN disconnected and idle try to connect */
      /* Don't set retry time too low as retry interfere the softAP operation */
      connect = true;
    }
    if (status != s) { // WLAN status change
      Serial.print ( "Status: " );
      Serial.println ( s );
      status = s;
      if (s == WL_CONNECTED) {
        /* Just connected to WLAN */
        
       
        Serial.println ("IP=192.168.4.2 \n DNS=192.168.4.1");
        Serial.print ( "Connected to wirelles wifi " );
        Serial.println ( ssid );
        Serial.print ( "BOMBNETWORK IP address: " );
        
        Serial.println ( WiFi.localIP() );

        // Setup MDNS responder
        if (!MDNS.begin(myHostname)) {
          Serial.println("Error setting up MDNS responder!");
        } else {
          Serial.println("mDNS responder started");
          // Add service to MDNS-SD
          MDNS.addService("http", "tcp", 80);
        }
      } else if (s == WL_NO_SSID_AVAIL) {
        WiFi.disconnect();
      }
    }
  }
  // Do work:
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  server.handleClient();
}


