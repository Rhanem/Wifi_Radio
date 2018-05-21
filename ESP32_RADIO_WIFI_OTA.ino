////////////////////////////////////////////////                 Librerias Incluidas                /////////////////////////////////////////
#include <Wire.h>
#include <WiFi.h>
#include "WiFi.h"
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "SPI.h"
#include "vs1053_ext.h"
#include "time.h"
#include <WiFiClient.h>
#include <time.h>

#include "credentials.h"

#include "logo.h"
#include "icono_sd_bluetooth_control_otros.h"

////////////////////////////////////////////////                    Modulos Incluidos                     //////////////////////////////////////////

//PIN OF ILI9341     TO ESP32     // TFT ILI9341 display 
#define _cs             22        // goes to TFT CS
#define _dc             21        // goes to TFT DC
#define _mosi           12        // goes to TFT MOSI
#define _sclk           14        // goes to TFT SCK/CLK
#define _rst            9         // ESP to TFT RESET
#define _miso                     // Not connected
//       3.3V                     // Goes to TFT LED  
//       5v                       // Goes to TFT Vcc
//       Gnd                      // Goes to TFT Gnd        

Adafruit_ILI9341 tft = Adafruit_ILI9341(_cs, _dc, _mosi, _sclk, _rst);


//PIN OF VS1053     TO     ESP32  
#define VS1053_CS           2            
#define VS1053_DCS          4
#define VS1053_DREQ         36

VS1053 mp3(VS1053_CS, VS1053_DCS, VS1053_DREQ);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////                 Declaraciones                /////////////////////////////////////////
int potPin = 35;
// int Bat = 33;
int lectura;
int v = 0;
int carga;
int porcentaje = 0;
float escala = 100;
int nivel;
int cuadro = 2;
const char* station;
const char* info;
int volume = 0;
int Canal = 0;
int estacion = 0;
int coneccion = 1;
int ap = 2;
int pass = 1;

String lista_conecciones[] = {
"EEEEE",
"EEEEE",
"EEEEE",
"EEEEE",
};

String lista_passes[] = {
"99999999",
"99999999",
"99999999",
"99999999",
};

String lista_aps[] = {
"AJUSTES",
"  SD",
" RADIO",
"BLUET.",
"CONTROL",
"OTROS..",
};

String lista_aplicaciones[] = {
"aplicacion_ajustes()",
"aplicacion_sd()",
"aplicacion_radio()",
"aplicacion_bluetooth()",
"aplicacion_control()",
"aplicacion_otros()",
};


int lista_cuadros[] = {
10,
60,
115,
168,
222,
273,
};

String lista_canales[] = {
"http://cristina.torontocast.com:8095/stream/;",
"http://listen.radionomy.com/eurodance-90.m3u",
"http://stream.laut.fm/eurodance.m3u",
"http://listen.radionomy.com/abcd-eurodance.m3u",
"http://listen.radionomy.com/80ssuperdance.m3u",
"www.surfmusic.de/m3u/100-5-das-hitradio,4529.m3u",  
"http://regulatedbeats.antload.net/Trance3/;",
"http://listen.radionomy.com/yourtranceradio",            
"stream.radioreklama.bg:80/radio1.ogg",
"http://ns331078.ip-176-31-120.eu:8000/mpd",
"http://184.173.185.71:9844/;",    // a hudaifi  11
"http://184.173.185.71:8008/;",  //a ajmi
"http://184.173.185.71:8004/;",   //s ghamdi     13
"http://184.173.185.71:9962/;",    //m jibril
"http://184.173.185.71:9858/;",    //a matroud     15
"http://184.173.185.71:9830/;",    //
"http://5.135.194.225:8000/;",
"http://www.quran-radio.org:8002/;",
"http://66.45.232.130:9994/;",
"http://198.178.123.11:8068/;stream/1",
"http://198.178.123.2:9042/;stream/1",
"http://66.45.232.130:9992/;",
"http://66.45.232.130:9996/;",
"http://184.173.185.71:8006/;",           // mp6quran  24
"http://184.173.185.71:9936/;",            // roqiah
"http://198.178.123.5:12330/;stream/1",
"http://184.173.185.71:9986/;",           // shuraim   27
"http://184.173.185.71:9780/;",           // h rifai   28
"http://stream1.coran.tk:8005/;",
"http://vps84666.ovh.net:8000/mpd",
"http://64.71.79.181:5124/stream",
"http://198.27.120.235:8750/live",
"http://5.39.71.159:8157/stream/;",
"http://213.239.218.99:7319/live",
"http://us2.internet-radio.com:8281/live",
"http://50.7.68.251:7064/;stream/1",
"http://109.169.46.197:8011/;stream/1",
"http://94.23.252.14:8067/;stream/1",
"http://www.partyviberadio.com:8056/;stream/1",
"http://radio.vocaltrance.fm/vocaltrance_320.m3u",
"http://radio.vocaltrance.fm/vocaltrance_256.m3u",
"http://radio.vocaltrance.fm/vocaltrance_128.m3u",
"http://radio.vocaltrance.fm/positive_320.m3u",
"http://radio.vocaltrance.fm/positive_256.m3u",
"http://radio.vocaltrance.fm/positive_128.m3u",
"http://radio.vocaltrance.fm/vocalchillout_320.m3u",
"http://radio.vocaltrance.fm/vocalchillout_256.m3u",
"http://radio.vocaltrance.fm/vocalchillout_128.m3u",
"http://radio.vocaltrance.fm/uplifting_320.m3u",
"http://radio.vocaltrance.fm/uplifting_256.m3u",
"http://radio.vocaltrance.fm/uplifting_128.m3u",
"http://radio.vocaltrance.fm/deep_320.m3u",
"http://radio.vocaltrance.fm/deep_256.m3u",
"http://radio.vocaltrance.fm/deep_128.m3u",
"http://icecast.pulsradio.com/pulstranceHD.mp3/;",
"http://192.240.102.133:11454/;",
"http://listen.radionomy.com/90s-dance-classics.m3u",
"www.accentfm.com/wp-content/plugins/station-pro/player/wmp.asx",
"94.23.222.12:8024/amysfm",
"184.75.223.178:8079#",
"51.255.104.137:10030/",
"78.129.234.163:35641/",
"64.34.175.149:8000/blurfm03",
"195.154.180.106:8058/autodj",
"148.251.57.185:9306/",
"stream.nsp.se:8000/RaTbg_MP3_Hi",
"icecast.commedia.org.uk:8000/soundart.mp3",
"stream.radioreklama.bg:80/nrj_low.ogg",
"stream.radioreklama.bg:80/radio1rock.ogg",
"csff.od.ua:8000/radio",
"bitsmitter.com:8006/omr.ogg",
"hellwegradiowest.radiovonhier.de/high/stream.mp3",
"149.210.188.75/proxy/friesepiraten?mp=/stream192",
"macslons-irish-pub-radio.com/media.asx",
"www.radiojackie.com:5873/",
"www.bigb21.nl/listen.asx",
"www.hitradiocentraal.nl/stream/HRC.asx",
"vtuner.stream.laut.fm/030-berlinfm",
"www.101fm.de/radio_links/listen.pls",
"rtlberlin.hoerradar.de/rtlberlin-live-mp3-128",
"www.surfmusic.de/m3u/100-5-das-hitradio,4529.m3u",
"stream.spreeradio.de/spree-live/mp3-128/vtuner/", 
"http://listen.radionomy.com/a-11-eurodance-90s.m3u",
"http://listen.radionomy.com/d80s-radio.m3u",
"http://streaming01.dwservers.net:8100/;", 
"http://149.56.175.167:5044/;stream/1",
"http://45.43.200.66:25258/;stream/1",
"stream.hamburg-zwei.de/hh2-live/mp3-128/vtuner/",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
};
  
