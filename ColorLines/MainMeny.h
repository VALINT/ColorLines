#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <map>
#include "Menu.h"

class MainMenu : public Menu
{
public:
	MainMenu(	std::pair< std::map< std::string, sf::Texture >, std::map< std::string, std::map< std::string, sf::Sprite > > >*,
				sf::RenderWindow*,
				sf::View*);
	void draw();

private:
	std::pair< std::map< std::string, sf::Texture >, std::map< std::string, std::map< std::string, sf::Sprite > > >* graphics;
	sf::RenderWindow    *outWindow;
	sf::View			*View;
};

#endif

