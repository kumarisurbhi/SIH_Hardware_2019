
#include <Wire.h>
const int MPU2=0x69,MPU1=0x68;
int temperature;
long loopTimer, loopTimer2;
long acc_x, acc_y, acc_z;
double accel_x, accel_y, accel_z;
long acc_x2, acc_y2, acc_z2;
double accel_x2, accel_y2, accel_z2;
double gyroRoll, gyroPitch, gyroYaw;
double gyroRoll2, gyroPitch2, gyroYaw2;
int gyro_x, gyro_y, gyro_z;
int gyro_x2, gyro_y2, gyro_z2;
long gyro_x_cal = -224, gyro_y_cal = 374, gyro_z_cal = 110;
long gyro_x_cal2 = -224, gyro_y_cal2= 366, gyro_z_cal2 = 112;
double rotation_x, rotation_y, rotation_z;
double rotation_x2, rotation_y2, rotation_z2;
double tau = 0.98;
double roll = 0;
double pitch = 0;
double tau2 = 0.98;
double roll2 = 0;
double pitch2 = 0;
double freq, dt;
double accelPitch, accelRoll;
double accelPitch2, accelRoll2;
// 250 deg/s --> 131.0, 500 deg/s --> 65.5, 1000 deg/s --> 32.8, 2000 deg/s --> 16.4
long scaleFactorGyro = 65.5;