const int boton1 = 15;                                                  // Boton Bajar Volumen
const int boton2 = 13;                                                  // Boton Subir Volumen
const int boton3 = 16;                                                  // Boton Canal Anterior
const int boton4 = 17;                                                  // Boton Canal Posterior
const int boton5 = 8; 
const int boton6 = 34; 

int boton1State = LOW;
int boton2State = LOW; 
int boton3State = LOW;
int boton4State = LOW; 
int boton5State = LOW;
int boton6State = LOW;

int estadoCanal;
int estadoAp;
int estadoCuadro;
int estadoConeccion;
int estadoPass;

int activar_bt = 32;

//////////////////   Servidor 1      /////////
WiFiServer server(80);  
char linebuf[80];        
int charcount=0;        
/////////////////////////////////////////////

///////////////////////////////////////////////////                      Defineciones                            //////////////////////////////////////

#define BLACK           0X0000
#define RED             0xF800
#define GREEN           0x07E0
#define CELESTE         0x79E0
#define BLUE2           0x0110
#define BLUE            0x102E
#define CYAN            0x07FF
#define CYAN2           0x3333
#define YELLOW          0xFFE0 
#define ORANGE          0xFD20
#define NARANJO         0x1358
#define GREENYELLOW     0xAFE5 
#define DARKGREEN       0x03E0
#define WHITE           0xFFFF
#define GRIS            0x5AEB 
#define VIOLETA         0xFCFD
#define DARKGREY        0x7BEF  
#define AZULOSCURO      0x0005


#define BARRA 4
#define VOLUMEN BARRA*porcentaje

///////////////////////////////////////////////////////////             Tiempo               ///////////////////////////////////////////////////////////////

#include <NTPClient.h>
#include <WiFiUdp.h>

#define NTP_OFFSET  +2  * 60 * 60 // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "1.europe.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

