#include <SFML\Network.hpp>
#include <iostream>
#include <mutex>
#include <Utils.hpp>
#include <Direction.hpp>
#include <Player.hpp>
#include <Map.hpp>

using namespace std;
using namespace utils;


void CalculateResources(int num, Node* nodes[], Player* player) {
	for (int i = 0; i < NUM_NODES; i++) {
		if (nodes[i]->idCons == player->id) {
			int res = 1;
			if (nodes[i]->building == utils::CITY)
				res = 2;
			for each (Terrain* t in nodes[i]->ter) {
				if (t->num == num && !t->ladron)
					switch (t->res) {
					case WOOD:
						player->cards[WOOD] += res;
						break;
					case SHEEP:
						player->cards[SHEEP] += res;
						break;
					case STRAW:
						player->cards[STRAW] += res;
						break;
					case STEEL:
						player->cards[STEEL] += res;
						break;
					case CLAY:
						player->cards[CLAY] += res;
						break;
					default:
						break;
					}
			}
		}
	}
}

void CalculateFirstResources(Node* nodes[], Player* player) {
	for (int i = 0; i < NUM_NODES; i++) {
		if (nodes[i]->idCons == player->id) {
			int res = 1;
			for each (Terrain* t in nodes[i]->ter) {
				switch (t->res) {
				case WOOD:
					player->cards[WOOD] += res;
					break;
				case SHEEP:
					player->cards[SHEEP] += res;
					break;
				case STRAW:
					player->cards[STRAW] += res;
					break;
				case STEEL:
					player->cards[STEEL] += res;
					break;
				case CLAY:
					player->cards[CLAY] += res;
					break;
				default:
					break;
				}
			}
		}
	}
}

