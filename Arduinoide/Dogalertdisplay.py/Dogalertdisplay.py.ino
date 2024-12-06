#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Replace with your network credentials
const char* ssid     = "strix";
const char* password = "12345678";

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define BUZZER_PIN 15     // Pin where the buzzer is connected

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

WiFiServer server(80);  // Create a Wi-Fi server listening on port 80

void setup() {
  Serial.begin(115200);
  
  // Set up the buzzer pin as output
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Make sure the buzzer is off initially

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(2);  // Set the text size once
  display.setTextColor(WHITE);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Connected!");

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();  // Check if a client has connected

  if (client) {
    Serial.println("Client connected");
    String inputData = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        inputData += c;

        if (c == '\n') {  // End of input
          Serial.print("Received: ");
          Serial.println(inputData);

          // Clear the display once per message
          display.clearDisplay();

          // Calculate text bounds for centering
          int16_t x1, y1;
          uint16_t width, height;
          display.getTextBounds(inputData, 0, 0, &x1, &y1, &width, &height);

          // Set cursor position to center the text
          int x = (SCREEN_WIDTH - width) / 2;
          int y = (SCREEN_HEIGHT - height) / 2;
          display.setCursor(x, y);

          // Display the received data on OLED
          display.print(inputData);
          display.display();  // Update the OLED screen

          // Check if the received message is "Dog Alert!"
          if (inputData == "Dog Alert!\n") {
            // Activate the buzzer
            digitalWrite(BUZZER_PIN, HIGH);
            delay(500);  // Reduce buzzer time to 500ms
            digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer
          }

          inputData = "";  // Clear the string for the next message
        }
      }
    }

    client.stop();
    Serial.println("Client disconnected");
  }
}
