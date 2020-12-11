/*********************************************************
 * 3.3 V + GND                  5V + GND      5V +GND
 * Arduino pro micro (3.3V)     HC05 (5V)    SSD1306 (5V)
 * 10           >>>               TX            --
 * 11           >>>               RX            --
 * GND          >>>               GND           GND
 * A4           >>>               --            SDA
 * A5           >>>               --            SCL
 *  
 *********************************************************/

#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define TX 10
#define RX 11
#define role1 5

Adafruit_SSD1306 oled(128, 64);
SoftwareSerial hc05(TX, RX);// TX 10 , RX 11

String komut = "";

// Serial Monitörden oku ve HC05 aracılığı ile (cep telefonuna) gönder)
void hc05write()
{
  while (Serial.available())
  {
    hc05.println(Serial.readString());
  }
}

// HC05 aracılığı ile (örneğin ceptelefonundan) gelen komutları Serial Monitörde yaz
String hc05read()
{
  String readhc05;
  while (hc05.available())
  {
    readhc05 = hc05.readString();
    Serial.println(readhc05);
  }
  return readhc05;
}

// Serial monütörden yada Bluetooth aygıttan gelen komut var mı bak
String updateserial()
{
  hc05write();
  return hc05read();
}

// OLED 96 display'de x,y koordinatlarında , siZe yazı büyüklüğünde , txt içerikli metni yaz
void printoled(int x, int y, int siZe, String txt)
{
  oled.clearDisplay();
  oled.setTextSize(siZe);             // Normal 1:1 pixel scale
  oled.setTextColor(SSD1306_WHITE);        // Draw white text
  oled.setCursor(x, y);            // Start at top-left corner
  oled.println(txt);
  oled.display();
}
// Röle 01 açılması komutu ve 3 ekranda açıldığını yazma işlemi 
void role01ac()
{
  digitalWrite(role1, HIGH);
  printoled(0, 5, 2, "role 1    acildi");
  hc05.println("role 1 acildi");
  Serial.println("role 1 acildi");
}

// Röle 01 kapanması komutı ve 3 ekranda açıldığını yazma işlemi
void role01kapa()
{
  digitalWrite(role1, LOW);
  printoled(0, 5, 2, "role 1    kapandi");
  hc05.println("role 1 kapandi");
  Serial.println("role 1 kapandi");
}


void setup()
{
  pinMode(role1,OUTPUT);
  Serial.begin(9600);// Serial Monitör iletişimini başlat
  hc05.begin(9600);//Bluetooth iletişiminibaşlat
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);// OLED display'i başlat
  printoled(0, 5, 2, "Sistem    Hazir");
}

void loop()
{
  komut = updateserial();// gelen giden komut var mı bak, varsa değişkende sakla
  if (komut > "") // eğer gelen komut boş değilse işlemi yap
  {
    Serial.println("Komut alındı: " + komut);
    hc05.println("Komut alındı: " + komut);
    printoled(0, 5, 2, "Komut: " + komut);
    if (komut.indexOf("role1ac") > -1) role01ac();// komut metni içerisinde role1ac geçiyorsa role01ac() fonksiyonunu çalıştır
    if (komut.indexOf("role1kapa") > -1) role01kapa();// komut metni içerisinde role1kapa geçiyorsa role01kapa() fonksiyonunu çalıştır
  }
}
