 /*
 * Wemos_D1_Mini_Wifi_Multi-Sensor_Sketch_v1.3.0.cpp
 *
 * TECHRAD
 *
 * @author     C.A Torino
 * @version    V1.3.0
 * @since      2020/10/02
 */
//C.A Torino ESP8266 WeMos D1 Mini Firmware v1.3.0
//https://hackingmajenkoblog.wordpress.com/2016/02/04/the-evils-of-arduino-strings/
//https://forum.arduino.cc/index.php?topic=638327.0
//static const char theme[] = F("Number of hours");
//If you have free FLASH memory space, you can easily indicate that the string must be saved in FLASH using the syntax:
//fix strings in loop problem
//https://cpp4arduino.com/2018/11/21/eight-tips-to-use-the-string-class-efficiently.html
////changing variable
//char string[30] = "This is a string";
//strcpy(string, "New content");
//
//
////adding 2 together
//char hi[7] = "Hello ";
//char name[5] = "Fred";
//char all[14] = "";
//strcat(all, hi);
//strcat(all, name);
//https://www.techcoil.com/blog/how-to-post-json-data-to-a-http-server-endpoint-from-your-esp32-development-board-with-arduinojson/
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

#define VERSION_ID "Wemos Multi-Sensor 2020-10-02 v1.3.0"

Adafruit_INA219 ina219;

float voltage_V = 0,shuntVoltage_mV,busVoltage_V;
float current_mA = 0;
float power_mW = 0;
float energy_Wh=0;
long time_s=0;

static const char ssid[] = "ssid";
static const char password[] = "password";
static const char host[] = "host.address.com";
const int RSSI_MAX =-50;
const int RSSI_MIN =-100;
const int displayEnc=1;
const long utcOffsetInSeconds = 7200;

const int ledPin =  LED_BUILTIN;
int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 1000;

String _wifiResult;
String _timeResult;
float _voltsResult;
float _Bus_Voltage;
float _Shunt_Voltage;
float _Load_Volts;
float _Current;
float _Power;
float _Energy;
float _TEMPResult;
int _LDRResult;
unsigned long currentMillis;
String ipadd;
String _url((char *)0);
String type;
String _scan((char *)0);
int n;
unsigned long epochTime;

int monthDay;
int currentMonth;
int currentYear;
String _date((char *)0);
String _time;
String encR;

//float correctionfactor;
//float vout; 
//float vin;
//float R1;
//float R2;
//int value;

/* Photocell reading program */
//const int lightPin = D0; // Pin connected to sensor
//int light; // Analog value from the sensor

/* Temp */
#define ONE_WIRE_BUS D3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float Celcius=0;
//float Fahrenheit=0;

WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup()
{
  uint32_t currentFrequency;
  ina219.begin();
  sensors.begin();  // Start up the library
  pinMode(D6, OUTPUT);
  digitalWrite(D6, HIGH);
  //pinMode(A0, INPUT);
  pinMode(ledPin, OUTPUT);
  WiFi.begin(ssid,password);
  Serial.begin(115200);
  delay(1000);
  digitalWrite(D6, LOW);
  Serial.println(VERSION_ID);
  Serial.flush();
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.flush();
  WiFi.mode(WIFI_STA);
  timeClient.begin();
}

void loop()
{
    _url = "";
    currentMillis = millis();
    //Led_fun(currentMillis);
    ipadd = WiFi.localIP().toString().c_str();
    _wifiResult = Wifi_Scan();
    _timeResult = Time_fun();
    //_voltsResult = Volts_fun();
    _Bus_Voltage = Bus_Voltage_fun();
    _Shunt_Voltage = Shunt_Voltage_fun();
    _Load_Volts = Load_Volts_fun();
    _Current = Current_fun();
    _Power = Power_fun();
    _Energy = Energy_fun();
    _LDRResult = LDR_fun();
    _TEMPResult = TEMP_fun();
    if(client.connect(host,80)) {
    _url.reserve(255);
    _url += "/api/in_a6.php?API_KEY=asdflrtjT&USERNAME=a6in&DATA=";
    _url += "{\"Result\":\"0\",\"UID\":\"450b87a4cc154179a5e1d6e3b1e31ec5\",\"Device_Modal\":\"ESP8266_WEMOS_D1_MINI\",\"Device_Type\":\"WEMOS_D1_MINI\",\"Device_Catagory\":\"WeMos_WiFi_Dev_Board\",\"Time\":\"";
    _url += _timeResult;
    _url += "\",\"Temp\":\"";
    _url += _TEMPResult;
    _url += "\",\"LDR\":\"";
    _url += _LDRResult;
    //_url += "\",\"Volts\":\"";
    //_url += _voltsResult;
    _url += "\",\"Bus_Voltage\":\"";
    _url += _Bus_Voltage;
    _url += "\",\"Shunt_Voltage\":\"";
    _url += _Shunt_Voltage;
    _url += "\",\"Load_Volts\":\"";
    _url += _Load_Volts;
    _url += "\",\"Current\":\"";
    _url += _Current;
    _url += "\",\"Power\":\"";
    _url += _Power;
    _url += "\",\"Energy\":\"";
    _url += _Energy;
    _url += "\",\"WiFi_Stations\":[";
    _url += _wifiResult;
    _url += "],\"Mac_Address\":\"";
    _url += ipadd;
    _url += "\",\"On_Time\":\"";
    _url += currentMillis;
    _url += "\"}";
    client.print(String("GET ")+_url+" HTTP/1.1\r\n"+"Host: "+host+"\r\n"+"Connection: keep-alive\r\n\r\n");
    Serial.println(">>> Response: ");
//    while (client.connected())
//    {
//      if (client.available())
//      {
//        String line = client.readStringUntil('\n');
//        Serial.println(line);
//      }
//    }
//    client.stop();
//    Serial.println("\n[Disconnected]");
//  }
//  else
//  {
//    Serial.println("connection failed!]");
//    client.stop();
//  }
    Serial.flush();
    client.stop();
  }
}

