/*************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need Adafruit DHT sensor libraries:
    https://github.com/adafruit/Adafruit_Sensor
    https://github.com/adafruit/DHT-sensor-library

  App project setup:
    Value Display widget attached to V5
    Value Display widget attached to V6
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLoa2DV9Sc"
#define BLYNK_DEVICE_NAME "Uas iot fix"
#define BLYNK_AUTH_TOKEN "D1B-99nORWKmiII82TGd0XNRxS3fr2XV"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "kevin";
char pass[] = "kevinalvaro";

#define DHTPIN 4          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

int LDR_pin = A0; //unuk pinnya
int LDR ; // nilai ldr setelah rumus
int LED_lux = D13; // lampu untuk lux hardware
int LED_temp = D11; // lampu untuk suhu hardware
int slider_temp = 40; // nilai slider dari app
int slider_lux = 15;//nilai slider dari app
int active = 1; //active =1 no = 0 // check active button on apa engga
int x; //untuk rumus
int y;// untuk rumus

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.


}
BLYNK_WRITE(V4)//baca green time slider
{
  slider_lux = param.asInt(); // assigning incoming value from pin V1 to a variable
}
BLYNK_WRITE(V5)//baca green time slider
{
  slider_temp = param.asInt(); // assigning incoming value from pin V1 to a variable
}
BLYNK_WRITE(V8)//baca green time slider
{
  active = param.asInt(); // assigning incoming value from pin V1 to a variable
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  pinMode(LDR,INPUT);
  pinMode(LED_lux,OUTPUT);
  pinMode(LED_temp,OUTPUT);
  
}
void checksuhu(){
  float t = dht.readTemperature();
  Blynk.virtualWrite(V3, t);
  Blynk.virtualWrite(V1, t);
  Serial.println(t);
  if (t >= slider_temp){
    Blynk.virtualWrite(V7, 1);
    digitalWrite(LED_temp,HIGH);
  }else{
    Blynk.virtualWrite(V7, 0);
    digitalWrite(LED_temp,LOW);
  }
  
}

void checkcahaya(){
  int x =analogRead(LDR_pin);
  int LDR = ((0.009768*x) + 10);
    

  Blynk.virtualWrite(V2, LDR);
  if (LDR >= slider_lux){
    Blynk.virtualWrite(V6, 1);
    digitalWrite(LED_lux,HIGH);

  }else{
    Blynk.virtualWrite(V6, 0);
    digitalWrite(LED_lux,LOW);
  }
}

void loop()
{
  Blynk.run();
  timer.run();
  if(active == 1){
    checksuhu();
    checkcahaya();   
  }

}
