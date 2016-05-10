# Polygonal Map Generator

###### 

## About

Procedural Content Generation for Polygonal Maps.

The main idea behind the algorithm is taken from [Amit's Polygonal Map Generation for Games](http://www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation/).

The main code of this repository is based on [Rellikiox's Map Generator](https://github.com/Rellikiox/MapGenerator).

## Features

### To-do

### Doing

* Code refactoring - No warning messages at warning level 4. 

### Done

* Place a number of random but uniformly distributed points (using Poisson Disk Sampling).
* Calculate the Voronoi diagram of the points.
* Using Perlin Noise determine which cells are land and which are water.
* Calculate the elevation of each point as its distance to the sea. Normalize all the heights.
* Place river sources in random spots and let them flow downwards.
* Calculate the moisture od each point given its distance to the sea (less moisture) and rivers (more moisture)

## Screenshots

### Biomes

<img src="https://github.com/utilForever/PolyMapGenerator/blob/master/Screenshots/Biome%20-%20Version%201.jpg"/>

### Moistures

<img src="https://github.com/utilForever/PolyMapGenerator/blob/master/Screenshots/Moisture%20-%20Version%201.jpg"/>

### Elevations

<img src="https://github.com/utilForever/PolyMapGenerator/blob/master/Screenshots/Elevation%20-%20Version%201.jpg"/>

## Dependencies

* libnoise
* SFML