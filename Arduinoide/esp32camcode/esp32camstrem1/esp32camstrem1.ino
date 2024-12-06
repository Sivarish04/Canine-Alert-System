#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>

const char* WIFI_SSID = "strix";
const char* WIFI_PASS = "12345678";

WebServer server(80);

void handleMJPEGStream() {
  WiFiClient client = server.client();
  // Set headers for MJPEG stream
  server.sendContent("HTTP/1.1 200 OK\r\n"
                     "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n");

  while (client.connected()) {
    auto frame = esp32cam::capture();
    if (frame == nullptr) {
      Serial.println("CAPTURE FAIL");
      break;
    }

    // Send frame as part of MJPEG stream
    server.sendContent("--frame\r\n");
    server.sendContent("Content-Type: image/jpeg\r\n\r\n");
    client.write(frame->data(), frame->size());
    server.sendContent("\r\n");
    delay(100); // Adjust delay to control frame rate
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  using namespace esp32cam;
  Config cfg;
  cfg.setPins(pins::AiThinker);
  cfg.setResolution(Resolution::find(640, 480)); // Set resolution
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
  Serial.println();
  Serial.print("Connected to WiFi. Stream MJPEG at: http://");
  Serial.println(WiFi.localIP());
  Serial.println("/stream");

  server.on("/stream", handleMJPEGStream);
  server.begin();
}

void loop() {
  server.handleClient();
}
