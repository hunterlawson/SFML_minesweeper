#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>
#include "board.h"
#include "button.h"
#include "counter.h"
using namespace std;

int main()
{
    //====== Set the random seed and create the board & window ======//
    srand(time(nullptr));
    Board board;
    board.createDefaultBoard();
    sf::RenderWindow window(sf::VideoMode(board.getWidth(), board.getHeight()), "Minesweeper");
    //====== Load texture files ======//
    map<string, sf::Texture> textures;
    sf::Texture temp;
    string fileNames[] = { "mine", "tile_hidden", "tile_revealed", "number_1", "number_2", "number_3", 
                           "number_4", "number_5", "number_6", "number_7", "number_8", "flag", "face_happy", 
                           "face_win", "face_lose", "digits", "debug", "test_1", "test_2", "test_3"};
    for (int i = 0; i < 20; i++) {
        temp.loadFromFile("images/" + fileNames[i] + ".png");
        textures.emplace(fileNames[i], temp);
    }
    //====== Load buttons ======//
    int smileX = board.getWidth() / 2 - 32;
    int smileY = board.getHeight() - 88;
    Button faceButton(smileX, smileY, 64, 64);
    Button debugButton(smileX + 64*2, smileY, 64, 64);
    Button test1Button(smileX + 64*3, smileY, 64, 64);
    Button test2Button(smileX + 64*4, smileY, 64, 64);
    Button test3Button(smileX + 64*5, smileY, 64, 64);
    //====== Load counter ======//
    Counter counter;
    //====== Event loop ======//
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    //====== Left Button Pressed ======//
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int x = floor(mousePos.x/32);
                    int y = floor(mousePos.y/32);
                    if (mousePos.y < board.getTileHeight()*32) {
                        board.clear(x, y);
                    } else if (faceButton.isPressed(mousePos.x, mousePos.y)) {
                        //====== Reset the board ======//
                        board.createDefaultBoard();
                    }
                    else if (debugButton.isPressed(mousePos.x, mousePos.y)) {
                        //====== Enable debug mode ======//
                        board.debug();
                    }
                    else if (test1Button.isPressed(mousePos.x, mousePos.y)) {
                        //====== Test board 1 ======//
                        board.createBoardFromTemplate("boards/testboard1.brd");
                    }
                    else if (test2Button.isPressed(mousePos.x, mousePos.y)) {
                        //====== Test board 2 ======//
                        board.createBoardFromTemplate("boards/testboard2.brd");
                    }
                    else if (test3Button.isPressed(mousePos.x, mousePos.y)) {
                        //====== Test board 3 ======//
                        board.createBoardFromTemplate("boards/testboard3.brd");
                    }
                }
                else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                    //====== Right Button Pressed ======//
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    int x = floor(mousePos.x / 32);
                    int y = floor(mousePos.y / 32);
                    if (mousePos.y < board.getTileHeight() * 32) {
                        board.placeFlag(x, y);
                    }
                }
                break;
            }
        }
        window.clear(sf::Color::White);
        //====== Draw the game board ======//
        board.display(window, textures);
        board.checkWin();
        //====== Draw the buttons, etc. ======//
        if (board.gameOver) {
            faceButton.display(window, textures["face_lose"]);
        }
        else if (board.gameWon) {
            faceButton.display(window, textures["face_win"]);
        }
        else {
            faceButton.display(window, textures["face_happy"]);
        }
        debugButton.display(window, textures["debug"]);
        test1Button.display(window, textures["test_1"]);
        test2Button.display(window, textures["test_2"]);
        test3Button.display(window, textures["test_3"]);
        counter.display(board.getTileHeight() * 32, board.counterNum(), window, textures["digits"]);
        window.display();
    }
    return 0;
}