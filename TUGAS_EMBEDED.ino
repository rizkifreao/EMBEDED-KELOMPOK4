#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define SERVER_IP "127.0.0.1" // Ip server dimana data akan dikirim
#define SERVER_PORT "5000"    //Set Port Server

const char *ssid = "Galaxy A50s";  //Nama Wifi
const char *password = "lupalagi"; //Password
String pubString;
const char *hosts = "127.0.0.1"; // Ip server dimana data akan dikirim
const int httpport = 5000;       //Set Port Server

//Control Pin dengan Multiplexer
int s0 = 16; //D0
// int s1 = 5;  //D1
// int s2 = 4;  //D2
// int s3 = 0;  //D3
float tempc;
float vout;
const int sensorSuhu = A0;
int toogleLed = 1;

//Mux in "SIG" pin
// int SIG_pin = 0;

WiFiClient client;
HTTPClient http;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(s1, OUTPUT);
  // pinMode(s2, OUTPUT);
  // pinMode(s3, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);
  // digitalWrite(s1, LOW);
  // digitalWrite(s2, LOW);
  // digitalWrite(s3, LOW);

  pinMode(sensorSuhu, INPUT);

  Serial.begin(9600);
  delay(100);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print("");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  if ((WiFi.status() == WL_CONNECTED))
  {
    POST();
    GET();
  }
  delay(100);
}

void GET(void)
{
  //CEK APAKAH KLIEN NYA TERKONEKSI
  if (!client.connect(hosts, httpport))
  {
    Serial.println("connection failed");
    return;
  }
  http.begin("http://" SERVER_IP ":" SERVER_PORT "/listening");
  int httpCode = http.GET();
  //Chek kode yang didapat
  if (httpCode > 0)
  {
    // Parsing
    const size_t bufferSize = JSON_OBJECT_SIZE(2) + 50;
    DynamicJsonDocument doc(bufferSize);
    deserializeJson(doc, http.getString());
    // Parameter
    int value = doc["value"];      // 1
    String action = doc["action"]; // "Nama"
    if (action == "ToggleLed")
    {
      Serial.println("===== Status Led Berubah ======");
      Serial.println(value);
      toogleLed = value;
      digitalWrite(LED_BUILTIN, value);
    }

    //    const char* username = doc["username"]; // "Username"
    //    const char* email = doc["email"]; // "email"
    // Output ke serial monitor
    Serial.print("Status LED:");
    Serial.println(toogleLed);
    //    Serial.print("Value:");
    //    Serial.println(id);
  }
  http.end(); //Tutup Koneksi
}

void POST(void)
{
  //CEK APAKAH KLIEN NYA TERKONEKSI
  if (!client.connect(hosts, httpport))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("[HTTP] begin...\n");
  http.begin(client, "http://" SERVER_IP ":" SERVER_PORT "/"); //HTTP
  http.addHeader("Content-Type", "application/json");

  //Read Sensor Suhu
  vout = analogRead(sensorSuhu);
  vout = (vout * 300) / 1024;
  tempc = vout;

  Serial.print("[HTTP] POST...\n");
  // start connection and send HTTP header and body
  int httpCode = http.POST("{\"sensor1Val\": " + String(tempc) + ", \"toogleLed\": " + String(toogleLed) + ", \"sensor3Val\": 0}");

  //  if (httpCode > 0) {
  //    // HTTP header telah dikirim dan header respons Server telah ditangani
  //    Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  //
  //    // AMBIL RESPONSE DARI SERVER
  //    if (httpCode == HTTP_CODE_OK) {
  //      const String& payload = http.getString();
  //      Serial.println("data diterima :\n<<");
  //      Serial.println(payload);
  //      Serial.println(">>");
  //    }
  //  } else {
  //    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  //  }

  http.end();
  // This is your NodeMCU IP address. Could be handy for other projects
  //  delay(1000);

  //  pubString = "{\"sensor1Val\": " + String(tempc) + ", \"sensor2Val\": 0 \"sensor3Val\": 0}";
  //  String pubStringLength = String(pubString.length(), DEC);
  //  Serial.print("Requesting POST: ");
  //  client.println("POST /?sensor1Val=" + String(tempc) + "&sensor2Val=0&sensor3Val=0 HTTP/1.1");
  //  client.println("Content-Type: application/json");
  //  client.println("Connection: close");
  //  client.print("Content-Length: ");
  //  client.println(pubStringLength);
  ////  Serial.println(pubStringLength);
  //  client.println();
  //  Serial.println("Sending Analog Value" + pubString);
  //  client.print(pubString);
  //  client.println();
  //  while (client.available())
  //  {
  //    String line = client.readStringUntil('\r');
  //    Serial.print(line);
  //  }
}

// Multiplexer Analog untuk memecah pin analog
// int readMux(int channel)
// {
//   int controlPin[] = {s0, s1, s2, s3};

//   int muxChannel[16][4] = {
//       {0, 0, 0, 0}, //channel 0
//       {1, 0, 0, 0}, //channel 1
//       {0, 1, 0, 0}, //channel 2
//       {1, 1, 0, 0}, //channel 3
//       {0, 0, 1, 0}, //channel 4
//       {1, 0, 1, 0}, //channel 5
//       {0, 1, 1, 0}, //channel 6
//       {1, 1, 1, 0}, //channel 7
//       {0, 0, 0, 1}, //channel 8
//       {1, 0, 0, 1}, //channel 9
//       {0, 1, 0, 1}, //channel 10
//       {1, 1, 0, 1}, //channel 11
//       {0, 0, 1, 1}, //channel 12
//       {1, 0, 1, 1}, //channel 13
//       {0, 1, 1, 1}, //channel 14
//       {1, 1, 1, 1}  //channel 15
//   };

//   //loop through the 4 sig
//   for (int i = 0; i < 4; i++)
//   {
//     digitalWrite(controlPin[i], muxChannel[channel][i]);
//   }
//   //read the value at the SIG pin
//   int val = analogRead(SIG_pin);
//   //return the value
//   return val;
// }
