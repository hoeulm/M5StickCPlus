#include <time.h>

RTC_TimeTypeDef rtc_Time;
RTC_DateTypeDef rtc_Date;

void delay_Btn(String s_txt="OK",int i_ms=5000)
{
	long l_ms0=millis();
	if(s_txt!=""){M5.Lcd.drawCentreString("["+s_txt+"]",M5.Lcd.width()/2,M5.Lcd.height(),2);}
	while(millis()-l_ms0<i_ms){M5.update(); if(M5.BtnA.wasPressed()) break;}
}

String mnu()
{
   bool btnA=false,btnB=false;
   while(true)
   {
      M5.Lcd.fillRect(0,i_lcdh-9,i_lcdw,9,DARKGREY);
      int i_prev=i_idxmnu-1; if(i_prev<0){i_prev=MNUMAX-1;} 
      int i_next=(i_idxmnu+1)%MNUMAX;
      M5.Lcd.
      setTextColor(WHITE); 
      M5.Lcd.drawCentreString(as_mnu[i_prev],i_lcdw/4-16,i_lcdh-8,1);
      M5.Lcd.drawRightString(as_mnu[i_next],3*i_lcdw/4+16,i_lcdh-8,1);
      M5.Lcd.setTextColor(BLACK,WHITE);
      M5.Lcd.drawCentreString(" "+as_mnu[i_idxmnu]+" ",i_lcdw/2,i_lcdh-8,1);
      while(true){M5.update(); btnA=M5.BtnA.wasPressed(); btnB=M5.BtnB.wasPressed(); if(btnA | btnB){break;}}
      if(btnA){M5.Lcd.fillScreen(BLACK); M5.Lcd.drawCentreString(as_mnu[i_idxmnu],i_lcdw/2,0,4); break;}
      if(btnB){i_idxmnu++; i_idxmnu%=MNUMAX;}
    }
    return(as_mnu[i_idxmnu]);
}

void drawLocalTime()
{
  M5.Lcd.fillScreen(BLACK);
  static const char *ac_wd[7] = {"So","Mo","Di","Mi","Do","Fr","Sa"};
  M5.Rtc.GetTime(&rtc_Time);
  M5.Rtc.GetData(&rtc_Date);
  M5.Lcd.setTextColor(GREEN,BLACK);
  uint16_t w=M5.Lcd.width();
  if(w==80) // x,y,font 7
    {M5.Lcd.setCursor( 0, 2,7); M5.Lcd.printf("%02d",rtc_Time.Hours); M5.Lcd.fillCircle(75,18,3,GREEN); M5.Lcd.fillCircle(75,36,3,GREEN);M5.Lcd.setCursor( 0,60,7); M5.Lcd.printf("%02d",rtc_Time.Minutes);M5.Lcd.setCursor(68,70,1); M5.Lcd.printf("%02d",rtc_Time.Seconds); M5.Lcd.setCursor(0,110, 1);}
  if(w==135)
    {M5.Lcd.setCursor( 0,25,7); M5.Lcd.printf("%02d",rtc_Time.Hours); M5.Lcd.fillCircle(66,38,3,GREEN); M5.Lcd.fillCircle(66,58,3,GREEN);M5.Lcd.setCursor(70,25,7); M5.Lcd.printf("%02d",rtc_Time.Minutes);M5.Lcd.setCursor(20,90, 2);}
   if(w==160) 
        { M5.Lcd.setCursor(0,2,7);M5.Lcd.printf("%02d:%02d", rtc_Time.Hours, rtc_Time.Minutes); M5.Lcd.setTextFont(2); M5.Lcd.printf(":%02d\n",rtc_Time.Seconds);M5.Lcd.setCursor(80,90, 2); }
  if(w>160) 
        { M5.Lcd.setCursor(45,2,7);M5.Lcd.printf("%02d:%02d", rtc_Time.Hours, rtc_Time.Minutes); M5.Lcd.setTextFont(2); M5.Lcd.printf(":%02d\n",rtc_Time.Seconds);M5.Lcd.setCursor(80,90, 2); }
  M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.printf("%04d-%02d-%02d %s\n", rtc_Date.Year, rtc_Date.Month, rtc_Date.Date, ac_wd[rtc_Date.WeekDay]);
//  if(i_hh_last!=rtc_Time.Hours){beep_Hours(rtc_Time.Hours);}
}

