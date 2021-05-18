#include "Game.h"

using namespace std;
using namespace sf;

#define TEXTURES first
#define SPRITES	 second
#define NAME	 first

#define BOUNCE			"Bounce"
#define EMERGENCE		"Emergence"
#define DISAPPEARANCE	"Disappearance"
#define HOVER			"Hover"
#define STAY			"Stay"

sf::Color colorTable[14] =
{
	sf::Color(0xFF, 0x00, 0x00),
	sf::Color(0xFF, 0xFF, 0x00),
	sf::Color(0xFF, 0x00, 0xFF),
	sf::Color(0xFF, 0xFF, 0xFF),
	sf::Color(0x00, 0xFF, 0x00),
	sf::Color(0x00, 0x00, 0xFF),
	sf::Color(0x00, 0xFF, 0xFF),

	sf::Color(0xFF, 0xB4, 0x00),
	sf::Color(0x1E, 0xFA, 0xA3),
	sf::Color(0x40, 0x41, 0xBF),
	sf::Color(0x29, 0xFF, 0x01),
	sf::Color(0xBF, 0xE4, 0xB1),
	sf::Color(0xFE, 0x00, 0x00),
	sf::Color(0x8D, 0x48, 0x0B)
};

typedef enum { EV_BUTTON_CLICK, EV_BUTTON_HOVER, EV_LINE, EV_INSERT, EV_MOVE, EV_GAME_OVER} EventType;
enum {NEW_GAME_BT, CONTINUE_BT, SETTINGS_BT, HIGHSCORE_BT, ABOUT_BT, EXIT_BT} buttons;
vector<pair<int, int>> EventList;

bool seekEvent(pair<int, int>);
bool seekEventType(int);
pair<int, int> ejectEvent(void);
void addEvent(pair<int, int> Gevent);
void clearEventList();
int getLenght(void);

