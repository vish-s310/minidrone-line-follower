#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

const float alpha = 0.98; 
float dt = 0.5; 

float accAngleX, accAngleY;
float gyroAngleX, gyroAngleY;
float angleX = 0, angleY = 0;  

unsigned long lastTime = 0;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); 

  Serial.println("Adafruit MPU6050 with Complementary Filter");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accAngleX = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
  accAngleY = atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;

  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0; 
  lastTime = currentTime;

  gyroAngleX += g.gyro.x * dt; 
  gyroAngleY += g.gyro.y * dt;

  angleX = alpha * (angleX + g.gyro.x * dt) + (1 - alpha) * accAngleX;
  angleY = alpha * (angleY + g.gyro.y * dt) + (1 - alpha) * accAngleY;

  Serial.print("Filtered Angle X: ");
  Serial.println(angleX);
  Serial.print("Filtered Angle Y: ");
  Serial.println(angleY);

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Gyro X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.println("");
  delay(500); 
}