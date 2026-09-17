#define BLYNK_TEMPLATE_ID "YOUR TEMPLATE ID"
#define BLYNK_TEMPLATE_NAME "Safegas IOT"
#define BLYNK_AUTH_TOKEN "YOUR_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// ==========================================
// WIFI
// ==========================================
char ssid[] = "WIFI_NAME";
char pass[] = "WIFI_PASSWORD";

// ==========================================
// PIN DEFINITIONS
// ==========================================
const int redLED = 26;
const int buzzer = 25;
const int relay = 33;
const int gasSensor = 34;
const int servoPin = 13;

// ==========================================
// GAS THRESHOLDS
// ==========================================
const int gasThreshold = 300;
const int safeThreshold = 250;

// ==========================================
// SERVO ANGLES
// ==========================================
// YOUR MECHANISM:
// 90° = OPEN
// 0°  = CLOSED

const int SERVO_OPEN_ANGLE = 90;
const int SERVO_CLOSED_ANGLE = 180;

// ==========================================
// SERVO
// ==========================================
Servo gasServo;

// ==========================================
// ALARM VARIABLES
// ==========================================
bool gasAlarm = false;

int gasCount = 0;
const int requiredGasReadings = 3;

// ==========================================
// BLYNK TIMER
// ==========================================
BlynkTimer timer;

// ==========================================
// SEND DATA TO BLYNK
// ==========================================
void sendToBlynk()
{
  int gasValue = analogRead(gasSensor);

  // V0 = Gas Sensor
  Blynk.virtualWrite(V0, gasValue);

  // V1 = Fan Status
  if (gasAlarm)
  {
    Blynk.virtualWrite(V1, 1);
  }
  else
  {
    Blynk.virtualWrite(V1, 0);
  }

  // V2 = System Status
  if (gasAlarm)
  {
    Blynk.virtualWrite(V2, "GAS LEAK DETECTED");
  }
  else
  {
    Blynk.virtualWrite(V2, "SYSTEM NORMAL");
  }

  // V3 = Alarm Status
  if (gasAlarm)
  {
    Blynk.virtualWrite(V3, 1);
  }
  else
  {
    Blynk.virtualWrite(V3, 0);
  }

  // V4 = Servo Angle
  if (gasAlarm)
  {
    Blynk.virtualWrite(V4, SERVO_CLOSED_ANGLE);
  }
  else
  {
    Blynk.virtualWrite(V4, SERVO_OPEN_ANGLE);
  }
}

// ==========================================
// SETUP
// ==========================================
void setup()
{
  Serial.begin(115200);

  // Pin setup
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay, OUTPUT);

  // Initial state
  digitalWrite(redLED, LOW);
  noTone(buzzer);

  // Fan OFF
  digitalWrite(relay, LOW);

  // ========================================
  // SERVO SETUP
  // ========================================
  gasServo.attach(servoPin);

  // Start OPEN
  gasServo.write(SERVO_OPEN_ANGLE);

  delay(1000);

  // ========================================
  // BLYNK
  // ========================================
  Serial.println("Connecting to WiFi and Blynk...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Send data every 1 second
  timer.setInterval(1000L, sendToBlynk);

  // ========================================
  // START MESSAGE
  // ========================================
  Serial.println("================================");
  Serial.println("       SafeGas IoT System");
  Serial.println("================================");
  Serial.println("System Normal");
  Serial.println("Servo = 90 degrees");
  Serial.println("Valve = OPEN");
}

// ==========================================
// LOOP
// ==========================================
void loop()
{
  Blynk.run();
  timer.run();

  // ========================================
  // READ GAS SENSOR
  // ========================================
  int gasValue = analogRead(gasSensor);

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  // ========================================
  // NORMAL CONDITION
  // ========================================
  if (!gasAlarm)
  {
    if (gasValue > gasThreshold)
    {
      gasCount++;

      Serial.print("Gas detected count: ");
      Serial.println(gasCount);

      // Three consecutive high readings
      if (gasCount >= requiredGasReadings)
      {
        gasAlarm = true;
        gasCount = 0;

        Serial.println("********************************");
        Serial.println("!!! GAS LEAK DETECTED !!!");
        Serial.println("********************************");

        // Red LED ON
        digitalWrite(redLED, HIGH);

        // Buzzer ON
        tone(buzzer, 1000);

        // Fan ON
        digitalWrite(relay, HIGH);

        // ==================================
        // SERVO CLOSE
        // ==================================
        gasServo.write(SERVO_CLOSED_ANGLE);

        Serial.println("Servo = 0 degrees");
        Serial.println("Valve = CLOSED");
        Serial.println("Fan = ON");
        Serial.println("Buzzer = ON");
      }
    }
    else
    {
      gasCount = 0;

      // LED OFF
      digitalWrite(redLED, LOW);

      // Buzzer OFF
      noTone(buzzer);

      // Fan OFF
      digitalWrite(relay, LOW);

      // Servo stays OPEN at 90°
      gasServo.write(SERVO_OPEN_ANGLE);
    }
  }

  // ========================================
  // GAS ALARM ACTIVE
  // ========================================
  else
  {
    // Keep alarm ON
    digitalWrite(redLED, HIGH);
    tone(buzzer, 1000);
    digitalWrite(relay, HIGH);

    // Keep valve CLOSED
    gasServo.write(SERVO_CLOSED_ANGLE);

    // ======================================
    // GAS BECOMES SAFE
    // ======================================
    if (gasValue < safeThreshold)
    {
      Serial.println("Gas level is safe.");
      Serial.println("Returning to normal...");

      gasAlarm = false;
      gasCount = 0;

      // Alarm OFF
      digitalWrite(redLED, LOW);
      noTone(buzzer);

      // Fan OFF
      digitalWrite(relay, LOW);

      // ==================================
      // SERVO OPEN
      // ==================================
      gasServo.write(SERVO_OPEN_ANGLE);

      Serial.println("Servo = 90 degrees");
      Serial.println("Valve = OPEN");
      Serial.println("Fan = OFF");
      Serial.println("Buzzer = OFF");
      Serial.println("System Normal");
    }
  }

  Serial.println("--------------------------------");

  delay(200);
}
