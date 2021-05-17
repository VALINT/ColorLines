#include "MainMeny.h"

MainMenu::MainMenu(	std::pair< std::map< std::string, sf::Texture >, std::map< std::string, std::map< std::string, sf::Sprite > > >* Graphics,
					sf::RenderWindow* Window,
					sf::View* View) : Menu(nullptr, Window, View)
{
	this->graphics = Graphics;
	this->outWindow = Window;
	this->View = View;
}

void MainMenu::draw()
{
	outWindow->draw(graphics->second["Menu_Background"]["Menu_Background"]);
	outWindow->draw(graphics->second["Menu_frame"]["Menu_frame"]);
	Menu::draw();
}
