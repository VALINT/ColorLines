#include "Menu.h"

Menu::Menu(std::map<std::string, sf::Sprite>* Graphics, sf::RenderWindow * Window, sf::View * View)
{
	this->graphics = Graphics;
	this->outWindow = Window;
	this->View = View;
}

void Menu::draw()
{
	for (auto i : ButtonsSet)
	{
		i.draw(*outWindow);
	}
}

void Menu::traceMouse(sf::Vector2f Coordinates)
{
	for (auto& i : ButtonsSet)
		i.traceMouse(Coordinates);
}

void Menu::addButton(Button bt)
{
	ButtonsSet.push_back(bt);
}
