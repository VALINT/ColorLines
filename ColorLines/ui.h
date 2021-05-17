//SFML-UI
//---------------------------------------------------------------------------------------------------------------------------------------
//
//
//
//
//
//
//---------------------------------------------------------------------------------------------------------------------------------------
//	Current version: v0.7
//---------------------------------------------------------------------------------------------------------------------------------------
//Story:
//	05.11.2020	/	update v0.1
//		Creation of this file.
//		- Realized Label class.
//		- Realized Button class.
//	
//	06.11.2020	/	update v0.2
//		- Realized Icon class.
//		- Button class updated. 
//		Now button can use Icon or Label for drawing TextButton or IconButton.
//
//	09.11.2020	/	update v0.3
//		- Creation of Background class.
//		Background is the ancestor of Label and Icon.
//		Label and Icon were simplified.
//		Button class inherits the Label and the Icon classes.
//		UDP: Bad idea(
//
//	10.11.2020	/	update v0.4
//		- Creation of Toggle class.
//		Toggle inherite Button class.
//		- Creation of Tab class.
//		Tab class inherite Toggle class.
//		- Creation of TabBar class.
//
//	16.11.2020	/	update v0.5
//		- TabBar class work logic was remade.
//		- Bar class work logic was remade, Bar class was simplified.
//
//	20.11.2020	/	update v0.6
//		I hate this shit and personaly @enexim.
//		I have to rewrite this fucking Button class realization
//		UDP: Fuck yeah, with the first try))
//		- Label and Icon, not ancestors of Button anymore.
//----------------------------------------------------------------------------------------------------------------------------------------

#ifndef _UI_H_
#define _UI_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <sstream>
#include <string>
#include "Event.h"

#define DELETE_KEY 8
#define ENTER_KEY  13

class Background
{
public:
	Background(sf::Vector2f, sf::Vector2f, sf::Color bgColor, bool);
	sf::Vector2f getPosition(void);
	sf::Vector2f getSize(void);
	sf::FloatRect getRect(void);
	void setBgColor(sf::Color);
	void setPosition(sf::Vector2f);
	void setSize(sf::Vector2f);
	void setSize(sf::FloatRect);
	void setTransparentBg(void);
	void draw(sf::RenderWindow &Window);

private:
	void buildBox(void);

	sf::Color bgColor;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f position;
	sf::Vector2f size;
	bool transparentBg;
};

class Label
{
public:
	Label(std::string, sf::Font*, sf::Vector2f, sf::Vector2f, int, sf::Color, sf::Color, bool = false, bool = false);
	void setText(std::string);
	void setTextColor(sf::Color);
	void setTextSize(int);
	void setFont(sf::Font*);
	sf::Vector2f getPosition(void);
	sf::Vector2f getSize(void);
	sf::FloatRect getRect(void);
	void setBgColor(sf::Color);
	void setPosition(sf::Vector2f);
	void setSize(sf::Vector2f);
	void setSize(sf::FloatRect);
	void setCenterAllign(void);
	std::string getText(void);
	void draw(sf::RenderWindow &Window);
	
private:
	void buildBox(void);
	Background background;
	sf::Text text;
	bool autoSizing;
	bool allignCenter;
};

class Icon : public Background
{
public:
	Icon(sf::Texture*, sf::IntRect, sf::Vector2f, sf::Vector2f, sf::Color, bool);
	Icon();
	void setSprite(sf::Sprite);
	void setRect(sf::IntRect);
	void draw(sf::RenderWindow &Window);

private:
	sf::Sprite sprite;
};

class TextBox : public Label
{
public:
	TextBox(sf::Font*, sf::Vector2f, sf::Vector2f, int, sf::Color, sf::Color);
	void setString(std::string);
	void setLimit(int);
	void setLimit(bool);
	void traceMouse(sf::Vector2f);
	void inputText(sf::Event);
	bool getStatus(void);
	std::string getString(void);

private:
	void input_logic(int unicode);
	void delete_last(void);

	std::string intermediate;
	int limit;
	bool hasLimit = false;
	bool active = false;
};

class Button
{
public:
	Button(std::string , 
			sf::Font*, 
			sf::Vector2f, 
			sf::Vector2f, 
			int, 
			sf::Color, 
			sf::Color, 
			sf::Color, 
			sf::Color, 
			void(*ev)(std::pair<int, int>),
			int = 0);
	Button(sf::Texture*, 
			sf::IntRect, 
			sf::Vector2f, 
			sf::Vector2f, 
			sf::Color, 
			sf::Color, 
			sf::Color,
			void(*ev)(std::pair<int, int>),
			int = 0);
	void setHoverColor(sf::Color);
	void setHoldColor(sf::Color);
	void setCallback(void(*ev)(std::pair<int, int>));
	bool traceMouse(sf::Vector2f);
	bool getClickEvent(void);
	void draw(sf::RenderWindow &Window);
	void setIndex(int);
	bool isHover(void);

protected:
	Label label;
	Icon  icon;
	sf::Color hoverColor;
	sf::Color holdColor;
	sf::Color freeColor;
	bool clickEvent;

	std::string name;
	int	index;
	void (* eventcallback)(std::pair<int, int>);
	//Support variables
	bool hover;
	bool hold;
	bool useIcon;
};

/*
class Toggle : public Button
{
public:
	Toggle(std::string, sf::Font*, sf::Vector2f, sf::Vector2f, int, sf::Color, sf::Color);
	Toggle(sf::Texture*, sf::IntRect, sf::Vector2f, sf::Vector2f, sf::Color, sf::Color);
	Toggle(sf::Texture*, sf::IntRect, sf::IntRect, sf::Vector2f, sf::Vector2f);
	Toggle(sf::Texture*, sf::Texture*, sf::IntRect, sf::Vector2f, sf::Vector2f);
	void setActiveState(void);
	void setPassiveState(void);
	void setActiveColor(sf::Color);
	void setPassiveColor(sf::Color);
	void traceMouse(sf::Vector2f);
	void renewState(bool);
	bool getState(void);


private:

	sf::Texture Texture;
	sf::Sprite activeSprite;
	sf::Sprite passiveSprite;
	sf::Color activeColor;
	sf::Color passiveColor;

	bool textMode;
	bool useColorIndication;
	bool state;
	bool hover;
	bool hold;
};

class RadioButton : public Toggle
{
public:
	RadioButton(sf::Texture*, sf::IntRect, sf::Vector2f, sf::Vector2f, sf::Color, sf::Color);
	RadioButton(sf::Texture*, sf::IntRect, sf::IntRect, sf::Vector2f, sf::Vector2f);
	RadioButton(sf::Texture*, sf::Texture*, sf::IntRect, sf::Vector2f, sf::Vector2f);

private:

};

class Tab : public Toggle
{
public:
	Tab(std::string, sf::Font*, sf::Vector2f, sf::Vector2f, int, sf::Color, sf::Color);
	bool traceMouse(sf::Vector2f);
};

class TabBar
{
public:
	TabBar();
	void addTab(Tab Tab);
	void createTab(std::string, sf::Font*, sf::Vector2f, sf::Vector2f, int, sf::Color, sf::Color, sf::Color);
	void traceMouse(sf::Vector2f);
	void clearBar(void);
	void draw(sf::RenderWindow &Window);
	int getCurrentTab(void);

private:
	std::vector<Tab> tabArray;
	int lastActive = 0;
};
*/
#endif