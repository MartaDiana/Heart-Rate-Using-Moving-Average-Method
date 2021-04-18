//milis sebagai pengganti delay
unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis(); //baca waktu milis saat ini
  if (currentMillis - previousMillis >= interval)
  {
    //update millis
    previousMillis = currentMillis;
    // put your main code here, to run repeatedly:
    // baca data dari NodeMCU
    String denyut ="";
    while (Serial.available()>0) 
    {
     denyut += char(Serial.read());
    }

  // tampilkan diserial monitori
  denyut.trim();
  Serial.println(denyut);
  }
} 
