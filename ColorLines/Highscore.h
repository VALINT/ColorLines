#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

#include <SFML/Graphics.hpp>
#include "ui.h"
#include "common.h"

class Highscore
{
public:
	Highscore(	std::pair< std::map< std::string, sf::Texture >, std::map< std::string, std::map< std::string, sf::Sprite > > >*,
				sf::RenderWindow*,
				sf::Font*);
	void setScoreLine(std::pair<int, std::string>);
	int  getPlace(int);
	void biuldTable(void);
	void draw();

private:
	//Label ScoresLabel;
	std::pair< std::map< std::string, sf::Texture >, std::map< std::string, std::map< std::string, sf::Sprite > > >* graphics;
	sf::RenderWindow    *outWindow;
	sf::View			*View;
	std::vector<std::pair<int, std::string>> scoreboard;
	std::vector<Label>	labels;
	sf::Font*			font;
	Background			bg;// (sf::Vector2f(500, 500), sf::Vector2f(0, 0), BG_COLOR, false);
	Label				lbl;
};

#endif