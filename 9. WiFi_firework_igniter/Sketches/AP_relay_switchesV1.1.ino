/*
C.A Torino
Open browser, visit 192.168.4.1
TODO: Add timer to reset buttons when turnoff is triggered
*/
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid     = "FireworksAP";
const char* password = "123456789";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

int sensevoltage = A0;
int cutoffvoltage = 800;
int timer1;
int timer2;
int timer3;
int turnoff=900000;//Roughly 7 seconds

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

unsigned long previousMillis = 0;

const long interval = 30000;//10 seconds

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Fireworks Trigger Dash</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .alert {padding: 20px;background-color: #b30000;color: white;margin-bottom: 15px;}
  </style>
</head>
<body>
  <h2>Fireworks Trigger Dash</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    //if(analogRead(sensevoltage) > cutoffvoltage)
    //{ 
      String buttons = "";
      buttons += "<h4>Firework 1 - GPIO 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\" " + outputState(5) + "><span class=\"slider\"></span></label>";
      buttons += "<h4>Firework 2 - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
      buttons += "<h4>Firework 3 - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
      return buttons;
    //}
    //else
    //{
       //String buttons = "";
       //buttons += "<div class=\"alert\"><span></span><strong>Danger!</strong> The battery voltage is too low to ignite!.</div>";
       //digitalWrite(5, LOW);
       //digitalWrite(4, LOW);
       //digitalWrite(2, LOW);
       //return buttons;
    //}

  }
  return String();
}

String processor2(const String& var){
  if(var == "BUTTONPLACEHOLDER"){
      String buttons = "";
      if(digitalRead(2)==HIGH)
      {
       buttons += "<h4>Firework 3 - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" ><span class=\"slider\"></span></label>";
      }
      else
      {
        buttons += "<h4>Firework 3 - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" checked><span class=\"slider\"></span></label>";
      }
        if(digitalRead(4)==HIGH)
      {
       buttons += "<h4>Firework 2 - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" ><span class=\"slider\"></span></label>";
      }
      else
      {
        buttons += "<h4>Firework 2 - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" checked><span class=\"slider\"></span></label>";
      }
        if(digitalRead(5)==HIGH)
      {
        buttons += "<h4>Firework 1 - GPIO 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\"><span class=\"slider\"></span></label>";
      }
      else
      {
        buttons += "<h4>Firework 1 - GPIO 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\" checked><span class=\"slider\"></span></label>";
      }
      return buttons;
  }
  return String();
}

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(sensevoltage, INPUT);

  timer1=0;
  timer2=0;
  timer3=0;

  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
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
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();

}
 
void loop(){
 if(digitalRead(2)==HIGH)
  {
    timer1++;
  }
  if(digitalRead(4)==HIGH)
  {
    timer2++;
  }
  if(digitalRead(5)==HIGH)
  {
    timer3++;
  }
  if(timer1>turnoff)
  {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor2);
    });
    digitalWrite(2,LOW);
    timer1=0;
  }
  if(timer2>turnoff)
  {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor2);
    });
    digitalWrite(4,LOW);
    timer2=0;
  }  
  if(timer3>turnoff)
  {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor2);
    });
    digitalWrite(5,LOW);
    timer3=0;
  }
}