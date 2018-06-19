#pragma once
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <Utils.hpp>

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
		while (!utils::end) {
			myPacket.clear();
			sf::Socket::Status status = sock->receive(myPacket);
			if (status != sf::Socket::Done) {
				cout << "Ha fallado la recepcion de datos\n";
				utils::end = true;
			}
			else {
				int x = 0;
				myPacket >> x;
				Protocol prot = (Protocol)x;
				cout << (int)prot << endl;
				switch (prot)
				{
				case utils::TURN:
					break;
				case utils::MSG:
					cout << "Mensaje recibido" << endl;
					myPacket >> message.first >> message.second;
					mu.lock();
					aMsj->push_back(message);
					if (aMsj->size() > 12) {
						aMsj->erase(aMsj->begin(), aMsj->begin() + 1);
					}
					message.first = "";
					message.second = "";
					mu.unlock();
					break;
				default:
					break;
				}
			}
		}
	}
};

class Game {
public:
	Game(sf::TcpSocket* _socket) {
		socket = _socket;
	}

	void Run() {
		string sendText;
		size_t received = 0;

		vector<pair<string, string>> aMensajes;
		pair<string, string> message;
		string messageSend;
		Receptor_Threading r(socket, &aMensajes);
		sf::Packet myPacket;
		thread t(r);

#pragma region ScreenDisplay
		sf::Vector2i screenDimensions(SCREEN_WIDTH, SCREEN_HEIGHT);

		sf::RenderWindow window;
		window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat");

		sf::Font font;
		if (!font.loadFromFile("courbd.ttf")) {
			std::cout << "Can't load the font file" << std::endl;
		}

		sf::String mensaje = " > ";

		// CHAT DISPLAY
		sf::RectangleShape titleSeparator(sf::Vector2f(800, 12));
		titleSeparator.setFillColor(sf::Color(200, 200, 200, 255));
		titleSeparator.setPosition(0, 438);

		sf::Text title("PUBLIC CHAT", font, 10);
		title.setFillColor(sf::Color(0, 0, 0));
		title.setPosition(375, 438);

		sf::Text chattingText(mensaje, font, 10);
		chattingText.setFillColor(sf::Color(0, 160, 0));

		sf::RectangleShape separator(sf::Vector2f(800, 3));
		separator.setFillColor(sf::Color(200, 200, 200, 255));
		separator.setPosition(0, 577);

		sf::Text text(mensaje, font, 10);
		text.setFillColor(sf::Color(0, 160, 0));
		text.setPosition(0, 583);
#pragma endregion

#pragma region InteractionLoop
		while (window.isOpen()) {
			sf::Event evento;
			Protocol prot;
			while (window.pollEvent(evento)) {
				switch (evento.type) {
				case sf::Event::Closed:
					prot = DISCONNECT;
					myPacket << (int)prot;
					socket->send(myPacket);
					window.close();
					myPacket.clear();
					utils::end = true;
					break;
				case sf::Event::KeyPressed:
					if (evento.key.code == sf::Keyboard::Escape)
						window.close();
					else if (evento.key.code == sf::Keyboard::Return) {
						if (mensaje == " > exit") {
							prot = DISCONNECT;
							myPacket << (int)prot;
							socket->send(myPacket);
							window.close();
							myPacket.clear();
							utils::end = true;
						}
						else {
							prot = MSG;
							//SEND
							messageSend = mensaje;

							myPacket << (int)prot << nick << messageSend;
							sf::Socket::Status status = socket->send(myPacket);
							if (status != sf::Socket::Done) {
								cout << "Ha fallado el envio de datos\n";
							}
							mensaje = " > ";
							myPacket.clear();
						}
					}
					break;
				case sf::Event::TextEntered:
					if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
						mensaje += (char)evento.text.unicode;
					else if (evento.text.unicode == 8 && mensaje.getSize() > 0)
						mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
					break;
				}
			}
			window.draw(titleSeparator);
			window.draw(title);
			window.draw(separator);
#pragma endregion

#pragma region DrawMessages
			for (size_t i = 0; i < aMensajes.size(); i++) {
				string chatting = aMensajes[i].first;
				chatting += aMensajes[i].second;
				chattingText.setPosition(sf::Vector2f(0, 452 + (10 * (float)i)));
				chattingText.setString(chatting);
				if (aMensajes[i].first == nick) {
					chattingText.setFillColor(MINE_COLOR);
				}
				else {
					chattingText.setFillColor(OTHER_COLOR);
				}
				window.draw(chattingText);
			}
			string mensaje_ = mensaje + "_";
			text.setString(mensaje_);
			window.draw(text);
#pragma endregion

			window.display();
			window.clear();
		}

		t.join();
	}

private:
	sf::TcpSocket* socket;
};

