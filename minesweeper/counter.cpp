#include "counter.h"
Counter::Counter() {
	value = 0;
}

void Counter::display(int y, int value, sf::RenderWindow &window, const sf::Texture &texture) {
	// set the values of the digits
	sf::Sprite negative;
	sf::Sprite digit;
	digit.setTexture(texture);
	negative.setTexture(texture);
	negative.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
	negative.setPosition(0, y);
	if (value < 0) {
		window.draw(negative);
		value = abs(value);
	}
	string num = to_string(value);
	int numToAppend = 3 - num.length();
	for (int i = 0; i < numToAppend; i++) {
		num = "0" + num;
	}
	int temp = 0;
	for (int i = 0; i < 3; i++) {
		temp = num[i] - 48;
		digit.setTextureRect(sf::IntRect(temp * 21, 0, 21, 32));
		digit.setPosition(21 + 21 * i, y);
		window.draw(digit);
	}
}