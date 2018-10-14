int optimumsicaklik=33; /// ayarlanan sıcaklık

#include "DHT.h"
#define DHT1PIN 8     // what pin we're connected to
#define DHT2PIN 11
#define DHT1TYPE DHT11   // DHT 11 ^
#define role 9
#define role1 10
DHT dht1(DHT1PIN, DHT1TYPE);
DHT dht2(DHT2PIN, DHT1TYPE);

#include <LiquidCrystal.h>
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

int mode = 0;
int mode1 = 0;
int mode2 = 1;
int mode3 = 0;
int counter = 0;
int counter1 = 0;
int counter2 = 0;
int optimumSicaklikAnahtari = 0;
int saatAyarlamaAnahtari = 0;
int uyanmaUyumaAnahtari = 0;
int uyanmaSaati = 7;
int uyumaSaati = 23;
int uyanmaDakika = 0;
int uyumaDakika = 0;
int sayac = 0;
void setup() {
  lcd.begin(16, 2);
  Wire.begin();
  RTC.begin();
  Serial.begin(9600);
  pinMode(role, OUTPUT); // Röleye giden pin
  pinMode(role1, OUTPUT); // Röleye giden pin
  pinMode(A0, INPUT); //sıcaklık ayarlama potunun pini
  pinMode(A1, INPUT); //sıcaklık ayarlamak için kullanılan anahtarın pini
  pinMode(A2, INPUT); // saat için kullanılan pot
  pinMode(A3, INPUT); // dakika potu
  pinMode(A6, INPUT); // saat için kullanılan anahtar
  pinMode(A7, INPUT);  // dakika kullanilan anahtar
  pinMode(13, OUTPUT);
  dht1.begin();
//  dht2.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {
   float nem = dht1.readHumidity(); //semsörden alınan nem bilgisi
   float sicaklik = dht1.readTemperature(); // sensörden alınan sıcaklık bilgisi

  
  DateTime now = RTC.now();
  
  if ((now.hour() == uyanmaSaati) && (now.minute() == uyanmaDakika)) mode2 = 1;
  if ((now.hour() == uyumaSaati) && (now.minute() == uyumaDakika)) mode2 = 0;

  saatAyarlamaAnahtari = analogRead(A6);
  uyanmaUyumaAnahtari = analogRead(A7);
  
  if(saatAyarlamaAnahtari > 900){
    
    analogWrite(role, 1023);
    analogWrite(role1, 1023);
      if(uyanmaUyumaAnahtari > 900){
         digitalWrite(13, HIGH);
          uyanmaSaati = map(analogRead(A2),0,1023,0,24);
          uyanmaDakika = map(analogRead(A3),0,1023,0,60);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Uyanma Saati");
          lcd.setCursor(5,1);
          lcd.print(uyanmaSaati);
          lcd.print(":");
          lcd.print(uyanmaDakika);
          delay(100);
          digitalWrite(13, LOW);

      }
      else if(uyanmaUyumaAnahtari < 200){
          digitalWrite(13, HIGH);
          uyumaSaati = map(analogRead(A2),0,1023,23,-1);
          uyumaDakika = map(analogRead(A3),0,1023,0,60);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Uyuma Saati");
          lcd.setCursor(5,1);
          lcd.print(uyumaSaati); 
          lcd.print(":");
          lcd.print(uyumaDakika);
          delay(100);
          digitalWrite(13, LOW);

      }
      }
  else{
  if (mode2 == 1) 
  {

    ///////////////
      analogWrite(role1, 0);
    //  float h2 = dht2.readHumidity();
    //  float t2 = dht2.readTemperature();
    
      optimumSicaklikAnahtari = analogRead(A1);
      if(optimumSicaklikAnahtari > 500){  //anahtarın açık olduğundan emin olmak için kullandım
        digitalWrite(13, HIGH);
        optimumsicaklik = map(analogRead(A0), 0, 1023, 10, 60);  // map fonksiyonu değer aralığını istediğimiz aralığa getirir
        mode1 = 10;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Optimum Sicaklik");
        lcd.setCursor(7,1);
        lcd.print(optimumsicaklik);
        digitalWrite(13, LOW);

      }
      else { 
        mode1 = 0;}
      if(mode1 == 0){
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("!!!!HEILL!!!");
        lcd.setCursor(1,1);
        lcd.print("!!!RALAZABA!!!");
        mode1 = 1;
        delay(1000);
      }
      if(mode1 == 1){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sicaklik: ");
        lcd.print(sicaklik);
        lcd.setCursor(0, 1);
        lcd.print("Nem: ");
        lcd.print(nem);
        mode1 = 2;
        delay(2000); 
      }
      if(mode1 == 2){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Optimum Sicaklik: ");
          lcd.setCursor(7,1);
          lcd.print(optimumsicaklik);
          mode1 = 3;
          delay(1500);
        }
      if(mode1 == 3){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Acma: ");
        lcd.print(uyanmaSaati);
        lcd.print(":");
        lcd.print(uyanmaDakika);
        lcd.setCursor(0,1);
        lcd.print("Kapanma:");
        lcd.print(uyumaSaati);
        lcd.print(":");
        lcd.print(uyumaDakika); 
        mode1 = 4;       
        delay(2000);  
      }
      if(mode1 == 4){
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("Saat ");
        lcd.setCursor(5,1);
        lcd.print(now.hour());
        lcd.print(":");
        lcd.print(now.minute());
        mode1 = 0;
        delay(2000);
      }
      
      if(sicaklik <= optimumsicaklik && mode == 0){  
          analogWrite(role, 0);
          mode = 1;
      }
      else if(sicaklik <= (optimumsicaklik - 3) && mode == 1){  
          analogWrite(role, 0);
          mode = 0;
      }
      else {
          analogWrite(role, 1023);
      }
      delay(1000);
  }
  
  else {
    analogWrite(role, 1023);
    analogWrite(role1, 1023);
      if(mode1 == 0){
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("!!!!HEILL!!!");
        lcd.setCursor(1,1);
        lcd.print("!!!RALAZABA!!!");
        mode1 = 1;
        delay(1000);
      }
      if(mode1 == 1){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sicaklik: ");
        lcd.print(sicaklik);
        lcd.setCursor(0, 1);
        lcd.print("Nem: ");
        lcd.print(nem);
        mode1 = 2;
        delay(2000); 
      }
      if(mode1 == 2){
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("Saat ");
        lcd.setCursor(5,1);
        lcd.print(now.hour());
        lcd.print(":");
        lcd.print(now.minute());
        delay(2000);
        mode1 = 0;
      }

    
  }
  }
}