int game(void)
{
	FSM_STATE fsm_st = MAIN_MENU_ST;

	setlocale(LC_ALL, "Ukr");
	float w = 960, h = 600;
	float nw = 960, nh = 600;
	RenderWindow window(VideoMode(w, h), "Color lines");
	//window.setFramerateLimit(60);
	sf::View view(sf::FloatRect(0, 0, w, h));
	view.setViewport(sf::FloatRect(0, 0, w /nw, h/nh));
	//view.setCenter(sf::Vector2f(480, 300 ));
	//view.setSize(sf::Vector2f(960, 600));
	window.setView(view);
	
	sf::Clock clock;
	sf::Font font;
	font.loadFromFile("consola.ttf");

	// Single structure for all graphicals resources
	UnityGraph GraphicsRes;
	UnitySound SoundRes;
	std::vector<std::pair<int, string>> scoresTable;
	std::string aboutstr = "\n             COLOR LINES \
							\n\
							\n\
							\n              Made with\
							\n              SFML-3.2.3\
							\n\
							\n\
							\n Programer - Vlad Lapshuda (VAL)\
							\n Graphics - VAL\
							\n Link: github.com/VALINT/ColorLines\
							\n\
							\n\
							\n\
							\n\
							\n             2017 - 2021";

	MainMenu	menu(&GraphicsRes, &window, nullptr);
	Highscore	hscore(&GraphicsRes, &window, &font);
	MyGame		game(&GraphicsRes, &window, nullptr);
	game.setCallback(&addEvent);

	//Parse file with graphical sources and create textures array.
	ifstream TexturesList("Graphics_list.dat", std::ifstream::binary);
	if (!TexturesList.is_open())
	{
		cout << "#ERROR: Textures_list.dat not founded!" << endl;
	}
	else
	{
		string str;
		while (getline(TexturesList, str))
		{
			Texture* temp = new Texture;
			temp->loadFromFile(str);
			string name = str.substr(str.find_last_of('/') + 1, (str.find_last_of('.') - str.find('/') - 1));
			GraphicsRes.TEXTURES[name] = *temp;
			delete(temp);
		}
	}
	TexturesList.close();

	//Parse file with sound resources and create sounds array.
	ifstream SoundList("Sound_list.dat", std::ifstream::binary);
	if (!SoundList.is_open())
	{
		cout << "#ERROR: Sound_list.dat not founded!" << endl;
	}
	else
	{
		string str;
		while (getline(SoundList, str))
		{
			sf::SoundBuffer* tempsb = new sf::SoundBuffer;
			sf::Sound*		 temps  = new sf::Sound;
			tempsb->loadFromFile(str);
			temps->setBuffer(*tempsb);
			string name = str.substr(str.find_last_of('/') + 1, (str.find_last_of('.') - str.find('/') - 1));
			SoundRes[name] = { *tempsb, *temps };
			SoundRes[name].second.setBuffer(SoundRes[name].first);
			delete(tempsb);
			delete(temps);
		}
	}
	SoundList.close();


	//Parse file with sprites data and create sprites array.
	ifstream SpriteList("Sprites.dat", std::ifstream::binary);
	if (!SpriteList.is_open())
	{
		cout << "#ERROR: Sprites.dat not founded!" << endl;
	}
	else
	{
		string str;
		string blockName;
		while (getline(SpriteList, str))
		{
			if (str[0] == '<')
			{
				blockName = str.substr(str.find('<') + 1, (str.find('>') - 1));
				GraphicsRes.SPRITES[blockName];
				continue;
			}
			Sprite* temp = new Sprite;
			temp->setTexture(GraphicsRes.TEXTURES[blockName]);
			string name = str.substr(0, str.find(' '));
			string rect, x, y, w, h;
			rect = str.substr(str.find(' ') + 1, (str.length() - str.find(' ') - 1));
			x = rect.substr(0, str.find(','));
			rect = rect.substr(rect.find(',') + 1, (rect.length() - rect.find(' ') - 1));
			y = rect.substr(0, str.find(','));
			rect = rect.substr(rect.find(',') + 1, (rect.length() - rect.find(' ') - 1));
			w = rect.substr(0, str.find(','));
			rect = rect.substr(rect.find(',') + 1, (rect.length() - rect.find(' ') - 1));
			h = rect.substr(0, str.find(','));

			temp->setTextureRect(IntRect(atoi(x.c_str()), atoi(y.c_str()), atoi(w.c_str()), atoi(h.c_str())));

			map<string, Sprite> a{ { name, *temp } };
			GraphicsRes.SPRITES[blockName].insert(pair<string, Sprite>(name, *temp));
			delete(temp);
		}
	}
	SpriteList.close();

	ifstream inscoretable("ScoreTable.dat");
	if (!inscoretable.is_open())
	{
		ofstream scoretable("ScoreTable.dat");
		cout << "ScoreTable.dat was made!" << endl;
		scoretable.close();
	}
	else
	{
		std::string str;
		while (getline(inscoretable, str))
		{
			int scoreT = atoi( str.substr( 0, str.find(',') ).c_str() );
			game.setHighscore(scoreT);
			string pname = str.substr(str.find(',') + 1, str.size() - 1);
			hscore.setScoreLine({ scoreT , pname });
			scoresTable.push_back({ scoreT , pname });
		}
		inscoretable.close();
	}
	hscore.biuldTable();
	ofstream scoretable("ScoreTable.dat");
	for (auto i : scoresTable)
	{
		scoretable << i.first << "," << i.second << "\n";
	}

	GraphicsRes.SPRITES["Menu_frame"]["Menu_frame"].setPosition(298, 44);

	menu.addButton(Button ("New Game", &font, sf::Vector2f(300, 50), sf::Vector2f(330, 100), 32, BG_COLOR, TEXT_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 0));
	menu.addButton(Button ("Continue", &font, sf::Vector2f(300, 50), sf::Vector2f(330, 170), 32, BG_COLOR, TEXT_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 1));
	menu.addButton(Button ("Settings", &font, sf::Vector2f(300, 50), sf::Vector2f(330, 310), 32, BG_COLOR, TEXT_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 2));
	menu.addButton(Button ("Highscore", &font, sf::Vector2f(300, 50), sf::Vector2f(330, 240), 32, BG_COLOR, TEXT_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 3));
	menu.addButton(Button ("About", &font, sf::Vector2f(300, 50), sf::Vector2f(330, 380), 32, BG_COLOR, TEXT_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 4));
	menu.addButton(Button ("Exit", &font, sf::Vector2f(300, 50), sf::Vector2f(330, 450), 32, BG_COLOR, TEXT_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 5));

	game.addButton(Button (&GraphicsRes.TEXTURES["Interface_Tr"], sf::IntRect(0,0,51,51),sf::Vector2f(51, 51), sf::Vector2f(25, 400), BG_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 100));
	game.addButton(Button (&GraphicsRes.TEXTURES["Interface_Tr"], sf::IntRect(50, 0, 51, 51), sf::Vector2f(51, 51), sf::Vector2f(25, 460), BG_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 200));
	game.addButton(Button (&GraphicsRes.TEXTURES["Interface_Tr"], sf::IntRect(100,0, 51, 51), sf::Vector2f(51, 51), sf::Vector2f(25, 520), BG_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 300));
	
	game.addLabel(Label ("Highscore", &font, sf::Vector2f(150, 35), sf::Vector2f(30, 10), 25, BG_COLOR, TEXT_COLOR, false, true));
	game.addLabel(Label (" ", &font, sf::Vector2f(150, 35), sf::Vector2f(30, 45), 25, BG_COLOR, TEXT_COLOR, false, true));
	game.addLabel(Label ("Score", &font, sf::Vector2f(100, 35), sf::Vector2f(820, 300), 25, BG_COLOR, TEXT_COLOR, false, true));
	game.addLabel(Label (" ", &font, sf::Vector2f(100, 35), sf::Vector2f(820, 335), 25, BG_COLOR, TEXT_COLOR, false, true));

	Background gmovr_shading (sf::Vector2f(960, 600), sf::Vector2f(0, 0),sf::Color(0x25, 0x25, 0x25, 0x80), false);
	Background settings_bg   (sf::Vector2f(500, 500), sf::Vector2f(230, 50), BG_SEMI_COLOR, false);
	Label  gmovrlable		("GAME OVER",		&font, sf::Vector2f(600, 300), sf::Vector2f(180, 100), 72, BG_COLOR, TEXT_COLOR, false, true);
	Label  gmovrscorelb		("Your result: ",	&font, sf::Vector2f(250, 50), sf::Vector2f(200, 220), 32, BG_COLOR, TEXT_COLOR, false, false);
	Label  gmovrplacelb		("Your place:  ",	&font, sf::Vector2f(250, 50), sf::Vector2f(200, 280), 32, BG_COLOR, TEXT_COLOR, false, false);
	Label  gmovrnamelb		("Your name:  ",	&font, sf::Vector2f(250, 50), sf::Vector2f(200, 340), 32, BG_COLOR, TEXT_COLOR, false, false);
	Label  gmovrscore		("",				&font, sf::Vector2f(250, 50), sf::Vector2f(480, 220), 32, BG_COLOR, TEXT_COLOR, false, false);
	Label  gmovrplace		("",				&font, sf::Vector2f(250, 50), sf::Vector2f(480, 280), 32, BG_COLOR, TEXT_COLOR, false, false);
	Label  aboutgame		(aboutstr,			&font, sf::Vector2f(500, 500),sf::Vector2f(230, 50),  24, BG_SEMI_COLOR, TEXT_COLOR, false, false);
	Label  aboutlbl			("ABOUT",			&font, sf::Vector2f(500, 50), sf::Vector2f(230, 5),   40, BG_COLOR, TEXT_COLOR, false, true);
	Label  settingslbl		("SETTINGS",		&font, sf::Vector2f(500, 50), sf::Vector2f(230, 5),   40, BG_COLOR, TEXT_COLOR, false, true);
	TextBox gmovrname		(					&font, sf::Vector2f(275, 50), sf::Vector2f(480, 340), 32, BG_COLOR, TEXT_COLOR);
	Button gmovr_newgame	("New Game",		&font, sf::Vector2f(275, 50), sf::Vector2f(180, 450), 32, BG_COLOR, TEXT_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 400);
	Button gmovr_back		("Exit",			&font, sf::Vector2f(275, 50), sf::Vector2f(505, 450), 32, BG_COLOR, TEXT_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 500);
	Button back_btn			("Back",			&font, sf::Vector2f(500, 50), sf::Vector2f(230, 500), 40, BG_COLOR, TEXT_COLOR, HOVER_COLOR, HOLD_COLOR, &addEvent, 600);

	gmovrname.setLimit(15);
	game.initGraphics();
	game.setSound(&SoundRes);

	srand(time(NULL));

	Event event;
	while (window.isOpen())
	{

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				scoretable.close();
				window.close();
			}
				
			if (event.type == sf::Event::TextEntered)
			{
				gmovrname.inputText(event);
			}
		}
		
		window.clear(Color::Black);
		sf::Vector2u screenSize = window.getSize();
		if (screenSize.x != w || screenSize.y != h)
		{
			window.setSize(sf::Vector2u(w, h));
		}
		sf::Vector2f mouseCoord = sf::Vector2f(sf::Mouse::getPosition(window));

		mouseCoord.x = mouseCoord.x / (screenSize.x / w);
		mouseCoord.y = mouseCoord.y / (screenSize.y / h);


		switch (fsm_st)
		{
		case (IDLE_ST):
				fsm_st = MAIN_MENU_ST;
			break;
		case(MAIN_MENU_ST):
			
			menu.traceMouse(mouseCoord);
			menu.draw();
			//pair<int, int> ev = ejectEvent();
			if (seekEvent({ NEW_GAME_BT, EV_BUTTON_CLICK }))
			{
				game.InitGame();
				fsm_st = GAME_ST;
			}
			else if (seekEvent({ CONTINUE_BT, EV_BUTTON_CLICK }))
				fsm_st = CONTINUE;
			else if (seekEvent({ SETTINGS_BT, EV_BUTTON_CLICK }))
				fsm_st = SETTINGS_ST;
			else if (seekEvent({ ABOUT_BT, EV_BUTTON_CLICK }))
				fsm_st = ABOUT_ST;
			else if (seekEvent({ HIGHSCORE_BT, EV_BUTTON_CLICK }))
				fsm_st = HIGHSCORES_ST;
			else if (seekEvent({ EXIT_BT, EV_BUTTON_CLICK }))
			{
				scoretable.close();
				window.close();
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
				fsm_st = MAIN_MENU_ST;
			break;
		case(GAME_ST):
			game.CheckMouse(mouseCoord);
			game.setTime(clock.getElapsedTime().asMicroseconds()/1000);
			clock.restart();

			if (seekEventType(EV_BUTTON_HOVER))
			{
				if (seekEvent({ 100, 1 }) || seekEvent({ 200, 1 }) || seekEvent({ 300, 1 }))
					SoundRes["button_hover"].second.play();
			}
			if (seekEvent({ 100, 0 }))
			{
				game.InitGame();
				SoundRes["button_click"].second.play();
			}
			else if (seekEvent({ 200,0 }))
			{
				game.savegame("save.dat");
				SoundRes["button_click"].second.play();
			}
			else if (seekEvent({ 300,0 }))
			{
				fsm_st = MAIN_MENU_ST;
				SoundRes["button_click"].second.play();
			}

			game.processField();
			
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
				fsm_st = GAME_OVER_ST;
			if (seekEvent({ 1000,EV_LINE }))
			{
				game.setDisappearance();
				game.setTransition();
				fsm_st = BALL_DISAPPEARANCE_ST;
			}
			else if (seekEvent({ 1000,EV_MOVE }))
			{
				game.clearBall();
				game.setTransition();
				fsm_st = BALL_MOVE_ST;
			}
			else if (seekEvent({ 1000,EV_INSERT }))
			{
				game.setEmergence();
				game.setTransition();
				fsm_st = BALL_EMERGENCE_ST;
			}
			if (seekEvent({ 1000,EV_GAME_OVER }))
			{
				fsm_st = GAME_OVER_ST;
			}

			game.draw();
			break;
		case(CONTINUE):
			game.loadgame("save.dat");
			fsm_st = GAME_ST;
			break;
		case(BALL_EMERGENCE_ST):
			game.setTime(clock.getElapsedTime().asMicroseconds() / 1000);
			clock.restart();
			game.CheckMouse(mouseCoord);
			game.processField();
			game.draw();
			if (game.checkEmergence())
			{
				game.setStable();
				fsm_st = GAME_ST;
			}
			break;

		case(BALL_MOVE_ST):
			game.setTime(clock.getElapsedTime().asMicroseconds() / 1000);
			clock.restart();
			game.CheckMouse(mouseCoord);
			game.processField();
			game.draw();
			//game.moveball();
			if (game.moveball() || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				game.setBallAfteMove();
				game.afterMove();
				if (seekEvent({ 1000,EV_LINE }))
				{
					game.setDisappearance();
					fsm_st = BALL_DISAPPEARANCE_ST;
				}
				else if (seekEvent({ 1000,EV_INSERT }))
				{
					game.setEmergence();
					fsm_st = BALL_EMERGENCE_ST;
				}
				else
				{
					game.setStable();
					fsm_st = GAME_ST;
				}
				if (seekEvent({ 1000,EV_GAME_OVER }))
				{
					SoundRes["game_over"].second.play();
					fsm_st = GAME_OVER_ST;
				}
					
			}
			break;

		case(BALL_DISAPPEARANCE_ST):
			game.setTime(clock.getElapsedTime().asMicroseconds() / 1000);
			clock.restart();
			game.CheckMouse(mouseCoord);
			game.processField();
			game.draw();
			if (game.checkDisappearance())
			{
				game.deleteLine();
				game.setStable();
				fsm_st = GAME_ST;
			}

			break;
		case(GAME_OVER_ST):
			gmovr_newgame.traceMouse(mouseCoord);
			gmovr_back.traceMouse(mouseCoord);
			game.draw();
			gmovr_shading.draw(window);
			gmovrlable.draw(window);
			gmovrscorelb.draw(window);
			gmovrplacelb.draw(window);
			gmovrnamelb.draw(window);
			gmovrscore.setText(to_string(game.getScore()));
			gmovrplace.setText(to_string(hscore.getPlace(game.getScore())));
			gmovrname.traceMouse(mouseCoord);
			gmovrscore.draw(window);
			gmovrplace.draw(window);
			gmovrname.draw(window);
			gmovr_newgame.draw(window);
			gmovr_back.draw(window);

			if (seekEvent({ 400,EV_BUTTON_CLICK }))
			{
				string pname = gmovrname.getString();
				if (pname == "") pname = "Player Anonimus";
				hscore.setScoreLine({ game.getScore(), pname });
				hscore.biuldTable();
				scoresTable.push_back({ game.getScore(), pname });
				scoretable << game.getScore() << "," << pname << "\n";
				game.InitGame();
				fsm_st = GAME_ST;
			}
			else if (seekEvent({ 500,EV_BUTTON_CLICK }))
			{
				string pname = gmovrname.getString();
				if (pname == "") pname = "Player Anonimus";
				hscore.setScoreLine({ game.getScore(), pname });
				hscore.biuldTable();
				scoresTable.push_back({ game.getScore(), pname });
				scoretable << game.getScore() << "," << pname << "\n";
				game.InitGame();
				fsm_st = MAIN_MENU_ST;
			}

			break;
		case(SETTINGS_ST):
			window.draw(GraphicsRes.second["Menu_Background"]["Menu_Background"]);
			gmovr_shading.draw(window);
			settings_bg.draw(window);
			settingslbl.draw(window);
			back_btn.traceMouse(mouseCoord);
			back_btn.draw(window);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
				fsm_st = MAIN_MENU_ST;
			if (seekEvent({ 600,EV_BUTTON_CLICK }))
				fsm_st = MAIN_MENU_ST;
			break;
		case(ABOUT_ST):
			window.draw(GraphicsRes.second["Menu_Background"]["Menu_Background"]);
			gmovr_shading.draw(window);
			aboutgame.draw(window);
			aboutlbl.draw(window);
			back_btn.traceMouse(mouseCoord);
			back_btn.draw(window);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
				fsm_st = MAIN_MENU_ST;

			if (seekEvent({ 600,EV_BUTTON_CLICK }))
				fsm_st = MAIN_MENU_ST;

			break;

		case(HIGHSCORES_ST):
			window.draw(GraphicsRes.second["Menu_Background"]["Menu_Background"]);
			gmovr_shading.draw(window);
			hscore.draw();
			back_btn.traceMouse(mouseCoord);
			back_btn.draw(window);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
				fsm_st = MAIN_MENU_ST;
			

			if (seekEvent({ 600,EV_BUTTON_CLICK }))
			{
				fsm_st = MAIN_MENU_ST;
			}
		break;
		}

		if (seekEventType(EV_BUTTON_CLICK))
			SoundRes["button_click"].second.play();
		if (seekEventType(EV_BUTTON_HOVER))
			SoundRes["button_hover"].second.play();
		
		clearEventList();
		window.display();
	}
	return 0;
}

