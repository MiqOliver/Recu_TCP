#pragma once
#include <string>
#include <SFML\Network.hpp>

struct Direction {

	Direction(std::string nick, std::string ip, unsigned short port, unsigned short id) :	nick(nick),
																							ip(ip),
																							port(port),
																							id(id) {
	}
	Direction() = default;

	std::string nick;
	std::string ip;
	unsigned short port;
	unsigned short id;
};