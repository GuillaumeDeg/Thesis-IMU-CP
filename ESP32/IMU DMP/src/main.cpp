/****************************************************************
 * Example9_DMP_MultipleSensors.ino
 * ICM 20948 Arduino Library Demo
 * Initialize the DMP based on the TDK InvenSense ICM20948_eMD_nucleo_1.0 example-icm20948
 * Paul Clark, April 25th, 2021
 * Based on original code by:
 * Owen Lyke @ SparkFun Electronics
 * Original Creation Date: April 17 2019
 *
 * ** This example is based on InvenSense's _confidential_ Application Note "Programming Sequence for DMP Hardware Functions".
 * ** We are grateful to InvenSense for sharing this with us.
 *
 * ** Important note: by default the DMP functionality is disabled in the library
 * ** as the DMP firmware takes up 14301 Bytes of program memory.
 * ** To use the DMP, you will need to:
 * ** Edit ICM_20948_C.h
 * ** Uncomment line 29: #define ICM_20948_USE_DMP
 * ** Save changes
 * ** If you are using Windows, you can find ICM_20948_C.h in:
 * ** Documents\Arduino\libraries\SparkFun_ICM-20948_ArduinoLibrary\src\util
 *
 * Please see License.md for the license information.
 *
 * Distributed as-is; no warranty is given.
 ***************************************************************/

#include "ICM_20948.h"  // Click here to get the library: http://librarymanager/All#SparkFun_ICM_20948_IMU

#define SERIAL_PORT Serial

#define WIRE_PORT Wire  // Your desired Wire port.
#define AD0_VAL 1       // The value of the last bit of the I2C address.                \
                       // On the SparkFun 9DoF IMU breakout the default is 1, and when \
                       // the ADR jumper is closed the value becomes 0

ICM_20948_I2C myICM;  // Otherwise create an ICM_20948_I2C object

