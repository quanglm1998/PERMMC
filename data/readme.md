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