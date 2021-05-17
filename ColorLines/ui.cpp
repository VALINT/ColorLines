#include "ui.h"

//Background methods
//Private
void Background::buildBox()
{
	sf::Image img;
	img.create(int(size.x), int(size.y), bgColor);
	texture.loadFromImage(img);
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(position), sf::Vector2i(size)));
	sprite.setPosition(position);
}

//Public
Background::Background(sf::Vector2f Size, sf::Vector2f Position, sf::Color BgColor, bool Transparent)
{
	size = Size;
	position = Position;
	bgColor = BgColor;
	transparentBg = Transparent;
	buildBox();
}

sf::Vector2f Background::getPosition(void)
{
	return position;
}

sf::FloatRect Background::getRect(void)
{
	return sf::FloatRect(position, size);
}

sf::Vector2f Background::getSize(void)
{
	return size;
}

void Background::setBgColor(sf::Color BgColor)
{
	bgColor = BgColor;
	buildBox();
}

void Background::setPosition(sf::Vector2f Position)
{
	position = Position;
	buildBox();
}

void Background::setSize(sf::FloatRect Rect)
{
	size = sf::Vector2f(Rect.width + 10, Rect.height + 8);
	buildBox();
}

void Background::setSize(sf::Vector2f Size)
{
	size = Size;
	buildBox();
}

void Background::setTransparentBg(void)
{
	transparentBg = true;
}

void Background::draw(sf::RenderWindow &Window)
{
	if (!transparentBg) Window.draw(sprite);
}

// Label methods //
// Private
void Label::buildBox()
{
	if (allignCenter)	text.setPosition(background.getPosition().x + (background.getSize().x - text.getLocalBounds().width) / 2, background.getPosition().y);
	else 				text.setPosition(background.getPosition().x + 5, background.getPosition().y);
}

// Public
Label::Label(	std::string OutLabel = "", 
				sf::Font *Font = nullptr, 
				sf::Vector2f Size = sf::Vector2f(0, 0), 
				sf::Vector2f Position = sf::Vector2f(0, 0), 
				int TextSize = 20, 
				sf::Color BgColor = sf::Color(0xFF, 0xFF, 0xFF), 
				sf::Color TextColor = sf::Color(0x00, 0x00, 0x00), 
				bool Transparent,
				bool centr) : background(Size, Position,BgColor, Transparent)
{
	text.setString(OutLabel);
	text.setCharacterSize(TextSize);
	text.setFont(*Font);
	text.setColor(TextColor);

	//If string is not empty and Size = (0,0), autosize background shape.
	if (OutLabel != "")
	{
		if (Size == sf::Vector2f(0, 0)) background.setSize(sf::Vector2f(text.getLocalBounds().width + 10, text.getLocalBounds().height + 8));
		else
		{
			if (Size.y == 0)	background.setSize(sf::Vector2f(Size.x, text.getLocalBounds().height + 8));
			else				background.setSize(Size);

			if (Size.x == 0)	background.setSize(sf::Vector2f(text.getLocalBounds().width + 10, Size.y));
			else				background.setSize(Size);
		}
	}

	//text.setFillColor(TextColor);
	allignCenter = centr;

	// If string is empty and reseived size = (0,0), turn on autosizing mode
	autoSizing = (Size == sf::Vector2f(0, 0));

	// Create text content and create background shape.
	buildBox();
}

void Label::setText(std::string OutLabel)
{
	text.setString(OutLabel);
	buildBox();
	if (autoSizing) { background.setSize(text.getLocalBounds()); buildBox(); }
}

void Label::setTextColor(sf::Color TextColor)
{
	//text.setFillColor(TextColor);
}

void Label::setFont(sf::Font *Font)
{
	text.setFont(*Font);
	if (autoSizing) { background.setSize(text.getLocalBounds()); buildBox(); }
}

sf::Vector2f Label::getPosition(void)
{
	return background.getPosition();
}

sf::Vector2f Label::getSize(void)
{
	return background.getSize();
}

sf::FloatRect Label::getRect(void)
{
	return background.getRect();
}

void Label::setBgColor(sf::Color cl)
{
	background.setBgColor(cl);
}

void Label::setPosition(sf::Vector2f pos)
{
	background.setPosition(pos);
}

void Label::setSize(sf::Vector2f size)
{
	background.setSize(size);
}

void Label::setSize(sf::FloatRect size)
{
	background.setSize(size);
}

void Label::setTextSize(int TextSize)
{
	text.setCharacterSize(TextSize);
	if (autoSizing) { background.setSize(text.getLocalBounds()); buildBox(); }
}

void Label::setCenterAllign(void)
{
	allignCenter = true; 
	buildBox();
}