/////////////////////////////////////////////////////////                 Setup                ///////////////////////////////////////////////////////
void setup() {


//////////////////// Servidor 1  /////////////
    WiFi.begin(ssid, password);
    server.begin(); 

  /////////////////   OTA    ///////////////////
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  // Port defaults to 3232                            // ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]               // ArduinoOTA.setHostname("myesp32");
  // No authentication by default                     // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)         Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)   Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)     Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

//////////////////////////////////////////////

 pinMode(boton1,INPUT); 
  pinMode(boton2,INPUT); 
   pinMode(boton3,INPUT); 
  pinMode(boton4,INPUT);
  
// pinMode(Bat, INPUT);
  pinMode(potPin, INPUT);
  pinMode(boton6, INPUT_PULLUP);
 pinMode(activar_bt, OUTPUT); 
 
    Serial.begin(115200);
      SPI.begin();
  SPI.setFrequency( 40000000 );
  SPI.setHwCs( false );
 
  tft.begin( 40000000, SPI );
    tft.setRotation(3);
  tft.fillScreen(BLACK);

                           WiFi.disconnect();
                          WiFi.mode(WIFI_STA);
    WiFi.begin(lista_conecciones[coneccion].c_str(), lista_passes[pass].c_str());

/////////////////////////////////

   tft.drawRGBBitmap(100, 60, logo, 120, 105);
                   delay(1000);
                  tft.setTextColor(WHITE);  tft.setTextSize(2);
                  tft.setCursor(30, 10);
                    tft.print("Cargando ");
                       delay(350);
                  tft.setCursor(130, 10);
                      tft.setTextSize(2);
                      tft.setTextColor(VIOLETA);
                       tft.print("."); delay(200); 
 tft.setCursor(140, 10); tft.print("."); delay(200); 
 tft.setCursor(150, 10); tft.print("."); delay(200); 
 tft.setCursor(160, 10); tft.print(".");  delay(200); 
 tft.setCursor(170, 10); tft.print("."); delay(200); 
 tft.setCursor(180, 10); tft.print("."); delay(200); 
 tft.setCursor(190, 10); tft.print("."); delay(200); 
 tft.setCursor(200, 10); tft.print("."); delay(200); 
 tft.setCursor(210, 10); tft.print("."); delay(200); 
 tft.setCursor(220, 10); tft.print("."); delay(200); 
 tft.setCursor(230, 10); tft.print("."); delay(200);
 tft.setCursor(240, 10); tft.print("."); delay(200); 
 tft.setCursor(250, 10); tft.print("."); delay(200); 
 tft.setCursor(260, 10); tft.print("."); delay(200);

                 tft.setCursor(110, 200);
                     tft.setTextColor(GREEN);
                      tft.print("CARGADO!");
                          delay(1500);
                    tft.fillScreen(BLACK);
                           aplicacion_radio();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    tft.setTextColor(DARKGREY);  tft.setTextSize(2);
    tft.setCursor(1, 0);
    tft.print(lista_conecciones[coneccion]);
               
        //////////////////////////////////////////////////////////

    tft.setCursor(260, 0);
             tft.setTextColor(DARKGREY);
                 tft.print(nivel);
                     tft.setCursor(300, 0);
    tft.print("%");    
                    tft.fillRect(0, 90, 320, 28, BLACK);

delayMicroseconds(100);

  tft.fillRect(1, 25, 97, 27, CYAN2);                   //// cuadro de visualizar los Kbps   /////////
   tft.drawRoundRect(0, 24, 99, 29, 3, WHITE); 

  tft.fillRect(227, 25, 91, 27, CYAN2);                /////    cuadro de visualizar el volumen   ////////
     tft.drawRoundRect(226, 24, 93, 29, 3, WHITE); 

 tft.setTextColor(WHITE);
          tft.setCursor(14, 38);
  tft.setTextSize(1);

  tft.fillRect(104, 25, 117, 27, GRIS); 
   tft.drawRoundRect(103, 24, 119, 29, 3, WHITE);              ///////  cuadro de visualizar el elemento seleccionado ///////////
       tft.setTextColor(WHITE);
          tft.setCursor(116, 32);
  tft.setTextSize(2);

    tft.print(lista_aps[ap]);  
tft.drawRect(9, 202, 37, 37, BLACK);
tft.drawRect(10, 203, 35, 35, BLACK);
tft.drawRect(59, 202, 37, 37, BLACK);
tft.drawRect(60, 203, 35, 35, BLACK);
tft.drawRect(114, 202, 37, 37, BLACK);
tft.drawRect(115, 203, 35, 35, BLACK);
tft.drawRect(167, 202, 37, 37, BLACK);
tft.drawRect(168, 203, 35, 35, BLACK);
tft.drawRect(221, 202, 37, 37, BLACK);
tft.drawRect(222, 203, 35, 35, BLACK);
tft.drawRect(272, 202, 37, 37, BLACK);
tft.drawRect(273, 203, 35, 35, BLACK);
       tft.drawRect(lista_cuadros[cuadro]-1, 202, 37, 37, WHITE);
       tft.drawRect(lista_cuadros[cuadro], 203, 35, 35, WHITE);

           
}  //Fin de Setup

/////////////////////////////////////////////////////////                 Loop               ////////////////////////////////////////////////////////////

