#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>



#define USE_I2C_LCD  1

#ifdef USE_I2C_LCD

//#define LCD_SCL_PIN SCL
//#define LCD_SDA_PIN SDA

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setUpLcd(){
  Wire.begin();
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Charging voltage");
  lcd.setCursor(0,1);
  lcd.print("DC: ");
}


#endif //USE_I2C_LCD
















///*******************************************************///
///*******************************************************///
///              SERVER CODE START FROM HERE              ///
///*******************************************************///
///*******************************************************///
#define LOLIN_LED D4
#define PRODUCTION  1
//#define DEBUG_SERVER  1
String HOME = "/"; 

/* Global vareiables are here */
unsigned long timestamp = 0;


/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);


//@auto generated code
//QR Format
//WIFI:S:MySSID;T:WPA;P:MyPassW0rd;;
//hotspot config
const char* hotspot_name = "iota0322-charg";
const char* hotspot_password = "iota0322";

struct{
	String message;
	String message_class = "hide";

	float voltage;
	String voltage_class = "success";

} dataPacket;



struct{
	 const int BTN_NONE = -1;
} btnAction;
int userBtnAction = btnAction.BTN_NONE;



#ifdef PRODUCTION 
String getDataJson(){
	return "{\"message\":\""+dataPacket.message+"\",\"message_class\":\""+dataPacket.message_class+"\", "
	"\"voltage\":\""+String(dataPacket.voltage)+"\",\"voltage_class\":\""+dataPacket.voltage_class+"\"}";
}
#endif



void handel_UserAction(){
	for (uint8_t i = 0; i < server.args(); i++) {
	}
	server.send(200, "text/json", getDataJson());
}



#ifdef DEBUG_SERVER
String getTestClass(){
	int r = random(0,4);
	switch (r){
		case 0: return "primary";
		case 1: return "secondary";
		case 2: return "success";
		case 3: return "danger";
		case 4: return "warning";
	}
}
String getDataJson(){
	return "{\"message\":\""+String(random(10,99))+"\",\"message_class\":\""+getTestClass()+"\", "
	"\"voltage\":\""+String(random(10,99))+"\",\"voltage_class\":\""+getTestClass()+"\"}";
}
#endif



// auto generated code
void forwardTo(String location){
  server.sendHeader("Location", location, true);
  server.send( 302, "text/plain", "");
}

void handle_Home() {
  server.send( 200, "text/html", getTemplate());
}

void handle_DataRequest(){ 
  server.send( 200, "text/json", getDataJson());
}


void handle_NotFound(){
  forwardTo(HOME);
}


