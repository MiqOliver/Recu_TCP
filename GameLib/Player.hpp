#pragma once
#include <string>
#include <SFML\Network.hpp>

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
	int mana = 100;
	int attack = 10;
	int mAttack = 15;
	int defense = 10;
	int mDefense = 5;
	bool alive = true;

};	

class Enemy {
public:
	Enemy() = default;

	int life = 300;
	int attack = 30;
	int mAttack = 30;
	bool alive = true;
};