std::string Label::getText(void)
{
	return text.getString();
}

void Label::draw(sf::RenderWindow &Window)
{
	background.draw(Window);
	Window.draw(text);
}

//Button methods
//Private

//Public

Button::Button(std::string Name = "Button",
	sf::Font* Font = nullptr,
	sf::Vector2f Size = sf::Vector2f(0, 0),
	sf::Vector2f Pos = sf::Vector2f(0, 0),
	int TextSize = 20,
	sf::Color BgColor = sf::Color(0x00, 0x00, 0x00),
	sf::Color TextColor = sf::Color(0x00, 0x00, 0x00),
	sf::Color HoverColor = sf::Color(0x00, 0x00, 0x00),
	sf::Color HoldColor = sf::Color(0x00, 0x00, 0x00),
	void(*ev)(std::pair<int, int> data) = nullptr,
	int index) :
	label(Name, Font, Size, Pos, TextSize, BgColor, TextColor, 0)
{
	this->name = Name;
	this->eventcallback = ev;
	this->freeColor = BgColor;
	this->hoverColor = HoverColor;
	this->holdColor = HoldColor;
	this->hover = false;
	this->hold = false;
	this->clickEvent = false;
	this->useIcon = false;
	this->index = index;
	label.setCenterAllign();
}

Button::Button(sf::Texture *Texture,
	sf::IntRect Rect,
	sf::Vector2f Size,
	sf::Vector2f Pos,
	sf::Color BgColor,
	sf::Color HoverColor,
	sf::Color HoldColor,
	void(*ev)(std::pair<int, int>) = nullptr, 
	int index) :
	icon(Texture, Rect, Size, Pos, BgColor, 0)
{
	this->freeColor = BgColor;
	this->holdColor = HoldColor;
	this->hoverColor = HoverColor;
	this->hover = false;
	this->hold = false;
	this->clickEvent = false;
	this->useIcon = true;
	this->eventcallback = ev;
	this->index = index;
}

void Button::setHoverColor(sf::Color Color)
{
	hoverColor = Color;
}

void Button::setHoldColor(sf::Color Color)
{
	holdColor = Color;
}

void Button::setCallback(void(*ev)(std::pair<int, int>))
{
	this->eventcallback = ev;
}

bool Button::traceMouse(sf::Vector2f Coordinates)
{
	clickEvent = false;
	if ((useIcon)? icon.getRect().contains(Coordinates) : label.getRect().contains(Coordinates))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (!hold && hover)
			{
				if (useIcon)	icon.setBgColor(holdColor);
				else			label.setBgColor(holdColor);

				hold = true;
				return false;
			}
		}
		else
		{
			if (!hover)
			{
				hover = true;
				if (eventcallback != nullptr)
				{
					eventcallback(std::pair<int, int>(index, 1));
				}

				if (useIcon)	icon.setBgColor(hoverColor);
				else			label.setBgColor(hoverColor);
				return false;
			}
			if (hover && hold)
			{
				if (useIcon)	icon.setBgColor(hoverColor);
				else			label.setBgColor(hoverColor);
				hover = true;
				hold = false;
				clickEvent = true;
				
				if (eventcallback != nullptr)
				{
					eventcallback(std::pair<int, int>(index, 0));
				}
				return true;	
			}
		}
	}
	else
	{
		if (useIcon)	icon.setBgColor(freeColor);
		else			label.setBgColor(freeColor);
		hold = false;
		hover = false;
		return false;
	}
	return false;
}

bool Button::getClickEvent(void)
{
	return clickEvent;
}

void Button::draw(sf::RenderWindow &Window)
{
	if (useIcon)	icon.draw(Window);
	else			label.draw(Window);
}

void Button::setIndex(int index)
{
	this->index = index;
}

bool Button::isHover(void)
{
	return hover;
}

//Icon methods
//Private

//Public
Icon::Icon(	sf::Texture *Texture, 
			sf::IntRect Rect,
			sf::Vector2f Size, 
			sf::Vector2f Position, 
			sf::Color BgColor,
			bool Transparent) : Background(Size, Position, BgColor, Transparent)
{
	if (Texture)
	{
		sprite.setTexture(*Texture);
		sprite.setTextureRect(Rect);
		sprite.setPosition(Position);
	}
	
}

Icon::Icon() : Background(sf::Vector2f(0,0), sf::Vector2f(0, 0), sf::Color(0,0,0), 1)
{

}

void Icon::draw(sf::RenderWindow &Window)
{
	Background::draw(Window);
	Window.draw(sprite);

}

void Icon::setSprite(sf::Sprite Sprite)
{
	sprite = Sprite;
}