String encType(int id){
  if(WiFi.encryptionType(id) == ENC_TYPE_WEP){
    type="WEP";
  }else if(WiFi.encryptionType(id) == ENC_TYPE_TKIP){
    type="WPA-PSK";    
  }else if(WiFi.encryptionType(id) == ENC_TYPE_CCMP){
    type="WPA2-PSK";    
  }else if(WiFi.encryptionType(id) == ENC_TYPE_AUTO){
    type="WPA-WPA2-PSK";    
  }else if(WiFi.encryptionType(id) == ENC_TYPE_NONE){
    type="-OPEN-";    
  }
  return type;
}


int dBmtoPercentage(int dBm)
{
  int quality;
    if(dBm <= RSSI_MIN)
    {
        quality = 0;
    }
    else if(dBm >= RSSI_MAX)
    {  
        quality = 100;
    }
    else
    {
        quality = 2 * (dBm + 100);
    }
     return quality;
}

String Wifi_Scan()
{
  _scan = "";
  n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println(">>> no networks found");
    Serial.flush();
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    Serial.flush();

    for (int i = 0; i < n; ++i) {
     if(displayEnc)
     {
      encR = encType(i);
     }
     else
     {
      encR = "none";
     }
      _scan.reserve(255);
      _scan += "{\"SSID\":\"";
      _scan += WiFi.SSID(i);
      _scan += "\",\"CH\":\"";
      _scan += WiFi.channel(i);
      _scan += "\",\"ENC\":\"";
      _scan += encR;
      _scan += "\",\"RSSI\":\"";
      _scan += WiFi.RSSI(i);
      _scan += "\",\"PER\":\"";
      _scan += dBmtoPercentage(WiFi.RSSI(i));
      _scan += "\",\"MAC\":\"";
      _scan += WiFi.BSSIDstr(i);
      _scan += "\"},";
    }
  }
  WiFi.scanDelete();
  _scan = _scan.substring(0, _scan.length() - 1);
  return _scan;
}

String Time_fun()
{
  _date = "";
  timeClient.update();
  epochTime = timeClient.getEpochTime(); 
  tm *ptm = gmtime ((time_t *)&epochTime); 
  monthDay = ptm->tm_mday;
  currentMonth = ptm->tm_mon+1;
  currentYear = ptm->tm_year+1900;
  _time = timeClient.getFormattedTime(); 
  _date.reserve(64);
  _date += currentYear;
  _date += "-";
  _date += currentMonth;
  _date += "-";
  _date += monthDay;
  _date += "-";
  _date += _time; 
  return _date;
}

//float Volts_fun()
//{
//   correctionfactor = 0.400;
//   vout = 0.0; 
//   vin = 0.0;
//   R1 = 30000;
//   R2 = 7500;
//   value = 0;
//   value = analogRead(A0);
//   vout = (value * 3.3) / 1023.0;
//   vin = vout / (R2/(R1+R2));
//   vin = vin - correctionfactor;
//   return vin;
//}

float Bus_Voltage_fun()
{
  busVoltage_V = ina219.getBusVoltage_V();
  return busVoltage_V;
}
float Shunt_Voltage_fun()
{
  shuntVoltage_mV = ina219.getShuntVoltage_mV();
  return shuntVoltage_mV;
}
float Load_Volts_fun()
{
  voltage_V = busVoltage_V + (shuntVoltage_mV / 1000);
  return voltage_V;
}
float Current_fun()
{
  current_mA = ina219.getCurrent_mA();
  return current_mA;
}
float Power_fun()
{
  power_mW = ina219.getPower_mW();
  return power_mW;
}
float Energy_fun()
{
  time_s=millis()/(1000); // convert time to sec
  power_mW = ina219.getPower_mW();
  energy_Wh=(power_mW*time_s)/3600;
  return energy_Wh;
}

int LDR_fun()
{
   //light = analogRead(lightPin);
   //light = digitalRead(lightPin);
   //return light;
//    pinMode(lightPin, OUTPUT);
//    digitalWrite(lightPin, LOW);
//    pinMode(lightPin, INPUT);
    return analogRead(A0);
}

float TEMP_fun()
{ 
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
    delay(1000);
  return Celcius;
  //Fahrenheit=sensors.toFahrenheit(Celcius);
  //Serial.print(" C  ");
  //Serial.print(Celcius);
  //Serial.print(" F  ");
  //Serial.println(Fahrenheit);
}

void Led_fun(unsigned long currentMillis)
{
    if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
}