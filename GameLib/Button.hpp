#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include "Utils.hpp"

struct Button {
	int id;
	sf::IntRect* rect;
	sf::Texture texture;
	sf::Sprite sprite;

	bool MouseContact() {
		return rect->contains(sf::Mouse::getPosition(*window));
	}

	Button() = default;

	Button(sf::Vector2i pos) {
		if (!texture.loadFromFile(BUTTON))
			std::cout << "Couldn't load the button texture" << std::endl;

		rect = new sf::IntRect(pos, (sf::Vector2i)texture.getSize());

		sprite.setTexture(texture);
		sprite.setPosition(rect->left, rect->top);
	}

	Button(int x, int y) {
		sf::Vector2i pos(x, y);

		if (!texture.loadFromFile(BUTTON))
			std::cout << "Couldn't load the button texture" << std::endl;

		rect = new sf::IntRect(pos, (sf::Vector2i)texture.getSize());

		sprite.setTexture(texture);
		sprite.setPosition(rect->left, rect->top);
	}

	~Button() {
		delete rect;
	}

	sf::Window* window;
private:
};