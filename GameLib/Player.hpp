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
		
	}

	Player() = default;

	std::string nick;
	std::string ip;
	unsigned short port;
	unsigned short id;
	int life = 100;
	int attack = 10;
	int mAttack = 15;

};	

class Enemy {
public:
	Enemy() = default;

	int life = 300;
	int attack = 30;
	int mAttack = 30;
};

