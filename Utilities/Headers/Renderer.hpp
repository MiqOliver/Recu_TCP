#pragma once
#include "Utils.hpp"
#include "../../deps64/include/SFML/Graphics.hpp"

using namespace sf;
using namespace graphics;

#define R Renderer::Instance()

class Renderer {

#pragma region Singleton

public:
	inline static Renderer &Instance(void) {
		static Renderer renderer;
		return renderer;
	}

private:
	Renderer() = default;
	Renderer(const Renderer &r) = delete;
	inline void operator=(const Renderer &r) = delete;
	~Renderer() {
		delete myTurn;
	}

#pragma endregion

#pragma region Variables

private:
	Texture map_texture[6];
	Texture numbers_texture[10];
	Sprite map_sprite[NUM_CELLS];
	Sprite numbers_sprite[NUM_CELLS - 1];

protected:
	RenderWindow window;
	bool *myTurn = nullptr;
	Text turn;
	Font font;

	Texture res_texture[5];
	Sprite res_sprite[5];
	Text res_text[5];

public:
	int res_mount[5];

#pragma endregion


#pragma region Functions

public:

	inline void SetVariables(bool* _turn) {
		myTurn = _turn;
	}

	inline void CreateWindow() {
		window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Catan");
		Image icon;
		icon.loadFromFile(MAP_0);
		window.setIcon(64, 74, icon.getPixelsPtr());

		LoadSprites();
		ScaleSprites();
		MoveSprites();

		font.loadFromFile(FONT);
		turn.setFont(font);
		turn.setFillColor(Color::Green);
		turn.setPosition(Vector2f(SCREEN_WIDTH / 2 - 100, 25));

		for (int i = 0; i < 5; i++) {
			res_text[i].setFont(font);
			res_text[i].setFillColor(Color::White);
			res_text[i].setPosition(res_sprite[i].getPosition() + Vector2f(75, 25));
			res_text[i].setScale(Vector2f(0.5, 0.5));
			res_text[i].setString("0");
		}

		CreateInputRect();
	}

	inline bool IsOpen() {
		return window.isOpen();
	}
	inline void Close() {
		window.close();
	}

