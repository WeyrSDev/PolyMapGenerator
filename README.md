# Polygonal Map Generator

###### Polygonal map generator for games. Generates island maps with a focus on mountains, rivers, coastlines.

## About

Procedural Content Generation for Polygonal Maps.

The main idea behind the algorithm is taken from [Amit's Polygonal Map Generation for Games](http://www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation/).

The main code of this repository is based on [Rellikiox's Map Generator](https://github.com/Rellikiox/MapGenerator).

## Features

### To-do

* Using OpenGL or DirectX to represent 3D terrain (consider biomes, moistures, and elevations).
* Implement math library for Vector3 to structure 3-dimensional points.
* Add parameters to control map generation (e.g. the number of islands, maximum heights of map, and so on).
* Add polygonal map generator for basement (e.g. lava, mineral, cave, ground water, and so on).

### Doing

* Code refactoring - No warning messages at warning level 4. 

### Done

* Place a number of random but uniformly distributed points (using Poisson Disk Sampling).
* Calculate the Voronoi diagram of the points.
* Using Perlin Noise determine which cells are land and which are water.
* Calculate the elevation of each point as its distance to the sea. Normalize all the heights.
* Place river sources in random spots and let them flow downwards.
* Calculate the moisture od each point given its distance to the sea (less moisture) and rivers (more moisture).
* Using Conex Hull algorithm determine the order of points (for sf::ConvexShape). 

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

## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

The class is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2016 [Chris Ohk](http://www.github.com/utiLForever)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.