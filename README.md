# PolyMapGenerator

============

TBA

Generation of procedural maps. The main idea behind the algorithm is taken from [Amit's Polygonal Map Generation for Games](http://www-cs-students.stanford.edu/~amitp/game-programming/polygon-map-generation/).

It has the following steps:

* Place a number of random but uniformly distributed points (using Poisson Disk Sampling). \[Done!\]
* Calculate the Voronoi diagram of the points. \[Doing...\]
* Using Perlin Noise determine which cells are land and which are water.
* Calculate the elevation of each point as its distance to the sea. Normalize all the heights.
* Place river sources in random spots and let them flow downwards.
* Calculate the moisture od each point given its distance to the sea (less moisture) and rivers (more moisture)

And that's pretty much it. The repo also contains a Poisson Disk Sampling implementation and a Markov Chains based name generator.

## Screenshots

### Biomes

<img src="https://github.com/utilForever/PolyMapGenerator/blob/master/Screenshots/Biome%20-%20Version%201.jpg"/>

### Moistures

<img src="https://github.com/utilForever/PolyMapGenerator/blob/master/Screenshots/Moisture%20-%20Version%201.jpg"/>

### Elevations

<img src="https://github.com/utilForever/PolyMapGenerator/blob/master/Screenshots/Elevation%20-%20Version%201.jpg"/>