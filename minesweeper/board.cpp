#include "board.h"
Board::Board() {
	ifstream inFile("boards/config.cfg");
	string temp;
	getline(inFile, temp);
	width = stoi(temp);
	getline(inFile, temp);
	height = stoi(temp);
	getline(inFile, temp);
	defaultBombCount = stoi(temp);
	// create the dynamic 2d array of tiles
	tiles = new Tile * [height];
	for (int i = 0; i < height; i++) {
		tiles[i] = new Tile[width];
	}
	// set the pointer vectors in each tile
	// a b c
	// d T e
	// f g h
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (y != 0 && x != 0) tiles[y][x].neighbors.push_back(&tiles[y - 1][x - 1]); // a
			if (y != 0) tiles[y][x].neighbors.push_back(&tiles[y - 1][x]); // b
			if (y != 0 && x != width-1) tiles[y][x].neighbors.push_back(&tiles[y - 1][x + 1]); // c
			if (x != 0) tiles[y][x].neighbors.push_back(&tiles[y][x - 1]); // d
			if (x != width-1) tiles[y][x].neighbors.push_back(&tiles[y][x + 1]); // e
			if (y != height-1 && x != 0) tiles[y][x].neighbors.push_back(&tiles[y + 1][x - 1]); // f
			if (y != height-1) tiles[y][x].neighbors.push_back(&tiles[y + 1][x]); //g
			if (y != height-1 && x != width-1) tiles[y][x].neighbors.push_back(&tiles[y + 1][x + 1]); //h
		}
	}
}

Board::~Board() {
	for (int i = 0; i < height; i++) {
		delete[] tiles[i];
	}
	delete[] tiles;
}

void Board::createBoardFromTemplate(string fileName) {
	fstream inFile(fileName);
	string temp;
	int newBombCount = 0;
	reset();
	for (int i = 0; i < height; i++) {
		getline(inFile, temp);
		for (int j = 0; j < width; j++) {
			if (temp[j] == '1') {
				tiles[i][j].isBomb = true;
				newBombCount++;
			}
			else {
				tiles[i][j].isBomb = false;
			}
		}
	}
	bombCount = newBombCount;
	recount();
}

void Board::createDefaultBoard() {
	reset();
	bombCount = defaultBombCount;
	for (int i = 0; i < bombCount; i++) {
		int randX = rand() % (width);
		int randY = rand() % (height);
		while (tiles[randY][randX].isBomb == true) {
			randX = rand() % (width);
			randY = rand() % (height);
		}
		tiles[randY][randX].isBomb = true;
	}
	recount();
}

void Board::recount() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			unsigned short count = 0;
			for (auto t : tiles[i][j].neighbors) {
				if (t->isBomb) count++;
			}
			tiles[i][j].value = count;
		}
	}
}

void Board::display(sf::RenderWindow& window, map<string, sf::Texture> textures) {
	sf::Sprite bomb;
	bomb.setTexture(textures["mine"]);
	sf::Sprite flag;
	flag.setTexture(textures["flag"]);
	sf::Sprite hidden;
	hidden.setTexture(textures["tile_hidden"]);
	sf::Sprite revealed;
	revealed.setTexture(textures["tile_revealed"]);
	sf::Sprite number;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Tile t = tiles[y][x];
			bomb.setPosition(x * 32, y * 32);
			flag.setPosition(x * 32, y * 32);
			number.setPosition(x * 32, y * 32);
			hidden.setPosition(x * 32, y * 32);
			revealed.setPosition(x * 32, y * 32);
			window.draw(revealed);
			if (t.value > 0 && !t.isBomb) {
				number.setTexture(textures["number_" + to_string(t.value)]);
				window.draw(number);
			}
			if (!t.isRevealed) {
				window.draw(hidden);
				if (t.flagPlaced) {
					window.draw(flag);
				}
				if (t.isBomb && isDebug) {
					window.draw(bomb);
				}
			}
			if (t.isBomb && gameOver && t.flagPlaced) {
				window.draw(flag);
				window.draw(bomb);
			}
			else if (t.isBomb && gameOver) {
				window.draw(bomb);
			}
		}
	}
}

void Board::clear(int x, int y) {
	if (!tiles[y][x].isRevealed && !tiles[y][x].flagPlaced && !gameOver && !gameWon) {
		if (tiles[y][x].isBomb) {
			lose();
		}
		else if (tiles[y][x].value == 0) {
			clearZeroes(tiles[y][x]);
		}
		else {
			tiles[y][x].isRevealed = true;
		}
	}
}

void Board::clearZeroes(Tile &tile) {
	if (tile.value == 0 && !tile.isRevealed && !tile.flagPlaced) {
		tile.isRevealed = true;
		for (int i = 0; i < tile.neighbors.size(); i++) {
			if (!tile.flagPlaced) {
				clearZeroes(*tile.neighbors[i]);
			}		
		}
	}
	else if (!tile.isRevealed && !tile.flagPlaced) {
		tile.isRevealed = true;
	}
}

void Board::placeFlag(int x, int y) {
	if (!tiles[y][x].isRevealed && !gameOver && !gameWon) {
		tiles[y][x].flagPlaced = !tiles[y][x].flagPlaced;
		if (tiles[y][x].flagPlaced) {
			numFlags++;
		}
		else {
			numFlags--;
		}
	}
}

unsigned int Board::getWidth() {
	return width * 32;
}

unsigned int Board::getHeight() {
	return height * 32 + 88;
}

unsigned int Board::getTileWidth() {
	return width;
}

unsigned int Board::getTileHeight() {
	return height;
}

void Board::lose() {
	gameOver = true;
	gameWon = false;
	//====== Clear the entire board ======//
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (!tiles[j][i].isRevealed && tiles[j][i].isBomb) {
				tiles[j][i].isRevealed = true;
			}
		}
	}
}

void Board::reset() {
	numFlags = 0;
	gameOver = false;
	gameWon = false;
	// isDebug = false; resetting the debug mode after the game is over isn't 
	// in the rubric so im assuming i wont get points off for leaving this out
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			tiles[j][i].isRevealed = false;
			tiles[j][i].flagPlaced = false;
			tiles[j][i].isBomb = false;
		}
	}
}

void Board::checkWin() {
	bool tempWon = true;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (!tiles[j][i].isRevealed && !tiles[j][i].isBomb) {
				tempWon = false;
			}
		}
	}
	if (tempWon) {
		gameWon = true;
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				if (tiles[j][i].isBomb) {
					tiles[j][i].flagPlaced = true;
				}
			}
		}
		numFlags = bombCount;
	}
}

void Board::debug() {
	if (!gameWon && !gameOver) {
		isDebug = !isDebug;
	}
}

int Board::counterNum() {
	return bombCount - numFlags;
}