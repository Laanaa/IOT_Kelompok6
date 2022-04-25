#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <DHT.h>
//#include <MQ2.h>
#define DHTTYPE DHT11
DHT dht(D6, DHTTYPE);
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


// inialisasi pin sensor dan alarm/buzzer
#define pinApi D5
#define pinAlarm D3
#define redLED D4
#define pinGas D0
#define yellowLED D7
#define greenLED D8

//MQ2 mq2(pinGas);

// inialisasi variabel data
int dataFire;
int dataGas;
//float dataGas2;

//Nilai threshold
int gasSensorThres = 100;
int fireSensorThres = 1;
//int coldTemperatureThres = 15;
int hotTemperatureThres = 20;
//int highHumidityThres = 110;
int lowHumidityThres = 100;

// ----------- running text ----------- //
void scrollText(int row, String message, int delayTime, int lcdColumns)
{
  for (int i = 0; i < lcdColumns; i++)
  {
    message = " " + message;
  }
  message = message + " ";
  for (int pos = 0; pos < message.length(); pos++)
  {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void setup()
{
 // inialisasi status I/O pin
 Serial.begin(9600);
 
 pinMode(pinApi, INPUT); // pin sebagai input
 pinMode(pinGas, INPUT); // pin sebagai input
 pinMode(pinAlarm, OUTPUT); // pin sebagai output
 pinMode(redLED, OUTPUT);
 pinMode(yellowLED, OUTPUT);
 pinMode(greenLED, OUTPUT);
 
 lcd.init(); // initialize the lcd
 lcd.setBacklight(HIGH);
 lcd.setCursor(0,0);
 lcd.print("Kelompok 6");
 lcd.setCursor(0,1);
 scrollText(1, "Sistem Pendeteksi Kebakaran", 250, 16);
 delay(1000);
 
}


void loop()
{
 // pendefinisian dari inputan data-data sensor yang dibutuhkan
// mq2.begin();
 float dataHumidity = dht.readHumidity();
 float dataTemperature = dht.readTemperature();
 dataFire = digitalRead(pinApi);
 dataGas = analogRead(pinGas);
//  float lpg = mq2.readLPG();
//  // co = values[1];
//  float co = mq2.readCO();
//  // smoke = values[2];
//  float smoke = mq2.readSmoke();
 // Mengecheck inputan data yang diterima
 Serial.print(" Sensor API: ");
 Serial.print(dataFire);
 Serial.print(" Sensor ASAP: ");
 Serial.println(dataGas);
// Serial.print(" Sensor ASAP Rinci: ");
// Serial.println(lpg);
// Serial.println(co);
// Serial.println(smoke);
 Serial.print(" Sensor Suhu: ");
 Serial.print(dataTemperature);
 Serial.print(" Sensor Kelembapan: ");
 Serial.println(dataHumidity);

 // Memastikan semua sensor berfungsi
 if(isnan(dataTemperature) && isnan(dataHumidity) && isnan(dataFire) && isnan(dataGas) ){
   scrollText(1, "Ada Sensor yang tidak terdeteksi !!!", 250, 16);
 }
 fuzzyfikasiSuhuDanGas(dataTemperature,dataGas);


 // jika data pada sensor API mendeteksi adanya api
 if (dataFire < fireSensorThres)
 {
    dangerCondition();
 }

// jika data Analog pada sensor Gas mendeteksi adanya gas
 else if (dataGas > gasSensorThres)
  {
    alertCondition1();
  }
  
//jika data suhu memasuki kondisi suhu tinggi dan temperatur memasuki kondisi kelembapan rendah 
 else if (dataHumidity < lowHumidityThres && dataTemperature > hotTemperatureThres)
  {
    alertCondition2();
  }
  
 // jika semuanya normal 
 else
 {
    safeCondition();
 }
 delay(1000);
}

// kondisi normal dan potensi terjadinya kebakaran kecil
void safeCondition(){
  //Lampu Dimatikan
  digitalWrite(redLED, LOW);
  digitalWrite(yellowLED, LOW);
 // alarm dimatikan
  digitalWrite(greenLED, HIGH);
  digitalWrite(pinAlarm, LOW);
  lcd.setCursor(0,0);
  lcd.print("SEMUA BAIK BAIK SAJA!!!");
  lcd.setCursor(0,1);
  lcd.print("STATUS NORMAL");
}

// kondisi saat  terjadi kebocoran gas atau potensi terjadinya kebakaran tinggi
void alertCondition1(){
  lcd.clear();
  // alarm siaga dinyalakan
  digitalWrite(yellowLED, HIGH);
  digitalWrite(greenLED, LOW);
  tone(pinAlarm, 1000, 200);
  lcd.setCursor(0,0);
  lcd.print("WASPADA!!!");
  lcd.setCursor(0,1);
  scrollText(1, "Terjadi Kebocoran GAS!!!", 250, 16);
  lcd.clear();
}

// kondisi saat terjadi perubahan suhu yang melebihi batas tinggi dan kelembapan yang melebihi batas rendah
void alertCondition2(){
  lcd.clear();
  // alarm siaga dinyalakan
  digitalWrite(yellowLED, HIGH);
  digitalWrite(greenLED, LOW);
  tone(pinAlarm, 1000, 200);
  lcd.setCursor(0,0);
  lcd.print("WASPADA!!!");
  lcd.setCursor(0,1);
  scrollText(1, "Teradi Perubahan Suhu dan kelembapan UPNormal!!!", 250, 16);
  lcd.clear();
}

// kondisi saat terjadi kebakaran
void dangerCondition(){
  lcd.clear();
  // alarm bahaya dinyalakan
  digitalWrite(pinAlarm, HIGH);
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  // pengumuman peringatan
  lcd.setCursor(0,0);
  lcd.print("SEGERA MENGUNGSI!!!");
  lcd.setCursor(0,1);
  scrollText(1, "Terjadi KEBAKARAN!!!", 250, 16);
}

void fuzzyfikasiSuhuDanGas(int T, int h){
   float dingin, hangat, panas;
   float sedikit, sedang, banyak;
   float lb, sd, ke;
   float hasillb, hasilsd, hasilke;
   float Coglb, Cogsd, Cogke;
   float Cogxlb, Cogxsd, Cogxke;
   float Komlb, Komsd, Komke;
   float Penlb, Pensd, Penke;
   float Totalkom, Totalpen;
   float Cog;
   float lb1, lb2, lb3;
   float sd1, sd2, sd3;
   float ke1, ke2, ke3;
   //======================
   //   fuzzyfikasi suhu
   //======================
   
   if(T <= 25){
   dingin = 1;
   hangat = 0;
   panas = 0;
   }
   else if(T >= 35){
   panas = 1;
   hangat = 0;
   dingin = 0;
   }
   else if(T == 30){
   hangat = 1;
   dingin = 0;
   panas = 0;
   }
   else if((T > 25)&&(T < 30)){
   hangat = (T - 25)/5;
   dingin = 1-(T - 25)/5; 
   panas = 0;
   }
   else if((T > 30)&&(T < 35)){
   panas = (T - 30)/5;
   hangat = 1-(T - 30)/5;
   dingin = 0; 
   }
   
   Serial.print("panas= ");   
   Serial.println(panas);
   Serial.print("hangat= ");   
   Serial.println(hangat);
   Serial.print("dingin= ");   
   Serial.println(dingin);

   //===============================
   //       fuzzyfikasi gas
   //===============================
   
   if(h <= 150){
   sedikit = 1;
   sedang = 0;
   banyak = 0;
   }
   else if(h >= 300){
   banyak = 1;
   sedang = 0;
   sedikit = 0;
   }
   else if(h == 200){
   banyak = 0; 
   sedang = 1;
   sedikit = 0;
   }
   else if((h > 150)&&(h < 200)){
   sedang = (h - 150)/50;
   sedikit = 1-(h - 150)/50;
   banyak = 0;
   }
   else if((h > 200)&&(h < 300)){
   sedang = (h - 200)/100;
   banyak = 1-(h - 200)/100;
   sedikit = 0; 
   }

   //  Serial.print("Sedikit= ");   
   //  Serial.println(sedikit);
   //  Serial.print("sedang= ");   
   //  Serial.println(sedang);
   //  Serial.print("banyak= ");   
   //  Serial.println(banyak);

    //==================================
   //           fuzzy rule
   //===================================
      
   //============================================================================
   if((T <= 25 )&&(h <= 150)){ //1A
    if(dingin < sedikit){
    lb = dingin;
    }
    else if(dingin > sedikit){
    lb = sedikit;
    }
    else if(dingin == sedikit){
    lb = sedikit;
    }
    
    hasillb = lb;
    hasilsd = 0;
    hasilke = 0;
     
   }
   
   //=========================================================================
   
   else if((T <= 25 )&&(h > 150)&&(h < 200)){ //1AB
    if(dingin < sedikit){
    lb1 = dingin;
    }
    else if(dingin > sedikit){
    lb1 = sedikit;
    }
    else if(dingin == sedikit){
    lb1 = sedikit;
    }
    
    if(dingin < sedang){
    lb2 = dingin;
    }
    else if(dingin > sedang){
    lb2 = sedang;
    }
    else if(dingin == sedang){
    lb2 = sedang ;
    }
    
    if(lb1 < lb2){    
     hasillb = lb2;   
    }
    else if(lb1 > lb2){    
     hasillb = lb1;   
    }
    else if(lb1 == lb2){    
     hasillb = lb1;   
    }
    
    hasilsd = 0;
    hasilke = 0;
     
   }
   
   //========================================================================
    else if((T <= 25 )&&(h > 200)&&(h < 300)){ //1BC
    if(dingin < sedang){
    lb = dingin;
    }
    else if(dingin > sedang){
    lb = sedang;
    }
    else if(dingin == sedang){
    lb = sedang;
    }
    
    if(dingin < banyak){
    sd = dingin;
    }
    else if(dingin > banyak){
    sd = banyak;
    }
    else if(dingin == banyak){
    sd = banyak;
    }
    
    hasillb = lb;   
    hasilsd = sd;
    hasilke = 0;
     
   }
   
   //=======================================================================
    else if((T <= 25 )&&(h >= 300)){ //1C

    if(dingin < banyak){
    sd = dingin;
    }
    else if(dingin > banyak){
    sd = banyak;
    }
    else if(dingin == banyak){
    sd = banyak;
    }
    
    hasillb = 0;   
    hasilsd = sd;
    hasilke = 0;
     
   }
   
   
   //==========================================================================
   
    else if((T > 25 )&&(T < 30 )&&(h <= 150)){ //12A

    if(dingin < sedikit){
    lb1 = dingin;
    }
    else if(dingin > sedikit){
    lb1 = sedikit;
    }
    else if(dingin == sedikit){
    lb1 = sedikit;
    }
    
    
    if(hangat > sedikit){
    lb2 = sedikit;
    }
    else if(hangat < sedikit){
    lb2 = hangat;
    }
    else if(hangat == sedikit){
    lb2 = hangat;
    }
    
    
    if(lb1 < lb2){    
     hasillb = lb2;   
    }
    else if(lb1 > lb2){    
     hasillb = lb1;   
    }
    else if(lb1 == lb2){    
     hasillb = lb1;   
    }
    
    hasilsd = 0;
    hasilke = 0;
     
   }
   
   //=========================================================================
   
    else if((T > 25 )&&(T < 30 )&&(h > 150)&&(h < 200)){ //12AB

    if(dingin < sedikit){
    lb1 = dingin;
    }
    else if(dingin > sedikit){
    lb1 = sedikit;
    }
    else if(dingin == sedikit){
    lb1 = sedikit;
    }
    
    
    if(dingin < sedang){
    lb2 = dingin;
    }
    else if(dingin > sedang){
    lb2 = sedang;
    }
    else if(dingin == sedang){
    lb2 = sedang;
    }
    
    
    if(hangat < sedikit){
    lb3 = dingin;
    }
    else if(hangat > sedikit){
    lb3 = sedikit;
    }
    else if(hangat == sedikit){
    lb3 = sedikit;
    }
    
    
    if(hangat < sedang){
    sd = hangat;
    }
    else if(hangat > sedang){
    sd = sedang;
    }
    else if(hangat == sedang){
    sd = sedang;
    }
    
    
   if((lb1 > lb2)&&(lb1 > lb3)){                          
   hasillb = lb1;
   }
   else if((lb1 > lb2)&&(lb1 == lb3)){
   hasillb = lb1;
   }
   else if((lb1 == lb2)&&(lb1 > lb3)){
   hasillb = lb1;
   }
   else if((lb1 == lb2)&&(lb1 == lb3)){
   hasillb = lb1;
   }
   else if((lb2 > lb1)&&(lb2 > lb3)){
   hasillb = lb2;
   }
   else if((lb2 > lb1)&&(lb2 == lb3)){
   hasillb = lb2;
   }
   else if((lb2 == lb1)&&(lb2 > lb3)){
   hasillb = lb2;
   }
   else if((lb2 == lb1)&&(lb2 == lb3)){
   hasillb = lb2;
   }
   else if((lb3 > lb1)&&(lb3 > lb2)){
   hasillb = lb3;
   }
   else if((lb3 > lb1)&&(lb3 == lb2)){
   hasillb = lb3;
   }
   else if((lb3 == lb1)&&(lb3 > lb2)){
   hasillb = lb3;
   }
   else if((lb3 == lb1)&&(lb3 == lb2)){
   hasillb = lb3;
   }

   hasilsd = sd;
   hasilke = 0;
    
   }
   
   //===============================================================================
   
   else if((T > 25)&&(T < 30)&&(h > 200)&&(h < 300)){ //12BC
   if (dingin < sedang){
   lb = dingin;
   }
   else if(dingin > sedang){
   lb = sedang;
   }
   else if(dingin == sedang){
   lb = sedang;
   }

   if(dingin < banyak){
   sd1 = dingin;
   }
   else if(dingin > banyak){
   sd1 = banyak;
   }
   else if(dingin == banyak){
   sd1 = banyak;
   }

   if(hangat < sedang){
   sd2 = hangat;
   }
   else if(hangat > sedang){
   sd2 = sedang;
   }
   else if(hangat == sedang){
   sd2 = sedang;
   }

   if(hangat < banyak){
   ke = hangat;
   }
   else if(hangat > banyak){
   ke = banyak;
   }
   else if(hangat == banyak){
   ke = banyak;
   }

   if(sd1 > sd2){
   hasilsd = sd1;
   }
   else if(sd2 > sd1){
   hasilsd = sd2;
   }
   else if(sd2 == sd1){
   hasilsd = sd2;
   }

   hasillb = lb;
   hasilke = ke;

   }

   //=======================================================================

   else if((T > 25)&&(T < 30)&&(h >= 300)){ //12C
     
   if(dingin < banyak){
   sd = dingin;
   }
   else if(dingin > banyak){
   sd = banyak;
   }
   else if(dingin == banyak){
   sd = banyak;
   }

   if(hangat > banyak){
   ke = banyak;
   }
   else if(hangat < banyak){
   ke = hangat;
   }
   else if(hangat == banyak){
   ke = hangat;
   }
   
   hasilsd = sd;
   hasilke = ke;
   hasillb = 0;
   }

   //===============================================================
   else if ((T == 30) && (h <= 150)){ //2A
   if (hangat > sedikit){     
   lb = sedikit;
   }
   else if (hangat < sedikit){
   lb = hangat;
   }
   else if (hangat == sedikit){
   lb = hangat;
   }

   hasillb = lb;
   hasilsd = 0;
   hasilke = 0;
   }
   
   //============================================================
   else if ((T == 30) && (h > 150) && (h < 200)){ //2AB
   if (hangat < sedikit){
   lb = hangat;
   }
   else if (hangat > sedikit){
   lb = sedikit;
   }
   else if (hangat == sedikit){
   lb = sedikit;
   }
   

   if (hangat < sedang){
   sd = hangat;
   }
   else if (hangat > sedikit){
   sd = sedikit;
   }
   else if (hangat == sedikit){
   sd = sedikit;
   }

   hasillb = lb;
   hasilsd = sd;
   hasilke = 0;
   
   }

//===================================================================
   else if ((T == 30) && (h > 200) && (h < 300)){ //2BC
   if (hangat < sedang){
   sd = hangat;
   }
   else if (hangat > sedang){
   sd = sedang;
   }
   else if (hangat == sedang){
   sd = sedang;
   }

   if (hangat < banyak){
   ke = hangat;
   }
   else if (hangat > banyak){
   ke = banyak;
   }
   else if (hangat == banyak){
   ke = banyak;
   }

   hasilsd = sd;
   hasilke = ke;
   hasillb = 0;
   
   }

 //=============================================================
   else if ((T == 30) && (h >= 300)){  //2C
   if (hangat < banyak){
   ke = hangat;
   }
   else if (hangat > banyak){
   ke = banyak;
   }
   else if (hangat == banyak){
   ke = banyak;
   }

   hasilke = ke;
   hasilsd = 0;
   hasillb = 0;

}


//====================================================================

   else if ((T > 30) && (T < 35) && (h <= 150)){ //23A
   if (hangat < sedikit){ 
   lb = hangat;
   }
   else if (hangat > sedikit){
   lb = sedikit;
   }
   else if (hangat == sedikit){
   lb = sedikit;
   }
   
   if (panas < sedikit){
   sd = panas;
   }
   else if (panas > sedikit){
   sd = sedikit;
   }
   else if (panas == sedikit){
   sd = sedikit;
   }

   hasillb = lb;
   hasilsd = sd;
   hasilke = 0;
   }

//=======================================================================

   else if ((T > 30) && (T < 35) && (h > 150) && (h < 200)){  //23AB
   if (hangat < sedikit){
   lb = hangat;
   }
   else if (hangat > sedikit){
   lb = sedikit;
   }
   else if (hangat == sedikit){
   lb = sedikit;
   }

   if (hangat < sedang){
   sd1 = hangat;
   }
   else if (hangat > sedang){
   sd1 = sedang;
   }
   else if (hangat == sedang){
   sd1 = sedang;
   }
   
   if (panas < sedikit){
   sd2 = panas;
   }
   else if (panas > sedikit){
   sd2 = sedikit;
   }
   else if (panas == sedikit){
   sd2 = sedikit;
   }

   if (panas < sedang){
   ke = panas;
   }
   else if (panas > sedang){
   ke = sedang;
   }
   else if (panas == sedang){
   ke = sedang;
   }

   if (sd1 < sd2){
   hasilsd = sd2;
   }
   else if (sd1 > sd2){
   hasilsd = sd1;
   }
   else if (sd1 == sd2){
   hasilsd = sd1;
   }

   hasilke = ke;
   hasillb = lb;
   }

//=========================================================================

   else if ((T > 30) && (T < 35) && (h > 200) && (h < 300)){ //23BC
   if (hangat < sedang){
   sd = hangat;
   }
   else if (hangat > sedang){
   sd = sedang;
   }
   else if (hangat == sedang){
   sd = sedang;
   }

   if (hangat < banyak){
   ke1 = hangat;
   }
   else if (hangat > banyak){
   ke1 = banyak;
   }
   else if (hangat == banyak){
   ke1 = banyak;
   }

   if (panas < sedang){
   ke2 = panas;
   }
   else if (panas > sedang){
   ke2 = sedang;
   }
   else if (panas == sedang){
   ke2 = sedang;
   }

   if (panas < banyak){
   ke3 = panas;
   }
   else if (panas > banyak){
   ke3 = banyak;
   }
   else if (panas == banyak){
   ke3 = banyak;
   }
   
   if ((ke1 > ke2) && (ke1 > ke3)){
   hasilke = ke1;
   }
   else if ((ke1 > ke2) && (ke1 == ke3)){
   hasilke = ke1;
   }
   else if ((ke1 == ke2) && (ke1 > ke3)){
   hasilke = ke1;
   }
   else if ((ke1 == ke2) && (ke1 == ke3)){
   hasilke = ke1;
   }
   else if ((ke2 > ke1) && (ke2 > ke3)){
   hasilke = ke2;
   }
   else if ((ke2 > ke1) && (ke2 == ke3)){
   hasilke = ke2;
   }
   else if ((ke2 == ke1) && (ke2 > ke3)){
   hasilke = ke2;
   }
   else if ((ke2 == ke1) && (ke2 == ke3)){
   hasilke = ke2;
   }

   else if ((ke3 > ke1) && (ke3 > ke2)){
   hasilke = ke3;
   }
   else if ((ke3 > ke1) && (ke3 == ke2)){
   hasilke = ke3;
   }
   else if ((ke3 == ke1) && (ke3 > ke2)){
   hasilke = ke3;
   }
   else if ((ke3 == ke1) && (ke3 == ke2)){
   hasilke = ke3;
   }

   hasilsd = sd;
   hasillb = 0;
   }

//===========================================================================

   else if ((T > 30) && (T < 35) && (h >= 300)){ //23C
   if (hangat < banyak){
   ke1 = hangat;
   }
   else if (hangat > banyak){
   ke1 = banyak;
   }
   else if (hangat == banyak){
   ke1 = banyak;
   }

   if (panas < banyak){
   ke2 = panas;
   }
   else if (panas > banyak){
   ke2 = banyak;
   }
   else if (panas == banyak){
   ke2 = banyak;
   }

   if (ke1 < ke2){
   hasilke = ke2;
   }
   else if (ke1 > ke2){
   hasilke = ke1;
   }
   else if (ke1 == ke2){
   hasilke = ke1;
   }

   hasilsd = 0;
   hasillb = 0;
   }

//===========================================================================

  else if ((T >= 35) && (h <= 150)){  //3A
   if (panas < sedikit){
   sd = panas;
   }
   else if (panas > sedikit){
   sd = sedikit;
   }
   else if (panas == sedikit){
   sd = sedikit;
   }

   hasilsd = sd;
   hasillb = 0;
   hasilke = 0;
   }

//============================================================================

else if ((T >= 35) && (h > 150) && (h < 200)){  //3AB
   if (panas < sedikit){
   sd = panas;
   }
   else if (panas > sedikit){
   sd = sedikit;
   }
   else if (panas == sedikit){
   sd = sedikit;
   }

   if (panas < sedang){
   ke = panas;
   }
   else if (panas > sedang){
   ke = sedang;
   }
   else if (panas == sedang){
   ke = sedang;
   }

   hasilsd = sd;
   hasilke = ke;
   hasillb = 0;
   }
   
   
//======================================================================   
   
else if ((T >= 35) && (h > 200) && (h < 300)){ //3BC
   if (panas < sedang){
   ke1 = panas;
   }
   else if (panas > sedang){
   ke1 = sedang;
   }
   else if (panas == sedang){
   ke1 = sedang;
   }

   if (panas < banyak){
   ke2 = panas;
   }
   else if (panas > banyak){
   ke2 = banyak;
   }
   else if (panas == banyak){
   ke2 = banyak;
   }

   if (ke1 < ke2){
   hasilke = ke2;
   }
   else if (ke1 > ke2){
   hasilke = ke1;
   }
   else if (ke1 == ke2){
   hasilke = ke1;
   }

   hasillb = 0;
   hasilsd = 0;
   }

//=========================================================================

else if ((T >= 35) && (h >= 300)){ //3C
   if (panas < banyak){
   ke = panas;
   }
   else if (panas > banyak){
   ke = banyak;
   }
   else if (panas == banyak){
   ke = banyak;
   }

   hasilke = ke;
   hasilsd = 0;
   hasillb = 0;
}


//======================================================================

else if ((T <= 25) && (h == 200)){ //1B
   if (dingin < sedang){
   lb = dingin;
   }
   else if (dingin > sedang){
   lb = sedang;
   }
   else if (dingin == sedang){
   lb = sedang;
   }

   hasillb = lb;
   hasilsd = 0;
   hasilke = 0;
}

//===================================================================

else if ((T > 25) && (T < 30) && (h == 200)){ //12B
   if (dingin < sedang){
   lb = dingin;
   }
   else if (dingin > sedang){
   lb = sedang;
   }
   else if (dingin == sedang){
   lb = sedang;
   }

   if (hangat < sedang){
   sd = hangat;
   }
   else if (hangat > sedang){
   sd = sedang;
   }
   else if (hangat == sedang){
   sd = sedang;
   }

   hasillb = lb;
   hasilsd = sd;
   hasilke = 0;
}

//==========================================================

else if ((T == 30) && (h == 200)){ //2B
   if (hangat < sedang){
   sd = hangat;
   }
   else if (hangat > sedang){
   sd = sedang;
   }
   else if (hangat == sedang){
   sd = sedang;
   }

   hasilsd = sd;
   hasillb = 0;
   hasilke = 0;
   
}


//================================================================

else if ((T > 30) && (T < 35) && (h == 200)){ //23B
   if (hangat < sedang){ 
   sd = hangat;
   }
   else if (hangat > sedang){
   sd = sedang;
   }
   else if (hangat == sedang){
   sd = sedang;
   }

   if (panas < sedang){
   ke = panas;
   }
   else if (panas > sedang){
   ke = sedang;
   }
   else if (panas == sedang){
   ke = sedang;
   }
   
   hasilsd = sd;
   hasilke = ke;
   hasillb = 0;
}



//================================================================

else if ((T >= 35) && (h == 200)){  //3B
   if (panas < sedang){
   ke = panas;
   }
   else if (panas > sedang){
   ke = sedang;
   }
   else if (panas == sedang){
   ke = sedang;
   }

   hasilke = ke;
   hasilsd = 0;
   hasillb = 0;

}



//==================================================
//               Defuzzifikasi
//==================================================


//0-10-20-30
Coglb = 60;
Cogxlb = 4;

//40-50-60-70
Cogsd = 220;
Cogxsd = 4;

//80-90-100-110-120
Cogke = 500;
Cogxke = 5;

/*
//0-10-20-30-40-50
Coglb = 150;
Cogxlb = 6;

//60-70-80-90-100-110-120
Cogsd = 630;
Cogxsd = 7;

//130-140-150-160-170-180-190-200
Cogke = 1320;
Cogxke = 8;
*/
//Komlb = Coglb * hasillb;
//Komsd = Cogsd * hasilsd;
//Komke = Cogke * hasilke;

//Totalkom = Komlb + Komsd;
//Totalkom = Totalkom + Komke;

//Penlb = hasillb * Cogxlb;
//Pensd = hasilsd * Cogxsd;
//Penke = hasilke * Cogxke;

//Totalpen = Penlb + Pensd;
//Totalpen = Totalpen + Penke;

Totalkom = (Coglb * hasillb) + (Cogsd * hasilsd) + (Cogke * hasilke); 

Totalpen = (hasillb * Cogxlb) + (hasilsd * Cogxsd) + (hasilke * Cogxke);

Cog = Totalkom / Totalpen;
   
   float dimming = 128 - Cog;
   
   Serial.print("Totalkom= ");
   Serial.println(Totalkom);
   Serial.print("Totalpen= ");
   Serial.println(Totalpen);
   Serial.print("COG= ");
   Serial.println(Cog);
   Serial.println(" ");
   
  //kondisi LED nyala dengan parameter asap
      if(Cog<33){
         safeCondition();
        }

       else if(Cog>33 && Cog<66){
         alertCondition2();
        }
        else{
         dangerCondition();
        }
   
}
