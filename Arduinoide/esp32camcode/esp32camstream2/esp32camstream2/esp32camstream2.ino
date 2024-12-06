#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>

const char* WIFI_SSID = "strix";
const char* WIFI_PASS = "12345678";

WebServer server(80);

static auto hiRes = esp32cam::Resolution::find(800, 600);

void streamJpg()
{
  WiFiClient client = server.client();
  String response = "HTTP/1.1 200 OK\r\n"
                    "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n"
                    "\r\n";
  client.print(response);

  while (client.connected()) {
    auto frame = esp32cam::capture();
    if (frame == nullptr) {
      Serial.println("CAPTURE FAIL");
      return;
    }

    // Send a boundary to separate each frame
    client.print("--frame\r\n");
    client.print("Content-Type: image/jpeg\r\n");
    client.print("Content-Length: " + String(frame->size()) + "\r\n\r\n");
    frame->writeTo(client);
    client.print("\r\n");

    // Delay to control frame rate
    delay(50); // Adjust delay to optimize streaming speed vs quality
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  using namespace esp32cam;
  Config cfg;
  cfg.setPins(pins::AiThinker);
  cfg.setResolution(hiRes);
  cfg.setBufferCount(2);
  cfg.setJpeg(80);

  bool ok = Camera.begin(cfg);
  Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("Stream URL: http://");
  Serial.println(WiFi.localIP());
  Serial.println("/stream");

  server.on("/stream", streamJpg);
  server.begin();
}

void loop() {
  server.handleClient();
}