void loop()
{

//////////////// Servidor 1  /////////////////
  // Esperamos a que se conecte un cliente
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Nuevo Cliente");
                tft.setCursor(100, 184);
    tft.setTextColor(WHITE);
  tft.setTextSize(1);
    tft.print("Coneccion Remota Activada");
    memset(linebuf,0,sizeof(linebuf));
    charcount=0;
    // Una petición http termina con línea en blanco.
    boolean currentLineIsBlank = true;

    while (client.connected()) {  //Mientras que el cliente está conectado
      if (client.available()) {   //Si se recibe algun dato del cliente.
        char c = client.read();
        Serial.write(c);  
        //Lee caracter por caracter la petición http
        linebuf[charcount]=c;
        if (charcount<sizeof(linebuf)-1) charcount++;
        // si se recibe un caracter de nueva línea (/n) y la siguiente línea 
        // es una líne en blanco entonces la petición http ha terminado y podemos
        // responder al cliente

////////////////////////////////////////////            el codigo de control desde una pagina HTML del servidor                ////////////////////////////////////////////
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML><html><head>");
          client.println("<body style=\"background-color:#2369de;\">");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head>");
          client.println("<h2 align=\"center\" style=\"width:300px;height:35px;color:#ffffff;background-color: black;\"> MiniWeb Radio Rhanem</h2>");
          client.println("<p style=\"color:#ffffff\"><b>Canales: &nbsp;&nbsp;&nbsp;&nbsp;</b><a href=\"Canal-anterior\"><button><b> Anterior</b></button></a>&nbsp;&nbsp;&nbsp;<a href=\"Canal-siguiente\"><button><b>Siguiente</b></button></a></p>");
          client.print("<style=\"color:#ffffff\"><b>El canal actual es:   </b>"); client.print(Canal+1); //client.print(" &nbsp; "); client.print(station);
          client.println("<br><br>");
          client.println("<p style=\"color:#ffffff\"><b>Volumen: &nbsp;&nbsp;</b><a href=\"Bajar-volumen\"><button><b> Bajar</b></button></a>&nbsp;&nbsp;&nbsp;<a href=\"Subir-volumen\"><button><b>Subir</b></button></a></p>");
          client.print("<style=\"color:#ffffff\"><b>El volumen actual es:     </b>"); client.print(porcentaje+v);
          client.println("<br><br>");
          client.println("<p style=\"color:#ffffff\"><b>Wifi: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b><a href=\"Wifi-anterior\"><button><b> Anterior</b></button></a>&nbsp;&nbsp;&nbsp;<a href=\"Wifi-siguiente\"><button><b>Siguiente</b></button></a></p>");
          client.print("<style=\"color:#ffffff\"><b>La coneccion actual es:    </b>"); client.print(coneccion+1); client.print(" "); client.print(ssid);
          client.println("<br><br>");
          client.println("<p style=\"color:#ffffff\"><b>Ap: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b><a href=\"Ap-anterior\"><button><b>Izquierda</b></button></a>&nbsp;&nbsp;&nbsp;<a href=\"Ap-siguiente\"><button><b>Derecha</b></button></a></p>");
          client.print("<style=\"color:#ffffff\"><b>La aplicacion actual es:    </b>"); client.print(ap+1); client.print(" "); client.print(lista_aps[ap]);
          client.println("<br><br>");
          client.println("<br><br>");          
          client.println("<p align=\"center\"><a href=\"Reset\"><button><b>Reset</b></button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"Confirmar\"><button><b>&nbsp; OK &nbsp;&nbsp;</b></button></a></p>");

          client.println("</html>");
          break;
        }
        
        if (c == '\n') {
          // Empieza una nueva línea
          currentLineIsBlank = true;
          if (strstr(linebuf,"GET /Canal-anterior") > 0){
            Serial.println("Canal Anterior");
            if (Canal>0){
            station--;
            Canal--;}
tft.fillRect(0, 53, 320, 42, BLACK); 
tft.setCursor(0, 59);    
tft.setTextColor(DARKGREY);  tft.setTextSize(2); 
tft.print(Canal+1); tft.print(": ");
            mp3.connecttohost(lista_canales[Canal]);
          }
          else if (strstr(linebuf,"GET /Canal-siguiente") > 0){
            Serial.println("Siguiente Canal");  
            if (Canal<100){
            station++;
            Canal++;}
tft.fillRect(0, 53, 320, 42, BLACK); 
tft.setCursor(0, 59);    
tft.setTextColor(DARKGREY);  tft.setTextSize(2); 
tft.print(Canal+1); tft.print(": ");
            mp3.connecttohost(lista_canales[Canal]);
          }
          else if (strstr(linebuf,"GET /Bajar-volumen") > 0){
            Serial.println("Bajar Volumen");
            if (v>0){
            v--;}
            mp3.setVolume(porcentaje);
          }
          else if (strstr(linebuf,"GET /Subir-volumen") > 0){
            Serial.println("Subir Volumen");
            if (v<20){
            v++;}
            mp3.setVolume(porcentaje);
          }

                    else if (strstr(linebuf,"GET /Wifi-anterior") > 0){
            Serial.println("wifi anterior");
            if (coneccion>0){
              if (pass>0){
            coneccion--;
            pass--;}}
            WiFi.disconnect();
                          WiFi.mode(WIFI_STA);
              WiFi.begin(lista_conecciones[coneccion].c_str(), lista_passes[pass].c_str()); 
                tft.fillRect(0, 0, 110, 16, BLACK);
            tft.setTextColor(DARKGREY);  tft.setTextSize(2);
    tft.setCursor(1, 0);
    tft.print(lista_conecciones[coneccion]);
                    aplicacion_radio();   
          }
          else if (strstr(linebuf,"GET /Wifi-siguiente") > 0){
            Serial.println("siguiente wifi");
            if (coneccion<3){
              if (pass<3){
            coneccion++;
            pass++;}}
            WiFi.disconnect();
                          WiFi.mode(WIFI_STA);
              WiFi.begin(lista_conecciones[coneccion].c_str(), lista_passes[pass].c_str()); 
                tft.fillRect(0, 0, 110, 16, BLACK);
            tft.setTextColor(DARKGREY);  tft.setTextSize(2);
    tft.setCursor(1, 0);
    tft.print(lista_conecciones[coneccion]);
                          mp3.begin();
                          mp3.setVolume(porcentaje);
                   mp3.connecttohost(lista_canales[Canal]);      
          }
          else if (strstr(linebuf,"GET /Ap-anterior") > 0){
            Serial.println("Bajar Volumen");
            if (ap>0){
              if (cuadro>0){
            ap--;
            cuadro--;}}
            estadoAp=ap;
             estadoCuadro=cuadro; 
            tft.fillRect(109, 25, 98, 27, GRIS);            
    tft.setTextColor(WHITE);  tft.setTextSize(2);
    tft.setCursor(116, 32);
    tft.print(lista_aps[ap]);
tft.drawRect(9, 202, 37, 37, BLACK);
tft.drawRect(10, 203, 35, 35, BLACK);
tft.drawRect(59, 202, 37, 37, BLACK);
tft.drawRect(60, 203, 35, 35, BLACK);
tft.drawRect(114, 202, 37, 37, BLACK);
tft.drawRect(115, 203, 35, 35, BLACK);
tft.drawRect(167, 202, 37, 37, BLACK);
tft.drawRect(168, 203, 35, 35, BLACK);
tft.drawRect(221, 202, 37, 37, BLACK);
tft.drawRect(222, 203, 35, 35, BLACK);
tft.drawRect(272, 202, 37, 37, BLACK);
tft.drawRect(273, 203, 35, 35, BLACK);
       tft.drawRect(lista_cuadros[cuadro]-1, 202, 37, 37, WHITE);
       tft.drawRect(lista_cuadros[cuadro], 203, 35, 35, WHITE);
       lista_aplicaciones[ap];
          }
          else if (strstr(linebuf,"GET /Ap-siguiente") > 0){
            Serial.println("Subir Volumen");
            if (ap<5){
              if (cuadro<5){
            ap++;
            cuadro++;}}
            estadoAp=ap;
             estadoCuadro=cuadro; 
            tft.fillRect(109, 25, 98, 27, GRIS);            
             tft.setTextColor(WHITE);  tft.setTextSize(2);
               tft.setCursor(116, 32);
              tft.print(lista_aps[ap]);
tft.drawRect(9, 202, 37, 37, BLACK);
tft.drawRect(10, 203, 35, 35, BLACK);
tft.drawRect(59, 202, 37, 37, BLACK);
tft.drawRect(60, 203, 35, 35, BLACK);
tft.drawRect(114, 202, 37, 37, BLACK);
tft.drawRect(115, 203, 35, 35, BLACK);
tft.drawRect(167, 202, 37, 37, BLACK);
tft.drawRect(168, 203, 35, 35, BLACK);
tft.drawRect(221, 202, 37, 37, BLACK);
tft.drawRect(222, 203, 35, 35, BLACK);
tft.drawRect(272, 202, 37, 37, BLACK);
tft.drawRect(273, 203, 35, 35, BLACK);
       tft.drawRect(lista_cuadros[cuadro]-1, 202, 37, 37, WHITE);
       tft.drawRect(lista_cuadros[cuadro], 203, 35, 35, WHITE);
       lista_aplicaciones[ap];
          }
          else if (strstr(linebuf,"GET /Confirmar") > 0){
            Serial.println("Confirmar");
            digitalWrite(boton6, HIGH);
            delay(1);
            digitalWrite(boton6, LOW);
if (ap == 0){
  aplicacion_ajustes();
}
if (ap == 1){
  aplicacion_sd();
}
if (ap == 2){
 tft.fillRect(0, 56, 320, 38, BLACK);   
tft.setCursor(0, 61);    
    tft.setTextColor(DARKGREY);  tft.setTextSize(2);    
    tft.print(Canal+1); tft.print(": "); tft.setTextColor(DARKGREEN);  
    tft.setCursor(38, 61); tft.print(station);
   tft.fillRect(0, 97, 320, 97, BLUE);
            tft.setCursor(0, 101);
    tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println(info);
}
if (ap == 3){
  aplicacion_bluetooth();
}
if (ap == 4){
  aplicacion_control();
}
if (ap == 5){
  aplicacion_otros();
}
          }
          else if (strstr(linebuf,"GET /Reset") > 0){
            Serial.println("Reset");
            Canal=0;
            station=0;
            v=0;
            coneccion=0;
            pass=0;
            ap=2;
            cuadro=2;
                   aplicacion_radio();
              tft.fillRect(109, 25, 98, 27, GRIS);            
             tft.setTextColor(WHITE);  tft.setTextSize(2);
               tft.setCursor(116, 32);
              tft.print(lista_aps[ap]);
tft.drawRect(9, 202, 37, 37, BLACK);
tft.drawRect(10, 203, 35, 35, BLACK);
tft.drawRect(59, 202, 37, 37, BLACK);
tft.drawRect(60, 203, 35, 35, BLACK);
tft.drawRect(114, 202, 37, 37, BLACK);
tft.drawRect(115, 203, 35, 35, BLACK);
tft.drawRect(167, 202, 37, 37, BLACK);
tft.drawRect(168, 203, 35, 35, BLACK);
tft.drawRect(221, 202, 37, 37, BLACK);
tft.drawRect(222, 203, 35, 35, BLACK);
tft.drawRect(272, 202, 37, 37, BLACK);
tft.drawRect(273, 203, 35, 35, BLACK);
       tft.drawRect(lista_cuadros[cuadro]-1, 202, 37, 37, WHITE);
       tft.drawRect(lista_cuadros[cuadro], 203, 35, 35, WHITE);
                   tft.setCursor(0, 101);
                    tft.setTextColor(YELLOW);
                  tft.setTextSize(2);
                  tft.println(info);
          }
          // Empieza una nueva línea
          currentLineIsBlank = true;
          memset(linebuf,0,sizeof(linebuf));
          charcount=0;
        } else if (c != '\r') {
          // Todavía queda un caracter en la línea actual
          currentLineIsBlank = false;
        }
      }
    }
           
    delay(1);  // Esperamos un poco para que el navegador reciba los datos.

    // Cerramos la conexión con el cliente
    client.stop();
    Serial.println("Cliente desconectado");
  }
                      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
