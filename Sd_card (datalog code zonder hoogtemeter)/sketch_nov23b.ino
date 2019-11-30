#include <SD.h> //Load SD library
#include <Wire.h>
#include "i2c.h"
#include <TimerOne.h>

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  
  // Read Nbytes
  Wire.requestFrom(Address, Nbytes); 
  uint8_t index=0;
  while (Wire.available())
    Data[index++]=Wire.read();
}

void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}


int chipSelect = 4; //chip select pin for the MicroSD Card Adapter
File file; // file object that is used to read and write data

  long int ti;

  volatile bool intFlag=false;

long int cpt=0;

void setup() {
  Serial.begin(9600); // start file connection to print out debug messages and data
  Wire.begin();
  
  // Set accelerometers low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,29,0x06);
  // Set gyroscope low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,26,0x06);
 
  
  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_1000_DPS);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_4_G);
  // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_ADDRESS,0x37,0x02);
  
  // Request continuous magnetometer measurements in 16 bits
  I2CwriteByte(MAG_ADDRESS,0x0A,0x16);
  
   pinMode(13, OUTPUT);
  Timer1.initialize(1000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
 

  // Store initial time
  ti=millis();
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) { // Initialize SD card
    file.println("Could not initialize SD card."); // if return value is false, something went wrong.
  }

  if (SD.exists("file.txt")) { // if "file.txt" exists, fill will be deleted
    Serial.println("File exists.");
    if (SD.remove("file.txt") == true) {
      Serial.println("Successfully removed file.");
    } else {
      Serial.println("Could not remove file.");
    }
  }

}

void callback()
{ 
  intFlag=true;
  digitalWrite(13, digitalRead(13) ^ 1);
}

void loop() {

  while (!intFlag);
  intFlag=false;
  
  // Display time
  Serial.print (millis()-ti,DEC);
  Serial.print ("\t");

  
  // _______________
  // ::: Counter :::
  
  // Display data counter
// Serial.print (cpt++,DEC);
//  Serial.print ("\t");
  
 
 
  // ____________________________________
  // :::  accelerometer and gyroscope ::: 

  // Read accelerometer and gyroscope
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);
  
  // Create 16 bits values from 8 bits data
  
  // Accelerometer
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t az=Buf[4]<<8 | Buf[5];

  // Gyroscope
  int16_t gx=-(Buf[8]<<8 | Buf[9]);
  int16_t gy=-(Buf[10]<<8 | Buf[11]);
  int16_t gz=Buf[12]<<8 | Buf[13];
  
    // Display values
  
  // Accelerometer
  Serial.print (ax,DEC); 
  Serial.print ("\t");
  Serial.print (ay,DEC);
  Serial.print ("\t");
  Serial.print (az,DEC);  
  Serial.print ("\t");
  
  // Gyroscope
  Serial.print (gx,DEC); 
  Serial.print ("\t");
  Serial.print (gy,DEC);
  Serial.print ("\t");
  Serial.print (gz,DEC);  
  Serial.print ("\t");

  
  // _____________________
  // :::  Magnetometer ::: 

  
  // Read register Status 1 and wait for the DRDY: Data Ready
  
  uint8_t ST1;
  do
  {
    I2Cread(MAG_ADDRESS,0x02,1,&ST1);
  }
  while (!(ST1&0x01));

  // Read magnetometer data  
  uint8_t Mag[7];  
  I2Cread(MAG_ADDRESS,0x03,7,Mag);
  

  // Create 16 bits values from 8 bits data
  
  // Magnetometer
  int16_t mx=-(Mag[3]<<8 | Mag[2]);
  int16_t my=-(Mag[1]<<8 | Mag[0]);
  int16_t mz=-(Mag[5]<<8 | Mag[4]);
  
  
  // Magnetometer
  Serial.print (mx+200,DEC); 
  Serial.print ("\t");
  Serial.print (my-70,DEC);
  Serial.print ("\t");
  Serial.print (mz-700,DEC);  
  Serial.print ("\t");
  
  
  
  // End of line
 // Serial.println("");

  int tijd;
  tijd = millis();

  file = SD.open("file.txt", FILE_WRITE); // open "file.txt" to write data
  if (file) {
    file.print("Tijdstip: ! ");
    file.print(tijd); // schrijf waarden naar bestand
    file.println(" ms;");
    file.print("Versnellingsmeter: & ");  
    file.print (ax,DEC); 
    file.print ("\t");
    file.print (ay,DEC);
    file.print ("\t");
    file.print (az,DEC);  
    file.println ("\t");
    file.print("Gyroscoop: * ");  
    file.print (gx,DEC); 
    file.print ("\t");
    file.print (gy,DEC);
    file.print ("\t");
    file.print (gz,DEC);  
    file.println ("\t");
    file.print("Magnetometer: ? ");  
    file.print (mx+200,DEC); 
    file.print ("\t");
    file.print (my-70,DEC);
    file.print ("\t");
    file.print (mz-700,DEC);  
    file.println ("\t");
    file.close(); // close file
    Serial.print("Data naar sd geschreven: "); // debug output: show written number in serial monitor
  } else {
    Serial.println("Could not open file (writing).");
  }
}
