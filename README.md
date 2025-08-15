# sicaklik_nem_projesi
Cihazların belli iklim koşullarında davranışlarının test edilmesi için geliştirilen cihaz tasarımı.

# DHT11 Sıcaklık ve Nem Kontrollü Cihaz Yönetimi
Bu proje, iki adet DHT11 sıcaklık-nem sensöründen alınan verilerin ortalamasını kullanarak nemlendirici ve fön cihazını otomatik olarak kontrol eder.  
Cihazlar belirlenen eşik değerlerine göre röle aracılığıyla açılıp kapatılır.
Nem cihazı tek ve çift tıklama mantığıyla kontrol edilir (3 modlu cihaz olduğu için)
Veriler USB kablo aracılığıyla Raspberry Pi 4'e aktarılarak seri ekrana yazdırılır.
txt. dosyasına kaydedilir. 

# Özellikler
- Histerezis mantığı sayesinde ilk durum davranışı kontrolü
- Seri port üzerinden anlık veri görüntüleme
- Değiştirilebilir eşik değerleri
  
# Donanım Gereksinimleri
- 1x Arduino 
- 2x DHT11 Sıcaklık/Nem Sensörü
- 1x 2 Kanallı Röle Modülü
- Nemlendirici cihaz
- Fön cihazı
- Kablolar
- Breadboard
- Raspberry Pi 4 

# Çalışma Mantığı
1. **Başlangıç Aşaması**
   - 5 kez ölçüm alınır ve ortalama hesaplanır.
   - Nem `< 63%` → Nem cihazı **tek tık** ile açılır.
   - Nem `> 65%` → Nem cihazı **çift tık** ile kapatılır.
   - Sıcaklık `< 35°C` → Fön açılır.
   - Sıcaklık `> 40°C` → Fön kapatılır.

2. **Sürekli Kontrol**
   - Her 1 saniyede sensör verileri güncellenir.
   - Eşik değerleri aşıldığında cihazlar açılır/kapatılır.
   - Veriler seri porttan izlenebilir.

