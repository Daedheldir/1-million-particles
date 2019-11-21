#pragma once

#include <chrono>

#include <SFML/Graphics.hpp>
#include <memory>
#include <mutex>
#include <iostream>

//GAME DATA
struct GameData {

	bool bGameRunning;
	bool bSimulating;

	//render window
	sf::RenderWindow	renderWindow;

	//views
	sf::View	playerView, guiView;

	//fps counter				<--DEBUG
	//sf::Text	FPSCounter;
	//float		fFramesPerSecond = 0.0f;
	//float		fUpdatesPerSecond = 0.0f;
};

//reference to game data;
typedef std::shared_ptr<GameData> GameDataRef;

class SFMLGameEngine
{
public:
	SFMLGameEngine(unsigned int windowWidth, unsigned int windowHeigth, std::string appName, unsigned int maxUpdatesPerSecond, int8_t fpsLimit);
	virtual ~SFMLGameEngine();

	sf::Vector2u getWindowSize();

	void Run();

protected:
	virtual void loadResources() = 0;
	virtual void handleEvents(sf::Event & ev) = 0;
	virtual void handleInput() = 0;
	virtual void handleLogic() = 0;
	virtual void handleDrawing() = 0;
private:

	void m_pollEvents();
	void m_update();
	void m_draw();

	void m_renderingThread();
	void m_logicThread();

	//void m_updateText(std::string const & additionalString = "");
	void m_updateLogicClock();
	void m_updateRenderingClock();

protected:
	GameDataRef gameData = std::make_shared<GameData>();
	float		fUpdateElapsedTime, fEventsElapsedTime, fRenderElapsedTime;

private:

	unsigned int uWindowWidth, uWindowHeigth;
	//sf::Font font;

	//clocks
	sf::Clock	updateClk, eventsClk, renderClk;
	sf::Time	updateTime, eventsTime, renderTime;

	float	fMaxUpdatesPerSecond;	//max logic updates per second equal to 1.0f / (int) maxUpdatesPerSecond
	float	fFPSLimit;				//max rendering updates per second equal to 1.0f / (int) maxFps

	float	fUpdateCounter, fRenderCounter;
};