bool seekEvent(pair<int, int> ev)
{
	for (auto i : EventList)
	{
		if (i.first == ev.first && i.second == ev.second)
			return true;
	}
	return false;
}

bool seekEventType(int ev)
{
	for (auto i : EventList)
	{
		if (i.second == ev)
			return true;
	}
	return false;
}
	
void addEvent(pair<int, int> Gevent)
{
	EventList.push_back(Gevent);
}
	
void clearEventList()
{
	EventList.clear();
}

pair<int, int> ejectEvent(void)
{
	if (EventList.size())
	{
		pair<int, int> Gevent = EventList[EventList.size()-1];
		EventList.pop_back();
		return Gevent;
	}
	return pair<int, int>{-1, 0};
}

int getLenght(void)
{
	return EventList.size();
}

BallPlace::BallPlace(sf::Vector2f Size,
	sf::Vector2f Pos) : Button(nullptr, sf::IntRect(0, 0, 0, 0), Size, Pos, TRANSPARENT, TRANSPARENT, TRANSPARENT, nullptr)
{

}

void BallPlace::SetPosition(sf::Vector2i Pos)
{
	this->ballPosition = Pos;
}

void BallPlace::insertBall(Ball ball)
{
	this->ball = ball;
}

Ball BallPlace::getBall(void)
{
	return ball;
}

