#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Map.h"
#include "Structure.h"
#include "Noise/noise.h"

const int WIDTH = 800;
const int HEIGHT = 600;

const int POINT_SIZE = 2;
const int LINE_SIZE = 1;

struct InfoShown
{
	enum class Name
	{
		Elevation,
		Moisture,
		Biomes
	};
};

InfoShown::Name VideoMode;