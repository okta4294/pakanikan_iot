#include<Wire.h>
#include <ESP32Servo.h> //khusus untuk esp32
//#include <Servo.h> //untuk arduino atau esp8266
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "RTClib.h"
RTC_DS3231 rtc;
#define BLYNK_TEMPLATE_ID "ID template blynk "
#define BLYNK_TEMPLATE_NAME "nama tamplate blynk anda"
#define BLYNK_AUTH_TOKEN "Token blynk anda"
#include <BlynkSimpleEsp32.h>
#define ssid "WiFi bang"
#define pass "kosongan"
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
int jam, menit, detik;
int tanggal, bulan, tahun;
String hari;
Servo servoku;
int pakan;

void setup () {
  Serial.begin(9600);
  lcd.begin();
  if (! rtc.begin()) {
     lcd.setCursor(0,1);
    lcd.print("RTC Tidak ditemukan");
    lcd.clear();
    delay(500);
    Serial.flush();
    while (1) delay(10);
  }
  servoku.attach(26); //sesuaikan dengan pin GPio di board anda pakai port pwm
  servoku.write(0);

  WiFi.begin(ssid,pass);

  while(WiFi.status() != WL_CONNECTED){
    lcd.setCursor(0,1);
    lcd.print("Tidak Terkoneksi");
    lcd.clear();
    delay(500);
    }
    lcd.setCursor(0,1);
    lcd.print("Terkoneksi");
    lcd.clear();
    Blynk.begin(BLYNK_AUTH_TOKEN,ssid,pass );
  //hilangkan (//) untuk awal coding, setelah itu beri (//) kembali lalu upload ulang
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
}

//fungsi atau void untuk memberi pakan, jika terlalu cepat bisa menggunkan perulangan
  void pakanikan () {
  servoku.write(180);
  delay(2000);
  servoku.write(0);
  delay(2000);
  }

void loop () {
  Blynk.run();
  if(pakan == 1){
    pakanikan();
    Blynk.virtualWrite(V0,0);
    pakan =0;
    delay(1000);
    }
 DateTime now = rtc.now();
  jam     = now.hour();
  menit   = now.minute();
  detik   = now.second();
  tanggal = now.day();
  bulan   = now.month();
  tahun   = now.year();
  hari    = daysOfTheWeek[now.dayOfTheWeek()];
  lcd.setCursor(0,0);
  lcd.print(String() +hari+"," +tanggal+"-"+bulan+"-"+tahun);
  lcd.setCursor(0,1);
  lcd.print(String() +"Jam: " +jam+":" +menit+":"+detik);
}
//menggunakan pin virtual V0 di blynk bisa disesuaikan dengan kebutuhan 
BLYNK_WRITE(V0){
  pakan = param.asInt();
  }
