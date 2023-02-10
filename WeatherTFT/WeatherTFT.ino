/*
This code is an Arduino sketch that connects to a WiFi network,
retrieves the current weather information for a specific location (Somerset West) from the OpenWeatherMap API, 
and displays the information on an Adafruit ST7735 TFT display. The WiFi network details (SSID and password) are specified in 
the wifiMulti.addAP method, and the API key for the OpenWeatherMap API is specified in the appID constant. The code uses the WiFi and 
HTTPClient libraries to establish the connection to the WiFi network and retrieve the weather data from the API. The data is then parsed 
using the ArduinoJSON library and displayed on the TFT display. The code runs in a loop that continuously retrieves and displays the current weather 
information every 30 seconds.
 */
#include "WiFi.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiMulti.h>
#define TFT_CS 14
#define TFT_RST 27
#define TFT_DC 16
#define TFT_SDI 17
#define TFT_SCK 25
#define TFT_RED 0xF800

const char *host = "api.openweathermap.org";
const char *appID = "b045804ab93431828b3e101e2be26dc1";
const char *city = "Somerset%20West";

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_SDI, TFT_SCK, TFT_RST);
WiFiMulti wifiMulti;



void setup()
{
    Serial.begin(115200);

    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST7735_BLACK); // fill the screen with black
    tft.setCursor(0, 0);

    wifiMulti.addAP("ADG_FIBRE_CPT", "#ADG@2022");

    tft.println("Connecting Wifi...");
    if(wifiMulti.run() == WL_CONNECTED) {
        tft.println("");
        tft.println("WiFi connected");
        tft.println("IP address: ");
        tft.println(WiFi.localIP());
    }

    
}

void loop()
{
    tft.setCursor(0, 0);
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST7735_BLACK);      
    
    if(wifiMulti.run() != WL_CONNECTED) {        
        tft.println("WiFi not connected!");        
        delay(10000);
    }
    else{
        HTTPClient http;
        String url = String("http://") + host + "/data/2.5/weather?q=" + city + "&appid=" + appID;
        http.begin(url);
        int httpCode = http.GET();
        if (httpCode > 0)
        {
            String payload = http.getString();
            StaticJsonDocument<512> doc;
            DeserializationError error = deserializeJson(doc, payload);
            tft.setTextSize(2);
            tft.setTextColor(ST7735_YELLOW);        
            tft.println("Weather in Somerset West");        
            tft.setTextSize(1);
            tft.setTextColor(TFT_RED);        
            tft.println("Temperature: " + String(doc["main"]["temp"].as<float>() - 273.15) + "C");        
            tft.setTextSize(1);
            tft.setTextColor(ST7735_WHITE);
            tft.println("Humidity: " + String(doc["main"]["humidity"].as<float>()) + "%");        
            tft.setTextSize(1);
            tft.setTextColor(ST7735_BLUE);
            tft.println("Pressure: " + String(doc["main"]["pressure"].as<float>()) + "hPa");        
            tft.setTextSize(1);
            tft.setTextColor(ST7735_GREEN);
            tft.println("Wind Speed: " + String(doc["wind"]["speed"].as<float>()) + "m/s");
            tft.setTextSize(1);
            tft.setTextColor(ST7735_WHITE);
            tft.println(String(doc["weather"][0]["main"].as<String>()));
            
        }
        http.end();
        delay(30000);     
    }    
}
