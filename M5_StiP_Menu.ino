#include <M5StickCPlus.h>
//#include <M5StickC.h>
#include <SPIFFS.h>
#include <FS.h>
#include <WiFi.h>

#include "WiFi_Config.h"

int  i_dim=255;
uint16_t i_lcdw,i_lcdh;
#define MNUMAX 13 
String as_mnu[MNUMAX]={"Rot","Clock","AClk","Scan","Connect","Dir","BMP","Sleep","Dim","CLS","Covid","Bat","ENV"};
int i_idxmnu=0;

#include "helpers.h"

#include "covid.h"

void setup() 
{
    M5.begin();// initialize the M5Stack object
    M5.Lcd.setRotation(3); M5.Lcd.setTextColor(WHITE);  M5.Lcd.setTextSize(1); Clear_Screen();// configure the Lcd display
    i_lcdw=M5.Lcd.width(); i_lcdh=M5.Lcd.height();
    M5.Lcd.drawCentreString("Menu",i_lcdw/2,2,4);
    SPIFFS.begin(true);
    drawBMP565("/m5stick64.565", i_lcdw/2-20,24);
    M5.Lcd.drawCentreString("Version 2021-02-04", i_lcdw/2, i_lcdh-24, 1);
    delay_Btn("OK",5000);
}

void loop() 
{
  String s_mnu=mnu();
  if(s_mnu=="Scan") {scan_WiFi(); delay_Btn("",2000);}
  if(s_mnu=="Clock"){while(true){if(millis()%1000==0){drawLocalTime();   delay(1);} M5.update(); if(M5.BtnA.wasPressed()) break;  if(M5.BtnB.wasPressed()) break; }}
  if(s_mnu=="AClk") {while(true){if(millis()%1000==0){drawAnalogClock(); delay(1);} M5.update(); if(M5.BtnA.wasPressed()) break;  if(M5.BtnB.wasPressed()) break; }}
  if(s_mnu=="Dir")  {dirFiles();}
  if(s_mnu=="Cls")  {M5.Lcd.fillScreen(BLACK); delay_Btn("",2000);}
  if(s_mnu=="Dim")  {i_dim+=32;if(i_dim>255){i_dim=0;} Lcd_setBrightness(i_dim); M5.Lcd.drawCentreString("Dim="+String(i_dim),i_lcdw/2,10,4);}
  if(s_mnu=="Rot")  {int i_rot=M5.Lcd.getRotation(); i_rot++; i_rot%=4; M5.Lcd.setRotation(i_rot); i_lcdw=M5.Lcd.width(); i_lcdh=M5.Lcd.height(); M5.Lcd.fillScreen(BLACK); M5.Lcd.drawCentreString("Rot="+String(i_rot),i_lcdw/2,0,4);}
  if(s_mnu=="BMP")  {M5.Lcd.fillScreen(BLACK); drawBMP565("/tagesschau.565",i_lcdw/2-20,16); delay_Btn("",2000);
                     drawBMP565("/m5stack64.565", i_lcdw/2-20,16); delay_Btn("",2000); 
                     drawBMP565("/rowerc.565", i_lcdw/2-20,16); delay_Btn("",2000); 
                     drawBMP565("/virus64.565", i_lcdw/2-20,16); delay_Btn("",2000); 
                     drawBMP565("/rower3264.565", i_lcdw/2-20,16); delay_Btn("",2000); 
                     if(M5.Lcd.getRotation()%2==1){drawBMP565("/adk240135.565",0,0); delay_Btn("",2000); }
                     }
   if(s_mnu=="Connect"){setup_WiFi();  }
   if(s_mnu=="Covid"){covid_graf_tld("de"); covid_graf_tld("il"); }
   if(s_mnu=="Bat")  {AXP_Report(); delay_Btn("",2000);}
}