void drawAnalogClock()
{
  uint16_t xm,w=M5.Lcd.width(),ym,h=M5.Lcd.height(),r;
  if(h>w){r=w/2; xm=r; ym=h/2;} else{r=h/2; xm=w/2; ym=r;}
  M5.Lcd.fillCircle(xm,ym,r-1,BLACK);
  M5.Lcd.drawCircle(xm,ym,r-1,WHITE);
  M5.Rtc.GetTime(&rtc_Time);
  M5.Rtc.GetData(&rtc_Date);
  float  RAD=PI/180.0;
  float f_hg=rtc_Time.Hours*30*RAD, f_mg=rtc_Time.Minutes*6*RAD, f_sg=rtc_Time.Seconds*6*RAD;
  M5.Lcd.drawLine(xm,ym,xm+r*0.75*sin(f_hg),ym-r*0.70*cos(f_hg),RED);   // Stundenzeiger  70%
  M5.Lcd.drawLine(xm,ym,xm+r*0.85*sin(f_mg),ym-r*0.80*cos(f_mg),BLUE);  // Minutenzeiger  80%
  M5.Lcd.drawLine(xm,ym,xm+r*0.95*sin(f_sg),ym-r*0.90*cos(f_sg),GREEN); // Sekundenzeiger 90%
}

void dirFiles(String s_path="/")
{
  int i_cntd=0,i_cntf=0;
  M5.Lcd.fillScreen(BLACK); int i_lcdw=M5.Lcd.width(),i_lcdh=M5.Lcd.height();
  Serial.println("DIR");
  M5.Lcd.drawString("Files on SPIFFS:",0,0,2);
  M5.Lcd.setTextSize(1); M5.Lcd.setCursor(0,16);
  File root=SPIFFS.open(s_path);
  if(root)
  {
    while(true)
    {
      File entry =  root.openNextFile();
      if (!entry) break;
      if (entry.isDirectory())
          {i_cntd++; M5.Lcd.printf("DIR: %s\n",entry.name());}
        else{i_cntf++; M5.Lcd.printf("%-19s%7d\n",entry.name(),entry.size()); Serial.println(entry.name()); }
    }
    root.close();
  }
  int i_used=100*SPIFFS.usedBytes()/SPIFFS.totalBytes();
  M5.Lcd.printf("Total %2dDir %2dFile Use=%2d%%\n",i_cntd,i_cntf,i_used);
  Serial.printf("Total %2dDir %2dFile Use=%2d%%\n",i_cntd,i_cntf,i_used);
  M5.Lcd.printf("SIZE:%8d kB\n",SPIFFS.totalBytes()/1024);
}

//==============================================================
// function to set the brightness and the dimming timer
void Lcd_setBrightness(int brightness)
{
#ifdef M5_STACK
  { M5.Lcd.setBrightness(brightness);} // Brightness (0: Off - 255: Full)
#else
  if(brightness>250) {M5.Axp.ScreenBreath(15);} else{M5.Axp.ScreenBreath(7+brightness/32);}
#endif
}
/*
  void drawBMP565(String s_fn,int xo,int yo)
{
 int i,w,h,col,b,j,k,id,x,y;
  File bmp = SPIFFS.open(s_fn, FILE_READ);
  id=256*bmp.read()+bmp.read(); if(id!=0x424d){Serial.println("NO BMP"); return;}
  for(i=0;i<16;i++){b=bmp.read();}
  w=bmp.read()+256*bmp.read(); id=bmp.read()+256*bmp.read();
  h=bmp.read()+256*bmp.read(); id=bmp.read()+256*bmp.read();
  for(i=26;i<54;i++){b=bmp.read();}
  Serial.printf("BMP565 %s w=%4d h=%4d\n",s_fn.c_str(),w,h);
  for(y=h-1;y>=0;y--){for(x=0;x<w;x++){col=bmp.read()+256*bmp.read(); M5.Lcd.drawPixel(xo+x,yo+y,col);}}
}
 
 */
