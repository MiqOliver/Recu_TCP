#pragma once
#include <mutex>
#include <vector>

using namespace std;

namespace graphics {

	#define SCREEN_WIDTH 800
	#define SCREEN_HEIGHT 600

	#define MAP_0 "../../rsc/sprites/map_0.png"
	#define MAP_1 "../../rsc/sprites/map_1.png"
	#define MAP_2 "../../rsc/sprites/map_2.png"
	#define MAP_3 "../../rsc/sprites/map_3.png"
	#define MAP_4 "../../rsc/sprites/map_4.png"
	#define MAP_5 "../../rsc/sprites/map_5.png"

	#define RES_1 "../../rsc/sprites/res_1.png"
	#define RES_2 "../../rsc/sprites/res_2.png"
	#define RES_3 "../../rsc/sprites/res_3.png"
	#define RES_4 "../../rsc/sprites/res_4.png"
	#define RES_5 "../../rsc/sprites/res_5.png"

	#define NUMBER_2 "../../rsc/sprites/number_2.png"
	#define NUMBER_3 "../../rsc/sprites/number_3.png"
	#define NUMBER_4 "../../rsc/sprites/number_4.png"
	#define NUMBER_5 "../../rsc/sprites/number_5.png"
	#define NUMBER_6 "../../rsc/sprites/number_6.png"
	#define NUMBER_8 "../../rsc/sprites/number_8.png"
	#define NUMBER_9 "../../rsc/sprites/number_9.png"
	#define NUMBER_10 "../../rsc/sprites/number_10.png"
	#define NUMBER_11 "../../rsc/sprites/number_11.png"
	#define NUMBER_12 "../../rsc/sprites/number_12.png"

	#define FONT "../../rsc/fonts/courbd.ttf"
}

namespace utils {

	enum Protocol { INFOPLAYER, INFOENEMY, START, TURN, 
		DICE, DEAL, RESOURCES, CONSTRUCTION, POINTS, LOGRO, ENDTURN, 
		CONNECTIONFAILURE, STRUCTURES, CARDS, ENDGAME, ERROR, DISCONNECT };

	enum Resource { WOOD, SHEEP, STRAW, STEEL, CLAY, DESERT };

	enum Structures { TOWN, CITY, BRIDGE, NONE };

	static std::mutex mu;
	static std::string nick;

	std::string msg = "";
	static bool myTurn = false;
	static int playerTurn = 0;

	Protocol prot;

	#define PORT 5000
	#define MAX_PLAYERS 4
	#define MAX_MENSAJES 30
	#define MAX_LENGTH 250

	#define NUM_CELLS 19
	#define NUM_NODES 54
	#define NUM_EDGES 72

	#define MAX_WOOD 4
	#define MAX_SHEEP 4
	#define MAX_STRAW 4
	#define MAX_STEEL 3
	#define MAX_CLAY 3
	#define MAX_DESERT 1

	#define MAX_DISCARD 7
	#define MAX_TOWN 5
	#define MAX_CITY 4
	#define MAX_BRIDGE 15

	#define MINE_COLOR sf::Color(0, 160, 0) 
	#define COLOR_0 sf::Color(0, 160, 160) 
	#define COLOR_1 sf::Color(160, 0, 0) 
	#define COLOR_2 sf::Color(200, 200, 200)
}