////////////////////////   OTA    ///////////////////
  ArduinoOTA.handle();
  yield();
/////////////////////////////////////////////////////
  
int lectura = analogRead(potPin);
 porcentaje = map(lectura, 0, 1023, 0, 5)+v;
// Serial.println(porcentaje);
    mp3.setVolume(porcentaje);

        tft.fillRect(VOLUMEN+226, 30-0, 10, 20+0, CYAN2); 
  tft.fillRect(VOLUMEN+226, 30-0, 3, 20+0, WHITE); 

                mp3.loop(); 
/*                 if (Serial.available()){
                 tft.fillRect(0, 53, 320, 43, BLACK); 
                  mp3.connecttohost(Serial.readString());
                }*/

///////////////////////////////           mostrar el tiempo               /////////////////////////////

                          displayData();

///////////////////////////////////////////////////////////////////////////////////////////////////////


  boton1State = digitalRead(boton1);
  boton2State = digitalRead(boton2);
  boton3State = digitalRead(boton3);
  boton4State = digitalRead(boton4);
  boton5State = digitalRead(boton5);
  boton6State = digitalRead(boton6);
  
                                                    ///////////////////////////////////////////////
                                                    //           Control de Canales              //
                                                    ///////////////////////////////////////////////
if (boton6State == LOW){ 
  if (boton3State == HIGH){
    
    if (ap == 2){
    if (Canal>0){
         Canal--;
 tft.fillRect(0, 53, 320, 42, BLACK);    
tft.setCursor(0, 59);    
    tft.setTextColor(DARKGREY);  tft.setTextSize(2);    
   
tft.print(Canal+1); tft.print(": ");   tft.setTextColor(DARKGREEN);  
// tft.fillRect(38, 59, 320, 32, BLACK);    
tft.setCursor(38, 59);    tft.print(station);

                        mp3.connecttohost(lista_canales[Canal]);
      }
  }
estadoCanal = Canal;


      if (ap == 0){
    if (coneccion<3){
         coneccion++;
   pass++;
              tft.fillRect(140,120,320,20,BLUE);
             tft.setCursor(145, 125);
        tft.setTextColor(GREEN);  tft.setTextSize(2);
    tft.print("- ");    tft.print(lista_conecciones[coneccion]);
      }
      }
        estadoConeccion=coneccion;
       estadoPass=pass;
  }
}
/////////////////////

