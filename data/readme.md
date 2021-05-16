# Store randomized data here

Datafile's name is in the form: data_`kSeed`\_`kSize`\_`kConnectionRange`\_`kNumberOfSensors`\_`kDefaultConsumingRate`.txt.

* `kSeed`: seed use to randomizing.
* `kSize`: our map is a square of size `kSize` x `kSize`.
* `kConnectionRange`: radius of each sensors.
* `kNumberOfSensors`: ?
* `kDefaultConsumingRate`: default consuming rate of a sensor when it doesn't have to transmit data for others.

Datafile's structure:

* `E_max`, `E_mc_max`, `r`, `r_c`, `r_m`, `r_mc`
* `n`
* Each following line contains `x`, `y`, `p_i`

With:
* `E_max`: capacity of a sensor
* `E_mc_max`: capacity of a mobile charger
* `r`: energy to sensor rate
* `r_c`: energy from MC rate
* `r_m`: energy for moving per unit distance
* `r_m`: MC charging rate
* `n`: number of sensor in the network
* `x`, `y`: coordinate of a sensor
* `p_i`: energy consuming rate of a sensor

# Default parameters for all test cases
const unsigned int kSeed = 0; // for randomizing
const int kSize = 50; // kSize * kSize plane with (0,0) being the BS
const int kConnectionRange = 20; // radius of a sensor, ensure that the map is CONNECTED
const int kNumberOfSensors = 100;
const int kDefaultConsumingRate = 10;

const int kEMax = 10000;
const int kEMcMax = 100000;
const int kToSensorRate = 5000; // r
const int kFromMcRate = 5010; // r_c
const int kMovingRate = 100; // r_m
const int kToMcRate = 1000000; // r_mc