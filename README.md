AsteroidTracker
===
A library to track asteroids, written in C/C++.

## Description
This library enables you to calculate the position and velocity of asteroids that go around the Sun. 
The direction (azimuth and elevation) to the asteroid and doppler rate at your location can be also calculated 
so that you can use this for asteroid tracking at your ground station.

## Usage
A sample program is written in main.cpp. The orbit parameters defined as Keplerian elements in the sample are adjusted to ones of [ARTSAT](http://artsat.jp) space-probe, [DESPATCH](http://artsat.jp/en/project/despatch). To track another asteroids, specify its Keplerian elements using setAsteroidOrbitInfo( ) function that is defined in asteroid_tracker.cpp.

## Projects
[DESPATCH Orbit 3D Viewer](http://artsat.jp/en/project/despatch/orbit)

[DESPATCH in Starlit Sky](http://artsat.jp/en/project/despatch/celestial)

[DESPATCH Tracking API](http://api.artsat.jp/pass/)

## Licence

[MIT License](LICENSE.txt)

## Team

[Motoki Kimura](https://github.com/motokimura)

[ARTSAT Project](https://github.com/ARTSAT)