// 2g --> 16384 , 4g --> 8192 , 8g --> 4096, 16g --> 2048
long scaleFactorAccel = 8192;
void setup(){
     // Start
  Wire.begin();
  Serial.begin(9600);
  setup_mpu_registers(MPU1);
  setup_mpu_registers(MPU2);
   // Calibration
  Serial.println("Calibrating gyro, place on level surface and do not move.");

  // Take 3000 readings for each coordinate and then find average offset
  
 // Average the values
  gyro_x_cal /= 3000;
  gyro_y_cal /= 3000;
  gyro_z_cal /= 3000;
 
 // Display headers
  Serial.print("\nNote 1: Yaw is not filtered and will drift!\n");
  Serial.print("\nNote 2: Make sure sampling frequency is ~250 Hz\n");
  Serial.print("Sampling Frequency (Hz)\t\t");
  Serial.print("Roll (deg)\t\t");
  Serial.print("Pitch (deg)\t\t");
  Serial.print("Yaw (deg)\t\t\n");
  delay(2000);

}
void setup_mpu_registers( const int MPU)
{
     //Activate the MPU-6050
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  // Configure the accelerometer
  // Wire.write(0x__);
  // Wire.write; 2g --> 0x00, 4g --> 0x08, 8g --> 0x10, 16g --> 0x18
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0x08);
  Wire.endTransmission();

  // Configure the gyro
  // Wire.write(0x__);
  // 250 deg/s --> 0x00, 500 deg/s --> 0x08, 1000 deg/s --> 0x10, 2000 deg/s --> 0x18
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission();
}
void read_mpu_6050_data()
{
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 14);

  // Read data --> Temperature falls between acc and gyro registers
  while(Wire.available() < 14);
  acc_x = Wire.read() << 8 | Wire.read();
  acc_y = Wire.read() << 8 | Wire.read();
  acc_z = Wire.read() << 8 | Wire.read();
  temperature = Wire.read() <<8 | Wire.read();
  gyro_x = Wire.read()<<8 | Wire.read();
  gyro_y = Wire.read()<<8 | Wire.read();
  gyro_z = Wire.read()<<8 | Wire.read();

}
void read_mpu_6050_data2()
{
  Wire.beginTransmission(0x69);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x69, 14);

  // Read data --> Temperature falls between acc and gyro registers
  while(Wire.available() < 14);
  acc_x2 = Wire.read() << 8 | Wire.read();
  acc_y2 = Wire.read() << 8 | Wire.read();
  acc_z2 = Wire.read() << 8 | Wire.read();
  temperature = Wire.read() <<8 | Wire.read();
  gyro_x2 = Wire.read()<<8 | Wire.read();
  gyro_y2 = Wire.read()<<8 | Wire.read();
  gyro_z2 = Wire.read()<<8 | Wire.read();

}
void loop()
{
  freq = 1/((micros() - loopTimer2) * 1e-6);
  loopTimer2 = micros();
  dt = 1/freq;
    // Read the raw acc data from MPU-6050
  read_mpu_6050_data2();
    // Read the raw acc data from second MPU-6050
  read_mpu_6050_data();
  gyro_x -= gyro_x_cal;
  gyro_y -= gyro_y_cal;
  gyro_z -= gyro_z_cal;

  // Convert to instantaneous degrees per second
  rotation_x = (double)gyro_x / (double)scaleFactorGyro;
  rotation_y = (double)gyro_y / (double)scaleFactorGyro;
  rotation_z = (double)gyro_z / (double)scaleFactorGyro;

  // Convert to g force
  accel_x = (double)acc_x / (double)scaleFactorAccel;
  accel_y = (double)acc_y / (double)scaleFactorAccel;
  accel_z = (double)acc_z / (double)scaleFactorAccel;

  // Complementary filter
  accelPitch = atan2(accel_y, accel_z) * RAD_TO_DEG;
  accelRoll = atan2(accel_x, accel_z) * RAD_TO_DEG;

  pitch = (tau)*(pitch + rotation_x*dt) + (1-tau)*(accelPitch);
  roll = (tau)*(roll - rotation_y*dt) + (1-tau)*(accelRoll);

  gyroPitch += rotation_x*dt;
  gyroRoll -= rotation_y*dt;
  gyroYaw += rotation_z*dt;

  Serial.print("first mpu");
  // Data out serial monitor
  Serial.print(freq,0);   Serial.print(",");
  Serial.print(roll,1);   Serial.print(",");
  Serial.print(pitch,1);  Serial.print(",");
  Serial.print(gyroYaw,1);
  Serial.print('\t');

  // Wait until the loopTimer reaches 4000us (250Hz) before next loop
  while (micros() - loopTimer <= 4000);
  loopTimer = micros();

  //For second mpu
  gyro_x2 -= gyro_x_cal2;
  gyro_y2 -= gyro_y_cal2;
  gyro_z2 -= gyro_z_cal2;

  // Convert to instantaneous degrees per second
  rotation_x2 = (double)gyro_x2 / (double)scaleFactorGyro;
  rotation_y2 = (double)gyro_y2 / (double)scaleFactorGyro;
  rotation_z2 = (double)gyro_z2 / (double)scaleFactorGyro;

  // Convert to g force
  accel_x2 = (double)acc_x2 / (double)scaleFactorAccel;
  accel_y2 = (double)acc_y2 / (double)scaleFactorAccel;
  accel_z2 = (double)acc_z2 / (double)scaleFactorAccel;

  // Complementary filter
  accelPitch2 = atan2(accel_y2, accel_z2) * RAD_TO_DEG;
  accelRoll2 = atan2(accel_x2, accel_z2) * RAD_TO_DEG;

  pitch2 = (tau2)*(pitch2 + rotation_x2*dt) + (1-tau2)*(accelPitch2);
  roll2 = (tau2)*(roll2 - rotation_y2*dt) + (1-tau2)*(accelRoll2);

  gyroPitch2 += rotation_x2*dt;
  gyroRoll2 -= rotation_y2*dt;
  gyroYaw2 += rotation_z2*dt;
  // Data out serial monitor
  Serial.print("Second mpu readings");
  Serial.print(freq,0);   Serial.print(",");
  Serial.print(roll2,1);   Serial.print(",");
  Serial.print(pitch2,1);  Serial.print(",");
  Serial.println(gyroYaw2,1);
  
  // Wait until the loopTimer reaches 4000us (250Hz) before next loop
  while (micros() - loopTimer <= 4000);
  loopTimer = micros();
}

