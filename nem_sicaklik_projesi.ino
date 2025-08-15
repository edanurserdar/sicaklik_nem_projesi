#include <DHT.h>

#define DHTPIN1 2
#define DHTPIN2 3
#define DHTTYPE DHT11

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

#define RELAY_NEM 5 
#define RELAY_FON 4   

bool nem_cihaz_acik = false;  
bool fon_cihaz_acik = false;  

unsigned long oncekiZaman = 0;
const unsigned long okumaAraligi = 1000; 

void ilkKontrol();
void veriOkuVeKontrolEt();
void tekTik();
void ciftTik();
float ortalamaNem(int tekrar);
float ortalamaSicaklik(int tekrar);

void setup() {
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();

  pinMode(RELAY_NEM, OUTPUT);
  pinMode(RELAY_FON, OUTPUT);
  digitalWrite(RELAY_NEM, HIGH);
  digitalWrite(RELAY_FON, HIGH);

  delay(2000);

 
  ilkKontrol();
}

void loop() {
  unsigned long simdikiZaman = millis();

  if (simdikiZaman - oncekiZaman >= okumaAraligi) {
    oncekiZaman = simdikiZaman;
    veriOkuVeKontrolEt();
  }
}


void ilkKontrol() {
  float ort_nem = ortalamaNem(5);
  float ort_sicaklik = ortalamaSicaklik(5);

  Serial.print("BASLANGIC | ");
  Serial.print("SicaklikOrt: "); Serial.print(ort_sicaklik);
  Serial.print(" | NemOrt: "); Serial.println(ort_nem);

  if (ort_nem < 63.0) {
    Serial.println("BASLANGIC: Nem ACIK (tek tık)");
    tekTik();                 
    nem_cihaz_acik = true;
  } else if (ort_nem > 65.0) {
    Serial.println("BASLANGIC: Nem KAPALI (çift tık)");
    ciftTik();                
    nem_cihaz_acik = false;
  } else {
    Serial.println("BASLANGIC: Nem 63-65 arası → değişiklik yok");
  }

  
  if (ort_sicaklik < 35.0) {
    digitalWrite(RELAY_FON, LOW);   
    fon_cihaz_acik = true;
    Serial.println("BASLANGIC: Fon ACIK");
  } else if (ort_sicaklik > 40.0) {
    digitalWrite(RELAY_FON, HIGH);  
    fon_cihaz_acik = false;
    Serial.println("BASLANGIC: Fon KAPALI");
  } else {
    Serial.println("BASLANGIC: Fon 35-40 arası → değişiklik yok");
  }
}

void veriOkuVeKontrolEt() {
  // Tek tek ölçümler
  float t1 = dht1.readTemperature();
  float t2 = dht2.readTemperature();
  float h1 = dht1.readHumidity();
  float h2 = dht2.readHumidity();

  if (isnan(t1) || isnan(t2) || isnan(h1) || isnan(h2)) {
    Serial.println("Sensor okuma hatasi!");
    return;
  }

  float ort_sicaklik = (t1 + t2) / 2.0;
  float ort_nem      = (h1 + h2) / 2.0;

 
  if (ort_nem < 63.0 && !nem_cihaz_acik) {
    tekTik();                 
    nem_cihaz_acik = true;
  } else if (ort_nem > 65.0 && nem_cihaz_acik) {
    ciftTik();                
    nem_cihaz_acik = false;
  }

  if (ort_sicaklik < 35.0 && !fon_cihaz_acik) {
    digitalWrite(RELAY_FON, LOW); 
    fon_cihaz_acik = true;
  } else if (ort_sicaklik > 40.0 && fon_cihaz_acik) {
    digitalWrite(RELAY_FON, HIGH);  
    fon_cihaz_acik = false;
  }

  Serial.print("Sicaklik1: "); Serial.print(t1);
  Serial.print(" | Sicaklik2: "); Serial.print(t2);
  Serial.print(" | SicaklikOrt: "); Serial.print(ort_sicaklik);
  Serial.print(" || Nem1: "); Serial.print(h1);
  Serial.print(" | Nem2: "); Serial.print(h2);
  Serial.print(" | NemOrt: "); Serial.print(ort_nem);
  Serial.print(" || NemRole: "); Serial.print(nem_cihaz_acik ? 1 : 0);
  Serial.print(" | FonRole: "); Serial.println(fon_cihaz_acik ? 1 : 0);
}


void tekTik() {
  digitalWrite(RELAY_NEM, LOW);  
  delay(300);
  digitalWrite(RELAY_NEM, HIGH); 
}

void ciftTik() {
  tekTik();
  delay(200);
  tekTik();
}


float ortalamaNem(int tekrar) {
  float toplam = 0;
  for (int i = 0; i < tekrar; i++) {
    toplam += (dht1.readHumidity() + dht2.readHumidity()) / 2.0;
    delay(200);
  }
  return toplam / tekrar;
}

float ortalamaSicaklik(int tekrar) {
  float toplam = 0;
  for (int i = 0; i < tekrar; i++) {
    toplam += (dht1.readTemperature() + dht2.readTemperature()) / 2.0;
    delay(200);
  }
  return toplam / tekrar;
}
