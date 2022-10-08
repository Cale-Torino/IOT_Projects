/*
DBG("             _ _ _ _ ||_ESP8266_FIREWORKS_ _ _ _ _ _ _");
DBG("             \\  _  _ ||  _  _ _ ___");
DBG("             / /__/ \\|| |_/");
DBG("            / __   / || -  _ ___");
DBG("           / /  / /  ||/ /");
DBG("   _ _ _ _/ /  /  \\_/||  \\_ ______");
DBG(" ___________\\___\\____||______________");
DBG("");
DBG(" ***********************************");
DBG(" ***   FIREWORKS APPLICATION     ***");
DBG(" ***********************************");
DBG(" ***********************************");
DBG(" *** @author: C.A Torino         ***");
DBG(" *** @date: 07 October 2022      ***");
DBG(" ***********************************");

Open browser, visit 192.168.4.1

For com port access denied

1. close serial monitor tab in IDE

https://arduino.stackexchange.com/questions/506/is-it-better-to-use-define-or-const-int-for-constants

https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/soft-access-point-class.html

*/

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

//vars
const char* ssid     = "FireworksAP";
const char* password = "123456789";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";
const char* PARAM_INPUT_3 = "volts";

int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 1000;

const uint8_t sensevoltage = A0;
float vcc_adj = 1.096;
float batt_warn_volts = 2.4;
int Firework1 = 4;
int Firework2 = 5;
int Firework3 = 13;
int Firework4 = 14;
int cutoffvoltage = 800;
int timer1;
int timer2;
int timer3;
int timer4;
int turnoff = 900000;// Roughly 7 seconds
float R1 = 120000.0;// R1 for voltage divider input from external 0-15V
float R2 =  20000.0;// R2 for voltage divider output to ADC
float vRef = 4.8;// 9V on Vcc goes through the regulator
float vTmp, vIn;
int value;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Debug setup
#define DEBUG_ON 1
#define DEBUG_OFF 0
byte debugMode = DEBUG_OFF;

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
  }, 8000);
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
    //if(analogRead(sensevoltage) > cutoffvoltage)
    //{ 
      String buttons = "";
      buttons += "<h4>Firework 1 - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(Firework3) + "><span class=\"slider\"></span></label>";
      buttons += "<h4>Firework 2 - GPIO 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\" " + outputState(Firework2) + "><span class=\"slider\"></span></label>";
      buttons += "<h4>Firework 3 - GPIO 13</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"13\" " + outputState(Firework1) + "><span class=\"slider\"></span></label>";
      buttons += "<h4>Firework 4 - GPIO 14</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"14\" " + outputState(Firework1) + "><span class=\"slider\"></span></label>";
      return buttons;
    //}
    //else
    //{
       //String buttons = "";
       //buttons += "<div class=\"alert\"><span></span><strong>Danger!</strong> The battery voltage is too low to ignite!.</div>";
       //digitalWrite(Firework4, LOW);
       //digitalWrite(Firework3, LOW);
       //digitalWrite(Firework2, LOW);
       //digitalWrite(Firework1, LOW);
       //return buttons;
    //}
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
  //loop here while no AP is connected to this station
  while(WiFi.softAPgetStationNum() == 0)
  {
    blink();
    delay(100);
  }
    digitalWrite(LED_BUILTIN, LOW);
}

void setup(){

  // Serial port for debugging purposes
  Serial.begin(115200);

  // Pins setup
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Firework4, OUTPUT);
  digitalWrite(Firework4, LOW);
  pinMode(Firework3, OUTPUT);
  digitalWrite(Firework3, LOW);
  pinMode(Firework2, OUTPUT);
  digitalWrite(Firework2, LOW);
  pinMode(Firework1, OUTPUT);
  digitalWrite(Firework1, LOW);
  pinMode(sensevoltage, INPUT);

  // Simple timers for mosfets to switch off automatically
  timer1=0;
  timer2=0;
  timer3=0;
  timer4=0;

  DBG("Setting AP (Access Point)… \n");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password, 6, false, 1);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Route for volts / web page
  server.on("/volts", HTTP_GET, [](AsyncWebServerRequest *request){
      //float 33vcc = ESP.getVcc();
      // if((float)ESP.getVcc()* vcc_adj < batt_warn_volts)
      // {

      // }
      value=analogRead(sensevoltage);
      vTmp = value * ( vRef / 1024.0 );
      vIn = vTmp / (R2/(R1+R2));
      String volts = String(vIn);
      DBG("Volts: ");
      DBG(volts);
      DBG("\n");
      request->send(200, "text/plain", volts);
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    DBG("GPIO: ");
    DBG(inputMessage1);
    DBG(" - Set to: ");
    DBG(inputMessage2);
    DBG("\n");
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();

  // Allow ADC to stabilize
  value=analogRead(sensevoltage); delay(50); value=analogRead(sensevoltage); delay(50);
  value=analogRead(sensevoltage); delay(50); value=analogRead(sensevoltage); delay(50);
  value=analogRead(sensevoltage); delay(50); value=analogRead(sensevoltage);

  waitforAP();
}

void loop(){
  // Loop and check mosfet timers
 if(digitalRead(Firework1)==HIGH)
  {
    timer1++;
  }else{timer1=0;}
  if(digitalRead(Firework2)==HIGH)
  {
    timer2++;
  }else{timer2=0;}
  if(digitalRead(Firework3)==HIGH)
  {
    timer3++;
  }else{timer3=0;}
  if(digitalRead(Firework4)==HIGH)
  {
    timer4++;
  }else{timer4=0;}
  if(timer1>turnoff)
  {
    digitalWrite(Firework1,LOW);
    timer1=0;
  }
  if(timer2>turnoff)
  {
    digitalWrite(Firework2,LOW);
    timer2=0;
  }  
  if(timer3>turnoff)
  {
    digitalWrite(Firework3,LOW);
    timer3=0;
  }
    if(timer4>turnoff)
  {
    digitalWrite(Firework4,LOW);
    timer4=0;
  }
    if(!WiFi.softAPgetStationNum())
  {
    waitforAP();
  }
}