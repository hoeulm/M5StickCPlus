#define COVIDCSV "/covid_world.csv"

#define max_number_countries 200
int ai_conf[480],ai_dead[480];

String s_Line,s_tld,s_nam,s_nde,s_ew,s_ymd,s_ymd0;
String ss_ymdlast = "";          // String to hold the last date found in the COVID file
int i_countries_found=0,i_prozent,i_dead,i_conf,i_reco,i_idx,i_x,i_idxval;
int i_idC;
int j,i_iz7,i_iz7max,i_deadmax;
bool b_is_europe;
long l_ew;

uint16_t col_iz7(int iz7)
{
	uint16_t col=GREEN;
	if(iz7> 10) {col=YELLOW;}
	if(iz7> 35) {col=ORANGE;}
	if(iz7> 50) {col=RED;}
	if(iz7>100) {col=PINK;}
	if(iz7>200) {col=MAGENTA;}
	return col;
}

float f_iz7(int i)
{
	if(i>6){return 100000.0*(ai_conf[i]-ai_conf[i-7])/l_ew;}
	return 100000.0*(ai_conf[i]*7)/l_ew;
}
 
void covid_graf()
{   
	int i_lcdw=M5.Lcd.width(),i_lcdh=M5.Lcd.height();
	int x,y1,y2, xx=i_lcdw-1, yy=i_lcdh-1,i;
	float f_i;
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.drawString(s_nde,0,0,2);M5.Lcd.setCursor(0,16);
	M5.Lcd.println(s_ymd0+".."+s_ymd);
	M5.Lcd.print("Pop="); draw_NumberBig(l_ew);
	M5.Lcd.printf("Iz7=%4d\n",i_iz7);
	M5.Lcd.printf("Iz7=%4d max\n",i_iz7max);
	M5.Lcd.printf("Deaths=%d\n",i_deadmax);
	M5.Lcd.printf("DpM=%.1f\n ",1000000.0*i_deadmax/l_ew);
	i_idxval--;
	if(i_iz7max==0){i_iz7max=1;}
	if(i_deadmax==0){i_deadmax=1;}
	for(int x=0;x<=xx;x++)
	{f_i=1.0*x*i_idxval/xx; i=(int)f_i; i_iz7=f_iz7(i); y2=yy-yy*i_iz7/i_iz7max;       if(x>0){M5.Lcd.drawLine(x-1,y1,x,y2,col_iz7(i_iz7));} y1=y2;}
	for(int x=0;x<=xx;x++)
	{f_i=1.0*x*i_idxval/xx; i=(int)f_i;                 y2=yy-(yy*ai_dead[i])/i_deadmax; if(x>0){M5.Lcd.drawLine(x-1,y1,x,y2,WHITE);} y1=y2;}
	delay_Btn("",60000);
}

void eval_country()
{
	if(i_countries_found>0){covid_graf();}
	i_countries_found++;  i_prozent=(100*i_countries_found)/max_number_countries; // count the overall number of countries
	s_tld=s_Line.substring(0,2); s_Line=s_Line.substring(3);    j=s_Line.indexOf(';');
	s_nam=s_Line.substring(0,j); s_Line=s_Line.substring(j+1);  j=s_Line.indexOf(';');
	s_nde=s_Line.substring(0,j); s_Line=s_Line.substring(j+1);  j=s_Line.indexOf(';');
	l_ew=s_Line.toInt();
	i_idxval=0; i_iz7max=0; i_deadmax=0;
}

void eval_values()
{
	j=s_Line.indexOf(';');
	s_ymd =s_Line.substring(0,j);         s_Line=s_Line.substring(j+1);  j=s_Line.indexOf(';');
	i_conf=s_Line.substring(0,j).toInt(); s_Line=s_Line.substring(j+1);  j=s_Line.indexOf(';');
	i_dead=s_Line.substring(0,j).toInt(); s_Line=s_Line.substring(j+1);  j=s_Line.indexOf(';');
	i_reco=s_Line.substring(0,j).toInt();
	ai_conf[i_idxval]=i_conf; 
	ai_dead[i_idxval]=i_dead; i_deadmax=max(i_deadmax,i_dead); 
	if(i_idxval==0){s_ymd0=s_ymd;}
	if(i_idxval>7){i_iz7=100000.0*(ai_conf[i_idxval]-ai_conf[i_idxval-7])/l_ew; i_iz7max=max(i_iz7max,i_iz7); }
	i_idxval++;
} 

void covid_graf_tld(String tld)
{
	M5.Lcd.fillScreen(BLACK); M5.lcd.setCursor(0,0);
  M5.Lcd.setTextColor(WHITE);
 	File fhcovid=SPIFFS.open(COVIDCSV, "r");
  if(!fhcovid){Serial.println("NO covid-world.csv"); return;}
  M5.Lcd.drawString("COVID Graf for "+tld,0,0,2);
  M5.Lcd.print(fhcovid.size());
  bool b_done=false;
	while(fhcovid.available()) 
	{
		s_Line=fhcovid.readStringUntil('\n');
    Serial.println(s_Line);
		if(s_Line[0]!='#') // Kommentarzeile
		{ 
			if(s_Line[0]>'9') 
			{
				s_tld=s_Line.substring(0,2);
        Serial.println(s_Line);
				M5.Lcd.printf("%s ",s_tld);
				if(s_tld==tld) 
				{
					i_countries_found=0; eval_country();b_done=true; 
					while(fhcovid.available())
					{
						s_Line=fhcovid.readStringUntil('\n'); 
						if(s_Line[0]>'9')break;
						eval_values();
					}
					covid_graf();
				}
			}
		}
	}
	fhcovid.close();
} 
