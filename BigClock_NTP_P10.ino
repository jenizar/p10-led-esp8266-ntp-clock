//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Test P10
/*
 * Original source code : https://github.com/busel7/DMDESP/blob/master/examples/TeksDiamdanJalan/TeksDiamdanJalan.ino by  busel7
 * Links to download libraries : https://github.com/busel7/DMDESP
*/

//----------------------------------------Include Library
//----------------------------------------see here: https://www.youtube.com/watch?v=8jMr94B8iN0 to add NodeMCU ESP8266 library and board
#include <DMDESP.h>
#include <fonts/ElektronMart6x8.h>
#include <fonts/Mono5x7.h>
#include <fonts/EMSans8x16.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

//----------------------------------------
#include <ESP8266WiFi.h>
#include <time.h>

//----------------------------------------DMD Configuration (P10 Panel)
#define DISPLAYS_WIDE 2 //--> Panel Columns
#define DISPLAYS_HIGH 1 //--> Panel Rows
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  //--> Number of Panels P10 used (Column, Row)


int timezone = 7*3600;
int dst = 0;
bool a;
int WD = 64;

int nw = 0; 
String nww,nwh,nws,mytime;

// =======================================================================
// CHANGE YOUR CONFIG HERE:
// =======================================================================
const char* ssid     = "al-husna";     // SSID of local network
const char* password = "sekolahdasar";   // Password on network

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
//========================================================================VOID SETUP()
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(25200);


  
  //----------------------------------------DMDESP Setup
  Disp.start(); //--> Run the DMDESP library
  Disp.setBrightness(50); //--> Brightness level
  Disp.setFont(Mono5x7); //--> Determine the font used
  //----------------------------------------
  
    // configTime(timezone,dst,"ntp.nict.jp","ntp.jst.mfeed.ad.jp"); 
//     configTime(timezone,dst,"pool.ntp.org","time.nist.gov");
//     time_t now = time(nullptr);
//     struct tm* p_tm = localtime(&now);
//     nw = now;
//     a = false;

}    

//========================================================================VOID LOOP()

void loop() {
  timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);  

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  String currentMonthName = months[currentMonth-1];
  int currentYear = ptm->tm_year+1900;

  //Print complete date:
  String currentDate = String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear);
   
    Disp.loop(); // Jalankan Disp loop untuk refresh LED
    Disp.setBrightness(100); 
    Disp.setFont(Mono5x7);  
    Disp.drawText(8, 0, formattedTime);
   // Disp.drawText(2, 9, currentDate);
    Scrolling_Text(9, 40);

    //Disp.loop(); 
//    Disp.setBrightness(100); 
//    Disp.setFont(EMSans8x16);  
//    Disp.drawText(0, 0, formattedTime);

//    Disp.clear();
}
//========================================================================

//========================================================================Subroutines for scrolling Text
static char *Text[] = {"John Eswin Nizar 08561811500"};

void Scrolling_Text(int y, uint8_t scrolling_speed) {
  static uint32_t pM;
  static uint32_t x;  int width = Disp.width();
  Disp.setFont(Mono5x7);
 // Disp.setFont(ElektronMart6x8);
  
  int fullScroll = Disp.textWidth(Text[0]) + width;
  if((millis() - pM) > scrolling_speed) { 
    pM = millis();
    if (x < fullScroll) {
      ++x;
    } else {
      x = 0;
      return;
    }
    Disp.drawText(width - x, y, Text[0]);
  }  
}
//========================================================================

void Clock_itt() {
  
     time_t now = time(nullptr);
     struct tm* p_tm = localtime(&now);
        
     nwh = p_tm->tm_hour;
     nww = p_tm->tm_min;
     nws = p_tm->tm_sec;

    int v = nws.toInt();
    int va = nww.toInt();
  
  if (va < 10) {
    nww = '0'+nww;
    }
   
   if (v<10) {
      nws = '0'+nws;
    }

     mytime = nwh+":"+nww+":"+nws;

  if (v < 30) {
    a = true; 

   if (v==0) {
     
     for (int s=0; s<WD;s++) 
      {
    
               for (int j=0;j<17;j++) 
                   {
                  Disp.setPixel(s,j,0);
                   }             
      }
    }
    
  }
  else 
  {
    if (v==30) 
     { 
      for (int s=0; s<WD;s++) 
      {

                  for (int j=0;j<17;j++) 
                   {
                  Disp.setPixel(s,j,0);
                   }  
       
      }
    }
      
    a = false;
  }

     
// if (a==false)                // big clock 
//  {
//    Disp.setBrightness(100); 
//    Disp.loop(); 
//    Disp.setFont(EMSans8x16);  
//    Disp.drawText(0, 0, mytime);
//  }
//  else      // true                   // small clock  run
//  {   
//    Disp.setBrightness(100); 
//    Disp.setFont(Mono5x7);  
//    Disp.drawText(7, 8, mytime);
//  }
}
