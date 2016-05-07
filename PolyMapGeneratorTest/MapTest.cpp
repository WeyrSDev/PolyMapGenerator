#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Map.h"
#include "Structure.h"

const int WIDTH = 800;
const int HEIGHT = 600;

const int POINT_SIZE = 0;
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

sf::Color DELAUNAY_COLOR = sf::Color::Black;
sf::Color VORONOI_COLOR = sf::Color(sf::Uint8(52), sf::Uint8(58), sf::Uint8(94), sf::Uint8(127));
sf::Color WATER_COLOR = sf::Color(sf::Uint8(52), sf::Uint8(58), sf::Uint8(94));
sf::Color LAND_COLOR = sf::Color(sf::Uint8(178), sf::Uint8(166), sf::Uint8(148));
sf::Color LAKE_COLOR = sf::Color(sf::Uint8(95), sf::Uint8(134), sf::Uint8(169));
sf::Color RIVER_COLOR = sf::Color(sf::Uint8(40), sf::Uint8(88), sf::Uint8(132));

const sf::Color ELEVATION_COLOR[] =
{
	sf::Color(sf::Uint8(104), sf::Uint8(134), sf::Uint8(89)),
	sf::Color(sf::Uint8(119), sf::Uint8(153), sf::Uint8(102)),
	sf::Color(sf::Uint8(136), sf::Uint8(166), sf::Uint8(121)),
	sf::Color(sf::Uint8(153), sf::Uint8(179), sf::Uint8(148)),
	sf::Color(sf::Uint8(170), sf::Uint8(191), sf::Uint8(159)),
	sf::Color(sf::Uint8(187), sf::Uint8(204), sf::Uint8(179)),
	sf::Color(sf::Uint8(204), sf::Uint8(217), sf::Uint8(198)),
	sf::Color(sf::Uint8(221), sf::Uint8(230), sf::Uint8(217)),
	sf::Color(sf::Uint8(238), sf::Uint8(242), sf::Uint8(236)),
	sf::Color(sf::Uint8(251), sf::Uint8(252), sf::Uint8(251))
};

const sf::Color MOISTURE_COLOR[] =
{
	sf::Color(sf::Uint8(238), sf::Uint8(238), sf::Uint8(32)),
	sf::Color(sf::Uint8(218), sf::Uint8(238), sf::Uint8(32)),
	sf::Color(sf::Uint8(197), sf::Uint8(238), sf::Uint8(32)),
	sf::Color(sf::Uint8(176), sf::Uint8(238), sf::Uint8(32)),
	sf::Color(sf::Uint8(155), sf::Uint8(238), sf::Uint8(32)),
	sf::Color(sf::Uint8(135), sf::Uint8(238), sf::Uint8(32)),
	sf::Color(sf::Uint8(115), sf::Uint8(238), sf::Uint8(32)),
	sf::Color(sf::Uint8(94), sf::Uint8(238), sf::Uint8(32)),
	sf::Color(sf::Uint8(73), sf::Uint8(238), sf::Uint8(32)),
	sf::Color(sf::Uint8(52), sf::Uint8(238), sf::Uint8(32)),
	sf::Color(sf::Uint8(32), sf::Uint8(238), sf::Uint8(32))
};

const sf::Color BIOME_COLOR[] =
{
	sf::Color(sf::Uint8(248), sf::Uint8(248), sf::Uint8(248)),
	sf::Color(sf::Uint8(221), sf::Uint8(221), sf::Uint8(187)),
	sf::Color(sf::Uint8(153), sf::Uint8(153), sf::Uint8(153)),
	sf::Color(sf::Uint8(204), sf::Uint8(212), sf::Uint8(187)),
	sf::Color(sf::Uint8(196), sf::Uint8(204), sf::Uint8(187)),
	sf::Color(sf::Uint8(228), sf::Uint8(232), sf::Uint8(202)),
	sf::Color(sf::Uint8(164), sf::Uint8(196), sf::Uint8(168)),
	sf::Color(sf::Uint8(180), sf::Uint8(201), sf::Uint8(169)),
	sf::Color(sf::Uint8(196), sf::Uint8(212), sf::Uint8(170)),
	sf::Color(sf::Uint8(156), sf::Uint8(187), sf::Uint8(169)),
	sf::Color(sf::Uint8(169), sf::Uint8(204), sf::Uint8(164)),
	sf::Color(sf::Uint8(233), sf::Uint8(221), sf::Uint8(199)),
	sf::Color(sf::Uint8(52), sf::Uint8(58), sf::Uint8(94)),
	sf::Color(sf::Uint8(95), sf::Uint8(134), sf::Uint8(169)),
	sf::Color(sf::Uint8(178), sf::Uint8(166), sf::Uint8(148))
};

void DrawLine(Vector2 a, Vector2 b, double width, sf::Color c, sf::RenderWindow* window);
void DrawEdge(Edge* e, sf::RenderWindow* window);
void DrawCorner(Corner* c, sf::RenderWindow* window);
void DrawCenter(Center* c, sf::RenderWindow* window);

