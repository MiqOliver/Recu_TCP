#include <SFML\Network.hpp>
#include <iostream>
#include <mutex>
#include <Utils.hpp>
#include <Direction.hpp>
#include <Player.hpp>
#include <Map.hpp>

using namespace std;
using namespace utils;

//void PasaTurno(vector<pair<sf::TcpSocket*, Player>> socks) {
//	playerTurn++;
//	if (playerTurn > 3) {
//		playerTurn = 0;
//	}
//	if (!players[playerTurn]->alive)
//		PasaTurno();
//}

int main() {

	sf::Packet pck;
	sf::TcpListener listener;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::SocketSelector selector;
	bool userDisconected = false;
	pair<string, string> message;
	vector<pair<string, string>> aMsj;

	// Copia del mundo (Los atributos de Player y de Enemy)
	vector<pair<sf::TcpSocket*, Player>> socks;
	myEnemy = new Enemy();
	playerTurn = 0;

#pragma region Connection
	//ESTABLECER CONEXION
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
	// Se intercambia datos iniciales con otros jugadores
	for (int i = 0; i < socks.size(); i++) {
		prot = INFOPLAYER;
		pck << (int)prot;
		for (int j = 0; j < socks.size(); j++) {
			pck << socks[j].second.nick << socks[j].second.id << socks[j].second.life << socks[j].second.attack << socks[j].second.mAttack << socks[j].second.defense << socks[j].second.mDefense;
		}
		if (socks[i].first->send(pck) != sf::Socket::Done)
			cout << "No se pudo enviar la informacion de players al Socket " << i << endl;
		else
			cout << "Informacion de players enviada al Socket " << i << endl;
	}
	pck.clear();

	// El servidor indica la situación inicial
	for (int i = 0; i < socks.size(); i++) {
		prot = INFOENEMY;
		pck << (int)prot << myEnemy->life << myEnemy->attack << myEnemy->mAttack;
		if (socks[i].first->send(pck) != sf::Socket::Done)
			cout << "No se pudo enviar la informacion del enemigo al Socket " << i << endl;
		else
			cout << "Informacion del enemigo enviada al Socket " << i << endl;
	}
	pck.clear();

	// El servidor marca el inicio de partida
	for (int i = 0; i < socks.size(); i++) {
		prot = START;
		pck << (int)prot << playerTurn;
		if (socks[i].first->send(pck) != sf::Socket::Done)
			cout << "No se pudo enviar el inicio de partida al Socket " << i << endl;
		else
			cout << "Inicio de partida enviado al Socket " << i << endl;
	}
	pck.clear();
#pragma endregion

#pragma region Loop
	while (socks.size() != 0) {
		if (selector.wait()) {
			for (int i = 0; i < socks.size(); i++) {
				if (selector.isReady(*socks[i].first)) {
					sf::Packet myPack;
					sf::Packet sendPack;
					sf::Socket::Status status = socks[i].first->receive(myPack);
					if (status == sf::Socket::NotReady) {
						break;
					}
					else if (status == sf::Socket::Done) {
						int p;
						myPack >> p;
						prot = (Protocol)p;
						int _id = 0;
						int action = 0;
						switch (prot) {
						case ACTION:
							myPack >> action;
							socks[i].second.Attack(action, myEnemy);
							_id = playerTurn;
							//PasaTurno
							do {
								playerTurn++;
								if (playerTurn > socks.size() - 1) {
									playerTurn = 0;
									int dmg = myEnemy->attack - socks[_id].second.defense;
									if (dmg < 0)
										dmg = 0;
									socks[_id].second.life -= dmg;
								}
							} while (!socks[playerTurn].second.alive);

							sendPack << (int)prot << _id << socks[_id].second.life << socks[_id].second.mana << socks[_id].second.defense << myEnemy->life << (int)socks[playerTurn].second.id;
							for (int j = 0; j < socks.size(); j++) {
								if (socks[j].first->send(sendPack) != sf::Socket::Done)
									cout << "No se pudo enviar la accion al Socket " << j << endl;
								else
									cout << "Accion enviada al Socket " << j << endl;
							}
							myPack.clear();
							sendPack.clear();



							break;
						case MSG:
							myPack >> message.first >> message.second;
							aMsj.push_back(message);
							if (aMsj.size() > 25) {
								aMsj.erase(aMsj.begin(), aMsj.begin() + 1);
							}
							//cout << "Mensaje: " << message.first << " - " << message.second << " - " << (int)prot << endl;
							sendPack << (int)prot << message.first << message.second;
							for (int j = 0; j < socks.size(); j++) {
								if (socks[j].first->send(myPack) != sf::Socket::Done)
									cout << "No se pudo enviar el mensaje al Socket " << j << endl;
								else
									cout << "Mensaje enviado al Socket " << j << endl;
							}
							message.first = "";
							message.second = "";
							pck.clear();
							sendPack.clear();
							break;
						case DISCONNECT:
							prot = MSG;
							msg = " se ha desconectado.";
							sendPack << (int)prot << socks[i].second.nick << msg;

							selector.remove(*socks[i].first);
							socks[i].first->disconnect();
							socks.erase(socks.begin() + i);
							std::cout << "Elimino el socket que se ha desconectado" << endl;
							for (int j = 0; j < socks.size(); j++) {
								if (socks[j].first->send(sendPack) != sf::Socket::Done)
									cout << "No se pudo enviar desconexion al Socket " << j << endl;
								else
									cout << "Se envia desconexión al player " << j << " - " << socks[j].second.nick << endl;

							}
							msg = "";

							do {
								playerTurn++;
								if (playerTurn > socks.size() - 1) {
									playerTurn = 0;
								}
							} while (!socks[playerTurn].second.alive);
							pck.clear();
							sendPack.clear();
							break;
						}
					}
					else if (status == sf::Socket::Disconnected) {
						prot = MSG;
						msg = " se ha desconectado.";
						sendPack << (int)prot << socks[i].second.nick << msg;

						selector.remove(*socks[i].first);
						socks[i].first->disconnect();
						socks.erase(socks.begin() + i);
						std::cout << "Elimino el socket que se ha desconectado" << endl;
						for (int j = 0; j < socks.size(); j++) {
							if (socks[j].first->send(sendPack) != sf::Socket::Done)
								cout << "No se pudo enviar desconexion al Socket " << j << endl;
							else
								cout << "Se envia desconexión al player " << j << " - " << socks[j].second.nick << endl;

						}
						msg = "";

						do {
							playerTurn++;
							if (playerTurn > socks.size() - 1) {
								playerTurn = 0;
							}
						} while (!socks[playerTurn].second.alive);

						pck.clear();
						sendPack.clear();
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
