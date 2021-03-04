#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;
class Board
{
public:
	struct Tile {
		unsigned short value = 0;
		bool isBomb = false;
		bool isRevealed = false;
		bool flagPlaced = false;
		vector<Tile*> neighbors;
	};
	Board(); // create an empty board using the default values in config.cfg
	~Board(); // deconstructor
	void createBoardFromTemplate(string fileName); // generate a board from a bomb template file
	void createDefaultBoard(); // generate board with random bombs
	void recount(); // reassign values to the tiles
	void clear(int x, int y);
	void placeFlag(int x, int y);
	void display(sf::RenderWindow &window, map<string, sf::Texture> textures); // draw the board to the screen
	void checkWin(); // check if the game has been won
	void debug(); // enable / disable debug mode;
	unsigned int getWidth();
	unsigned int getHeight();
	unsigned int getTileWidth();
	unsigned int getTileHeight();
	int counterNum();
	bool gameWon = false;
	bool gameOver = false;
	int numFlags = 0;
private:
	unsigned int width, height;
	int bombCount = 0, defaultBombCount = 0;
	Tile** tiles;
	bool isDebug = false;
	void clearZeroes(Tile &tile); // clear any neighboring zero tiles (recursive);
	void lose(); // reveal all tiles if the game is over
	void reset(); // reset the board (set all tiles to default values)
};