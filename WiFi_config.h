// WiFi network configuration:
// A simple method to configure multiple WiFi Access Configurations:
// Add the SSID and the password to the list.
// IMPORTANT: keep both arrays with the same length!
String WIFI_ssid[]     = {"Home_SSID", "Work_ssid", "Mobile_ssid", "Best-Friend_ssid"};
String WIFI_password[] = {"Home_Pass",   "Work_pwd",  "Mobile_pwd",  "Best-Friend_pwd"};

void scan_WiFi()
{
  M5.Lcd.fillScreen(BLACK); M5.Lcd.drawString("WiFi scan ...",0,0,2); M5.Lcd.setCursor(0,16); M5.Lcd.setTextSize(1);
  int n = WiFi.scanNetworks();  //  WiFi.scanNetworks returns the number of networks found
  uint16_t col=WHITE;
  if (n == 0) {M5.Lcd.println("[ERR] no networks found"); return;}
  int w=M5.Lcd.width();
  for (int i = 0; i < n; ++i)// Print Info for each network found
  {
    if(WiFi.encryptionType(i)==0){M5.Lcd.setTextColor(WHITE,RED); col=RED;} else{M5.Lcd.setTextColor(BLACK,GREEN); col=GREEN;}
    M5.Lcd.printf("%d:%-17s%3d\n",i+1,WiFi.SSID(i).c_str(),WiFi.RSSI(i));
    if(w>135){M5.Lcd.fillRect( 135,M5.Lcd.getCursorY()-7,w-135,6,WHITE); M5.Lcd.fillRect(135,M5.Lcd.getCursorY()-6,(w-135)*(120+WiFi.RSSI(i))/100,4,col);}
    delay(10);
  }
  M5.Lcd.setTextColor(WHITE,BLACK);
}

//==============================================================
// establish the connection to an Wifi Access point
boolean connect_Wifi(const char * ssid, const char * password)
{
// 	Establish connection to the specified network until success.
//	Important to disconnect in case that there is a valid connection
	int i_lcdw=M5.Lcd.width();
	int i_lcdh=M5.Lcd.height();
	WiFi.disconnect();
	M5.Lcd.println("Connecting to "); M5.Lcd.println(ssid);
	delay(500);
	WiFi.begin(ssid, password); //Start connecting (done by the ESP in the background)
	wl_status_t wifi_Status = WiFi.status();   // read wifi Status
	int n_trials = 0;
//	loop while waiting for Wifi connection run only for 5 trials.
	while (wifi_Status != WL_CONNECTED && n_trials < 5)
	{
//		Check periodicaly the connection status using WiFi.status()
//		Keep checking until ESP has successfuly connected	or maximum number of trials is reached
    delay(500);
		wifi_Status = WiFi.status(); n_trials++;
		switch(wifi_Status)
		{
			case WL_NO_SSID_AVAIL:   M5.Lcd.println("[ERR] SSID not available");  break;
			case WL_CONNECT_FAILED:  M5.Lcd.println("[ERR] Connection failed");   break;
			case WL_CONNECTION_LOST: M5.Lcd.println("[ERR] Connection lost");     break;
			case WL_DISCONNECTED:    M5.Lcd.println("[ERR] WiFi disconnected");   break;
			case WL_IDLE_STATUS:     M5.Lcd.println("[ERR] WiFi idle status");    break;
			case WL_SCAN_COMPLETED:  M5.Lcd.println("[OK] WiFi scan completed");  break;
			case WL_CONNECTED:       M5.Lcd.println("[OK] WiFi connected");       break;
			default:                 M5.Lcd.println("[ERR] unknown Status");      break;
		}
	}
	if(wifi_Status == WL_CONNECTED) // Success
			{ M5.Lcd.fillScreen(BLACK); M5.Lcd.drawCentreString("IP(by DHCP)",i_lcdw/2,2,1); M5.Lcd.drawCentreString(WiFi.localIP().toString(),i_lcdw/2,20,4); return true;  }
		else{ M5.Lcd.println(""); M5.Lcd.println("[ERR] unable to connect Wifi"); return false; }
}

void setup_WiFi()
{
	if(WiFi.status() != WL_CONNECTED)
	{
		scan_WiFi(); delay(1000);
		int WIFI_location = 0;// Try all access configurations until a connection could be established
		while(WiFi.status() != WL_CONNECTED)
		{
			delay(1000);
			M5.Lcd.fillScreen(BLACK);
			connect_Wifi(WIFI_ssid[WIFI_location].c_str(), WIFI_password[WIFI_location].c_str());
			WIFI_location++;
			if(WIFI_location >= (sizeof(WIFI_ssid)/sizeof(WIFI_ssid[0]))){WIFI_location = 0;}
		}
		M5.Lcd.println("");
		M5.Lcd.println("[OK] Connected to WiFi");
	}
}
