#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <mutex>
#include "Utils.hpp"

using namespace utils;
using namespace std;

struct Receptor_Threading {
public:
	sf::TcpSocket* sock;
	vector<pair<string, string>>* aMsj;
	pair<string, string> message;
	sf::Packet myPacket;
	size_t receivedLength;

	Receptor_Threading(sf::TcpSocket* sock, vector<pair<string, string>>* aMsj) : sock(sock),
		aMsj(aMsj) {
	}

	void operator() () {
		bool end = false;
		while (!end) {
			sf::Socket::Status status = sock->receive(myPacket);
			if (status != sf::Socket::Done) {
				cout << "Ha fallado la recepcion de datos\n";
				end = true;
			}
			else {
				myPacket >> message.first >> message.second;
				mu.lock();
				aMsj->push_back(message);
				if (aMsj->size() > 25) {
					aMsj->erase(aMsj->begin(), aMsj->begin() + 1);
				}
				mu.unlock();
			}
		}
	}
};