void drawBMP565(String s_fn,int xo,int yo)
{
	int i,w,h,col,b,j,k,id,x,y,xx;
	File bmp = SPIFFS.open(s_fn,"r");
  if(!bmp){Serial.printf("ERR no %s on SPIFFS\n",s_fn.c_str()); dirFiles(); return;}
  id=256*bmp.read()+bmp.read(); 
	for(i=0;i<16;i++){b=bmp.read();}
	w=bmp.read()+256*bmp.read(); xx=bmp.read()+256*bmp.read();
	h=bmp.read()+256*bmp.read(); xx=bmp.read()+256*bmp.read();
	Serial.printf("BMP565 %s size=%d %4X w=%4d h=%4d\n",s_fn.c_str(),bmp.size(),id,w,h);
	//if(id!=0x424d){Serial.printf("NO BMP %4X\n",id); return;}
	for(i=26;i<54;i++){b=bmp.read();}
	for(y=0;y<h-1;y++){for(x=0;x<w;x++){col=bmp.read()+256*bmp.read(); M5.Lcd.drawPixel(xo+x,yo+y,col);}}
}

//==============================================================
// Clear the entire screen and add one row
// The added row is important. Otherwise the first row is not visible
void Clear_Screen()
{
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.setCursor(0, 0); M5.Lcd.setTextColor(WHITE,BLACK);
	M5.Lcd.println("");
}

void AXP_Report()
{
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 4, 1);
  M5.Lcd.printf("Battery\n");     M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.printf(" State:%6d\n"  , M5.Axp.GetBatState());
  M5.Lcd.printf(" Warn :%6d\n"  , M5.Axp.GetWarningLevel());
  M5.Lcd.print(" Temp :"); M5.Lcd.setTextColor(YELLOW,BLACK); M5.Lcd.printf("%6.1f\n",M5.Axp.GetTempInAXP192());M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.print(" U[V] :");  M5.Lcd.setTextColor(BLUE,BLACK);  M5.Lcd.printf("%6.3f\n",M5.Axp.GetBatVoltage());  M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.print(" I[mA]:");  M5.Lcd.setTextColor(RED ,BLACK);  M5.Lcd.printf("%6.1f\n",M5.Axp.GetBatCurrent());  M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.printf(" P[mW]:%6.1f\n", M5.Axp.GetBatPower());
  M5.Lcd.printf("ASP\n");
  M5.Lcd.printf(" U[V] :"); M5.Lcd.setTextColor(BLUE,BLACK);  M5.Lcd.printf("%6.3f\n",M5.Axp.GetAPSVoltage());  M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.printf("VBus(USB)\n");
  M5.Lcd.printf(" U[V] :"); M5.Lcd.setTextColor(BLUE,BLACK);  M5.Lcd.printf("%6.3f\n",M5.Axp.GetVBusVoltage()); M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.printf(" I[mA]:"); M5.Lcd.setTextColor(RED ,BLACK);  M5.Lcd.printf("%6.1f\n",M5.Axp.GetVBusCurrent()); M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.printf("VIN(5V-In)\n");
  M5.Lcd.printf(" U[V] :"); M5.Lcd.setTextColor(BLUE,BLACK);  M5.Lcd.printf("%6.3f\n",M5.Axp.GetVinVoltage());  M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.printf(" I[mA]:"); M5.Lcd.setTextColor(RED ,BLACK);  M5.Lcd.printf("%6.1f\n",M5.Axp.GetVinCurrent());  M5.Lcd.setTextColor(WHITE,BLACK);
  int s=millis()/1000;
  M5.Lcd.printf("Upti:%2d:%02d:%02d\n",s/3600,(s%3600)/60,s%60);
}