void setUpServer(){
    delay(500);
    WiFi.softAP(hotspot_name, hotspot_password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    server.begin();
    delay(300);
    Serial.println("server started.");

}



#define IR_PIN D1





void setUpGPIO(void);

void setup() {
  delay(500);
  Serial.begin(115200);
  Serial.println("\n\nstartng...");
  setUpServer();
  setUpGPIO();
#ifdef USE_I2C_LCD
  setUpLcd();
#endif //USE_I2C_LCD
  timestamp = millis();
}

void setUpGPIO(){
	pinMode(LOLIN_LED,OUTPUT);
	pinMode(IR_PIN,INPUT);
}

int heartRate(){
  return random(65, 85);
}
int oxigen(){
  return random(96, 100);
}



uint32_t led_time_stamp = 0;
void blinkLed(int mil){
  if(led_time_stamp+mil < millis()){
      led_time_stamp = millis();
      if(digitalRead(LOLIN_LED) == 0){
          digitalWrite(LOLIN_LED, HIGH);
      }
      else{
        digitalWrite(LOLIN_LED, LOW);
      } 
  }
}


uint32_t lcd_update_time = 0;
uint32_t lcd_update_stamp = 0;
int vant_open  = 0;
void loop() {
  server.handleClient();
  blinkLed(500);
  if(userBtnAction != btnAction.BTN_NONE){
    
   

    userBtnAction = btnAction.BTN_NONE;
    Serial.println("Button clicked");
    
  }

  


  if(lcd_update_time+100 < millis()){
      lcd_update_time = millis();

      double volt = analogRead(A0);

      volt = (volt*15)/1023;


      dataPacket.voltage = volt;
      


  }
	if(lcd_update_stamp +250 < millis()){
		lcd.setCursor(5,1);
  	lcd.print(dataPacket.voltage);
		lcd.print("    ");
	}

	
  


}



String getTemplate(){
	return 	"<!DOCTYPE html>\n"
	"<html>\n"
	"<head>\n"
	"<style>"
	"body{\n"
	"background-color: #F1FCFF;\n"
	"padding:0px;\n"
	"margin:0px;\n"
	"text-align: center;\n"
	"}\n"
	"header{\n"
	"height:35px;\n"
	"padding:10px;\n"
	"text-align:left;\n"
	"display: flex;\n"
	"background-color: #0093E9;\n"
	"position:fixed;\n"
	"width:100%;\n"
	"z-index:100;\n"
	"top:0;\n"
	"}\n"
	"footer{\n"
	"padding:20px;\n"
	"}\n"
	"\n"
	"form{\n"
	"margin:15px auto 0px auto;\n"
	"max-width:90%;\n"
	"background-color: #AAAAAA;\n"
	"padding: 15px 0 15px 0;\n"
	"border-radius: 5px;\n"
	"}\n"
	"\n"
	"button{\n"
	"margin:8px auto 0px auto;\n"
	"width:90%;\n"
	"background-color: #AAAAAA;\n"
	"padding: 10px 0 10px 0;\n"
	"border-radius: 5px;\n"
	"font-size: 24px;\n"
	"font-weight: bold;\n"
	"color:white;\n"
	"border: none;\n"
	"}\n"
	"button:active {\n"
	"width:89%;\n"
	"padding: 10px 0 10px 0;\n"
	"color:black;\n"
	"}\n"
	"input{\n"
	"margin:8px auto 0px auto;\n"
	"width:90%;\n"
	"padding: 10px 0 10px 0;\n"
	"border-radius: 5px;\n"
	"font-size: 22px;\n"
	"color:black;\n"
	"border: none;\n"
	"}\n"
	"\n"
	"label{\n"
	"margin:15px auto 0px auto;\n"
	"width:90%;\n"
	"padding: 0px 0 0px 0;\n"
	"font-size: 22px;\n"
	"display:block;\n"
	"}\n"
	"\n"
	".radio-group{\n"
	"margin:15px auto 15px auto;\n"
	"font-size: 24px;\n"
	"width:100%;\n"
	"display:flex;\n"
	"flex-direction: row;\n"
	"text-align:left;\n"
	"}\n"
	".radio-label{\n"
	"margin:0px;\n"
	"padding:0px;\n"
	"}\n"
	".radio{\n"
	"width:32px;\n"
	"margin:0px 10px 0px 15px;\n"
	"}\n"
	"\n"
	".content{\n"
	"margin-top:70px;\n"
	"}\n"
	".connection{\n"
	"margin-left:20px;\n"
	"color: white;\n"
	"}\n"
	".online{\n"
	"margin: 8px 0 0 -8px;\n"
	"font-size: 16px;\n"
	"color:white;\n"
	"}\n"
	".card{\n"
	"margin:15px auto 0px auto;\n"
	"max-width:90%;\n"
	"padding: 15px 0 15px 0;\n"
	"border-radius: 5px;\n"
	"}\n"
	".primary{\n"
	"background-color: #8BC6EC;\n"
	"visibility: visible;\n"
	"}\n"
	".secondary{\n"
	"background-color: #AAAAAA;\n"
	"visibility: visible;\n"
	"}\n"
	".success{\n"
	"background-color: #82c063;\n"
	"visibility: visible;\n"
	"}\n"
	".danger{\n"
	"background-color: #F76666;\n"
	"visibility: visible;\n"
	"}\n"
	".warning{\n"
	"background-color: #E3D377;\n"
	"visibility: visible;\n"
	"}\n"
	".hide{\n"
	"visibility: hidden;\n"
	"}\n"
	"@media only screen and (min-width: 500px) {\n"
	".card {\n"
	"max-width:400px;\n"
	"}\n"
	"button{\n"
	"max-width:400px;\n"
	"}\n"
	"form{\n"
	"max-width:400px;\n"
	"}\n"
	"label{\n"
	"max-width:400px;\n"
	"}\n"
	"}\n"
	"\n"
	"h1 {\n"
	"margin: 2px;\n"
	"color: white;\n"
	"}\n"
	"h2 {\n"
	"margin: 2px;\n"
	"color: black;\n"
	"}\n"
	"</style>\n"
	"<meta charset='utf-8'>\n"
	"<meta http-equiv='X-UA-Compatible' content='IE=edge'>\n"
	"<title>Iot Based Smart Vantilator</title>\n"
	"<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
	"<link rel='stylesheet' type='text/css' media='screen' href='main.css'>\n"
	"\n"
	"</head>\n"
	"<body onload=\"liveDataAjax()\">\n"
	"<header>\n"
	"<span class=\"connection\" id=\"connected\">\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"32\" height=\"32\" fill=\"currentColor\" class=\"connected\" viewBox=\"0 0 16 16\">\n"
	"<path d=\"M15.384 6.115a.485.485 0 0 0-.047-.736A12.444 12.444 0 0 0 8 3C5.259 3 2.723 3.882.663 5.379a.485.485 0 0 0-.048.736.518.518 0 0 0 .668.05A11.448 11.448 0 0 1 8 4c2.507 0 4.827.802 6.716 2.164.205.148.49.13.668-.049z\"/>\n"
	"<path d=\"M13.229 8.271a.482.482 0 0 0-.063-.745A9.455 9.455 0 0 0 8 6c-1.905 0-3.68.56-5.166 1.526a.48.48 0 0 0-.063.745.525.525 0 0 0 .652.065A8.46 8.46 0 0 1 8 7a8.46 8.46 0 0 1 4.576 1.336c.206.132.48.108.653-.065zm-2.183 2.183c.226-.226.185-.605-.1-.75A6.473 6.473 0 0 0 8 9c-1.06 0-2.062.254-2.946.704-.285.145-.326.524-.1.75l.015.015c.16.16.407.19.611.09A5.478 5.478 0 0 1 8 10c.868 0 1.69.201 2.42.56.203.1.45.07.61-.091l.016-.015zM9.06 12.44c.196-.196.198-.52-.04-.66A1.99 1.99 0 0 0 8 11.5a1.99 1.99 0 0 0-1.02.28c-.238.14-.236.464-.04.66l.706.706a.5.5 0 0 0 .707 0l.707-.707z\"/>\n"
	"</svg>\n"
	"</span>\n"
	"<span class=\"connection\" id=\"disconnected\">\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"32\" height=\"32\" fill=\"currentColor\" class=\"connected\" viewBox=\"0 0 16 16\">\n"
	"<path d=\"M10.706 3.294A12.545 12.545 0 0 0 8 3C5.259 3 2.723 3.882.663 5.379a.485.485 0 0 0-.048.736.518.518 0 0 0 .668.05A11.448 11.448 0 0 1 8 4c.63 0 1.249.05 1.852.148l.854-.854zM8 6c-1.905 0-3.68.56-5.166 1.526a.48.48 0 0 0-.063.745.525.525 0 0 0 .652.065 8.448 8.448 0 0 1 3.51-1.27L8 6zm2.596 1.404.785-.785c.63.24 1.227.545 1.785.907a.482.482 0 0 1 .063.745.525.525 0 0 1-.652.065 8.462 8.462 0 0 0-1.98-.932zM8 10l.933-.933a6.455 6.455 0 0 1 2.013.637c.285.145.326.524.1.75l-.015.015a.532.532 0 0 1-.611.09A5.478 5.478 0 0 0 8 10zm4.905-4.905.747-.747c.59.3 1.153.645 1.685 1.03a.485.485 0 0 1 .047.737.518.518 0 0 1-.668.05 11.493 11.493 0 0 0-1.811-1.07zM9.02 11.78c.238.14.236.464.04.66l-.707.706a.5.5 0 0 1-.707 0l-.707-.707c-.195-.195-.197-.518.04-.66A1.99 1.99 0 0 1 8 11.5c.374 0 .723.102 1.021.28zm4.355-9.905a.53.53 0 0 1 .75.75l-10.75 10.75a.53.53 0 0 1-.75-.75l10.75-10.75z\"/>\n"
	"</svg>\n"
	"</span>\n"
	"<span class=\"connection\">\n"
	"<p class=\"online\" id=\"online\" >Online</p>\n"
	"</span>\n"
	"</header>\n"
	"\n"
	"<div class=\"content\">\n"
	"<div class=\"card hide\" id=\"message\">\n"
	"<span></span>\n"
	"<h1></h1>\n"
	"</div>\n"
	"\n"
	"<div class=\"card primary\" id=\"voltage\">\n"
	"<span>\n"
	"<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"48\" height=\"48\" fill=\"yellow\" class=\"bi bi-thermometer-half\" viewBox=\"0 0 16 16\">\n"
	"<path d=\"M9.585 2.568a.5.5 0 0 1 .226.58L8.677 6.832h1.99a.5.5 0 0 1 .364.843l-5.334 5.667a.5.5 0 0 1-.842-.49L5.99 9.167H4a.5.5 0 0 1-.364-.843l5.333-5.667a.5.5 0 0 1 .616-.09z\"/>\n"
	"<path d=\"M2 4h4.332l-.94 1H2a1 1 0 0 0-1 1v4a1 1 0 0 0 1 1h2.38l-.308 1H2a2 2 0 0 1-2-2V6a2 2 0 0 1 2-2z\"/>\n"
	"<path d=\"M2 6h2.45L2.908 7.639A1.5 1.5 0 0 0 3.313 10H2V6zm8.595-2-.308 1H12a1 1 0 0 1 1 1v4a1 1 0 0 1-1 1H9.276l-.942 1H12a2 2 0 0 0 2-2V6a2 2 0 0 0-2-2h-1.405z\"/>\n"
	"<path d=\"M12 10h-1.783l1.542-1.639c.097-.103.178-.218.241-.34V10zm0-3.354V6h-.646a1.5 1.5 0 0 1 .646.646zM16 8a1.5 1.5 0 0 1-1.5 1.5v-3A1.5 1.5 0 0 1 16 8z\"/>\n"
	"</svg>\n"
	"</span>\n"
	"<h1></h1>\n"
	"</div>\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"</div>\n"
	"<footer>\n"
	"\n"
	"</footer>\n"
	"\n"
	"<script>"
	"var DRT = 100;\n"
	"function updateCSSClass(element, css){\n"
	"    if(css != 'primary')\n"
	"        element.classList.remove('primary');\n"
	"    if(css != 'secondary')\n"
	"        element.classList.remove('secondary');\n"
	"    if(css != 'success')\n"
	"        element.classList.remove('success');\n"
	"    if(css != 'danger')\n"
	"        element.classList.remove('danger');\n"
	"    if(css != 'warning')\n"
	"        element.classList.remove('warning');\n"
	"    if(css != 'hide')\n"
	"        element.classList.remove('hide');\n"
	"    element.classList.add(css);\n"
	"}\n"
	"\n"
	"function updateData(data){\n"
	"\tdocument.getElementById(\"message\").children[1].innerHTML = \"\"+data.message+\"\";\n"
	"\tupdateCSSClass(document.getElementById(\"message\"), data.message_class);\n"
	"\tdocument.getElementById(\"voltage\").children[1].innerHTML = \"\"+data.voltage+\" Volt\";\n"
	"\tupdateCSSClass(document.getElementById(\"voltage\"), data.voltage_class);\n"
	"}\n"
	"\n"
	"function getCommand(btn_id, value){\n"
	"    \n"
	"}\n"
	"\n"
	"function onClickBtn(btn_id){\n"
	"\tvar val = document.getElementById(btn_id).innerHTML;\n"
	"\tvar cmd = getCommand(btn_id,val);\n"
	"    console.log(cmd)\n"
	"\tsendButtonClick('/act?'+btn_id+'='+cmd)\n"
	"}\n"
	"\n"
	"\n"
	"\n"
	"function updateNetwork(connected){\n"
	"    if(connected){\n"
	"        document.getElementById('disconnected').style.display = 'none';\n"
	"        document.getElementById('connected').style.display = 'block';\n"
	"        document.getElementById('online').innerHTML = 'Online';\n"
	"    }\n"
	"    else{\n"
	"        document.getElementById('connected').style.display = 'none';\n"
	"        document.getElementById('disconnected').style.display = 'block';\n"
	"        document.getElementById('online').innerHTML = 'Offline';\n"
	"    }\n"
	"}\n"
	"\n"
	"\n"
	"function sendButtonClick(url){\n"
	"\t\n"
	"    const xhr = new XMLHttpRequest();\n"
	"    xhr.open('GET', url, true);\n"
	"    xhr.onload = () => {\n"
	"        if(xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {\n"
	"            var data= JSON.parse(xhr.responseText);\n"
	"            updateData(data);\n"
	"            updateNetwork(true);\n"
	"        }\n"
	"    }\n"
	"    xhr.onerror = function() {\n"
	"        updateNetwork(false);\n"
	"    };\n"
	"    xhr.send();\n"
	"}\n"
	"\n"
	"\n"
	"\n"
	"\n"
	"var netcount = 0;\n"
	"function reconnect(){\n"
	"    if(netcount == 0){\n"
	"        console.log(\"Retrying\");\n"
	"        document.getElementById('online').innerHTML = 'Retrying..';\n"
	"        setTimeout(liveDataAjax,1000);\n"
	"        return\n"
	"    }\n"
	"    netcount -= 1;\n"
	"    console.log(\"count\",netcount);\n"
	"    document.getElementById('online').innerHTML = 'Offline ('+netcount+')';\n"
	"    setTimeout(reconnect, 1000);\n"
	"}\n"
	"function liveDataAjax(){\n"
	"    const xhr = new XMLHttpRequest();\n"
	"    xhr.open('GET', '/data.json', true);\n"
	"    xhr.onload = () => {\n"
	"        if(xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {\n"
	"            var data= JSON.parse(xhr.responseText);\n"
	"            updateData(data);\n"
	"            updateNetwork(true);\n"
	"            setTimeout(liveDataAjax, DRT);\n"
	"        }\n"
	"        else if (xhr.readyState === XMLHttpRequest.DONE){\n"
	"            updateNetwork(false);\n"
	"            netcount = 5;\n"
	"            reconnect();\n"
	"        }\n"
	"    };\n"
	"    xhr.onerror = function() {\n"
	"        updateNetwork(false);\n"
	"        netcount = 5;\n"
	"        reconnect();\n"
	"    };\n"
	"    xhr.send();\t\n"
	"}\n"
	"\n"
	"</script>\n"
	"</body>\n"
	"</html>";
}
