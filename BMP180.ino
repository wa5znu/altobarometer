/* Updated by Leigh L. Klotz, Jr. WA5ZNU */

/* SFE_BMP180 altitude example sketch

This sketch shows how to use the Bosch BMP180 pressure sensor
as an altimiter.
https://www.sparkfun.com/products/11824

Like most pressure sensors, the BMP180 measures absolute pressure.
Since absolute pressure varies with altitude, you can use the pressure
to determine your altitude.

Because pressure also varies with weather, you must first take a pressure
reading at a known baseline altitude. Then you can measure variations
from that pressure

Hardware connections:

- (GND) to GND
+ (VDD) to 3.3V

(WARNING: do not connect + to 5V or the sensor will be damaged!)

You will also need to connect the I2C pins (SCL and SDA) to your
Arduino. The pins are different on different Arduinos:

Any Arduino pins labeled:  SDA  SCL
Uno, Redboard, Pro:        A4   A5
Mega2560, Due:             20   21
Leonardo:                   2    3

Leave the IO (VDDIO) pin unconnected. This pin is for connecting
the BMP180 to systems with lower logic levels such as 1.8V

Have fun! -Your friends at SparkFun.

The SFE_BMP180 library uses floating-point equations developed by the
Weather Station Data Logger project: http://wmrx00.sourceforge.net/

Our example code uses the "beerware" license. You can do anything
you like with this code. No really, anything. If you find it useful,
buy me a beer someday.

V10 Mike Grusin, SparkFun Electronics 10/24/2013
*/

// Your sketch must #include this library, and the Wire library.
// (Wire is a standard library included with Arduino.):

#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 bmp180;

double baseline;

#define DEBUGPRINT 0

void setup_bmp180()
{
#if DEBUGPRINT
  Serial.begin(9600);
  Serial.println("REBOOT");
#endif

  // Initialize the sensor (it is important to get calibration values stored on the device).

  if (bmp180.begin()) {
#if DEBUGPRINT
    Serial.println("BMP180 init success");
#endif
  } else {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.

#if DEBUGPRINT
    Serial.println("BMP180 init fail (disconnected?)\n\n");
#endif
    while(1); // Pause forever.
  }

  // Get the baseline pressure:
  
  baseline = getPressure();
  
#if DEBUGPRINT
  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb");  
#endif
}

double getPressure()
{
  // You must first get a temperature measurement to perform a pressure reading.
  double P,T;
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  char status = bmp180.startTemperature();
  if (status != 0)
    {
      // Wait for the measurement to complete:

      delay(status);

      // Retrieve the completed temperature measurement:
      // Note that the measurement is stored in the variable T.
      // Use '&T' to provide the address of T to the function.
      // Function returns 1 if successful, 0 if failure.

      status = bmp180.getTemperature(T);
      if (status != 0) {
        // Start a pressure measurement:
        // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
        // If request is successful, the number of ms to wait is returned.
        // If request is unsuccessful, 0 is returned.

        status = bmp180.startPressure(3);
        if (status != 0) {
          // Wait for the measurement to complete:
          delay(status);

          // Retrieve the completed pressure measurement:
          // Note that the measurement is stored in the variable P.
          // Use '&P' to provide the address of P.
          // Note also that the function requires the previous temperature measurement (T).
          // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
          // Function returns 1 if successful, 0 if failure.

          status = bmp180.getPressure(P,T);
          if (status != 0) {
            return(P);
          } else {
#if DEBUGPRINT
            Serial.println("error retrieving pressure measurement\n");
#endif
          }
        } else {
#if DEBUGPRINT
          Serial.println("error starting pressure measurement\n");
#endif
        }
      } else {
#if DEBUGPRINT
        Serial.println("error retrieving temperature measurement\n");
#endif
      }
    } else {
#if DEBUGPRINT
    Serial.println("error starting temperature measurement\n");
#endif
  }
}



double getAltitude() {

  // Get a new pressure reading:

  double p = getPressure();

  // Show the relative altitude difference between
  // the new reading and the baseline reading:

  const float to_feet = 3.28084;
  return bmp180.altitude(p,baseline) * to_feet;
  //return bmp180.altitude(baseline,baseline) * 10;
}
