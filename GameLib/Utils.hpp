#pragma once
#include <mutex>
#include <vector>
#include "Player.hpp"

using namespace std;

namespace graphics {

	#define SCREEN_WIDTH 800
	#define SCREEN_HEIGHT 600

	#define BG1 "../../rsc/sprites/bg1.png"
	#define BG2 "../../rsc/sprites/bg2.png"

	#define PERFIL_1 "../../rsc/sprites/perfil1.png"
	#define PERFIL_2 "../../rsc/sprites/perfil2.png"
	#define PERFIL_3 "../../rsc/sprites/perfil3.png"
	#define PERFIL_4 "../../rsc/sprites/perfil4.png"

	#define PLAYER_1 "../../rsc/sprites/player1.png"
	#define PLAYER_2 "../../rsc/sprites/player2.png"
	#define PLAYER_3 "../../rsc/sprites/player3.png"
	#define PLAYER_4 "../../rsc/sprites/player4.png"

	#define BOSS "../../rsc/sprites/boss.png"

	#define BUTTON "../../rsc/sprites/button.png"

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
		MSG, DEAL, RESOURCES, CONSTRUCTION, POINTS, LOGRO, ENDTURN, 
		CONNECTIONFAILURE, STRUCTURES, CARDS, ENDGAME, ERROR, DISCONNECT };

	enum Resource { WOOD, SHEEP, STRAW, STEEL, CLAY, DESERT };

	enum Structures { TOWN, CITY, BRIDGE, NONE };

	static std::mutex mu;
	static std::string nick;

	static bool end = false;
	std::string msg = "";
	static bool myTurn = false;
	static int playerTurn = 0;
	static int myID;

	Protocol prot;

	vector<Player*> players;

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
	#define OTHER_COLOR sf::Color(200, 200, 200)
	#define DIE_COLOR sf::Color(50, 50, 50)
	#define HP_COLOR sf::Color(76, 187, 23)
	#define MANA_COLOR sf::Color(15, 82, 186)
	#define BLACK sf::Color(0, 0, 0)

	#define POS_Y 245
	#define POS_Y_TURN 215
}