#pragma once
#include <string>
#include <iostream>
#include <SFML\Network.hpp>

class Enemy {
public:
	Enemy() = default;

	int life = 800;
	int attack = 60;
	int mAttack = 30;
	bool poison = false;
	bool alive = true;
};

class Player {
public:
	Player(std::string _nick, std::string _ip, unsigned short _port, unsigned short _id) :	nick(_nick),
																							ip(_ip),
																							port(_port),
																							id(_id) {
		switch (_id) {
		case 0:
			attack = 30;
			mAttack = 10;
			defense = 25;
			mDefense = 20;
			break;
		case 1:
			attack = 25;
			mAttack = 20;
			defense = 15;
			mDefense = 25;
			break;
		case 2:
			attack = 20;
			mAttack = 5;
			defense = 40;
			mDefense = 20;
			break;
		case 3:
			attack = 5;
			mAttack = 35;
			defense = 15;
			mDefense = 30;
			break;
		default:
			break;
		}

		tempDef = defense;
	}

	Player() = default;

	std::string nick;
	std::string ip;
	unsigned short port;
	unsigned short id;
	int life = 100;
	int mana = 100;
	int attack = 10;
	int mAttack = 15;
	int defense = 10;
	int mDefense = 5;
	int tempDef = 0;
	bool alive = true;

	void Attack(int _num, Enemy* _enemy) {
		defense = tempDef;
		switch (id) {
		case 0: // SWORDMAN SKILLS
			switch (_num) {
			case 0: // TACKLE
				_enemy->life -= attack;
				break;
			case 1: // SLASH
				if (mana >= 20) {
					_enemy->life -= (attack * 2);
					mana -= 20;
				}
				break;
			case 2: // FIRST AID
				if (mana >= 20) {
					mana -= 20;
					life += 40;
					if (life > 100)
						life = 100;
				}
				break;
			case 3: // GUARD
				defense += 10;
				break;
			default:
				break;
			}
			break;
		case 1: // ARCHER SKILLS
			switch (_num) {
			case 0: // SHOOT
				_enemy->life -= attack;
				break;
			case 1: // ENVENOM
				if (mana >= 20) {
					_enemy->life -= mAttack;
					_enemy->poison = true;
					mana -= 20;
				}
				break;
			case 2: // FLEE
				if (mana >= 20) {
					defense = 100;
					mana -= 20;
				}
				break;
			case 3: // GUARD
				defense += 10;
				break;
			default:
				break;
			}
			break;
		case 2: // TANK SKILLS
			switch (_num) {
			case 0: // BASH
				_enemy->life -= attack;
				break;
			case 1: // DIVINE CROSS
				if (mana >= 20) {
					_enemy->life -= (attack * 2);
					mana -= 20;
				}
				break;
			case 2: // THROW SHIELD
				if (mana >= 20) {
					_enemy->life -= attack;
					defense += 10;
					mana -= 20;
				}
				break;
			case 3: // GUARD
				defense += 10;
				break;
			default:
				break;
			}
			break;
		case 3: // MAGE SKILLS
			switch (_num) {
			case 0: // MAGIC ROD
				_enemy->life -= mAttack;
				break;
			case 1: // FIREBALL
				if (mana >= 20) {
					_enemy->life -= (mAttack * 2);
					mana -= 20;
				}
				break;
			case 2: // SOUL SIPHON
				if (mana >= 20) {
					_enemy->life -= (mAttack * 2);
					defense += 10;
					mana -= 20;
				}
				break;
			case 3: // GUARD
				defense += 10;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

};	

