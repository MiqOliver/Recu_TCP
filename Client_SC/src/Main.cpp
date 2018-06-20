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
#include <Game.h>

using namespace utils;

void main() {
	sf::TcpSocket sock;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::Packet pck;

	// Copia del mundo necesaria para el cliente
	// Los parametros de players y el mismo, junto a los del enemigo. No guarda direcciones.
	vector<Player*> players;
	Enemy myEnemy;

	nick = "";
	std::cout << "Enter your NickName:" << endl;
	std::cin >> nick;

#pragma region Connection
	// Connect to server
	if (sock.connect(ip, PORT) != sf::Socket::Done)
		cout << "No se puede conectar al servidor" << endl;
	else {
		sock.send(nick.c_str(), nick.length());

		cout << "Esperando Players..." << endl;
		if (sock.receive(pck) != sf::Socket::Done)
			cout << "No se pudo recibir la informacion de players" << endl;
		else {
			int x;
			pck >> x;
			Protocol prot = (Protocol)x;
			// Se intercambia datos iniciales con otros jugadores
			for (int i = 0; i < MAX_PLAYERS; i++) {
				Player* newPlayer = new Player();
				pck >> newPlayer->nick >> newPlayer->id >> newPlayer->life >> newPlayer->attack >> newPlayer->mAttack;
				cout << i << " - Player: " << newPlayer->nick << " - " << newPlayer->id << endl;
				if (newPlayer->nick == nick) {
					myID = i;
				}
				players.push_back(newPlayer);
			}
			pck.clear();
		}
	}
#pragma endregion

#pragma region Start
	// Se recibe la situación inicial
	if (sock.receive(pck) != sf::Socket::Done)
		cout << "No se pudo recibir la situacion inicial" << endl;
	else {
		int x;
		pck >> x;
		Protocol prot = (Protocol)x;
		pck >> myEnemy.life >> myEnemy.attack >> myEnemy.mAttack;
		cout << "Enemigo Recibido!" << endl;
	}
	pck.clear();

	//  El servidor marca el inicio de partida
	if (sock.receive(pck) != sf::Socket::Done)
		cout << "No se pudo recibir inicio de partida" << endl;
	else {
		int x;
		pck >> x;
		Protocol prot = (Protocol)x;
		pck >> playerTurn;
		cout << "START GAME!" << endl;
	}
	pck.clear();
#pragma endregion

	Game myGame = Game(&sock, players);

	myGame.Run();
	
#pragma region Disconnect
	sf::Packet chaoPck;
	chaoPck << (int)prot;
	if (sock.send(chaoPck) != sf::Socket::Done)
		cout << "No se pudo enviar la desconexion al Server" << endl;
	else {
		cout << "Informacion de desconexion enviada al Server" << endl;
	}
	sock.disconnect();
#pragma endregion
}