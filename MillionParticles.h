#pragma once
#include "SFMLGameEngine.h"

#include <array>
class MillionParticles :
	public SFMLGameEngine
{
public:
	MillionParticles();
	virtual ~MillionParticles();

protected:
	virtual void loadResources();
	virtual void handleEvents(sf::Event & ev);
	virtual void handleInput();
	virtual void handleLogic();
	virtual void handleDrawing();

private:
	void m_textUpdate();
private:
	sf::Font arial;

	std::mutex fpsCounterMutex;
	sf::Text fpsCounter;

	std::vector<sf::Vector2f> velocities;
	sf::VertexArray particles;
};

