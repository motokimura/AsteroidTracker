AsteroidTracker
===
A library to track asteroids, written in C/C++.

## Description
This library enables you to calculate the position and velocity of asteroids that go around the Sun. 
The direction (azimuth and elevation) to the asteroid and doppler rate at your location can be also calculated 
so that you can use this for asteroid tracking at your ground station.

## Usage
A sample program is written in main.cpp. The orbit parameters defined as Keplerian elements in the sample are adjusted to ones of ARTSAT space-probe, [DESPATCH](http://artsat.jp/en/project/despatch). To track another asteroids, specify its Keplerian elements using setAsteroidOrbitInfo() function that is defined in asteroid_tracker.cpp.

## License

This software is released under the MIT License, see LICENSE.txt.

## Author

[Motoki Kimura](https://github.com/motokimura)