if (boton6State == LOW){ 
if (boton4State == HIGH){
  
            if (ap == 2){   
    if (Canal<100){
         Canal++;    
 tft.fillRect(0, 53, 320, 42, BLACK);  
tft.setCursor(0, 59);    
    tft.setTextColor(DARKGREY);  tft.setTextSize(2); 
tft.print(Canal+1); tft.print(": ");   tft.setTextColor(DARKGREEN);  
// tft.fillRect(38, 59, 320, 32, BLACK);    
tft.setCursor(38, 59);  tft.print(station);

                         mp3.connecttohost(lista_canales[Canal]);
                      
      }
      }
estadoCanal=Canal;

          if (ap == 0){  
    if (coneccion>0){
         coneccion--;
      pass--;
                 tft.fillRect(140,120,320,20,BLUE);
             tft.setCursor(145, 125);
        tft.setTextColor(GREEN);  tft.setTextSize(2);
    tft.print("- ");    tft.print(lista_conecciones[coneccion]);
      }
      }
    
estadoConeccion=coneccion;
estadoPass=pass;
}
}

                                                    ///////////////////////////////////////////////////
                                                    //          Seleccion de aplicaciones            //
                                                    ///////////////////////////////////////////////////

//if (boton6State == HIGH){ 
     if (boton1State == HIGH) 
  {
    if (ap>0)
      {
         ap--;
         cuadro--;


                        Serial.print("El menu selecionado es: ");
                         Serial.print(lista_aps[ap]);
    
                                 tft.fillRect(109, 25, 98, 27, GRIS);            

    tft.setTextColor(WHITE);  tft.setTextSize(2);
    tft.setCursor(116, 32);
    tft.print(lista_aps[ap]);
    
tft.drawRect(9, 202, 37, 37, BLACK);
tft.drawRect(10, 203, 35, 35, BLACK);
tft.drawRect(59, 202, 37, 37, BLACK);
tft.drawRect(60, 203, 35, 35, BLACK);
tft.drawRect(114, 202, 37, 37, BLACK);
tft.drawRect(115, 203, 35, 35, BLACK);
tft.drawRect(167, 202, 37, 37, BLACK);
tft.drawRect(168, 203, 35, 35, BLACK);
tft.drawRect(221, 202, 37, 37, BLACK);
tft.drawRect(222, 203, 35, 35, BLACK);
tft.drawRect(272, 202, 37, 37, BLACK);
tft.drawRect(273, 203, 35, 35, BLACK);
       tft.drawRect(lista_cuadros[cuadro]-1, 202, 37, 37, WHITE);
       tft.drawRect(lista_cuadros[cuadro], 203, 35, 35, WHITE);

                                  delay (150); 
  }
}
//  }
estadoAp=ap;
estadoCuadro=cuadro; 


                      ////////////////////////////////////   