void BallPlace::setBall(void)
{
	
	this->ball.set();
}

void BallPlace::removeBall(void)
{
	ball.clear();
}

void BallPlace::setAnimation(std::string name)
{
	ball.setAnimation(name);
}

std::string BallPlace::getAnimation(void)
{
	return ball.getAnimation();
}

bool BallPlace::isBall(void)
{
	return ball.get();
}

int BallPlace::getFrame(void)
{
	return ball.getFrame();
}

bool BallPlace::isPlaing(void)
{
	return ball.isPlaing();
}

int BallPlace::getColor(void)
{
	return ball.getColor();
}

void BallPlace::setColor(int color)
{
	this->ball.setColor(color);
}


void BallPlace::draw(sf::RenderWindow & Window, float time)
{
	ball.draw(Window, icon.getPosition(),sf::Color(0,0,0,0) , time);
}


MyGame::MyGame(UnityGraph* Graphics,
	sf::RenderWindow* Window,
	sf::View* View)
{
	this->graphics = Graphics;
	this->outWindow = Window;
	this->View = View;
}

void MyGame::InitGame(void)
{
	GameField.clear();
	nextBalls.clear();
	score = 0;
	transition = false;
	startJump = false;
	endJump = false;
	ballsCounter = 0;
	choosenBall = { 0,0 };
	ballPath.clear();
	emBalls.clear();
	deLine.clear();
	
	labels[1].setText(to_string(highscore));
	labels[3].setText(to_string(score));
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			BallPlace bp(sf::Vector2f(50, 50), sf::Vector2f(252 + 50 * (i - 1), 95 + 50 * (j - 1)));
			bp.SetPosition(sf::Vector2i(i, j));
			bp.insertBall(standartBall);
			bp.setIndex(j + (i - 1) * 9);
			bp.setCallback(&addEvent);
			bp.removeBall();
			GameField.push_back(bp);
		}
	}
	createNew();
	insertBalls();
	createNew();
}

