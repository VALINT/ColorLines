#ifndef _GAME_H_
#define _GAME_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <streambuf>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <time.h>
#include <string>
#include <map>
#include "MainMeny.h"
#include "Highscore.h"
#include "Animation.hpp"
#include "ui.h"
#include "common.h"

typedef enum {	IDLE_ST, 
				LOAD_RES_ST,
				START_ANIMATION_ST,
				MAIN_MENU_ST,
				CONTINUE,
				GAME_ST,
				BALL_EMERGENCE_ST,
				BALL_MOVE_ST,
				BALL_DISAPPEARANCE_ST,
				GAME_OVER_ST,
				LOAD_GAME_ST,
				HIGHSCORES_ST,
				SETTINGS_ST,
				ABOUT_ST,
				EXIT_ST
} FSM_STATE;

typedef std::pair< std::map< std::string, sf::Texture >, std::map< std::string, std::map< std::string, sf::Sprite > > > UnityGraph;

int game(void);

class Ball
{
public:
	Ball();
	void createAnimation(std::string, sf::Texture&, int, int, int, int, int, float, int, bool);
	void setColor(int);
	int getColor(void);
	void draw(sf::RenderWindow &Window, sf::Vector2f pos, sf::Color Color = sf::Color(0,0,0,0), float time = 0);
	void set(void);
	void clear(void);
	bool get(void);
	void tick(float);
	int  getFrame(void);
	bool isPlaing(void);
	void setAnimation(std::string);
	std::string getAnimation(void);
private:
	bool				isExist;
	AnimationManager	moves;
	int					ballColor;
};

class BallPlace : public Button
{
public:
	BallPlace(sf::Vector2f, sf::Vector2f);
	void SetPosition(sf::Vector2i);
	void insertBall(Ball);
	Ball getBall(void);
	void setBall(void);
	void removeBall(void);
	void setAnimation(std::string);
	std::string getAnimation(void);
	bool isBall(void);
	int  getFrame(void);
	bool isPlaing(void);
	int  getColor(void);
	void setColor(int);
	void draw(sf::RenderWindow &, float time = 0);
private:
	sf::Vector2i ballPosition = {0,0};
	Ball		 ball;
};

class MyGame
{
public:
	MyGame( UnityGraph*, sf::RenderWindow*,	sf::View*);
	void InitGame(void);
	void setTime(float);
	void setCallback(void(*callback) (std::pair<int, int>));
	void initGraphics();
	void CheckMouse(sf::Vector2f Coordinates);
	void insertBalls(void);
	void processField(void);
	bool moveball(void);
	void loadgame(std::string);
	void savegame(std::string);
	bool checkWay(sf::Vector2i position);
	bool checkLine();
	void clearBall();
	void setBall();
	void setBallAfteMove();
	void afterMove();
	void setTransition();
	void setStable();
	bool checkGameOver();
	void addButton(Button);
	void addLabel(Label);
	int  getScore();
	void setHighscore(int);
	void setEmergence();
	bool checkEmergence();
	void setDisappearance();
	bool checkDisappearance();
	void deleteLine(void);
	void createNew(void);
	void draw();
	std::string					scoreStr;
	std::string					highscoreStr;

private:
	int							score;
	int							ballsCounter;
	int							highscore;
	float						time;
	void (*callback) (std::pair<int, int>);
	std::vector<int>			ballPath;
	std::vector<int>			emBalls;
	std::vector<int>			deLine;
	std::vector<Button>			buttons;
	std::vector<Label>			labels;
	std::vector<BallPlace>		GameField;
	sf::Vector2i				choosenBall;
	std::map<sf::Color, Ball>	balls;
	Ball						standartBall;
	Ball						moveBall;
	std::vector<Ball>			nextBalls;
	sf::SoundBuffer				soundbuff;
	sf::Sound					sound;
	UnityGraph					*graphics;
	sf::RenderWindow			*outWindow;
	sf::View					*View;
	bool						transition = false;
	bool						startJump;
	bool						endJump = false;
	bool						soundTrigger = false;
};

#endif