int main()
{
	sf::Clock timer;

	VideoMode = InfoShown::Name::Biomes;

	sf::RenderWindow* app = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT, 32), "Map Generator", sf::Style::Default, sf::ContextSettings(24, 8, 8, 3, 3));
	app->setFramerateLimit(60);

	Map map(WIDTH, HEIGHT, 10, "");

	timer.restart();
	map.Generate();
	std::cout << timer.getElapsedTime().asMicroseconds() / 1000.0 << std::endl;

	std::vector<Edge*> edges = map.GetEdges();
	std::vector<Corner*> corners = map.GetCorners();
	std::vector<Center*> centers = map.GetCenters();

	std::vector<sf::ConvexShape> polygons;
	for (auto center : centers)
	{
		sf::ConvexShape polygon;
		polygon.setPointCount(center->m_corners.size());

		for (int i = 0; i < center->m_corners.size(); ++i)
		{
			Vector2 aux = center->m_corners[i]->m_position;
			polygon.setPoint(i, sf::Vector2f(aux.x, aux.y));
		}

		polygon.setFillColor(BIOME_COLOR[static_cast<int>(center->m_biome)]);
		polygon.setPosition(0, 0);
		polygons.push_back(polygon);
	}

	Center* selectedCenter = nullptr;

	bool running = true;
	while (running)
	{
		sf::Event event;
		while (app->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				running = false;
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				sf::Image screen;

				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
					running = false;
					break;
				case sf::Keyboard::M:
					VideoMode = InfoShown::Name::Moisture;
					break;
				case sf::Keyboard::B:
					VideoMode = InfoShown::Name::Biomes;
					break;
				case sf::Keyboard::E:
					VideoMode = InfoShown::Name::Elevation;
					break;
				case sf::Keyboard::F12:
					screen = app->capture();
					screen.saveToFile("screenshot.jpg");
					break;
				default:
					break;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					timer.restart();
					selectedCenter = map.GetCenterAt(Vector2(event.mouseButton.x, event.mouseButton.y));
				}
			}
		}

		app->clear(sf::Color::White);

		if (!centers.empty())
		{
			timer.restart();

			for (auto center : centers)
			{
				DrawCenter(center, app);
			}
		}

		if (!edges.empty())
		{
			for (auto edge : edges)
			{
				DrawEdge(edge, app);
			}
		}

		if (!corners.empty())
		{
			for (auto corner : corners)
			{
				DrawCorner(corner, app);
			}
		}

		if (selectedCenter != nullptr)
		{
			sf::ConvexShape polygon;
			polygon.setPointCount(selectedCenter->m_corners.size());

			convex_hull(selectedCenter->m_corners);

			for (int i = 0; i < selectedCenter->m_corners.size(); ++i)
			{
				Vector2 aux = selectedCenter->m_corners[i]->m_position;
				polygon.setPoint(i, sf::Vector2f(aux.x, aux.y));
				std::cout << aux.x << " " << aux.y << std::endl;
			}
			polygon.setFillColor(sf::Color::Black);
			polygon.setPosition(0, 0);
			polygons.push_back(polygon);
			app->draw(polygon);
		}

		app->display();
	}

	return 0;
}

void DrawLine(Vector2 a, Vector2 b, double width, sf::Color c, sf::RenderWindow* window)
{
	Vector2 lineVector(a, b);
	sf::RectangleShape line(sf::Vector2f(lineVector.Length(), width));

	line.setFillColor(c);
	line.setRotation(lineVector.GetAngleByDegree());
	line.setPosition(a.x, a.y);

	window->draw(line);
}

void DrawEdge(Edge* e, sf::RenderWindow* window)
{
	Vector2 v0;
	if (e->m_v0 == nullptr)
	{
		v0 = (e->m_d0->m_position + e->m_d1->m_position) / 2;
	}
	else
	{
		v0 = e->m_v0->m_position;
	}

	Vector2 v1;
	if (e->m_v1 == nullptr)
	{
		v1 = (e->m_d0->m_position + e->m_d1->m_position) / 2;
	}
	else
	{
		v1 = e->m_v1->m_position;
	}

	if (e->m_riverVolume > 0)
	{
		DrawLine(v0, v1, 1 + sqrt(e->m_riverVolume), RIVER_COLOR, window);
	}
	else
	{
		DrawLine(v0, v1, 1, VORONOI_COLOR, window);
	}
}

void DrawCorner(Corner* c, sf::RenderWindow* window)
{
	sf::CircleShape point;

	if (c->m_water)
	{
		point.setFillColor(WATER_COLOR);
	}
	else
	{
		point.setFillColor(LAND_COLOR);
	}

	point.setPosition(c->m_position.x - POINT_SIZE, c->m_position.y - POINT_SIZE);
	point.setRadius(POINT_SIZE);
	window->draw(point);
}

void DrawCenter(Center* c, sf::RenderWindow* window)
{
	sf::ConvexShape polygon;
	polygon.setPointCount(c->m_corners.size());

	convex_hull(c->m_corners);

	for (int i = 0; i < c->m_corners.size(); ++i)
	{
		Vector2 aux = c->m_corners[i]->m_position;
		polygon.setPoint(i, sf::Vector2f(aux.x, aux.y));
	}

	switch (VideoMode)
	{
	case InfoShown::Name::Biomes:
 		polygon.setFillColor(BIOME_COLOR[static_cast<int>(c->m_biome)]);
		break;
	case InfoShown::Name::Elevation:
		if (c->m_ocean)
		{
			polygon.setFillColor(WATER_COLOR);
		}
		else if (c->m_water)
		{
			polygon.setFillColor(LAKE_COLOR);
		}
		else
		{
			polygon.setFillColor(ELEVATION_COLOR[static_cast<int>(floor(c->m_elevation * 10))]);
		}
		break;
	case InfoShown::Name::Moisture:
		if (c->m_ocean)
		{
			polygon.setFillColor(WATER_COLOR);
		}
		else if (c->m_water)
		{
			polygon.setFillColor(LAKE_COLOR);
		}
		else
		{
			polygon.setFillColor(MOISTURE_COLOR[static_cast<int>(floor(c->m_moisture * 10))]);
		}
		break;
	default:
		break;
	}

	polygon.setPosition(0, 0);
	window->draw(polygon);
}