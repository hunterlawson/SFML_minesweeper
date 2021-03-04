#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
class Button {
public:
	Button(int _x, int _y, int _width, int _height);
	bool isPressed(int _x, int _y);
	void display(sf::RenderWindow &window, const sf::Texture &texture);
private:
	bool pressed = false;
	int x, y, width, height;
};