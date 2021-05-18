#include "Highscore.h"

Highscore::Highscore(std::pair< std::map< std::string, sf::Texture >, std::map< std::string, std::map< std::string, sf::Sprite > > >* Graphics,
					  sf::RenderWindow* Window,
					  sf::Font* font) : bg(sf::Vector2f(500, 500), sf::Vector2f(230, 50), BG_SEMI_COLOR, false),
									    lbl("Highscore", font, sf::Vector2f(500, 50), sf::Vector2f(230, 5), 40, BG_COLOR, TEXT_COLOR, false, true)
{
	this->graphics = Graphics;
	this->outWindow = Window;
	//this->View = View;
	this->font = font;
}

void Highscore::setScoreLine(std::pair<int, std::string> element)
{
	scoreboard.push_back(element);
	sort(scoreboard.begin(), scoreboard.end(), [=](std::pair<int, std::string>& a, std::pair<int, std::string>& b)
	{
		return a.first > b.first;
	});
}

int Highscore::getPlace(int score)
{
	int j = 1;
	for (auto i : scoreboard)
	{
		if (score >= i.first) return j;
		j++;
	}
	return j;
}

void Highscore::biuldTable(void)
{
	labels.clear();
	int j = 0;
	for (auto &i : scoreboard)
	{
		labels.push_back(Label(i.second,					font, sf::Vector2f(205, 40), sf::Vector2f(275, 55 + 45 * j), 24, BG_COLOR, TEXT_COLOR, true, false));
		labels.push_back(Label((std::to_string(i.first)),	font, sf::Vector2f(205, 40), sf::Vector2f(485, 55 + 45 * j), 24, BG_COLOR, TEXT_COLOR, true, false));
		if (++j >= 10) break;
	}
}

void Highscore::draw()
{
	//outWindow->draw(graphics->second["Menu_Background"]["Menu_Background"]);
	bg.draw(*outWindow);

	for (auto &i : labels)
	{
		i.draw(*outWindow);
	}
	lbl.draw(*outWindow);
}