	void Draw() {

		window.clear();

		//INPUTS
		Event event;
		while (window.pollEvent(event)) {
			Vector2i pos = Mouse::getPosition();
			switch (event.type)	{
			case Event::MouseButtonPressed:
				CheckInput();
				cout << "Mouse pressed at: " << Mouse::getPosition(window).x << " " << Mouse::getPosition(window).y << endl;
				break;
			case Event::MouseButtonReleased:
				cout << "Mouse released at: " << Mouse::getPosition(window).x << " " << Mouse::getPosition(window).y << endl;
				break;
			case Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}

		//DRAW MAP
		for each (Sprite s in map_sprite)
			window.draw(s);
		for each (Sprite s in numbers_sprite)
			window.draw(s);

		//DRAW RESOURCES
		for (int i = 0; i < 5; i++) {
			window.draw(res_sprite[i]);
			res_text[i].setString(std::to_string(res_mount[i]));
			window.draw(res_text[i]);
		}

		//DRAW TURN TEXT
		if (*myTurn)
			turn.setString("Your turn");
		else
			turn.setString("");

		window.draw(turn);
		window.draw(rect_render);
		window.display();

	}

private:
	void ScaleSprites() {
		//RESOURCES
		for (int i = 0; i < 5; i++)
			res_sprite[i].setScale(Vector2f(0.5, 0.5));
	}

#pragma region Trying input

	//TRYING INPUT
	RectangleShape rect_render;
	IntRect rect_collider;

	void CreateInputRect() {
		rect_render = RectangleShape(Vector2f(25, 25));
		rect_render.setPosition(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2);
		rect_render.setOutlineThickness(2);
		rect_render.setOutlineColor(Color::Red);
		rect_render.setFillColor(Color::Transparent);
		rect_collider = IntRect((Vector2i)rect_render.getPosition(), Vector2i(25, 25));
	}
	
	void CheckInput() {
		if (rect_collider.contains(Mouse::getPosition(window)))
			rect_render.setFillColor(Color::Blue);
	}
	
	//-----------------

#pragma endregion

	void LoadSprites() {
		try {
			//--------------------- MAP ---------------------

			if (!map_texture[0].loadFromFile(MAP_0) ||
				!map_texture[1].loadFromFile(MAP_1) ||
				!map_texture[2].loadFromFile(MAP_2) ||
				!map_texture[3].loadFromFile(MAP_3) ||
				!map_texture[4].loadFromFile(MAP_4) ||
				!map_texture[5].loadFromFile(MAP_5)) {
				throw 0;
			}

			for (int i = 0; i < 6; i++) {
				map_texture[i].setSmooth(true);
			}

			map_sprite[0].setTexture(map_texture[0]);
			map_sprite[1].setTexture(map_texture[1]);
			map_sprite[2].setTexture(map_texture[1]);
			map_sprite[3].setTexture(map_texture[1]);
			map_sprite[4].setTexture(map_texture[2]);
			map_sprite[5].setTexture(map_texture[2]);
			map_sprite[6].setTexture(map_texture[2]);
			map_sprite[7].setTexture(map_texture[2]);
			map_sprite[8].setTexture(map_texture[3]);
			map_sprite[9].setTexture(map_texture[3]);
			map_sprite[10].setTexture(map_texture[3]);
			map_sprite[11].setTexture(map_texture[4]);
			map_sprite[12].setTexture(map_texture[4]);
			map_sprite[13].setTexture(map_texture[4]);
			map_sprite[14].setTexture(map_texture[4]);
			map_sprite[15].setTexture(map_texture[5]);
			map_sprite[16].setTexture(map_texture[5]);
			map_sprite[17].setTexture(map_texture[5]);
			map_sprite[18].setTexture(map_texture[5]);


			//----------------- NUMBERS ------------------

			if (!numbers_texture[0].loadFromFile(NUMBER_2) ||
				!numbers_texture[1].loadFromFile(NUMBER_3) || 
				!numbers_texture[2].loadFromFile(NUMBER_4) ||
				!numbers_texture[3].loadFromFile(NUMBER_5) || 
				!numbers_texture[4].loadFromFile(NUMBER_6) ||
				!numbers_texture[5].loadFromFile(NUMBER_8) ||
				!numbers_texture[6].loadFromFile(NUMBER_9) ||
				!numbers_texture[7].loadFromFile(NUMBER_10) ||
				!numbers_texture[8].loadFromFile(NUMBER_11) ||
				!numbers_texture[9].loadFromFile(NUMBER_12)) {
				throw 1;
			}
			
			for (int i = 0; i < 10; i++) {
				numbers_texture[i].setSmooth(true);
			}

			numbers_sprite[0].setTexture(numbers_texture[0]);
			numbers_sprite[1].setTexture(numbers_texture[1]);
			numbers_sprite[2].setTexture(numbers_texture[1]);
			numbers_sprite[3].setTexture(numbers_texture[2]);
			numbers_sprite[4].setTexture(numbers_texture[2]);
			numbers_sprite[5].setTexture(numbers_texture[3]);
			numbers_sprite[6].setTexture(numbers_texture[3]);
			numbers_sprite[7].setTexture(numbers_texture[4]);
			numbers_sprite[8].setTexture(numbers_texture[4]);
			numbers_sprite[9].setTexture(numbers_texture[5]);
			numbers_sprite[10].setTexture(numbers_texture[5]);
			numbers_sprite[11].setTexture(numbers_texture[6]);
			numbers_sprite[12].setTexture(numbers_texture[6]);
			numbers_sprite[13].setTexture(numbers_texture[7]);
			numbers_sprite[14].setTexture(numbers_texture[7]);
			numbers_sprite[15].setTexture(numbers_texture[8]);
			numbers_sprite[16].setTexture(numbers_texture[8]);
			numbers_sprite[17].setTexture(numbers_texture[9]);
		
			//------------------ RESOURCES ---------------------

			if (!res_texture[0].loadFromFile(RES_1) ||
				!res_texture[1].loadFromFile(RES_2) ||
				!res_texture[2].loadFromFile(RES_3) ||
				!res_texture[3].loadFromFile(RES_4) ||
				!res_texture[4].loadFromFile(RES_5)){
				throw 0;
			}

			for (int i = 0; i < 6; i++) {
				res_texture[i].setSmooth(true);
			}

			res_sprite[0].setTexture(res_texture[0]);
			res_sprite[1].setTexture(res_texture[1]);
			res_sprite[2].setTexture(res_texture[2]);
			res_sprite[3].setTexture(res_texture[3]);
			res_sprite[4].setTexture(res_texture[4]);
		}
		catch (int error) {
			cout << "Error: " << error << endl;
		}
	}

	void MoveSprites() {
		//CENTER
		map_sprite[0].setPosition(SCREEN_WIDTH / 2 - map_sprite[0].getTexture()->getSize().x / 2 + 150, SCREEN_HEIGHT / 2 - map_sprite[0].getTexture()->getSize().y / 2);
		map_sprite[8].setPosition(map_sprite[0].getPosition() + Vector2f(map_sprite[1].getTexture()->getSize().x, 0));
		map_sprite[12].setPosition(map_sprite[0].getPosition() + Vector2f(2 * map_sprite[2].getTexture()->getSize().x, 0));
		map_sprite[11].setPosition(map_sprite[0].getPosition() - Vector2f(map_sprite[3].getTexture()->getSize().x, 0));
		map_sprite[15].setPosition(map_sprite[0].getPosition() - Vector2f(2 * map_sprite[4].getTexture()->getSize().x, 0));
		//1 DOWN
		map_sprite[9].setPosition(map_sprite[0].getPosition() + Vector2f(map_sprite[5].getTexture()->getSize().x / 2, map_sprite[1].getTexture()->getSize().y*0.75));
		map_sprite[1].setPosition(map_sprite[9].getPosition() + Vector2f(map_sprite[6].getTexture()->getSize().x, 0));
		map_sprite[16].setPosition(map_sprite[9].getPosition() - Vector2f(map_sprite[7].getTexture()->getSize().x, 0));
		map_sprite[4].setPosition(map_sprite[9].getPosition() - Vector2f(2 * map_sprite[8].getTexture()->getSize().x, 0));
		//2 DOWN
		map_sprite[13].setPosition(map_sprite[0].getPosition() + Vector2f(0, map_sprite[9].getTexture()->getSize().y*1.5));
		map_sprite[17].setPosition(map_sprite[13].getPosition() + Vector2f(map_sprite[10].getTexture()->getSize().x, 0));
		map_sprite[5].setPosition(map_sprite[13].getPosition() - Vector2f(map_sprite[11].getTexture()->getSize().x, 0));
		//1 UP
		map_sprite[18].setPosition(map_sprite[0].getPosition() - Vector2f(map_sprite[5].getTexture()->getSize().x / 2, map_sprite[1].getTexture()->getSize().y*0.75));
		map_sprite[6].setPosition(map_sprite[18].getPosition() + Vector2f(map_sprite[6].getTexture()->getSize().x, 0));
		map_sprite[14].setPosition(map_sprite[18].getPosition() - Vector2f(map_sprite[7].getTexture()->getSize().x, 0));
		map_sprite[2].setPosition(map_sprite[18].getPosition() + Vector2f(2 * map_sprite[8].getTexture()->getSize().x, 0));
		//2 UP
		map_sprite[10].setPosition(map_sprite[0].getPosition() - Vector2f(0, map_sprite[9].getTexture()->getSize().y*1.5));
		map_sprite[3].setPosition(map_sprite[10].getPosition() + Vector2f(map_sprite[10].getTexture()->getSize().x, 0));
		map_sprite[7].setPosition(map_sprite[10].getPosition() - Vector2f(map_sprite[11].getTexture()->getSize().x, 0));

		//NUMBERS
		//for (int i = 0; i < NUM_CELLS - 1; i++)
		//	numbers_sprite[i].setPosition(map_sprite[i + 1].getPosition());
		numbers_sprite[0].setPosition(map_sprite[7].getPosition());
		numbers_sprite[1].setPosition(map_sprite[10].getPosition());
		numbers_sprite[3].setPosition(map_sprite[3].getPosition());
		numbers_sprite[5].setPosition(map_sprite[14].getPosition());
		numbers_sprite[7].setPosition(map_sprite[18].getPosition());
		numbers_sprite[9].setPosition(map_sprite[6].getPosition());
		numbers_sprite[11].setPosition(map_sprite[2].getPosition());
		numbers_sprite[13].setPosition(map_sprite[15].getPosition());
		numbers_sprite[15].setPosition(map_sprite[11].getPosition());
		numbers_sprite[17].setPosition(map_sprite[8].getPosition());
		numbers_sprite[16].setPosition(map_sprite[12].getPosition());
		numbers_sprite[14].setPosition(map_sprite[4].getPosition());
		numbers_sprite[12].setPosition(map_sprite[16].getPosition());
		numbers_sprite[10].setPosition(map_sprite[9].getPosition());
		numbers_sprite[8].setPosition(map_sprite[1].getPosition());
		numbers_sprite[6].setPosition(map_sprite[5].getPosition());
		numbers_sprite[4].setPosition(map_sprite[13].getPosition());
		numbers_sprite[2].setPosition(map_sprite[17].getPosition());

		//RESOURCES
		res_sprite[0].setPosition(Vector2f(25, 50));
		res_sprite[1].setPosition(Vector2f(25, 150));
		res_sprite[2].setPosition(Vector2f(25, 250));
		res_sprite[3].setPosition(Vector2f(25, 350));
		res_sprite[4].setPosition(Vector2f(25, 450));
	}

#pragma endregion
};