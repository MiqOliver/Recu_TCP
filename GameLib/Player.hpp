#pragma once
#include <string>
#include <SFML\Network.hpp>
#include <Utils.hpp>

using namespace utils;

class Player {
public:
	Player(std::string _nick, std::string _ip, unsigned short _port, unsigned short _id) :	nick(_nick),
																							ip(_ip),
																							port(_port),
																							id(_id) {
		cards[STEEL] = 0;
		cards[WOOD] = 0;
		cards[CLAY] = 0;
		cards[SHEEP] = 0;
		cards[STRAW] = 0;
		points = 0;
	}

	Player() = default;

	std::string nick;
	std::string ip;
	unsigned short port;
	unsigned short id;
	std::map<Resource, int> cards;
	int points = 0;
};	

