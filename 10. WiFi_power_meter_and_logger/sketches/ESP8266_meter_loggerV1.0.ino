/*
DBG("             _ _ _ _ ||_ESP8266_METER_ _ _ _ _ _ _");
DBG("             \\  _  _ ||  _  _ _ ___");
DBG("             / /__/ \\|| |_/");
DBG("            / __   / || -  _ ___");
DBG("           / /  / /  ||/ /");
DBG("   _ _ _ _/ /  /  \\_/||  \\_ ______");
DBG(" ___________\\___\\____||______________");
DBG("");
DBG(" ***********************************");
DBG(" ***      METER APPLICATION      ***");
DBG(" ***********************************");
DBG(" ***********************************");
DBG(" *** @author: C.A Torino         ***");
DBG(" *** @date: 07 November 2022     ***");
DBG(" ***********************************");

Open browser, visit 192.168.4.1

install SSD1306Ascii lib

https://github.com/gilleshenrard/DataLogger/blob/master/DataLogger.ino

*/

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
//#include <SPI.h>
#include <Adafruit_SSD1306.h>
//#include <SSD1306AsciiAvrI2c.h>
//#include <SdFat.h>

//declare timer trigger flag and counter value
bool triggered = false;

#define VERSION "1.3"

//vars
const char* ssid     = "MeterLoggerAP";
const char* password = "123456789";

int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 1000;

int Firework1 = 4;
int Firework2 = 5;
int Firework3 = 12;
int Firework4 = 13;

//declare SSD1306 OLED display variables
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
//SSD1306AsciiAvrI2c display;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//declare INA219 variables
Adafruit_INA219 ina219;
float current_mA = 0.0, oldcurr = 0.0;
float loadvoltage = 0.0, oldvolt = 0.0;
float power_mW = 0.0, oldpow = 0.0;
float energy_mWh = 0.0, oldegy = 0.0;
unsigned long elapsed = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Debug setup
#define DEBUG_ON 1
#define DEBUG_OFF 0
byte debugMode = DEBUG_ON;

#define DBG(...) debugMode == DEBUG_ON ? Serial.print(__VA_ARGS__) : NULL

// HTML + Javascript code
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Fireworks Dash</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 1.5rem;}
    p {font-size: 1.5rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .alert {padding: 20px;background-color: #b30000;color: white;margin-bottom: 15px;}
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 800px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); }
    .reading { font-size: 1.4rem; }
  </style>
</head>
<body style="background-color:#aeb6b8;">
  <h2>Fireworks Dash</h2>
  %BUTTONPLACEHOLDER%
    <div class="content">
    <div class="cards">
      <div class="card">
        <p>VOLTAGE</p><p><span class="reading"><span id="volts">0</span> V</span></p>
      </div>
    </div>
  </div>
<script>
var Timeout;
function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
  //clearTimeout(Timeout);
  Timeout = setTimeout(() => {
    if(document.getElementById(element.id).checked = false){document.getElementById(element.id).setAttribute("checked", "checked");}
  }, 16000);
}
function updateVolts() {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (xhr.readyState == XMLHttpRequest.DONE) {
        document.getElementById("volts").innerHTML = xhr.responseText;
    }
  }
  xhr.open("GET", "/volts", true);
  xhr.send();
}
var Interval = setInterval(updateVolts, 3000);
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){

  if(var == "BUTTONPLACEHOLDER"){
      String buttons = "";
      buttons += "<h4>Firework 1 - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(Firework1) + "><span class=\"slider\"></span></label>";
      buttons += "<h4>Firework 2 - GPIO 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\" " + outputState(Firework2) + "><span class=\"slider\"></span></label>";
      buttons += "<h4>Firework 3 - GPIO 12</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"12\" " + outputState(Firework3) + "><span class=\"slider\"></span></label>";
      buttons += "<h4>Firework 4 - GPIO 13</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"13\" " + outputState(Firework4) + "><span class=\"slider\"></span></label>";
      return buttons;
  }
  return String();
}

//Returns checked state
String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

