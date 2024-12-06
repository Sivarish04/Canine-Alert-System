#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>  // Use ESP32Servo library instead of Servo

// Replace with your network credentials
const char* ssid     = "strix";
const char* password = "12345678";

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define BUZZER_PIN 15     // Pin where the buzzer is connected
#define SERVO_PIN 13      // Pin where the servo is connected

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Servo myServo;  // Create a servo object with ESP32Servo

WiFiServer server(80);  // Create a Wi-Fi server listening on port 80

unsigned long lastAlertTime = 0;  // Store the time of the last "Dog Alert!" message
bool alertActive = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(12, 14);

  // Set up the buzzer pin as output
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off initially

  // Attach the servo to its pin and set it to an initial position
  myServo.attach(SERVO_PIN);
  myServo.write(0);  // Initial position (0 degrees)

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);   
  display.setTextColor(WHITE);
  display.setCursor(0, 10);  
  display.display();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  
  unsigned long wifiConnectTimeout = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (millis() - wifiConnectTimeout > 10000) {  // 10-second timeout
      Serial.println("Failed to connect to Wi-Fi!");
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected!");
  } else {
    Serial.println("Wi-Fi connection failed.");
  }

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
          inputData.trim();  // Remove extra whitespace
          Serial.print("Received: ");
          Serial.println(inputData);

          // Clear the display
          display.clearDisplay();
          display.setTextSize(2);

          // Calculate text bounds for centering
          int16_t x1, y1;
          uint16_t width, height;
          display.getTextBounds(inputData, 0, 0, &x1, &y1, &width, &height);
          int x = (SCREEN_WIDTH - width) / 2;
          int y = (SCREEN_HEIGHT - height) / 2;
          display.setCursor(x, y);
          display.print(inputData);
          display.display();

          // Check if the received message is "Dog Alert!"
          if (inputData == "Dog Alert!") {
            // Activate the buzzer
            digitalWrite(BUZZER_PIN, HIGH);
            delay(1000);  // Keep the buzzer on for 1 second
            digitalWrite(BUZZER_PIN, LOW); // Turn off the buzzer

            // Move the servo to 90 degrees (alert position)
            myServo.write(90);
            lastAlertTime = millis();  // Update last alert time
            alertActive = true;  // Set alert status to active
          }

          inputData = "";  // Clear the string for the next message
        }
      }
    }

    client.stop();
    Serial.println("Client disconnected");
  }

  // Check if the alert is active and no new data was received within 3 seconds
  if (alertActive && (millis() - lastAlertTime > 3000)) {
    // Return the servo to the initial position
    myServo.write(0);
    alertActive = false;  // Reset alert status
  }
}
