#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <map>



class Animation
{
public:
    std::vector<sf::IntRect> frames, framesFlip;
    float currentFrame, speed;
    bool loop, flip, isPlaying;
	sf::Text DebugLine;
	sf::Font font;
    sf::Sprite sprite;

    Animation()
    {
        currentFrame = 0;
        isPlaying = true;
        flip = false;
        loop = true;
    }

    void tick(float time)
    {
        if(!isPlaying) return;
        currentFrame += abs(speed) * time;
        if(currentFrame >= (float)frames.size())
        {
			if((int)currentFrame - frames.size() > (frames.size()-1))
				currentFrame = 0;
			else
				currentFrame -= frames.size();

            if(!loop)
            {
				sprite.setTextureRect(frames[int(frames.size()-1)]);
                isPlaying = false;
                return;
            }
        }
		if (flip) {
			sprite.setTextureRect(framesFlip[int(currentFrame)]);
		}
		else
		{
			sprite.setTextureRect(frames[int(currentFrame)]);
		}
	}
};

class AnimationManager
{
public:
    std::string currentAnim;
    std::map<std::string, Animation> animList;

    AnimationManager(){}
    ~AnimationManager()
    {animList.clear();}

    void create(std::string name, sf::Texture &texture, int x, int y, int w, int h, int count, float speed, int step = 0, bool Loop = true)
    {
        Animation a;
        a.speed = speed;
        a.loop = Loop;
        a.sprite.setTexture(texture);
        a.sprite.setOrigin(0,h);

        for(int i = 0; i < count; i++)
        {
            a.frames.push_back( sf::IntRect(x + i * step, y, w, h) );
            a.framesFlip.push_back( sf::IntRect( x + i * step + w, y, -w, h) );
        }
        animList[name] = a;
        currentAnim = name;
    }

    //Load from XML
    // I will add this later

    void set(std::string name)
    {
        currentAnim = name;
		animList[currentAnim].isPlaying = true;
		animList[currentAnim].currentFrame = 0;
        animList[currentAnim].flip = false;
    }

	std::string get(void)
	{
		return currentAnim;
	}

	int getFrame(void)
	{
		return(animList[currentAnim].currentFrame);
	}

    void draw(sf::RenderWindow &window, int x = 0, int y = 0)
    {
		animList[currentAnim].sprite.setPosition(x, y);
		window.draw( animList[currentAnim].sprite );
    }

	void draw(sf::RenderWindow &window, int x = 0, int y = 0, sf::Color Color = sf::Color(0xFF, 0xFF, 0xFF))
	{
		animList[currentAnim].sprite.setColor(Color);
		animList[currentAnim].sprite.setPosition(x, y);
		window.draw(animList[currentAnim].sprite);
	}

    void flip(bool b)
    {
        animList[currentAnim].flip = b;
    }

    void tick(float time)
    {
        animList[currentAnim].tick(time);
    }

    void pause(void)
    {
        animList[currentAnim].isPlaying = false;
    }

    void play(void)
    {
        animList[currentAnim].isPlaying = true;
    }

    void play(std::string name)
    {
        animList[name].isPlaying = true;
    }

    bool isPlaying(void)
    {
        return animList[currentAnim].isPlaying;
    }

    float getH(void)
    {
        return animList[currentAnim].frames[0].height;
    }

    float getW(void)
    {
        return animList[currentAnim].frames[0].width;
    }
};

#endif