//if (boton6State == HIGH){                
    if (boton2State == HIGH) 
  {
    if (ap<5)
      {
         ap++;  
         cuadro++;


                         Serial.print("El menu selecionado es: ");
                        Serial.print(lista_aps[ap]);
                          

                                 tft.fillRect(109, 25, 98, 27, GRIS);  

    tft.setTextColor(WHITE);  tft.setTextSize(2);
    tft.setCursor(116, 32);
    tft.print(lista_aps[ap]);
    
tft.drawRect(9, 202, 37, 37, BLACK);
tft.drawRect(10, 203, 35, 35, BLACK);
tft.drawRect(59, 202, 37, 37, BLACK);
tft.drawRect(60, 203, 35, 35, BLACK);
tft.drawRect(114, 202, 37, 37, BLACK);
tft.drawRect(115, 203, 35, 35, BLACK);
tft.drawRect(167, 202, 37, 37, BLACK);
tft.drawRect(168, 203, 35, 35, BLACK);
tft.drawRect(221, 202, 37, 37, BLACK);
tft.drawRect(222, 203, 35, 35, BLACK);
tft.drawRect(272, 202, 37, 37, BLACK);
tft.drawRect(273, 203, 35, 35, BLACK);
       tft.drawRect(lista_cuadros[cuadro]-1, 202, 37, 37, WHITE);
       tft.drawRect(lista_cuadros[cuadro], 203, 35, 35, WHITE);

                                  delay (150); 
  }
  }
//  }
estadoAp=ap;
estadoCuadro=cuadro;

                             //////////////////////////////////////////

if (ap == 0){
  if (boton3State == LOW){
    if (boton4State == LOW){
      if (boton6State == HIGH){
      aplicacion_ajustes();
  }
  }
}
}
                         

if (ap == 1){
  if (boton3State == LOW){
    if (boton4State == LOW){
      if (boton6State == HIGH){
      aplicacion_sd();
  }
    }
  }
}


if (ap == 2){
  if (boton3State == LOW){
    if (boton4State == LOW){
      if (boton6State == HIGH){
       aplicacion_radio();
  }
  }
}
}    



if (ap == 3){
    if (boton3State == LOW){
    if (boton4State == LOW){
      if (boton6State == HIGH){
aplicacion_bluetooth();
 
}
    }
    }
}


if (ap == 4){
    if (boton3State == LOW){
    if (boton4State == LOW){
      if (boton6State == HIGH){
      aplicacion_control();
  }
    }
    }
}

if (ap == 5){
    if (boton3State == LOW){
    if (boton4State == LOW){
      if (boton6State == HIGH){
      aplicacion_otros();
  }
    }
    }
}


    
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void vs1053_showstreamtitle(const char *info){ 
tft.drawLine(0, 96,320,96,WHITE);
    tft.fillRect(0, 97, 320, 97, BLUE);
    tft.drawRGBBitmap(16, 204, ajustes, 32, 32);
    tft.drawRGBBitmap(68, 204, sd_ico, 32, 32);  
    tft.drawRGBBitmap(117, 204, radio, 32, 32);
    tft.drawRGBBitmap(168, 204, bluetooth, 32, 32);
    tft.drawRGBBitmap(225, 204, control, 32, 32);
    tft.drawRGBBitmap(274, 204, misc, 32, 32);
    tft.drawLine(0, 194,320,194,WHITE);
    
            tft.setCursor(0, 101);

    tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println(info);

 }


void vs1053_showstation(const char *station){ 
             tft.setCursor(38, 59); 
  tft.setTextColor(ORANGE);
  tft.setTextSize(2);
  tft.print(station);
 }

void vs1053_commercial(const char *info){           // called from vs1053
tft.fillRect(0, 184, 320, 10, BLUE);
            tft.setCursor(0, 184);
    tft.setTextColor(WHITE);
  tft.setTextSize(1);
 //   tft.println(String(info)+"sec");             // info is the duration of advertising
}

