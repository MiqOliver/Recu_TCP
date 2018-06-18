#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <Map.hpp>
#include <Player.hpp>
#include "Renderer.hpp"

using namespace std;
using namespace utils;

void main() {
	sf::TcpSocket sock;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	Map myMap = Map(); 
	vector<Player> players;
	Player myPlayer = Player();

	std::cout << "Enter your NickName:" << endl;
	std::cin >> nick;

#pragma region Connection
	//Connect to server
	if (sock.connect(ip, PORT) != sf::Socket::Done)
		cout << "No se puede conectar al servidor" << endl;
	else {
		sock.send(nick.c_str(), nick.length());

		sf::Packet infoPlayers;
		cout << "Esperando Players..." << endl;
		if (sock.receive(infoPlayers) != sf::Socket::Done)
			cout << "No se pudo recibir la informacion de players" << endl;
		else {
			int x;
			infoPlayers >> x;
			Protocol prot = (Protocol)x;
			for (int i = 0; i < MAX_PLAYERS; i++) {
				Player newPlayer = Player();
				infoPlayers >> newPlayer.nick >> newPlayer.id;
				cout << i << " - Player: " << newPlayer.nick << " - " << newPlayer.id << endl;
				if (newPlayer.nick == nick)
					myPlayer = newPlayer;
				else
					players.push_back(newPlayer);
			}
		}
	}

#pragma endregion

	bool myTurn = false;
	int firstTurns = 0;

	R.CreateWindow();
	R.SetVariables(&myTurn);
	//thread draw(R);
	
#pragma region Loop

	while (R.IsOpen()) {
		// The first 2 turns to put the first buildings
		while (firstTurns < 2) {
			if (myTurn) {
				sf::Packet packBuild;
				Protocol prot;
				Structures build;
				prot = utils::CONSTRUCTION;
				build = utils::TOWN;
				packBuild << (int)prot << 1 << (int)build;
				if (sock.send(packBuild) != sf::Socket::Done)
					cout << "No se pudo enviar el edificio al Server" << endl;
				else {
					cout << "Informacion de edificio enviada al Server" << endl;
					sf::Packet packBridge;
					build = utils::BRIDGE;
					packBridge << (int)prot << 1 << (int)build;
					if (sock.send(packBridge) != sf::Socket::Done)
						cout << "No se pudo enviar el puente al Server" << endl;
					else {
						cout << "Informacion del puente enviada al Server" << endl;
						sf::Packet packEnd;
						prot = utils::ENDTURN;
						packBridge << (int)prot;
						if (sock.send(packEnd) != sf::Socket::Done)
							cout << "No se pudo enviar el fin de turno al Server" << endl;
						else {
							cout << "Informacion del fin de turno enviada al Server" << endl;
							firstTurns++;
						}
					}
				}
			}
			R.Draw();
		}
		system("cls");
		cout << "-----------RESOURCES-----------" << endl;
		cout << "-- STEEL: " << myPlayer.cards[STEEL] << endl;
		cout << "-- WOOD: " << myPlayer.cards[WOOD] << endl;
		cout << "-- CLAY: " << myPlayer.cards[CLAY] << endl;
		cout << "-- SHEEP: " << myPlayer.cards[SHEEP] << endl;
		cout << "-- STRAW: " << myPlayer.cards[STRAW] << endl;
		cout << "-------------------------------" << endl;
		if (myTurn) {

		}
		else {

		}
		R.Draw();
	}
#pragma endregion

	R.Draw();
}