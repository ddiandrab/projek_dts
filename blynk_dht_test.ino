#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <DHT.h>      // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT22   // DHT 22
#define dht_dpin 5
DHT dht(dht_dpin, DHTTYPE);    

const int AOUTpin=4;  
const int DOUTpin=18;
int value;

char auth[] = "kEJ1mtKZHKFW465O3S9e5HqbObYMCVHH";
char ssid[] = "aff";
char pass[] = "affrielya";
int flag=0;

BlynkTimer timer;

void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  //Blynk.virtualWrite(V5, millis() / 1000);
  
  dht.begin();
  float h = dht.readHumidity();
  float t = dht.readTemperature();  
  float g = 20;
  Serial.println(h);
  Serial.println(t);
  delay(100);

  Blynk.virtualWrite(V4, t);
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, g);
  delay(100);

  value = analogRead(AOUTpin);
  delay(100);
  
  if (t>25 && h>50 && g>25 && flag==0) {
    Serial.println("Max safe temp and humid");
    Blynk.notify("Alert : Reach maximun safe temp and humid");
    Blynk.email("desy.bestari@gmail.com", "Subject: Monitoring Server", "Already reach maximun safe temp and humid");
    flag=1;
    delay(15000);
  }
  //else if (isButtonPressed==0)
  else
  {
    flag=0;
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  pinMode(DOUTpin, INPUT);  
  Blynk.begin(auth, ssid, pass);
  
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);

  // Setup a function to be called every second
  pinMode(dht_dpin,INPUT_PULLUP);
  timer.setInterval(5000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