void vs1053_bitrate(const char *br){        // called from vs1053
tft.fillRect(1, 25, 97, 27, CYAN2);
            tft.setCursor(7, 32);
    tft.setTextColor(WHITE);
  tft.setTextSize(2);
      tft.println(String(br)+"kb/s");          // bitrate of current stream
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void aplicacion_ajustes(){

        tft.fillRect(0, 56, 320, 38, GRIS);  
tft.setCursor(0, 61);    
    tft.setTextColor(WHITE);  tft.setTextSize(2);    
   
    tft.print("Selecciona tu red wifi..");  
   tft.fillRect(0, 97, 320, 97, BLUE);
    tft.drawLine(0, 194,320,194,WHITE);

                     tft.setTextColor(DARKGREY);
                     tft.setTextSize(2);
                     tft.setCursor(1, 101);
                     tft.print("- ");    tft.println(lista_conecciones[0]);
                     tft.print("- ");    tft.println(lista_conecciones[1]);
                     tft.print("- ");    tft.println(lista_conecciones[2]);
                     tft.print("- ");    tft.println(lista_conecciones[3]);

                                tft.fillRect(140,120,320,20,BLUE);
             tft.setCursor(145, 125);
        tft.setTextColor(GREEN);  tft.setTextSize(2);
    tft.print("- ");    tft.print(lista_conecciones[coneccion]);
    

                                                    ////////////////////////////////////////////////////
                                                    //           selector de Conecciones              //
                                                    ///////////////////////////////////////////////////


                               if (ap == 0){
                          if (boton1State == HIGH){
                            if (boton6State == HIGH){
  WiFi.begin(lista_conecciones[coneccion].c_str(), lista_passes[pass].c_str()); 
                tft.fillRect(0, 0, 110, 16, BLACK);
            tft.setTextColor(DARKGREY);  tft.setTextSize(2);
    tft.setCursor(1, 0);
    tft.print(lista_conecciones[coneccion]);
                            }
                          }
                               }
                               
}

/////////////////////////////////////////////

void aplicacion_sd() {
tft.setCursor(0, 61); 
        tft.fillRect(0, 56, 320, 38, GRIS); 
tft.setTextSize(2); 
tft.setTextColor(WHITE);
tft.print("titulo del archivo ejecutado desde la SD");
tft.drawLine(0, 96,320,96,WHITE);

   tft.fillRect(0, 97, 320, 97, BLUE);
    tft.drawLine(0, 194,320,194,WHITE);

tft.setCursor(40, 140); 
tft.setTextColor(NARANJO);
tft.print("PROXIMAMENTE");
 tft.drawRGBBitmap(200,115,icono_sd,64,64);
}

/////////////////////////////////////////////
void aplicacion_radio() {
    digitalWrite(32,LOW);
                                 mp3.begin();
                          mp3.setVolume(porcentaje);
                   mp3.connecttohost(lista_canales[Canal]);
                                  delay (5);   

 tft.fillRect(0, 56, 320, 38, BLACK);   
tft.setCursor(0, 61);    
    tft.setTextColor(DARKGREY);  tft.setTextSize(2);    
   
    tft.print(Canal+1); tft.print(": "); tft.setTextColor(DARKGREEN);  
    tft.setCursor(38, 61); tft.print(station);

tft.drawLine(0, 96,320,96,WHITE);
   tft.fillRect(0, 97, 320, 97, BLUE);
    tft.drawLine(0, 194,320,194,WHITE);
            tft.setCursor(0, 101);
    tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println(info);
  
}

/////////////////////////////////////////////

void aplicacion_bluetooth() {
tft.setCursor(0, 61); 
        tft.fillRect(0, 56, 320, 38, GRIS); 
tft.setTextSize(2); 
tft.setTextColor(WHITE);
tft.print("Envio y recepcion por bluetooth");
tft.drawLine(0, 96,320,96,WHITE);
   tft.fillRect(0, 97, 320, 97, BLUE);
    tft.drawLine(0, 194,320,194,WHITE);
tft.setCursor(40, 140); 
tft.setTextColor(NARANJO);
tft.print("PROXIMAMENTE");
 tft.drawRGBBitmap(200,115,icono_BlueTooth,64,64);
  delay(1);
  digitalWrite(32,HIGH);
}


/////////////////////////////////////////////

void aplicacion_control() {
  tft.setCursor(0, 61); 
        tft.fillRect(0, 56, 320, 38, GRIS); 
tft.setTextSize(2); 
tft.setTextColor(WHITE);
tft.print("Control remoto y sensores");
tft.drawLine(0, 96,320,96,WHITE);
   tft.fillRect(0, 97, 320, 97, BLUE);
    tft.drawLine(0, 194,320,194,WHITE);
tft.setCursor(40, 140); 
tft.setTextColor(NARANJO);
tft.print("PROXIMAMENTE");
 tft.drawRGBBitmap(200,115,icono_control,64,64);

}

/////////////////////////////////////////////

void aplicacion_otros() {
    tft.setCursor(0, 61); 
        tft.fillRect(0, 56, 320, 38, GRIS); 
tft.setTextSize(2); 
tft.setTextColor(WHITE);
tft.print("Otras Sorpresas..");
tft.drawLine(0, 96,320,96,WHITE);
   tft.fillRect(0, 97, 320, 97, BLUE);
    tft.drawLine(0, 194,320,194,WHITE);
tft.setCursor(40, 140); 
tft.setTextColor(NARANJO);
tft.print("PROXIMAMENTE");
 tft.drawRGBBitmap(200,115,icono_otros,64,64);
}

///////////////////////////////////////////////

void displayData() {
 // if (boton6State == HIGH){
     tft.fillRect(112, 0, 104, 16, BLACK);
String formattedTime = timeClient.getFormattedTime();
                    timeClient.begin();
                   timeClient.update();

                       tft.setTextColor(WHITE);
                      tft.setTextSize(2);
                   tft.setCursor(120, 0);
             tft.print(formattedTime);     
}

              ////////////////////////////////////////////////         FIN         ////////////////////////////////////////////////////
