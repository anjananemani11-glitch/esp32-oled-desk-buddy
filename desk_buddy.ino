
#include WiFi.h
#include Wire.h
#include Adafruit_GFX.h
#include Adafruit_SSD1306.h
#include time.h

 OLED settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

 WiFi details
const char ssid = YOUR_WIFI_NAME;
const char password = YOUR_WIFI_PASSWORD;

 Indian Standard Time
const char ntpServer = pool.ntp.org;
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;

 Spotify information
String song = Waiting for Spotify...;
String artist = ;

String receivedData = ;

void setup() {

  Serial.begin(115200);

   Initialize OLED
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(OLED not found!);
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(25, 25);
  display.println(Hello cutie!);
  display.display();

   Connect WiFi
  WiFi.begin(ssid, password);

  display.clearDisplay();
  display.setCursor(15, 25);
  display.println(Connecting WiFi...);
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println(WiFi Connected!);

   Synchronize Indian time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  struct tm timeinfo;

  while (!getLocalTime(&timeinfo)) {
    delay(500);
  }
}

void readSpotify() {

  while (Serial.available()) {

    char c = Serial.read();

    if (c == 'n') {

      int first = receivedData.indexOf('');

      if (first != -1) {
        song = receivedData.substring(0, first);
        artist = receivedData.substring(first + 1);
      }

      receivedData = ;
    }

    else if (c != 'r') {

      if (receivedData.length()  200) {
        receivedData += c;
      }
    }
  }
}

void loop() {

   Receive Spotify data
  readSpotify();

  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    return;
  }

  char timeString[15];
  char dateString[20];

  strftime(timeString, sizeof(timeString),
           %H%M%S, &timeinfo);

  strftime(dateString, sizeof(dateString),
           %d %b %Y, &timeinfo);

  display.clearDisplay();

   Digital clock
  display.setTextSize(2);
  display.setCursor(16, 0);
  display.print(timeString);

   Date
  display.setTextSize(1);
  display.setCursor(35, 19);
  display.print(dateString);

   Divider
  display.drawLine(0, 29, 127, 29, SSD1306_WHITE);

   Spotify section
  display.setCursor(0, 32);
  display.print(NOW PLAYING);

  display.setCursor(0, 41);
  display.print(song.substring(0, 21));

  display.setCursor(0, 50);
  display.print(artist.substring(0, 21));

   Cute heart
  display.setCursor(53, 57);
  display.print(3);

  display.display();

  delay(100);
}