void setup() {
    SERIAL_PORT.begin(115200);  // Start the serial console
    SERIAL_PORT.println(F("ICM-20948 Example"));

    delay(100);

    while (SERIAL_PORT.available())  // Make sure the serial RX buffer is empty
        SERIAL_PORT.read();

    WIRE_PORT.begin();
    WIRE_PORT.setClock(400000);

    // myICM.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial

    bool initialized = false;
    while (!initialized) {
        // Initialize the ICM-20948
        // If the DMP is enabled, .begin performs a minimal startup. We need to configure the sample mode etc. manually.
        myICM.begin(WIRE_PORT, AD0_VAL);

        SERIAL_PORT.print(F("Initialization of the sensor returned: "));
        SERIAL_PORT.println(myICM.statusString());
        if (myICM.status != ICM_20948_Stat_Ok) {
            SERIAL_PORT.println(F("Trying again..."));
            delay(500);
        } else {
            initialized = true;
        }
    }

    SERIAL_PORT.println(F("Device connected!"));

    bool success = true;  // Use success to show if the DMP configuration was successful

    // Initialize the DMP. initializeDMP is a weak function. You can overwrite it if you want to e.g. to change the sample rate
    success &= (myICM.initializeDMP() == ICM_20948_Stat_Ok);

    // DMP sensor options are defined in ICM_20948_DMP.h
    //    INV_ICM20948_SENSOR_ACCELEROMETER               (16-bit accel)
    //    INV_ICM20948_SENSOR_GYROSCOPE                   (16-bit gyro + 32-bit calibrated gyro)
    //    INV_ICM20948_SENSOR_RAW_ACCELEROMETER           (16-bit accel)
    //    INV_ICM20948_SENSOR_RAW_GYROSCOPE               (16-bit gyro + 32-bit calibrated gyro)
    //    INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED (16-bit compass)
    //    INV_ICM20948_SENSOR_GYROSCOPE_UNCALIBRATED      (16-bit gyro)
    //    INV_ICM20948_SENSOR_STEP_DETECTOR               (Pedometer Step Detector)
    //    INV_ICM20948_SENSOR_STEP_COUNTER                (Pedometer Step Detector)
    //    INV_ICM20948_SENSOR_GAME_ROTATION_VECTOR        (32-bit 6-axis quaternion)
    //    INV_ICM20948_SENSOR_ROTATION_VECTOR             (32-bit 9-axis quaternion + heading accuracy)
    //    INV_ICM20948_SENSOR_GEOMAGNETIC_ROTATION_VECTOR (32-bit Geomag RV + heading accuracy)
    //    INV_ICM20948_SENSOR_GEOMAGNETIC_FIELD           (32-bit calibrated compass)
    //    INV_ICM20948_SENSOR_GRAVITY                     (32-bit 6-axis quaternion)
    //    INV_ICM20948_SENSOR_LINEAR_ACCELERATION         (16-bit accel + 32-bit 6-axis quaternion)
    //    INV_ICM20948_SENSOR_ORIENTATION                 (32-bit 9-axis quaternion + heading accuracy)

    // Enable additional sensors / features
    success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_ACCELEROMETER) == ICM_20948_Stat_Ok);
    // success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_GYROSCOPE_UNCALIBRATED) == ICM_20948_Stat_Ok);
    // success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_GEOMAGNETIC_FIELD) == ICM_20948_Stat_Ok);

    // Configuring DMP to output data at multiple ODRs:
    // DMP is capable of outputting multiple sensor data at different rates to FIFO.
    // Setting value can be calculated as follows:
    // Value = (DMP running rate / ODR ) - 1
    // E.g. For a 5Hz ODR rate when DMP is running at 55Hz, value = (55/5) - 1 = 10.
    success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Accel, 54) == ICM_20948_Stat_Ok);  // Set to 55Hz
    // success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Gyro, 54) == ICM_20948_Stat_Ok);         // Set to 55Hz
    // success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Cpass_Calibr, 54) == ICM_20948_Stat_Ok);  // Set to 55Hz

    // Enable the FIFO
    success &= (myICM.enableFIFO() == ICM_20948_Stat_Ok);

    // Enable the DMP
    success &= (myICM.enableDMP() == ICM_20948_Stat_Ok);

    // Reset DMP
    success &= (myICM.resetDMP() == ICM_20948_Stat_Ok);

    // Reset FIFO
    success &= (myICM.resetFIFO() == ICM_20948_Stat_Ok);

    // Check success
    if (success) {
        SERIAL_PORT.println(F("DMP enabled!"));
    } else {
        SERIAL_PORT.println(F("Enable DMP failed!"));
        SERIAL_PORT.println(F("Please check that you have uncommented line 29 (#define ICM_20948_USE_DMP) in ICM_20948_C.h..."));
        while (1)
            ;  // Do nothing more
    }
}
int k = 0;
int sum = 0;
float angle = 0;
int sum2 = 0;
float angle2 = 0;
void loop() {
    icm_20948_DMP_data_t data;
    while (myICM.readDMPdataFromFIFO(&data) == (ICM_20948_Stat_FIFONoDataAvail))
        ;

    // if ((data.header & DMP_header_bitmap_Gyro) > 0)  // Check for Gyro calibrated
    // {
    //     k++;
    //     int div = 32;
    //     int16_t x = data.Raw_Gyro.Data.X;  // Extract the raw gyro data
    //     int16_t y = data.Raw_Gyro.Data.Y;
    //     int16_t z = data.Raw_Gyro.Data.Z;
    //     int16_t xBias = data.Raw_Gyro.Data.BiasX >> 5;
    //     int16_t yBias = data.Raw_Gyro.Data.BiasY >> 5;
    //     int16_t zBias = data.Raw_Gyro.Data.BiasZ >> 5;
    //     sum = sum + ((k > 10 && xBias) ? (int)(x - xBias) : 0);
    //     angle += (k > 10 && xBias) ? ((float)(x - xBias) * 1.0f / 155.0f) : 0;
    //     SERIAL_PORT.printf("Gyr_X:%d \tGyr_Y:%d \tGyr_Z:%d\n", x - xBias, y - yBias, z - zBias);
    // }

    // if ((data.header & DMP_header_bitmap_Compass_Calibr) > 0)  // Check for Gyro calibrated
    // {
    //     float scale_div = 1.0f / (1 << 16);
    //     float x = (int32_t)data.Compass_Calibr.Data.X * scale_div;
    //     float y = (int32_t)data.Compass_Calibr.Data.Y * scale_div;
    //     float z = (int32_t)data.Compass_Calibr.Data.Z * scale_div;
    //     SERIAL_PORT.printf("Mag_Cal_X:%f \tMag_Cal_Y:%f \tMag_Cal_Z:%f\n", x, y, z);
    // }

    if ((data.header & DMP_header_bitmap_Accel) > 0)  // Check for Gyro calibrated
    {
        // float scale_div = 1.0f / (1 << 16);
        float x = (int16_t)data.Raw_Accel.Data.X;
        float y = (int16_t)data.Raw_Accel.Data.Y;
        float z = (int16_t)data.Raw_Accel.Data.Z;
        SERIAL_PORT.printf("Acc_Cal_X:%f \tAcc_Cal_Y:%f \tAcc_Cal_Z:%f\n", x, y, z);
    }

    // get calibrations, 0 = uncalibrated, 3 = fully calibrated
    if ((data.header2 & DMP_header2_bitmap_Accel_Accuracy) > 0)  // Check for Compass calibrated
    {
        uint16_t aq = (uint16_t)data.Accel_Accuracy;
        SERIAL_PORT.println("---------------------------------");
        SERIAL_PORT.printf("Acceleration Accuracy:%d\n", aq);
        SERIAL_PORT.println("---------------------------------");
    }
    if ((data.header2 & DMP_header2_bitmap_Gyro_Accuracy) > 0)  // Check for Compass calibrated
    {
        uint16_t aq = (uint16_t)data.Gyro_Accuracy;
        SERIAL_PORT.println("---------------------------------");
        SERIAL_PORT.printf("Gyroscope Accuracy:%d\n", aq);
        SERIAL_PORT.println("---------------------------------");
    }
    if ((data.header2 & DMP_header2_bitmap_Compass_Accuracy) > 0)  // Check for Compass calibrated
    {
        uint16_t aq = (uint16_t)data.Compass_Accuracy;
        SERIAL_PORT.println("---------------------------------");
        SERIAL_PORT.printf("Magnetometer Accuracy:%d\n", aq);
        SERIAL_PORT.println("---------------------------------");
    }
}