// buffer for formatting numbers with thousand separator see function description for further  details
char format_buffer[11];
char thousand_separator = '.';
//==============================================================
// Function to format a integer number into a string 
// with thousand separator and a fixed length
// useful to get a nice right aligned string output
// inspired from:
// https://stackoverflow.com/questions/1449805/how-to-format-a-number-from-1123456789-to-1-123-456-789-in-c
// Use as following:
//
// char buffer[11]; // 10 char + \0 = 11
// M5.Lcd.printf("%s\n", formatNumber(1234567, format_buffer, sizeof(buffer)));
// output: " 1.234.567"
const char *formatNumber (int value, char *buffer, int len)
{
	int charCount;
	char *endOfbuffer = buffer+len;
	int savedValue = value;
	if (value < 0){ value = - value;}
	// \0 Termination char at the end
	*--endOfbuffer = 0;
	charCount = -1;
	do
	{
		if (++charCount == 3)
		{
			charCount = 0;
			*--endOfbuffer = thousand_separator;
			if(endOfbuffer <= buffer) break;
		}
		*--endOfbuffer = (char) (value % 10 + '0');
	} while((value /= 10) != 0 && endOfbuffer > buffer);
//	add a minus sign if the original value was negative
	if (savedValue < 0 && endOfbuffer > buffer){*--endOfbuffer = '-';}
//	fill up with spaces, to the full string length
	while(endOfbuffer > buffer){ *--endOfbuffer = ' ';}
	return endOfbuffer;
}
void draw_NumberBig(long l_num)
{
  if(l_num>1e9){M5.Lcd.printf("%d.%03d.%03d.%03d\n",l_num/1000000000,(l_num%1000000000)/1000000,(l_num%1000000)/1000,l_num%1000); return;}
  if(l_num>1e6){M5.Lcd.printf("%d.%03d.%03d\n",l_num/1000000,(l_num%1000000)/1000,l_num%1000); return;}
  if(l_num>1e3){M5.Lcd.printf("%d.%03d\n",l_num/1000,l_num%1000); return;}
  M5.Lcd.printf("%d\n",l_num);
}
/*
bool download_to_SPIFFS(const char* achost, const char* acpath,const char* ac_fnlocal) 
{
  int i_cnt=0;
  String s_line;
  WiFiClient client;
  if(!client.connect(achost, 80))
  { M5.Lcd.printf("[ERR] %s Connection failed!",achost); return false;}
  File fh_down=SPIFFS.open(ac_fnlocal,"w");
  if(!fh_down)
  { M5.Lcd.printf("[ERR] %s open on SPIFFS failed!",ac_fnlocal); return false;}
  client.println("GET http://"+String(achost)+String(acpath)+" HTTP/1.0");
  client.println("Host: "+String(achost));
  client.println("Connection: close"); // Close AFTER download NO KEEPALIVE!!
  client.println(); // Endof REQUEST
  while(client.connected())  //Header LOOP
  {
    s_line = client.readStringUntil('\n');
    Serial.println(s_line.substring(0,14));
    if(s_line.substring(0,14)=="Content-Length"){Serial.println(s_line.substring(16));}
    if(s_line.length()==1) { M5.Lcd.println("[OK] header received");  break; }
  }
  while(client.connected())  // DAta Loop
  {
    while (client.available()) 
    {
      s_line= client.readStringUntil('\n'); // get one line from the server data
      if(s_line[0]>'9') {Serial.println(s_line);}
      fh_down.println(s_line);
    }
  } 
  fh_down.close();
  return true;
}
*/
