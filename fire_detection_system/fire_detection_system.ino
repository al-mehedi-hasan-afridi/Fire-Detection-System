
// DS18B20 temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//Flame Sensor and Buzzer
const int flame = D0; 
const int buzz = D1;

//firebase
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
//Provide the token generation process info.
#include <addons/TokenHelper.h>
//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
/* 1. Define the WiFi credentials */
#define WIFI_SSID "iphone"
#define WIFI_PASSWORD "12345678"
// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino
/* 2. Define the API Key */
#define API_KEY "AIzaSyC82REO7UVbySbCnUnZ6x6jj7BXjb9uX_E"
/* 3. Define the RTDB URL */
#define DATABASE_URL "https://fire-detection-system-14d92-default-rtdb.firebaseio.com"  



//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {

  pinMode(flame,INPUT);
  pinMode(buzz,OUTPUT);

  // protocol begin
  Serial.begin(9600);

  // Wifi connection Setup
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("Wifi Connected");
  Serial.println();


  /* Assign the api key (required) */
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.setDoubleDigits(5);
  if (!Firebase.ready()) {
    Serial.println("Firebase Connection fail !");
    Serial.println("");
  }
}

void loop() {

  //Flame and Buzzer
  int t = digitalRead(flame);
  String y = "Yes";
  String n = "No";
   if (t==0) {         // t=0; there is fire  
  digitalWrite(buzz,HIGH);
  Serial.print("Fire Dectected: ");
  Serial.print(y); 
  Firebase.setString(fbdo, "/data/box2", y);
  }else{             // t=1; there is no fire  
  digitalWrite(buzz,LOW);
  Serial.print("Fire Dectected: ");
  Serial.print(n); 
  Firebase.setString(fbdo, "/data/box2", n);
  }

  // Temperature 
  sensors.requestTemperatures();
  int tempC = sensors.getTempCByIndex(0);
  Serial.println();
  Serial.print("Temperature: "); Serial.print(tempC); Serial.println(" °C");
  Firebase.setInt(fbdo, "/data/box1", tempC);

}
