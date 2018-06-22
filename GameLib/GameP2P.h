#pragma once
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include "Utils.hpp"
//#include <Utils.hpp>
#include "Button.hpp"

using namespace utils;
using namespace std;

struct Receptor_P2P {
public:
	vector<sf::TcpSocket*>* sock;
	vector<pair<string, string>>* aMsj;
	pair<string, string> message;
	sf::Packet myPacket;
	size_t receivedLength;
	sf::SocketSelector selector;

	Receptor_P2P(vector<sf::TcpSocket*>* sock, vector<pair<string, string>>* aMsj) : sock(sock),
		aMsj(aMsj) {

		for each (sf::TcpSocket* s in *sock) {
			selector.add(*s);
		}
	}

	void operator() () {
		while (!utils::end || sock->size() != 0) {
			if (selector.wait()) {
				for (int i = 0; i < sock->size(); i++) {
					if (selector.isReady(*sock->at(i))) {
						sf::Socket::Status status = sock->at(i)->receive(myPacket);
						if (status == sf::Socket::Done) {
							int x = 0;
							myPacket >> x;
							int _id = -1;
							Protocol prot = (Protocol)x;
							switch (prot) {
							case utils::ACTION:
								myPacket >> _id;
								myPacket >> players[_id]->life >> players[_id]->mana >> players[_id]->defense;
								myPacket >> myEnemy->life;
								myPacket >> playerTurn;
								if (players[_id]->life <= 0) {
									players[_id]->alive = false;
									players[_id]->life = 0;
								}
								break;
							case utils::MSG:
								cout << "Mensaje recibido" << endl;
								myPacket >> message.first >> message.second;
								mu.lock();
								aMsj->push_back(message);
								if (aMsj->size() > 12) {
									aMsj->erase(aMsj->begin(), aMsj->begin() + 1);
								}
								if (message.second == " se ha desconectado.") {
									selector.remove(*sock->at(i));
									sock->at(i)->disconnect();
									sock->erase(sock->begin() + i);
									std::cout << "Elimino el socket que se ha desconectado" << endl;
									for each (Player* p in  players) {
										if (message.first == p->nick) {
											p->alive = false;
										}
									}
									//myPacket >> playerTurn;
								}
								message.first = "";
								message.second = "";
								mu.unlock();
								break;
							case ENDGAME:
								cout << "ENDGAME" << endl;
								utils::end = true;
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

class GameP2P {
public:
	GameP2P(vector<sf::TcpSocket*> _socket, vector<Player*> _players) {
		socket = _socket;
		players = _players;
	}

	void Run() {
		string sendText;
		size_t received = 0;

		vector<pair<string, string>> aMensajes;
		pair<string, string> message;
		string messageSend;
		Receptor_P2P r(&socket, &aMensajes);
		sf::Packet myPacket;
		thread t(r);

#pragma region ScreenDisplay
		sf::Vector2i screenDimensions(SCREEN_WIDTH, SCREEN_HEIGHT);

		sf::RenderWindow window;
		window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "RPG - " + nick);

		sf::Font font;
		if (!font.loadFromFile("courbd.ttf")) {
			std::cout << "Can't load the font file" << std::endl;
		}

		sf::String mensaje = " > ";

		// PERFIL DISPLAY
		sf::RectangleShape pSeparator(sf::Vector2f(5, 438));
		pSeparator.setFillColor(sf::Color(200, 200, 200, 255));
		pSeparator.setPosition(400, 0);

		sf::Texture perfilTex[4];
		sf::Sprite perfilSprite[4];
		sf::Text perfilName[4];

		perfilName[0] = sf::Text(players[0]->nick, font, 13);
		perfilName[0].setFillColor(sf::Color(200, 200, 200));
		perfilName[0].setStyle(1);
		perfilName[0].setPosition(425, 0);

		perfilName[1] = sf::Text(players[1]->nick, font, 13);
		perfilName[1].setFillColor(sf::Color(200, 200, 200));
		perfilName[1].setStyle(1);
		perfilName[1].setPosition(425, 109);

		perfilName[2] = sf::Text(players[2]->nick, font, 13);
		perfilName[2].setFillColor(sf::Color(200, 200, 200));
		perfilName[2].setStyle(1);
		perfilName[2].setPosition(425, 219);

		perfilName[3] = sf::Text(players[3]->nick, font, 13);
		perfilName[3].setFillColor(sf::Color(200, 200, 200));
		perfilName[3].setStyle(1);
		perfilName[3].setPosition(425, 329);

		perfilTex[0].loadFromFile(PERFIL_1);
		perfilTex[1].loadFromFile(PERFIL_2);
		perfilTex[2].loadFromFile(PERFIL_3);
		perfilTex[3].loadFromFile(PERFIL_4);

		perfilSprite[0].setTexture(perfilTex[0]);
		perfilSprite[1].setTexture(perfilTex[1]);
		perfilSprite[2].setTexture(perfilTex[2]);
		perfilSprite[3].setTexture(perfilTex[3]);

		perfilSprite[0].setPosition(410, 15);
		perfilSprite[1].setPosition(410, 125);
		perfilSprite[2].setPosition(410, 235);
		perfilSprite[3].setPosition(410, 345);

		perfilSprite[0].setScale(sf::Vector2f(0.9, 0.9));
		perfilSprite[1].setScale(sf::Vector2f(0.9, 0.9));
		perfilSprite[2].setScale(sf::Vector2f(0.9, 0.9));
		perfilSprite[3].setScale(sf::Vector2f(0.9, 0.9));


		sf::Text hpText[4];
		sf::RectangleShape hpBar[4];
		sf::RectangleShape hpBarBg[4];

		hpText[0] = sf::Text("HP", font, 12);
		hpText[0].setFillColor(sf::Color(200, 200, 200));
		hpText[0].setPosition(500, 0);

		hpText[1] = sf::Text("HP", font, 12);
		hpText[1].setFillColor(sf::Color(200, 200, 200));
		hpText[1].setPosition(500, 109);

		hpText[2] = sf::Text("HP", font, 12);
		hpText[2].setFillColor(sf::Color(200, 200, 200));
		hpText[2].setPosition(500, 219);

		hpText[3] = sf::Text("HP", font, 12);
		hpText[3].setFillColor(sf::Color(200, 200, 200));
		hpText[3].setPosition(500, 329);

		hpBarBg[0] = sf::RectangleShape(sf::Vector2f(100, 15));
		hpBarBg[0].setFillColor(BLACK);
		hpBarBg[0].setOutlineThickness(1);
		hpBarBg[0].setPosition(500, 15);

		hpBarBg[1] = sf::RectangleShape(sf::Vector2f(100, 15));
		hpBarBg[1].setFillColor(BLACK);
		hpBarBg[1].setOutlineThickness(1);
		hpBarBg[1].setPosition(500, 125);

		hpBarBg[2] = sf::RectangleShape(sf::Vector2f(100, 15));
		hpBarBg[2].setFillColor(BLACK);
		hpBarBg[2].setOutlineThickness(1);
		hpBarBg[2].setPosition(500, 235);

		hpBarBg[3] = sf::RectangleShape(sf::Vector2f(100, 15));
		hpBarBg[3].setFillColor(BLACK);
		hpBarBg[3].setOutlineThickness(1);
		hpBarBg[3].setPosition(500, 345);

		hpBar[0] = sf::RectangleShape(sf::Vector2f(players[0]->life, 15));
		hpBar[0].setFillColor(HP_COLOR);
		hpBar[0].setPosition(500, 15);

		hpBar[1] = sf::RectangleShape(sf::Vector2f(players[1]->life, 15));
		hpBar[1].setFillColor(HP_COLOR);
		hpBar[1].setPosition(500, 125);

		hpBar[2] = sf::RectangleShape(sf::Vector2f(players[2]->life, 15));
		hpBar[2].setFillColor(HP_COLOR);
		hpBar[2].setPosition(500, 235);

		hpBar[3] = sf::RectangleShape(sf::Vector2f(players[3]->life, 15));
		hpBar[3].setFillColor(HP_COLOR);
		hpBar[3].setPosition(500, 345);

		sf::Text manaText[4];
		sf::RectangleShape manaBar[4];
		sf::RectangleShape manaBarBg[4];

		manaText[0] = sf::Text("MANA", font, 12);
		manaText[0].setFillColor(sf::Color(200, 200, 200));
		manaText[0].setPosition(500, 40);

		manaText[1] = sf::Text("MANA", font, 12);
		manaText[1].setFillColor(sf::Color(200, 200, 200));
		manaText[1].setPosition(500, 149);

		manaText[2] = sf::Text("MANA", font, 12);
		manaText[2].setFillColor(sf::Color(200, 200, 200));
		manaText[2].setPosition(500, 259);

		manaText[3] = sf::Text("MANA", font, 12);
		manaText[3].setFillColor(sf::Color(200, 200, 200));
		manaText[3].setPosition(500, 369);

		manaBarBg[0] = sf::RectangleShape(sf::Vector2f(100, 15));
		manaBarBg[0].setFillColor(BLACK);
		manaBarBg[0].setOutlineThickness(1);
		manaBarBg[0].setPosition(500, 55);

		manaBarBg[1] = sf::RectangleShape(sf::Vector2f(100, 15));
		manaBarBg[1].setFillColor(BLACK);
		manaBarBg[1].setOutlineThickness(1);
		manaBarBg[1].setPosition(500, 165);

		manaBarBg[2] = sf::RectangleShape(sf::Vector2f(100, 15));
		manaBarBg[2].setFillColor(BLACK);
		manaBarBg[2].setOutlineThickness(1);
		manaBarBg[2].setPosition(500, 275);

		manaBarBg[3] = sf::RectangleShape(sf::Vector2f(100, 15));
		manaBarBg[3].setFillColor(BLACK);
		manaBarBg[3].setOutlineThickness(1);
		manaBarBg[3].setPosition(500, 385);

		manaBar[0] = sf::RectangleShape(sf::Vector2f(players[0]->mana, 15));
		manaBar[0].setFillColor(MANA_COLOR);
		manaBar[0].setPosition(500, 55);

		manaBar[1] = sf::RectangleShape(sf::Vector2f(players[1]->mana, 15));
		manaBar[1].setFillColor(MANA_COLOR);
		manaBar[1].setPosition(500, 165);

		manaBar[2] = sf::RectangleShape(sf::Vector2f(players[2]->mana, 15));
		manaBar[2].setFillColor(MANA_COLOR);
		manaBar[2].setPosition(500, 275);

		manaBar[3] = sf::RectangleShape(sf::Vector2f(players[3]->mana, 15));
		manaBar[3].setFillColor(MANA_COLOR);
		manaBar[3].setPosition(500, 385);

		sf::Text attackText[4];
		sf::Text mAttackText[4];
		sf::Text defenseText[4];
		sf::Text mDefenseText[4];

		attackText[0] = sf::Text("Attack: " + std::to_string(players[0]->attack), font, 12);
		attackText[0].setFillColor(sf::Color(200, 200, 200));
		attackText[0].setPosition(610, 15);

		mAttackText[0] = sf::Text("Magic: " + std::to_string(players[0]->mAttack), font, 12);
		mAttackText[0].setFillColor(sf::Color(200, 200, 200));
		mAttackText[0].setPosition(610, 30);

		defenseText[0] = sf::Text("Defense: " + std::to_string(players[0]->defense), font, 12);
		defenseText[0].setFillColor(sf::Color(200, 200, 200));
		defenseText[0].setPosition(700, 15);

		mDefenseText[0] = sf::Text("M.Defense: " + std::to_string(players[0]->mDefense), font, 12);
		mDefenseText[0].setFillColor(sf::Color(200, 200, 200));
		mDefenseText[0].setPosition(700, 30);

		attackText[1] = sf::Text("Attack: " + std::to_string(players[1]->attack), font, 12);
		attackText[1].setFillColor(sf::Color(200, 200, 200));
		attackText[1].setPosition(610, 125);

		mAttackText[1] = sf::Text("Magic: " + std::to_string(players[1]->mAttack), font, 12);
		mAttackText[1].setFillColor(sf::Color(200, 200, 200));
		mAttackText[1].setPosition(610, 140);

		defenseText[1] = sf::Text("Defense: " + std::to_string(players[1]->defense), font, 12);
		defenseText[1].setFillColor(sf::Color(200, 200, 200));
		defenseText[1].setPosition(700, 125);

		mDefenseText[1] = sf::Text("M.Defense: " + std::to_string(players[1]->mDefense), font, 12);
		mDefenseText[1].setFillColor(sf::Color(200, 200, 200));
		mDefenseText[1].setPosition(700, 140);

		attackText[2] = sf::Text("Attack: " + std::to_string(players[2]->attack), font, 12);
		attackText[2].setFillColor(sf::Color(200, 200, 200));
		attackText[2].setPosition(610, 235);

		mAttackText[2] = sf::Text("Magic: " + std::to_string(players[2]->mAttack), font, 12);
		mAttackText[2].setFillColor(sf::Color(200, 200, 200));
		mAttackText[2].setPosition(610, 250);

		defenseText[2] = sf::Text("Defense: " + std::to_string(players[2]->defense), font, 12);
		defenseText[2].setFillColor(sf::Color(200, 200, 200));
		defenseText[2].setPosition(700, 235);

		mDefenseText[2] = sf::Text("M.Defense: " + std::to_string(players[2]->mDefense), font, 12);
		mDefenseText[2].setFillColor(sf::Color(200, 200, 200));
		mDefenseText[2].setPosition(700, 250);

		attackText[3] = sf::Text("Attack: " + std::to_string(players[3]->attack), font, 12);
		attackText[3].setFillColor(sf::Color(200, 200, 200));
		attackText[3].setPosition(610, 345);

		mAttackText[3] = sf::Text("Magic: " + std::to_string(players[3]->mAttack), font, 12);
		mAttackText[3].setFillColor(sf::Color(200, 200, 200));
		mAttackText[3].setPosition(610, 360);

		defenseText[3] = sf::Text("Defense: " + std::to_string(players[3]->defense), font, 12);
		defenseText[3].setFillColor(sf::Color(200, 200, 200));
		defenseText[3].setPosition(700, 345);

		mDefenseText[3] = sf::Text("M.Defense: " + std::to_string(players[3]->mDefense), font, 12);
		mDefenseText[3].setFillColor(sf::Color(200, 200, 200));
		mDefenseText[3].setPosition(700, 360);

		// ACTIONS DISPLAY
		sf::RectangleShape aSeparator(sf::Vector2f(400, 5));
		aSeparator.setFillColor(sf::Color(200, 200, 200, 255));
		aSeparator.setPosition(0, 300);

		Button buttons[4];

		for (int i = 0; i < 4; i++) {
			buttons[i].window = &window;
			buttons[i].id = i;
			buttons[i].texture.loadFromFile(BUTTON);

			buttons[i].sprite.setTexture(buttons[i].texture);
		}
		buttons[0].rect = new sf::IntRect(sf::Vector2i(30, 320), sf::Vector2i(160, 50));
		buttons[0].sprite.setPosition(buttons[0].rect->left, buttons[0].rect->top);

		buttons[1].rect = new sf::IntRect(sf::Vector2i(220, 320), sf::Vector2i(160, 50));
		buttons[1].sprite.setPosition(buttons[1].rect->left, buttons[1].rect->top);

		buttons[2].rect = new sf::IntRect(sf::Vector2i(30, 375), sf::Vector2i(160, 50));
		buttons[2].sprite.setPosition(buttons[2].rect->left, buttons[2].rect->top);

		buttons[3].rect = new sf::IntRect(sf::Vector2i(220, 375), sf::Vector2i(160, 50));
		buttons[3].sprite.setPosition(buttons[3].rect->left, buttons[3].rect->top);

		sf::Text buttonText[4];

		switch (myID)
		{
		case 0:
			buttonText[0] = sf::Text("TACKLE", font, 20);
			buttonText[0].setFillColor(sf::Color(20, 20, 20));
			buttonText[0].setPosition(72, 332);

			buttonText[1] = sf::Text("SLASH", font, 20);
			buttonText[1].setFillColor(sf::Color(20, 20, 20));
			buttonText[1].setPosition(270, 332);

			buttonText[2] = sf::Text("FIRST AID", font, 20);
			buttonText[2].setFillColor(sf::Color(20, 20, 20));
			buttonText[2].setPosition(55, 387);
			break;
		case 1:
			buttonText[0] = sf::Text("SHOOT", font, 20);
			buttonText[0].setFillColor(sf::Color(20, 20, 20));
			buttonText[0].setPosition(80, 332);

			buttonText[1] = sf::Text("ENVENOM", font, 20);
			buttonText[1].setFillColor(sf::Color(20, 20, 20));
			buttonText[1].setPosition(258, 332);

			buttonText[2] = sf::Text("FLEE", font, 20);
			buttonText[2].setFillColor(sf::Color(20, 20, 20));
			buttonText[2].setPosition(86, 387);
			break;
		case 2:
			buttonText[0] = sf::Text("BASH", font, 20);
			buttonText[0].setFillColor(sf::Color(20, 20, 20));
			buttonText[0].setPosition(86, 332);

			buttonText[1] = sf::Text("DIVINE CROSS", font, 15);
			buttonText[1].setFillColor(sf::Color(20, 20, 20));
			buttonText[1].setPosition(245, 335);

			buttonText[2] = sf::Text("THROW SHIELD", font, 15);
			buttonText[2].setFillColor(sf::Color(20, 20, 20));
			buttonText[2].setPosition(55, 390);
			break;
		case 3:
			buttonText[0] = sf::Text("MAGIC ROD", font, 19);
			buttonText[0].setFillColor(sf::Color(20, 20, 20));
			buttonText[0].setPosition(59, 332);

			buttonText[1] = sf::Text("FIREBALL", font, 20);
			buttonText[1].setFillColor(sf::Color(20, 20, 20));
			buttonText[1].setPosition(253, 332);

			buttonText[2] = sf::Text("SOUL SIPHON", font, 15);
			buttonText[2].setFillColor(sf::Color(20, 20, 20));
			buttonText[2].setPosition(60, 390);
			break;
		default:
			break;
		}

		buttonText[3] = sf::Text("GUARD", font, 20);
		buttonText[3].setFillColor(sf::Color(20, 20, 20));
		buttonText[3].setPosition(270, 387);

		// VIDEOGAME DISPLAY
		sf::Texture bgTex;
		sf::Sprite bgSprite;
		sf::Texture playerTex[4];
		sf::Sprite playerSprite[4];
		sf::Texture bossTex;
		sf::Sprite bossSprite;

		bgTex.loadFromFile(BG2);

		playerTex[0].loadFromFile(PLAYER_1);
		playerTex[1].loadFromFile(PLAYER_2);
		playerTex[2].loadFromFile(PLAYER_3);
		playerTex[3].loadFromFile(PLAYER_4);

		bossTex.loadFromFile(BOSS);

		bgSprite.setTexture(bgTex);

		playerSprite[0].setTexture(playerTex[0]);
		playerSprite[1].setTexture(playerTex[1]);
		playerSprite[2].setTexture(playerTex[2]);
		playerSprite[3].setTexture(playerTex[3]);

		bossSprite.setTexture(bossTex);

		bgSprite.setPosition(0, 0);

		playerSprite[0].setPosition(20, 245);
		playerSprite[1].setPosition(120, 245);
		playerSprite[2].setPosition(220, 245);
		playerSprite[3].setPosition(320, 245);

		bossSprite.setPosition(130, 5);

		playerSprite[0].setScale(sf::Vector2f(1.5, 1.5));
		playerSprite[1].setScale(sf::Vector2f(1.5, 1.5));
		playerSprite[2].setScale(sf::Vector2f(1.5, 1.5));
		playerSprite[3].setScale(sf::Vector2f(1.5, 1.5));

		bossSprite.setScale(sf::Vector2f(0.95, 0.95));

		sf::RectangleShape hpBoss;
		sf::RectangleShape hpBossBg;

		hpBossBg = sf::RectangleShape(sf::Vector2f((myEnemy->life / 4), 15));
		hpBossBg.setFillColor(BLACK);
		hpBossBg.setOutlineThickness(1);
		hpBossBg.setPosition(100, 2);

		hpBoss = sf::RectangleShape(sf::Vector2f((myEnemy->life / 4), 15));
		hpBoss.setFillColor(RED);
		hpBoss.setPosition(100, 2);


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
					for (int i = 0; i < socket.size(); i++) {
						if (i != myID) {
							socket[i]->send(myPacket);
						}
					}
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
							for (int i = 0; i < socket.size(); i++) {
								if (i != myID) {
									socket[i]->send(myPacket);
								}
							}
							window.close();
							myPacket.clear();
							utils::end = true;
						}
						else {
							prot = MSG;
							messageSend = mensaje;

							//WRITE SCREEN
							aMensajes.push_back({nick, messageSend});
							if (aMensajes.size() > 12) {
								aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
							}

							//SEND
							myPacket << (int)prot << nick << messageSend;
							cout << (int)socket.size() << endl;
							for (int i = 0; i < socket.size(); i++) {
								if (i != myID) {
									sf::Socket::Status status = socket[i]->send(myPacket);
									if (status != sf::Socket::Done) {
										cout << "Ha fallado el envio del mensaje al socket " << i << endl;
									}
									else {
										cout << "Mensaje enviado al socket " << i << endl;
									}
								}
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
				case sf::Event::MouseButtonPressed:
					if (myID == playerTurn) {
						for (int i = 0; i < 4; i++) {
							if (buttons[i].MouseContact()) {
								if (!((i == 1 || i == 2) && players[myID]->mana < 20)) {
									myPacket.clear();
									std::cout << "Button " << buttons[i].id << " was pressed" << std::endl;
									prot = ACTION;
									myPacket << (int)prot << i;

									for (int j = 0; j < socket.size(); j++) {
										if (j != myID) {
											sf::Socket::Status status = socket[j]->send(myPacket);
											if (status != sf::Socket::Done) {
												cout << "Ha fallado el envio de la accion al socket " << j << endl;
											}
										}
									}
									myPacket.clear();
									playerTurn = -1;
								}
							}
						}
					}
					break;
				default:
					break;
				}
			}
#pragma endregion

#pragma region DrawPerfil
			window.draw(bgSprite);
			window.draw(pSeparator);
			window.draw(bossSprite);
			window.draw(hpBossBg);
			hpBoss.setSize(sf::Vector2f((myEnemy->life / 4), 15));
			window.draw(hpBoss);
			window.draw(aSeparator);
			if (myID != playerTurn) {
				buttons[0].sprite.setColor(DIE_COLOR);
				buttons[1].sprite.setColor(DIE_COLOR);
				buttons[2].sprite.setColor(DIE_COLOR);
				buttons[3].sprite.setColor(DIE_COLOR);
			}
			else {
				buttons[0].sprite.setColor(WHITE);
				if (players[myID]->mana >= 20) {
					buttons[1].sprite.setColor(WHITE);
					buttons[2].sprite.setColor(WHITE);
				}
				buttons[3].sprite.setColor(WHITE);
			}
			for (int i = 0; i < 4; i++) {
				hpBar[i].setSize(sf::Vector2f(players[i]->life, 15));
				manaBar[i].setSize(sf::Vector2f(players[i]->mana, 15));
				defenseText[i].setString("Defense: " + to_string(players[i]->defense));
				if (!players[i]->alive) {
					perfilSprite[i].setColor(DIE_COLOR);
					playerSprite[i].setColor(DIE_COLOR);
				}
				if (i == playerTurn) {
					playerSprite[i].setPosition(sf::Vector2f(playerSprite[i].getPosition().x, POS_Y_TURN));
				}
				else {
					playerSprite[i].setPosition(sf::Vector2f(playerSprite[i].getPosition().x, POS_Y));
				}
				window.draw(hpBarBg[i]);
				window.draw(hpBar[i]);
				window.draw(hpText[i]);
				window.draw(manaBarBg[i]);
				window.draw(manaBar[i]);
				window.draw(manaText[i]);
				window.draw(attackText[i]);
				window.draw(mAttackText[i]);
				window.draw(defenseText[i]);
				window.draw(mDefenseText[i]);
				window.draw(perfilSprite[i]);
				window.draw(perfilName[i]);
				window.draw(playerSprite[i]);
				window.draw(buttons[i].sprite);
				window.draw(buttonText[i]);
			}

#pragma endregion

#pragma region DrawMessages
			window.draw(titleSeparator);
			window.draw(title);
			window.draw(separator);
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

			// ENG GAME
			if (utils::end == true) {
				window.close();
			}
		}

		t.join();
	}

private:
	vector<sf::TcpSocket*> socket;
};


