#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;
class Counter {
public:
	Counter();
	void display(int y, int value, sf::RenderWindow &window, const sf::Texture &texture);
private:
	int value;
};