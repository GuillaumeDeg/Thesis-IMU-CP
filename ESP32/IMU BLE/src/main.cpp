#include <Arduino.h>
#include <BluetoothSerial.h>
#include "ICM_20948.h"

/* Check if Bluetooth configurations are enabled in the SDK */
/* If not, then you have to recompile the SDK */
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define SERIAL_PORT Serial
#define WIRE_PORT Wire
#define AD0_VAL 1
#define CLOCK 80
int i = 1;

ICM_20948_I2C myICM;
BluetoothSerial SerialBT;

void printStructed(ICM_20948_I2C *sensor);
void IRAM_ATTR onTimer();
volatile bool interruptflag = false;
// void loop() {
//   if (Serial.available())
//   {
//     SerialBT.write(Serial.read());
//   }
//   if (SerialBT.available())
//   {
//     Serial.write(SerialBT.read());
//   }
// }

void setup()
{
  setCpuFrequencyMhz(CLOCK);
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  SerialBT.begin("IMU");
  // SERIAL_PORT.println("Bluetooth Started! Ready to pair...");
  SERIAL_PORT.begin(115200);
  while (!SERIAL_PORT){};
  int Freq = getCpuFrequencyMhz();
  Serial.print("CPU Freq = ");
  Serial.print(Freq);
  Serial.println(" MHz");
  Freq = getXtalFrequencyMhz();
  Serial.print("XTAL Freq = ");
  Serial.print(Freq);
  Serial.println(" MHz");
  Freq = getApbFrequency();
  Serial.print("APB Freq = ");
  Serial.print(Freq);
  Serial.println(" Hz");
  WIRE_PORT.begin();
  WIRE_PORT.setClock(400000);

  // myICM.enableDebugging()

  bool initialized = false;
  while (!initialized)
  {
    myICM.begin(WIRE_PORT, AD0_VAL);

    // SERIAL_PORT.print(F("Initialization of the sensor returned: "));
    // SERIAL_PORT.println(myICM.statusString());

    if (myICM.status != ICM_20948_Stat_Ok)
    {
      // SERIAL_PORT.println("Trying again...");
      delay(500);
    }
    else
    {
      initialized = true;
      // SERIAL_PORT.println("Device connected!");
    }
  }

  // Here we are doing a SW reset to make sure the device starts in a known state
  myICM.swReset();
  if (myICM.status != ICM_20948_Stat_Ok)
  {
    // SERIAL_PORT.print(F("Software Reset returned: "));
    // SERIAL_PORT.println(myICM.statusString());
  }
  delay(250);

  // Now wake the sensor up
  myICM.sleep(false);
  myICM.lowPower(false);

  // Set Gyro and Accelerometer to a particular sample mode
  // options: ICM_20948_Sample_Mode_Continuous
  //          ICM_20948_Sample_Mode_Cycled
  myICM.setSampleMode((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), ICM_20948_Sample_Mode_Continuous);
  if (myICM.status != ICM_20948_Stat_Ok)
  {
    // SERIAL_PORT.print(F("setSampleMode returned: "));
    // SERIAL_PORT.println(myICM.statusString());
  }

  // Set full scale ranges for both acc and gyr
  ICM_20948_fss_t myFSS; // This uses a "Full Scale Settings" structure that can contain values for all configurable sensors

  myFSS.a = gpm16; // (ICM_20948_ACCEL_CONFIG_FS_SEL_e)
                  // gpm2
                  // gpm4
                  // gpm8
                  // gpm16

  myFSS.g = dps2000; // (ICM_20948_GYRO_CONFIG_1_FS_SEL_e)
                    // dps250
                    // dps500
                    // dps1000
                    // dps2000

  myICM.setFullScale((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myFSS);
  if (myICM.status != ICM_20948_Stat_Ok)
  {
    // SERIAL_PORT.print(F("setFullScale returned: "));
    // SERIAL_PORT.println(myICM.statusString());
  }

  // Set up Digital Low-Pass Filter configuration
  ICM_20948_dlpcfg_t myDLPcfg;  // Similar to FSS, this uses a configuration structure for the desired sensors
  myDLPcfg.a = acc_d5bw7_n8bw3; // (ICM_20948_ACCEL_CONFIG_DLPCFG_e)
                                // acc_d246bw_n265bw      - means 3db bandwidth is 246 hz and nyquist bandwidth is 265 hz
                                // acc_d111bw4_n136bw
                                // acc_d50bw4_n68bw8
                                // acc_d23bw9_n34bw4
                                // acc_d11bw5_n17bw
                                // acc_d5bw7_n8bw3        - means 3 db bandwidth is 5.7 hz and nyquist bandwidth is 8.3 hz
                                // acc_d473bw_n499bw

  myDLPcfg.g = gyr_d5bw7_n8bw9; // (ICM_20948_GYRO_CONFIG_1_DLPCFG_e)
                                // gyr_d196bw6_n229bw8
                                // gyr_d151bw8_n187bw6
                                // gyr_d119bw5_n154bw3
                                // gyr_d51bw2_n73bw3
                                // gyr_d23bw9_n35bw9
                                // gyr_d11bw6_n17bw8
                                // gyr_d5bw7_n8bw9
                                // gyr_d361bw4_n376bw5

  // myICM.setDLPFcfg((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), myDLPcfg);
  if (myICM.status != ICM_20948_Stat_Ok)
  {
    // SERIAL_PORT.print(F("setDLPcfg returned: "));
    // SERIAL_PORT.println(myICM.statusString());
  }

  // Choose whether or not to use DLPF
  // Here we're also showing another way to access the status values, and that it is OK to supply individual sensor masks to these functions
  // ICM_20948_Status_e accDLPEnableStat = myICM.enableDLPF(ICM_20948_Internal_Acc, false);
  // ICM_20948_Status_e gyrDLPEnableStat = myICM.enableDLPF(ICM_20948_Internal_Gyr, false);
  // ICM_20948_Status_e DLPEnableStat = myICM.enableDLPF((ICM_20948_Internal_Acc | ICM_20948_Internal_Gyr), true);
  // SERIAL_PORT.print(F("Enable DLPF for Accelerometer returned: "));
  // SERIAL_PORT.println(myICM.statusString(accDLPEnableStat));
  // SERIAL_PORT.print(F("Enable DLPF for Gyroscope returned: "));
  // SERIAL_PORT.println(myICM.statusString(gyrDLPEnableStat));
  // SERIAL_PORT.print(F("Enable DLPF returned: "));
  // SERIAL_PORT.println(myICM.statusString(DLPEnableStat));

  // Choose whether or not to start the magnetometer
  myICM.startupMagnetometer();
  if (myICM.status != ICM_20948_Stat_Ok)
  {
    // SERIAL_PORT.print(F("startupMagnetometer returned: "));
    // SERIAL_PORT.println(myICM.statusString());
  }

  // SERIAL_PORT.println();
  // SERIAL_PORT.println(F("Configuration complete!"));

  int freq = 100;
  int counter = 80000000 / (256 * freq);
  hw_timer_t *timer = timerBegin(0, 256, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, counter, true);
  timerAlarmEnable(timer);
}

void loop()
{
  if (!interruptflag)
    return;
  if (myICM.dataReady())
  {
    interruptflag = false;
    myICM.getAGMT();
    printStructed(&myICM);
  }
  else
  {
    // SERIAL_PORT.println("Waiting for data");
  }
}

void IRAM_ATTR onTimer()
{
  interruptflag = true;
}

// Below here are some helper functions to print the data nicely!

void printStructed(ICM_20948_I2C *sensor)
{
  // SERIAL_PORT.print(sensor->accX());
  // SERIAL_PORT.print("\t");
  // SERIAL_PORT.print(sensor->accY());
  // SERIAL_PORT.print("\t");
  // SERIAL_PORT.print(sensor->accZ());
  // SERIAL_PORT.print("\t");
  // SERIAL_PORT.print(sensor->gyrX());
  // SERIAL_PORT.print("\t");
  // SERIAL_PORT.print(sensor->gyrY());
  // SERIAL_PORT.print("\t");
  // SERIAL_PORT.print(sensor->gyrZ());
  // SERIAL_PORT.print("\t");
  // SERIAL_PORT.print(sensor->magX());
  // SERIAL_PORT.print("\t");
  // SERIAL_PORT.print(sensor->magY());
  // SERIAL_PORT.print("\t");
  // SERIAL_PORT.print(sensor->magZ());
  // SERIAL_PORT.println();
  char *string;
  int num;
  // if (0 > (num = asprintf(&string, "%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f", i++, sensor->accX(), sensor->accY(), sensor->accZ(), sensor->gyrX(), sensor->gyrY(), sensor->gyrZ(), sensor->magX(), sensor->magY(), sensor->magZ())))
  //   return;
  // SerialBT.println(string);
  SerialBT.printf("%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\r\n", sensor->accX(), sensor->accY(), sensor->accZ(), sensor->gyrX(), sensor->gyrY(), sensor->gyrZ(), sensor->magX(), sensor->magY(), sensor->magZ());
  // if (0 > (num = asprintf(&string, "%d", i)))
  //   return;
  // SERIAL_PORT.println(string);
  // if (0 > (num = asprintf(&string, "2 %d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f", i++, sensor->accX(), sensor->accY(), sensor->accZ(), sensor->gyrX(), sensor->gyrY(), sensor->gyrZ(), sensor->magX(), sensor->magY(), sensor->magZ())))
  //   return;
  // SerialBT.println(string);
  free(string);
}