void MyGame::setTime(float time)
{
	this->time = time;
}

void MyGame::setCallback(void(*callback)(std::pair<int, int>))
{
	this->callback = callback;
}

void MyGame::initGraphics()
{
	standartBall.createAnimation("Bounce",			graphics->TEXTURES["Balls_Bounce_2"], 0, 0, 50, 50, 6, 0.015, 50, true);
	standartBall.createAnimation("Move",			graphics->TEXTURES["Balls_Bounce_2"], 0, 0, 50, 50, 6, 0.065, 50, true);
	standartBall.createAnimation("Emergence",		graphics->TEXTURES["Balls_Bounce_2"], 0, 50, 50, 50, 5, 0.015, 50, false);
	standartBall.createAnimation("Disappearance",	graphics->TEXTURES["Balls_Bounce_2"], 250, 50, 50, 50, 5, 0.015, 50, false);
	standartBall.createAnimation("Hover",			graphics->TEXTURES["Balls_Bounce_2"], 300, 0, 50, 50, 3, 0.008, 50, false);
	standartBall.createAnimation("Stay",			graphics->TEXTURES["Balls_Bounce_2"], 0, 0, 50, 50, 1, 0.0025, 50, false);

	//soundbuff.loadFromFile("Sounds/ball_bounce_2.wav");
	//sound.setBuffer(soundbuff);
	//sound.setVolume(50.f);
}

void MyGame::CheckMouse(sf::Vector2f Coordinates)
{
	for (auto &i : GameField)
	{
		i.traceMouse(Coordinates);
	}

	for (auto &i : buttons)
	{
		i.traceMouse(Coordinates);
	}
}

