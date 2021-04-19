#include <WiFi.h>

WiFiClient client;

#define LED 2
//setting jaringan

String request_string;
const char* host = "192.168.0.115"; //alamat IP server 

//milis sebagai pengganti delay
unsigned long previousMillis = 0;
const long interval = 1000;

//sediakan variable untuk indikator koneksi
//#define PIN_LED 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.disconnect();
  WiFi.begin("Muchlas", "21041974");
  
  // cek koneksi wifi
  while((!(WiFi.status() == WL_CONNECTED)))
  {
    digitalWrite(LED, HIGH);
  }
}

void loop() {
  unsigned long currentMillis = millis(); //baca waktu milis saat ini
  if (currentMillis - previousMillis >= interval)
  {
    String denyut ="";
    //update millis
    previousMillis = currentMillis;
    // put your main code here, to run repeatedly:
    // baca data dari NodeMCU
    while (Serial.available()>0) 
    {
     denyut += char(Serial.read());
    }
  
    // tampilkan diserial monitori
    if (client.connect(host,80))
    {
      request_string = "/websiteMonitoringTatalaksanaDehidrasi/ceksensor/kirimdata.php?cekdenyuts=";
      request_string += denyut;
      //proses pengiriman data ke server 
      client.print(String("GET ") + request_string + "HTTP/1.1\r\n" + "Host: 192.168.0.115\r\n" + "Connection: close\r\n\r\n");
      denyut.trim();
      Serial.println(denyut);
      while (client.available() == 0) 
      {
      if (millis() - currentMillis  > 1000) 
      {
       client.stop();
       return;
      } 
      }
  }
}
}