void Icon::setRect(sf::IntRect Rect)
{
	sprite.setTextureRect(Rect);
}

//TextBox methods
//Private
void TextBox::input_logic(int unicode)
{
	if (unicode != DELETE_KEY && unicode != ENTER_KEY)
	{
		intermediate += static_cast<char>(unicode);
	}
	else if(unicode == DELETE_KEY)
	{
		if (intermediate.length() > 0)
		{
			delete_last();
		}
	}
	else if (unicode == ENTER_KEY)
	{
		active = false;
	}

	Label::setText(intermediate);
}

void TextBox::delete_last(void)
{
	intermediate.pop_back();
}

//Public
TextBox::TextBox(sf::Font *Font = nullptr,
	sf::Vector2f Size = sf::Vector2f(10,10),
	sf::Vector2f Position = sf::Vector2f(0, 0),
	int TextSize = 20,
	sf::Color BgColor = sf::Color(0xFF, 0xFF, 0xFF),
	sf::Color TextColor = sf::Color(0x00, 0x00, 0x00)) :
	Label("_", Font, Size, Position, TextSize, BgColor, TextColor, 0)
{
	active = false;
}

bool TextBox::getStatus(void)
{
	return active;
}

std::string TextBox::getString(void)
{
	return intermediate;
}

void TextBox::setString(std::string String)
{
	intermediate = String;
	if (intermediate == "")
		this->setText("_");
	else
		this->setText(intermediate);
}

void TextBox::setLimit(int LimitSize)
{
	limit = LimitSize;
	hasLimit = true;
}

void TextBox::setLimit(bool Limit)
{
	hasLimit = Limit;
}

void TextBox::traceMouse(sf::Vector2f Coordinates)
{
	if (Label::getRect().contains(Coordinates))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			active = true;
		}
	}
	else
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			active = false;
		}
	}
}


void TextBox::inputText(sf::Event Event)
{
	if (active)
	{
		int unicode = Event.text.unicode;
		if (unicode < 128)
		{
			if (hasLimit && (intermediate.length() <= limit) || !hasLimit ||unicode  == DELETE_KEY || unicode == ENTER_KEY)
			{
				input_logic(unicode);
			}
		}
	}
}