void MyGame::insertBalls(void)
{
	emBalls.clear();
	for (int i = 0; i < 3; i++)
	{
		int x;
		int y;
		do {
			x = 1 + rand() % 9;
			y = 1 + rand() % 9;
			
		} while (GameField[((x - 1) + (y - 1) * 9)].isBall() && !checkGameOver());
		ballsCounter++;
		checkGameOver();
		standartBall= nextBalls[nextBalls.size()-1];
		nextBalls.pop_back();
		GameField[((x - 1) + (y - 1) * 9)].insertBall(standartBall);
		GameField[((x - 1) + (y - 1) * 9)].setBall();
		emBalls.push_back(((x - 1) + (y - 1) * 9));
		callback({1000, EV_INSERT});
	}
}

void MyGame::processField(void)
{
	if (getLenght())
	{
		pair<int, int> tmp = ejectEvent();
		if (tmp.first <= 81)
		{
			if (tmp.second == 1)	// If HOVER event
			{
				if (GameField[tmp.first - 1].getAnimation() == "Stay")
					GameField[tmp.first - 1].setAnimation("Hover");
			}
			else	//If CLICK event
			{
				//std::cout << "Click: x - "<<(tmp.first - 1) / 9 + 1<< " y - "<< (tmp.first - 1) % 9 + 1 << " tmp - "<< tmp.first <<std::endl;
				if (GameField[tmp.first - 1].getAnimation() == "Bounce")
				{
					GameField[tmp.first - 1].setAnimation("Stay");
					choosenBall = { 0,0 };
				}
				else
				{
					//Move ball
					if (choosenBall.x && choosenBall.y && !GameField[tmp.first - 1].isBall())
					{
						if (checkWay({ (tmp.first - 1) / 9 + 1, (tmp.first - 1) % 9 + 1 }) && !transition)
						{
						Ball tmpball;
						tmpball = GameField[(choosenBall.x + 9 * (choosenBall.y - 1)) - 1].getBall();
						GameField[(choosenBall.x + 9 * (choosenBall.y - 1)) - 1].removeBall();
						GameField[tmp.first - 1].insertBall(tmpball);
						GameField[tmp.first - 1].setAnimation("Stay");
						callback({ 1000, EV_MOVE });
						choosenBall = { 0,0 };
						}
					}
					else
					{
						for (auto &i : GameField)
						{
							if (i.getAnimation() != DISAPPEARANCE)
								i.setAnimation("Stay");
						}
						if (GameField[tmp.first - 1].getAnimation() != DISAPPEARANCE)
						{
							GameField[tmp.first - 1].setAnimation("Bounce");
							choosenBall = (GameField[tmp.first - 1].isBall()) ? sf::Vector2i((tmp.first - 1) % 9 + 1, (tmp.first - 1) / 9 + 1) : sf::Vector2i(0, 0);
							soundRes->at("ball_chosen").second.play();
						}
					}
				}
			}
		}
	}
	//clearEventList();
}

bool MyGame::moveball()
{
	moveBall.tick(time);
	if (ballPath.size() < 2) return true;
	int currentPos  = ballPath[ballPath.size() - 1] + 1;
	int nextPos		= ballPath[ballPath.size() - 2] + 1;
	int frame		= (moveBall.getFrame());
	if (frame == 0) 
	{ 
		if (endJump)
		{
			if (ballPath.size() > 2) 
			{ 
				ballPath.pop_back();
				currentPos = ballPath[ballPath.size() - 1] + 1;
				nextPos = ballPath[ballPath.size() - 2] + 1;
			}
			else { return true; }
			endJump = false; 
		}
		startJump = true;  
	}
	else if (startJump && frame == 5) 
	{ 
		endJump = true;  
		startJump = false;
	}
	int xc  = (currentPos - 1) / 9 + 1;
	int yc  = (currentPos - 1) % 9 + 1;
	int xn  = (nextPos - 1) / 9 + 1;
	int yn  = (nextPos - 1) % 9 + 1;
	int x = (252 + 50 * (xc-1)) + (frame * (50*(xn - xc) / 5));
	int y = ( 95 + 50 * (yc-1)) + (frame * (50*(yn - yc) / 5));
	moveBall.draw(*outWindow, sf::Vector2f(x, y), colorTable[GameField[ballPath[0]].getColor()]);

	if (moveBall.getFrame() == 5 && !soundTrigger)
	{
		soundRes->at("ball_bounce_2").second.stop();
		soundRes->at("ball_bounce_2").second.play();
		//sound.stop();
		//sound.play();
		soundTrigger = true;
	}
	else if (moveBall.getFrame() == 0)
	{
		soundTrigger = false;
	}


	return false;
}

void MyGame::loadgame(std::string name)
{
	if (GameField.size() > 0) return;

	InitGame();
	std::string str;

	labels[1].setText(to_string(highscore));
	labels[3].setText(to_string(score));

	ifstream save(name);
	if (!save.is_open())
	{
		ofstream save(name);
		cout << name<<" was made" << endl;
	}
	else
	{
		ballsCounter = 0;
		for (auto &i : GameField)
		{
			getline(save, str);
			if (str != "")
			{
				if (atoi(str.c_str()) != 0)
				{
					i.setColor(atoi(str.c_str()));
					i.setBall();
					ballsCounter++;
				}
				else
					i.removeBall();
			}
		}
		nextBalls.clear();
		for (int i = 0; i < 3; i++)
		{
			getline(save, str);
			standartBall.setColor(atoi(str.c_str()));
			nextBalls.push_back(standartBall);
			nextBalls[i].set();
		}
	}
	getline(save, str);
	score = atoi(str.c_str());
	save.close();
	labels[3].setText(str);
}

