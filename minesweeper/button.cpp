#include "button.h"
Button::Button(int _x, int _y, int _width, int _height) {
	x = _x;
	y = _y;
	width = _width;
	height = _height;
}

bool Button::isPressed(int _x, int _y) {
	if (_x >= x && _x <= x + width && _y >= y && _y <= y + height) {
		return true;
	}
	return false;
}

void Button::display(sf::RenderWindow& window, const sf::Texture &texture) {
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(x, y);
	window.draw(sprite);
}