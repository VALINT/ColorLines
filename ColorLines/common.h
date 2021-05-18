#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define BG_COLOR sf::Color(0x6C, 0x6C, 0x6C)
#define BG_SEMI_COLOR sf::Color(0x6C, 0x6C, 0x6C, 0xE0)
#define BG2_COLOR sf::Color(0x1A,0x1A,0x1A)
#define BG3_COLOR sf::Color(0x11,0x11,0x11)
#define TEXT_COLOR sf::Color(0xD0, 0xD0, 0xDf)
#define HOVER_COLOR sf::Color(0x85, 0x85, 0x85)
#define HOLD_COLOR sf::Color(0x14, 0x14, 0x14)
#define TRANSPARENT sf::Color(0xFF, 0xFF, 0xFF, 0x00)

typedef std::pair< std::map< std::string, sf::Texture >, std::map< std::string, std::map< std::string, sf::Sprite > > > UnityGraph;
typedef std::map<std::string, std::pair<sf::SoundBuffer, sf::Sound>> UnitySound;