void MyGame::savegame(std::string name)
{
	ofstream save(name);

	for (auto i : GameField)
	{
		if (i.isBall())
			save << i.getColor() << "\n";
		else
			save << "0" << "\n";
	}
	for (auto i : nextBalls)
	{
			save << i.getColor() << "\n";
	}
	save << score << "\n";
	save.close();
}

bool MyGame::checkWay(sf::Vector2i position)
{
	const int H = 11;
	const int W = 11;
	unsigned int x;
	unsigned int y;
	int step = 0;
	ballPath.clear();
	bool WaySearched = false;

	int Grid[H][W] = {
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
		{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1 },
		{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1 },
		{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1 },
		{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1 },
		{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1 },
		{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1 },
		{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1 },
		{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1 },
		{ -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1 },
		{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 }
	};

	for (y = 1; y <= 9; y++)
		for (x = 1; x <= 9; x++)
		{
			Grid[y][x] = -1 * GameField[(y + 9 * (x - 1))-1].isBall();
		}

	Grid[choosenBall.x][choosenBall.y] = 1;

	step = 1;
	

	do
	{
		for (x = 1; x < 10; x++)
			for (y = 1; y < 10; y++)
			{
				if (Grid[x][y] == step)
				{
					if (Grid[x][y + 1] == 0)Grid[x][y + 1] = step + 1;
					if (Grid[x + 1][y] == 0)Grid[x + 1][y] = step + 1;
					if (Grid[x - 1][y] == 0)Grid[x - 1][y] = step + 1;
					if (Grid[x][y - 1] == 0)Grid[x][y - 1] = step + 1;
				}
			}
		step++;
		if (Grid[position.y][position.x] != 0) break;

	} while (step < 81);

	x = position.x;
	y = position.y;
	if (Grid[position.y][position.x] != 0)
	{
		ballPath.push_back(position.y - 1 + (position.x - 1) * 9);
		for (int i = step; i > 0; i--)
		{
			if		(Grid[y][x + 1] == i - 1) { x += 1; if (x == 10)  x = 9; ballPath.push_back(y - 1 + (x - 1) * 9);}
			else if (Grid[y + 1][x] == i - 1) { y += 1; if (y == 10)  y = 9; ballPath.push_back(y - 1 + (x - 1) * 9);}
			else if (Grid[y - 1][x] == i - 1) { y -= 1; if (y == 0)   y = 1; ballPath.push_back(y - 1 + (x - 1) * 9);}
			else if (Grid[y][x - 1] == i - 1) { x -= 1; if (x == 0)   x = 1; ballPath.push_back(y - 1 + (x - 1) * 9);}
		}
		moveBall = GameField[(choosenBall.x - 1 + (choosenBall.y - 1) * 9)].getBall();
		moveBall.setAnimation("Move");
		callback({ 1000,EV_MOVE });
		return true;
	}

	return false;
}

bool MyGame::checkLine()
{
	std::vector<int> vline, hline, dlline, drline;
	int bx, by, color;
	for (int i = 1; i <= 81; i++)
	{
		bx = (i - 1) % 9 + 1; 
		by = (i - 1) / 9 + 1;

		deLine.clear();
		vline.clear();
		hline.clear();
		dlline.clear();
		drline.clear();
		if (GameField[i-1].isBall())
		{
			color = GameField[i-1].getColor();
			vline.push_back(i-1);
			hline.push_back(i-1);
			dlline.push_back(i-1);
			drline.push_back(i-1);
			
			for (int j = 1; j < 9; j++)
			{
				if ((bx + j < 10))
				{
					if(GameField[bx - 1 + j + (by - 1) * 9].isBall())
						if(GameField[bx - 1 + j + (by - 1) * 9].getColor() == color)
							vline.push_back(bx - 1 + j + (by - 1) * 9);
						else break;
					else break;
				}
				else break;
			}

			for (int j = 1; j < 9; j++)
			{
				if ((by + j < 10))
				{
					if(GameField[bx - 1 + (by - 1 + j) * 9].isBall())
						if(GameField[bx - 1 + (by - 1 + j) * 9].getColor() == color)
							hline.push_back(bx - 1 + (by - 1 + j) * 9);
						else break;
					else break;
				}
				else break;
			}

			for (int j = 1; j < 9; j++)
			{
				if ((bx - j > 0) && (by + j < 10))
				{
					if(GameField[bx - 1 - j + (by - 1 + j ) * 9].isBall())
						if(GameField[bx - 1 - j + (by - 1 + j) * 9].getColor() == color)
							drline.push_back(bx - 1 - j + (by - 1 + j) * 9);
						else break;
					else break;
				}
				else break;
			}

			for (int j = 1; j < 9; j++)
			{
				if ((bx + j < 10) && (by + j < 10))
				{
					if(GameField[bx - 1 + j + (by - 1 + j) * 9].isBall())
						if(GameField[bx - 1 + j + (by - 1 + j) * 9].getColor() == color)
							dlline.push_back(bx - 1 + j + (by - 1 + j) * 9);
						else break;
					else break;
				}
				else break;
			}

			if (vline.size() >= 5)
				std::copy(vline.begin(), vline.end(), std::back_inserter(deLine));
			if (hline.size() >= 5)
				std::copy(hline.begin(), hline.end(), std::back_inserter(deLine));
			if (drline.size() >= 5)
				std::copy(drline.begin(), drline.end(), std::back_inserter(deLine));
			if (dlline.size() >= 5)
				std::copy(dlline.begin(), dlline.end(), std::back_inserter(deLine));
		}
		if (deLine.size())
		{
			callback({1000,EV_LINE});
			return true;
		}

	}
	return false;
}

