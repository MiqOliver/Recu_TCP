#include <SFML\Network.hpp>
#include <iostream>
#include <mutex>
#include <Utils.hpp>
#include <Direction.hpp>
#include <Player.hpp>
#include <Map.hpp>

using namespace std;
using namespace utils;

int main() {

	sf::Packet pck;
	sf::TcpListener listener;
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::SocketSelector selector;
	Protocol prot;
	bool userDisconected = false;

#pragma region Connection
	//ESTABLECER CONEXION

	// Copia del mundo (Los atributos de Player y de Enemy)
	vector<pair<sf::TcpSocket*, Player>> socks;
	Enemy myEnemy = Enemy();

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
	for (int i = 0; i < MAX_PLAYERS; i++) {
		prot = INFOPLAYER;
		pck << (int)prot;
		for (int j = 0; j < MAX_PLAYERS; j++) {
			pck << socks[j].second.nick << socks[j].second.id << socks[j].second.life << socks[j].second.attack << socks[j].second.mAttack;
		}
		if (socks[i].first->send(pck) != sf::Socket::Done)
			cout << "No se pudo enviar la informacion de players al Socket " << i << endl;
		else
			cout << "Informacion de players enviada al Socket " << i << endl;
	}
	pck.clear();

	// El servidor indica la situación inicial
	for (int i = 0; i < MAX_PLAYERS; i++) {
		prot = INFOENEMY;
		pck << (int)prot << myEnemy.life << myEnemy.attack << myEnemy.mAttack;
		if (socks[i].first->send(pck) != sf::Socket::Done)
			cout << "No se pudo enviar la informacion del enemigo al Socket " << i << endl;
		else
			cout << "Informacion del enemigo enviada al Socket " << i << endl;
	}
	pck.clear();

	// El servidor marca el inicio de partida
	for (int i = 0; i < MAX_PLAYERS; i++) {
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
							break;
						case ENDTURN:
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
