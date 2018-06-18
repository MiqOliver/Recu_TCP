#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <mutex>
#include "Utils.hpp"

using namespace utils;
using namespace std;


struct Receptor {
public:
	vector<sf::TcpSocket*>* sock;
	sf::SocketSelector selector;
	char receivedText[MAX_LENGTH];
	size_t receivedLength;
	int playerTurn = 0;

	Receptor(vector<sf::TcpSocket*>* sock) : sock(sock){
	}

	void operator() () {
		bool end = false;
		while (!end) {
			if (selector.wait()) {
				for (int i = 0; i < sock->size(); i++) {
					if (selector.isReady(*sock->at(i))) {
						sf::Packet pack;
						Protocol prot;
						int p;
						sf::Socket::Status status = sock->at(i)->receive(pack);
						if (status == sf::Socket::Done) {
							pack >> p;
							prot = (Protocol)p;
							switch (prot) {
							case INFOPLAYER:
								break;
							case DEAL:
								break;
							case CONSTRUCTION:
								break;
							case ENDTURN:
								playerTurn++;
								if (playerTurn > 4)
									playerTurn = 0;

								break;
							default:
								break;
							}
						}
						else if (status == sf::Socket::Disconnected) {
							selector.remove(*sock->at(i));
							sock->at(i)->disconnect();
							sock->erase(sock->begin() + i);
							std::cout << "Elimino el socket que se ha desconectado" << endl;
							end = true;
							break;
						}
						else {
							cout << "Error al recibir mensaje" << endl;
						}
					}
				}
			}
		}
	}
};