/******************************************************************************/
/*  I : /                                                                     */
/*  P : setup procedure                                                       */
/*  O : /                                                                     */
/******************************************************************************/
void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  DBG("Setting AP (Access Point)… \n");
  // Remove the password parameter, if you want the AP (Access Point) to be open;
    if (!WiFi.softAP(ssid, password, 6, false, 1)) {
    Serial.println("WiFi.softAP initialization failed!");
    //return;
  }

    // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  DBG("ina219.begin \n");
  //setup the INA219
    if (!ina219.begin()) {
    Serial.println("ina219.begin() initialization failed!");
    //return;
  }

  DBG("display.begin \n");
  //setup the display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display initialization failed!");
    //return;
  }

  // Start server
  DBG("server.begin \n");
  server.begin();

  DBG("waitforAP.begin \n");
  waitforAP();
}

/******************************************************************************/
/*  I : /                                                                     */
/*  P : main program loop                                                     */
/*  O : /                                                                     */
/******************************************************************************/
void loop() {
  //if timer has been reached
  if (triggered)
  {
    //get the values measured by the INA219
    ina219values();
	
	//
	//	Display update procedure in main loop to avoid
	//		wasting clock time in function call
	//
    //update the voltage line on the SSD1306 display
	if(loadvoltage != oldvolt){
		displayline(loadvoltage, 0, " V");
		oldvolt = loadvoltage;
	}
	
    //update the current line on the SSD1306 display
	if(current_mA != oldcurr){
		displayline(current_mA, 2, " mA");
		oldcurr = current_mA;
	}
	
    //update the power line on the SSD1306 display
	if(power_mW != oldpow){
		displayline(power_mW, 4, " mW");
		oldpow = power_mW;
	}
	
    //update the energy line on the SSD1306 display
	if(energy_mWh != oldegy){
		displayline(energy_mWh, 6, " mWh");
		oldegy = energy_mWh;
	}

    //reset the flag
    triggered = false;
  }
}

void blink()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    if (ledState == LOW) 
    {
      ledState = HIGH;
    } 
    else 
    {
      ledState = LOW;
    }   
  }
    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);
 }

 void waitforAP()
{
  DBG("waitforAP\n");
  //loop here while no AP is connected to this station
  while(WiFi.softAPgetStationNum() == 0)
  {
    blink();
    delay(100);
  }
    digitalWrite(LED_BUILTIN, LOW);
}

/****************************************************************************/
/*  I : timer1 comparator vector                                            */
/*  P : set the flag indicating timer has been reached + increment time var.*/
/*  O : /                                                                   */
/****************************************************************************/
//void ISR(TIMER1_COMPA_vect){
  //triggered = true;
//}
void Timer()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    triggered = true; 
  }
    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);
 }

/****************************************************************************/
/*  I : Value measured to display                                           */
/*		Buffer holding the last saved measurment							*/
/*		Line number at which display the value								*/
/*		End of line (unit) to append to the line							*/
/*  P : Format and display a measurment at the right line, only if changed	*/
/*  O : /                                                                   */
/****************************************************************************/
void displayline(const float measurment, const uint8_t line_num, const char line_end[]) {
  char floatbuf[16]={0};
  
  //format the line ([-]xxxxx.xxx [unit])
  dtostrf(measurment, 10, 3, floatbuf);
  strcat(floatbuf, line_end);
  
  //place the cursor and write the line
  display.setCursor(0, line_num);
  display.print(floatbuf);
}

/******************************************************************************/
/*  I : /                                                                     */
/*  P : get the values from the INA219 via I²C (takes 11ms)                   */
/*  O : /                                                                     */
/******************************************************************************/
void ina219values() {
  float shuntvoltage = 0.0;
  float busvoltage = 0.0;
  
  //turn the INA219 on
  ina219.powerSave(false);
  
  //get the shunt voltage, bus voltage, current and power consumed from the INA219
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  elapsed = millis();

  //turn the INA219 off
  ina219.powerSave(true);

  //compute the load voltage
  loadvoltage = busvoltage + (shuntvoltage / 1000.0);

  //compute the power consumed
  power_mW = loadvoltage*current_mA;
  
  //compute the energy consumed (t = elapsed[ms] / 3600[s/h] * 1000[ms/s])
  energy_mWh += power_mW * ( elapsed / 3600000.0);
}