//Toggle methods
//Private
/*
//Public
Toggle::Toggle(	std::string Name, 
				sf::Font* Font, 
				sf::Vector2f Size, 
				sf::Vector2f Pos, 
				int TextSize, 
				sf::Color BgColor, 
				sf::Color TextColor) :
				Button(Name, Font, Size, Pos, TextSize, BgColor, TextColor, BgColor, BgColor)
{
	activeColor = BgColor;
	passiveColor = BgColor;
	textMode = true;
	useColorIndication = true;
	state = false;
}

Toggle::Toggle(	sf::Texture* Texture, 
				sf::IntRect Rect, 
				sf::Vector2f Size, 
				sf::Vector2f Pos, 
				sf::Color BgColor, 
				sf::Color ActiveColor) : 
				Button(Texture, Rect, Size, Pos, BgColor, BgColor, BgColor)
{
	passiveSprite.setTexture(*Texture);
	activeSprite.setTexture(*Texture);
	passiveSprite.setTextureRect(Rect);
	activeSprite.setTextureRect(Rect);
	activeColor = ActiveColor;
	passiveColor = BgColor;
	useColorIndication = true;
	textMode = false;
	state = false;
}

Toggle::Toggle(	sf::Texture* Texture, 
				sf::IntRect ActiveRect, 
				sf::IntRect PassiveRect, 
				sf::Vector2f Size, 
				sf::Vector2f Pos) : 
				Button(Texture, PassiveRect, Size, Pos, sf::Color(0,0,0), sf::Color(0, 0, 0), sf::Color(0, 0, 0))
{
	passiveSprite.setTexture(*Texture);
	activeSprite.setTexture(*Texture);
	passiveSprite.setTextureRect(PassiveRect);
	activeSprite.setTextureRect(ActiveRect);
	activeColor = sf::Color(0, 0, 0);
	passiveColor = sf::Color(0, 0, 0);
	textMode = false;
	useColorIndication = false;
	state = false;
}

Toggle::Toggle(	sf::Texture* Texture, 
				sf::Texture* ActiveTexture, 
				sf::IntRect Rect, 
				sf::Vector2f Size, 
				sf::Vector2f Pos) : 
				Button(Texture, Rect, Size, Pos, sf::Color(0, 0, 0), sf::Color(0, 0, 0), sf::Color(0, 0, 0))
{
	passiveSprite.setTexture(*Texture);
	activeSprite.setTexture(*ActiveTexture);
	passiveSprite.setTextureRect(Rect);
	activeSprite.setTextureRect(Rect);
	textMode = false;
	useColorIndication = false;
	state = false;
}

void Toggle::setActiveState(void)
{
	state = true;
}

void Toggle::setPassiveState(void)
{
	state = false;
}

void Toggle::traceMouse(sf::Vector2f Coordinates)
{
	if ((textMode) ? label.getRect().contains(Coordinates) : icon.getRect().contains(Coordinates))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (hover && !hold)
			{
				hold = true;
				if (!state)
				{
					if (useColorIndication)		(textMode) ? label.setBgColor(activeColor) : icon.setBgColor(activeColor);
					else						icon.setSprite(activeSprite);

					state = true;
				}
				else
				{
					if (useColorIndication)		(textMode) ? label.setBgColor(passiveColor) : icon.setBgColor(passiveColor);
					else						icon.setSprite(passiveSprite);

					state = false;
				}
			}
		}
		else
		{
			if (!hover)
			{
				hover = true;
			}
			if (hover && hold)
			{
				hover = true;
				hold = false;
			}
		}
	}
	else
	{
		hover = false;
		hold = false;
	}
}

void Toggle::setActiveColor(sf::Color ActiveColor)
{
	activeColor = ActiveColor;
	if (state) 
	{
		if (useColorIndication)	(textMode) ? label.setBgColor(activeColor) : icon.setBgColor(activeColor);
		else					icon.setSprite(activeSprite);
	}
	else
	{
		if (useColorIndication)	(textMode) ? label.setBgColor(passiveColor) : icon.setBgColor(passiveColor);
		else					icon.setSprite(passiveSprite);
	}						
}

void Toggle::setPassiveColor(sf::Color PassiveColor)
{
	passiveColor = PassiveColor;

	if (state)
	{
		if (useColorIndication)	(textMode) ? label.setBgColor(activeColor) : icon.setBgColor(activeColor);
		else					icon.setSprite(activeSprite);
	}
	else
	{
		if (useColorIndication)	(textMode) ? label.setBgColor(passiveColor) : icon.setBgColor(passiveColor);
		else					icon.setSprite(passiveSprite);
	}
}

bool Toggle::getState(void)
{
	return state;
}

void Toggle::renewState(bool State)
{
	state = State;
	if (state)
	{
		if (useColorIndication)	(textMode) ? label.setBgColor(activeColor) : icon.setBgColor(activeColor);
		else					icon.setSprite(activeSprite);
	}
	else
	{
		if (useColorIndication)	(textMode) ? label.setBgColor(passiveColor) : icon.setBgColor(passiveColor);
		else					icon.setSprite(passiveSprite);
	}
}

//Tab methods
//Private

//Public

Tab::Tab(std::string Name,
	sf::Font* Font,
	sf::Vector2f Size,
	sf::Vector2f Pos,
	int TextSize,
	sf::Color BgColor,
	sf::Color TextColor) :
	Toggle(Name, Font, Size, Pos, TextSize, BgColor, TextColor)
{
	bool hover = false;
	bool hold = false;
}

bool Tab::traceMouse(sf::Vector2f Position)
{
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (label.getRect().contains(Position))
		{
			if (!Toggle::getState())
			{
				Toggle::renewState(true);
			}
			return true;
		}
	}
	return false;
}

//TabBar methods
//Private

//Public

TabBar::TabBar()
{

}

void TabBar::createTab(std::string Name,
	sf::Font* Font,
	sf::Vector2f Size,
	sf::Vector2f Pos,
	int TextSize,
	sf::Color Active,
	sf::Color Passive,
	sf::Color TextColor)
{
	Tab tab(Name, Font, Size, Pos, TextSize, Active, TextColor);
	tabArray.push_back(tab);
	tabArray.back().setPassiveColor(Passive);
	clearBar();
	tabArray[0].renewState(1);
}

void TabBar::addTab(Tab Tab)
{
	tabArray.push_back(Tab);
}

void TabBar::traceMouse(sf::Vector2f Position)
{
	if (!tabArray[lastActive].traceMouse(Position))
	{
		int c = 0;
		for (auto& it : tabArray)
		{
			if (it.traceMouse(Position))
			{
				lastActive = c;
				clearBar();
				it.renewState(true);
			}
			c++;
		}
	}
}

void TabBar::clearBar(void)
{
	for (auto& it : tabArray)
		it.renewState(false);
}

void TabBar::draw(sf::RenderWindow &Window)
{
	for (auto it = 0; it < tabArray.size(); it++)
		tabArray[it].draw(Window);
}

int TabBar::getCurrentTab(void)
{
	return lastActive;
}*/