int main() {
#pragma region Connection
	//ESTABLECER CONEXION
	sf::TcpListener listener;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	vector<pair<sf::TcpSocket*, Player>> socks;
	sf::SocketSelector selector;
	Protocol prot;
	bool userDisconected = false;
	Map myMap = Map();
	int playerTurn = 0;

	//Ponemos el listener a escuchar por el puerto
	if (listener.listen(PORT) != sf::Socket::Done) {
		cout << "No se puede vincular al puerto 5000" << endl;
	}
	//Iteramos hasta que se conectan todos los jugadores
	for (int i = 0; i < MAX_PLAYERS; i++) {
		sf::TcpSocket* sock = new sf::TcpSocket();
		if (listener.accept(*sock) != sf::Socket::Done) {
			cout << "No se pudo conectar al socket" << endl;
		}
		//Nickname
		char receivedText[MAX_LENGTH] = {""};
		size_t receivedLength;
		sock->receive(receivedText, MAX_LENGTH, receivedLength);
		string nick = "";
		nick = receivedText;
		Player p(nick, sock->getRemoteAddress().toString(), sock->getRemotePort(), i);
		cout << "Socket " << p.id << ": NAME - " << nick  << endl;

		socks.push_back({ sock, p });
		selector.add(*sock);
	}
	listener.close();
#pragma endregion

#pragma region StartGame
	//Send all the info of the players
	for (int i = 0; i < MAX_PLAYERS; i++) {
		sf::Packet infoPlayers;
		prot = INFOPLAYER;
		infoPlayers << (int)prot;
		for (int j = 0; j < MAX_PLAYERS; j++) {
			infoPlayers << socks[j].second.nick << socks[j].second.id;
		}
		if (socks[i].first->send(infoPlayers) != sf::Socket::Done)
			cout << "No se pudo enviar la informacion de players al Socket " << i << endl;
		else
			cout << "Informacion de players enviada al Socket " << i << endl;
	}

	//First Loop for first constructions
	for (int i = 0; i < MAX_PLAYERS; i++) {
		// Send Turn
		sf::Packet sendPack;
		prot = YOURTURN;
		sendPack << (int)prot;
		if (socks[i].first->send(sendPack) != sf::Socket::Done)
			cout << "No se pudo enviar el turno al Socket " << i << endl;
		else
			cout << "Informacion de turno enviada al Socket " << i << endl;
		// Receive the first construction
		bool wait = true;
		while (wait) {
			sf::Packet receivePack;
			int p;
			sf::Socket::Status stat = socks[i].first->receive(receivePack);
			if (stat == sf::Socket::Disconnected) {
				userDisconected = true;
				wait = false;
			}
			else if (stat == sf::Socket::Error) {
				cout << "No se pudo recibir la acción del Socket " << i << endl;
				wait = false;
			}
			else {
				Structures build;
				int b;
				int node;
				receivePack >> p >> node >> b;
				build = (Structures)b;
				if (myMap.CanConstructNodeFirst(node)) { // Comprueba si se puede realizar la acción solicitada
					sf::Packet sendCons;
					myMap.Construct(socks[i].second.id, build, node);
					sendCons << p << socks[i].second.id << b << node;
					for (int j = 0; j < MAX_PLAYERS; j++) {
						if (socks[j].first->send(sendCons) != sf::Socket::Done) // Envia la construcción a todos los jugadores
							cout << "No se pudo enviar la construccion al Socket " << j << endl;
						else
							cout << "Informacion de construccion enviada al Socket " << j << endl;
					}
					wait = false;
				}
				else { // Si no se puede realizar la acción avisa al player
					sf::Packet sendError;
					p = ERROR;
					sendError << (int)p;
					if (socks[i].first->send(sendError) != sf::Socket::Done) // Envia el error al jugador
						cout << "No se pudo enviar el error al Socket " << i << endl;
					else
						cout << "Informacion de error enviada al Socket " << i << endl;
				}
			}
		}
		// Receive the first Bridge
		wait = true;
		while (wait) {
			sf::Packet receivePack;
			int p;
			sf::Socket::Status stat = socks[i].first->receive(receivePack);
			if (stat == sf::Socket::Disconnected) {
				userDisconected = true;
				wait = false;
			}
			else if (stat == sf::Socket::Error) {
				cout << "No se pudo recibir la acción del Socket " << i << endl;
				wait = false;
			}
			else {
				Structures build;
				int b;
				int node;
				receivePack >> p >> node >> b;
				build = (Structures)b;
				if (myMap.CanConstructBridge(socks[i].second.id, node)) { // Comprueba si se puede realizar la acción solicitada
					sf::Packet sendCons;
					myMap.Construct(socks[i].second.id, build, node);
					sendCons << p << socks[i].second.id << b << node;
					for (int j = 0; j < MAX_PLAYERS; j++) {
						if (socks[j].first->send(sendCons) != sf::Socket::Done) // Envia la construcción a todos los jugadores
							cout << "No se pudo enviar la construccion al Socket " << j << endl;
						else
							cout << "Informacion de construccion enviada al Socket " << j << endl;
					}
					wait = false;
				}
				else { // Si no se puede realizar la acción avisa al player
					sf::Packet sendError;
					p = ERROR;
					sendError << (int)p;
					if (socks[i].first->send(sendError) != sf::Socket::Done) // Envia el error al jugador
						cout << "No se pudo enviar el error al Socket " << i << endl;
					else
						cout << "Informacion de error enviada al Socket " << i << endl;
				}
			}
		}
	}

	//Secon Loop for first constructions
	for (int i = 3; i >= 0; i--) {
		// Send Turn
		sf::Packet sendPack;
		prot = YOURTURN;
		sendPack << (int)prot;
		if (socks[i].first->send(sendPack) != sf::Socket::Done)
			cout << "No se pudo enviar el turno al Socket " << i << endl;
		else
			cout << "Informacion de turno enviada al Socket " << i << endl;
		// Receive the first construction
		bool wait = true;
		while (wait) {
			sf::Packet receivePack;
			int p;
			sf::Socket::Status stat = socks[i].first->receive(receivePack);
			if (stat == sf::Socket::Disconnected) {
				userDisconected = true;
				wait = false;
			}
			else if (stat == sf::Socket::Error) {
				cout << "No se pudo recibir la acción del Socket " << i << endl;
				wait = false;
			}
			else {
				Structures build;
				int b;
				int node;
				receivePack >> p >> node >> b;
				build = (Structures)b;
				if (myMap.CanConstructNodeFirst(node)) { // Comprueba si se puede realizar la acción solicitada
					sf::Packet sendCons;
					myMap.Construct(socks[i].second.id, build, node);
					sendCons << p << socks[i].second.id << b << node;
					for (int j = 0; j < MAX_PLAYERS; j++) {
						if (socks[j].first->send(sendCons) != sf::Socket::Done) // Envia la construcción a todos los jugadores
							cout << "No se pudo enviar la construccion al Socket " << j << endl;
						else
							cout << "Informacion de construccion enviada al Socket " << j << endl;
					}
					wait = false;
				}
				else { // Si no se puede realizar la acción avisa al player
					sf::Packet sendError;
					p = ERROR;
					sendError << (int)p;
					if (socks[i].first->send(sendError) != sf::Socket::Done) // Envia el error al jugador
						cout << "No se pudo enviar el error al Socket " << i << endl;
					else
						cout << "Informacion de error enviada al Socket " << i << endl;
				}
			}
		}
		// Receive the first Bridge
		wait = true;
		while (wait) {
			sf::Packet receivePack;
			int p;
			sf::Socket::Status stat = socks[i].first->receive(receivePack);
			if (stat == sf::Socket::Disconnected) {
				userDisconected = true;
				wait = false;
			}
			else if (stat == sf::Socket::Error) {
				cout << "No se pudo recibir la acción del Socket " << i << endl;
				wait = false;
			}
			else {
				Structures build;
				int b;
				int node;
				receivePack >> p >> node >> b;
				build = (Structures)b;
				if (myMap.CanConstructBridge(socks[i].second.id, node)) { // Comprueba si se puede realizar la acción solicitada
					sf::Packet sendCons;
					myMap.Construct(socks[i].second.id, build, node);
					sendCons << p << socks[i].second.id << b << node;
					for (int j = 0; j < MAX_PLAYERS; j++) {
						if (socks[j].first->send(sendCons) != sf::Socket::Done) // Envia la construcción a todos los jugadores
							cout << "No se pudo enviar la construccion al Socket " << j << endl;
						else
							cout << "Informacion de construccion enviada al Socket " << j << endl;
					}
					wait = false;
				}
				else { // Si no se puede realizar la acción avisa al player
					sf::Packet sendError;
					p = ERROR;
					sendError << (int)p;
					if (socks[i].first->send(sendError) != sf::Socket::Done) // Envia el error al jugador
						cout << "No se pudo enviar el error al Socket " << i << endl;
					else
						cout << "Informacion de error enviada al Socket " << i << endl;
				}
			}
		}
	}

	//Se envian los recursos iniciales a cada uno de los players
	for (int i = 0; i < MAX_PLAYERS; i++) {
		sf::Packet resPack;
		prot = RECURSOS;
		resPack << (int)prot;
		CalculateFirstResources(myMap.nodes, &socks[i].second);
		for (int j = 0; j < 5; j++) {
			resPack << socks[i].second.cards[(Resource)j];
		}
		if (socks[i].first->send(resPack) != sf::Socket::Done)
			cout << "No se pudo enviar los recursos al Socket " << i << endl;
		else
			cout << "Informacion de recursos enviada al Socket " << i << endl;
	}

	sf::Packet startGame;
	prot = YOURTURN;
	startGame << (int)prot;
	socks[playerTurn].first->send(startGame);
#pragma endregion

#pragma region Loop
	while (socks.size() != 0) {
		if (selector.wait()) {
			for (int i = 0; i < socks.size(); i++) {
				if (selector.isReady(*socks[i].first)) {
					sf::Packet myPack;
					sf::Socket::Status status = socks[i].first->receive(myPack);
					if (status == sf::Socket::NotReady) {
						break;
					}
					else if (status == sf::Socket::Done) {
						int p;
						myPack >> p;
						prot = (Protocol)p;
						sf::Packet sendPack;
						int num = 6;
						switch (prot) {
						case CONSTRUCTION:
							Structures build;
							int b;
							int node;
							myPack >> p >> node >> b;
							build = (Structures)b;
							if (myMap.CanConstructNode(socks[i].second.id, node)) { // Comprueba si se puede realizar la acción solicitada
								sf::Packet sendCons;
								myMap.Construct(socks[i].second.id, build, node);
								sendCons << p << socks[i].second.id << b << node;
								for (int j = 0; j < MAX_PLAYERS; j++) {
									if (socks[j].first->send(sendCons) != sf::Socket::Done) // Envia la construcción a todos los jugadores
										cout << "No se pudo enviar la construccion al Socket " << j << endl;
									else
										cout << "Informacion de construccion enviada al Socket " << j << endl;
								}
							}
							else if (myMap.CanConstructBridge(socks[i].second.id, node)) { // Comprueba si se puede realizar la acción solicitada
								sf::Packet sendCons;
								myMap.Construct(socks[i].second.id, build, node);
								sendCons << p << socks[i].second.id << b << node;
								for (int j = 0; j < MAX_PLAYERS; j++) {
									if (socks[j].first->send(sendCons) != sf::Socket::Done) // Envia la construcción a todos los jugadores
										cout << "No se pudo enviar la construccion al Socket " << j << endl;
									else
										cout << "Informacion de construccion enviada al Socket " << j << endl;
								}
							}
							else { // Si no se puede realizar la acción avisa al player
								sf::Packet sendError;
								p = ERROR;
								sendError << (int)p;
								if (socks[i].first->send(sendError) != sf::Socket::Done) // Envia el error al jugador
									cout << "No se pudo enviar el error al Socket " << i << endl;
								else
									cout << "Informacion de error enviada al Socket " << i << endl;
							}
							break;
						case ENDTURN:
							prot = YOURTURN;
							sendPack << (int)prot;
							playerTurn++;
							if (playerTurn == 4)
								playerTurn = 0;
							socks[playerTurn].first->send(sendPack);
							for (int j = 0; j < MAX_PLAYERS; j++) {
								CalculateResources(num, myMap.nodes, &socks[j].second);
								prot = RECURSOS;
								sf::Packet sendRec;
								sendRec << (int)prot;
								sendRec << socks[j].second.cards[STEEL] << socks[j].second.cards[WOOD] << socks[j].second.cards[CLAY] << socks[j].second.cards[SHEEP] << socks[j].second.cards[STRAW];
								socks[j].first->send(sendRec);
							}
							break;
						case DISCONNECT:
							msg = "El jugador " + std::to_string(i) + " - " + socks[i].second.nick + " se ha desconectado.";

							selector.remove(*socks[i].first);
							socks[i].first->disconnect();
							socks.erase(socks.begin() + i);
							std::cout << "Elimino el socket que se ha desconectado" << endl;
							for each (pair<sf::TcpSocket*, Player> s in socks) {
								s.first->send(msg.c_str(), msg.length());
								std::cout << "Se envia desconexión al player " << s.second.id << " - " << s.second.nick;
							}
							msg = "";
							break;
						}
					}
					else if (status == sf::Socket::Disconnected) {
						msg = "El jugador " + std::to_string(i) + " - " + socks[i].second.nick + " se ha desconectado.";

						selector.remove(*socks[i].first);
						socks[i].first->disconnect();
						socks.erase(socks.begin() + i);
						std::cout << "Elimino el socket que se ha desconectado" << endl;
						for each (pair<sf::TcpSocket*, Player> s in socks) {
							s.first->send(msg.c_str(), msg.length());
							std::cout << "Se envia desconexión al player " << s.second.id << " - " << s.second.nick;
						}
						msg = "";
						break;
					}
					else {
						cout << "Error al recibir mensaje" << endl;
					}
				}
			}
		}
	}
#pragma endregion

#pragma region LiberaMemoria
	selector.clear();
	for each (pair<sf::TcpSocket*, Player> s in socks) {
		s.first->disconnect();
	}
	socks.clear();
#pragma endregion

	return 0;
}