void MyGame::clearBall()
{
	if(ballPath.size())
		GameField[ballPath[0]].removeBall();
}

void MyGame::setBall()
{
	GameField[ballPath[0]].setBall();
}

void MyGame::setBallAfteMove()
{
	GameField[ballPath[0]].setBall();
}

void MyGame::afterMove()
{
	if (!checkLine())
	{
		insertBalls();
		checkLine();
		createNew();
	}
}

void MyGame::setTransition()
{
	transition = true;
}

void MyGame::setStable()
{
	transition = false;
}

bool MyGame::checkGameOver()
{
	//for (auto i : GameField)
	//	if (!i.isBall()) return false;
	if (ballsCounter < 81) return false;
	std::cout << "You looser" << std::endl;
	callback({ 1000, EV_GAME_OVER });
	return true;
}

void MyGame::addButton(Button bt)
{
	this->buttons.push_back(bt);
}

void MyGame::addLabel(Label lb)
{
	this->labels.push_back(lb);
}

int MyGame::getScore(void)
{
	return score;
}

void MyGame::setHighscore(int in)
{
	if (in > highscore) highscore = in;
}

void MyGame::setEmergence()
{
	for (auto i : emBalls)
	{
		GameField[i].setAnimation(EMERGENCE);
	}
}

bool MyGame::checkEmergence()
{
	for (auto i : emBalls)
	{
		return(!GameField[i].isPlaing());
	}
	return false;
}

void MyGame::setDisappearance()
{
	for (auto i : deLine)
	{
		GameField[i].setAnimation(DISAPPEARANCE);
	}
}

bool MyGame::checkDisappearance()
{
	for (auto i : deLine)
	{
		return(!GameField[i].isPlaing());
	}
	return false;
}

void MyGame::deleteLine()
{
	score += deLine.size();
	ballsCounter -= deLine.size();
	labels[3].setText(to_string(score));
	if (score > highscore)
	{
		highscore = score;
		labels[1].setText(to_string(highscore));
	}
	for (auto i : deLine)
	{
		GameField[i].removeBall();
	}
	//std::cout << "score - " << score << std::endl;
	
	soundRes->at("ball_disappearing").second.play();
}

void MyGame::createNew(void)
{
	nextBalls.clear();
	for (int i = 0; i < 3; i++)
	{
		standartBall.setColor(1 + rand() % 7);
		nextBalls.push_back(standartBall);
		nextBalls[i].set();
	}
}

void MyGame::setSound(UnitySound *sound)
{
	this->soundRes = sound;
}

void MyGame::draw()
{
	outWindow->draw(graphics->second["Background"]["BackgroundGame"]);
	for (auto &i : GameField)
		if (i.getAnimation() == "Hover" && !i.isPlaing() && !i.isHover())
			i.setAnimation("Stay");

	for (auto &i : GameField)
	{
		i.draw(*outWindow, time);
		if (i.getAnimation() == "Bounce" && i.getFrame() == 5 && !soundTrigger)
		{
			soundRes->at("ball_bounce_2").second.play();
			soundTrigger = true;
		}
		else if(i.getAnimation() == "Bounce" && i.getFrame() == 0)
			soundTrigger = false;
	}
		

	for (auto &i : buttons)
		i.draw(*outWindow);

	for (auto i : labels)
		i.draw(*outWindow);

	for (int i = 0; i < 3; i++)
	{
		nextBalls[i].draw(*outWindow, sf::Vector2f(400+(50*i), 22));
	}
}

Ball::Ball()
{
	isExist = false;
}

void Ball::createAnimation(std::string name, sf::Texture & texture, int x, int y, int w, int h, int count, float speed, int step, bool Loop)
{
	moves.create(name, texture, x, y, w, h, count, speed, step, Loop);
}

void Ball::setColor(int Color)
{
	this->ballColor = Color;
}

int  Ball::getColor(void)
{
	return ballColor;
}

void Ball::draw(sf::RenderWindow & Window, sf::Vector2f pos, sf::Color Color, float time)
{
	if (isExist)
	{
		moves.tick(time);
		moves.draw(Window, pos.x, pos.y+50, colorTable[7+ballColor-1]);
	}
}

void Ball::set(void)
{
	isExist = true;
}

void Ball::clear(void)
{
	isExist = false;
}

bool Ball::get(void)
{
	return isExist;
}

void Ball::tick(float time)
{
	moves.tick(time);
}

int Ball::getFrame(void)
{
	return moves.getFrame();
}

bool Ball::isPlaing(void)
{
	return moves.isPlaying();
}

void Ball::setAnimation(std::string name)
{
	this->moves.set(name);
}

std::string Ball::getAnimation(void)
{
	return moves.get();
}

