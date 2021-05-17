#ifndef _MENU_H_
#define _MENU_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <map>
#include "ui.h"

#define BG_COLOR sf::Color(0x21, 0x21, 0x21)
#define BG2_COLOR sf::Color(0x1A,0x1A,0x1A)
#define BG3_COLOR sf::Color(0x11,0x11,0x11)
#define TEXT_COLOR sf::Color(0xf0, 0xf0, 0xff)
#define HOVER_COLOR sf::Color(0x35, 0x35, 0x35)
#define HOLD_COLOR sf::Color(0x0B, 0x0B, 0x0B)

class Menu
{
public:
	Menu(std::map<std::string, sf::Sprite>* Graphics, sf::RenderWindow* Window, sf::View* View);
	void draw();
	void traceMouse(sf::Vector2f Coordinates);
	void addButton(Button);

private:
	std::vector<Button> ButtonsSet;
	std::map<std::string, sf::Sprite>* graphics;
	sf::RenderWindow    *outWindow;
	sf::View			*View;
};

#endif // !_MENU_H_

