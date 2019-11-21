#include "SFMLGameEngine.h"

SFMLGameEngine::SFMLGameEngine(unsigned int windowWidth, unsigned int windowHeigth, std::string appName, unsigned int maxUpdatesPerSecond, int8_t fpsLimit) {
	//creating render window 
	std::cout << "Creating window..." << std::endl;

	this->uWindowHeigth = windowHeigth;
	this->uWindowWidth = windowWidth;

	this->fMaxUpdatesPerSecond = 1.0f / static_cast<float>(maxUpdatesPerSecond);
	this->fFPSLimit = 1.0f / static_cast<float>(fpsLimit);

	this->gameData->renderWindow.create({ uWindowWidth,uWindowHeigth }, appName, sf::Style::Fullscreen | sf::Style::Titlebar);

	std::cout << "\tWindow created.\n" << std::endl;

	this->gameData->renderWindow.setVerticalSyncEnabled(true);

	//setting up views
	std::cout << "Setting up views..." << std::endl;
	this->gameData->playerView.setSize(static_cast<float>(uWindowWidth), static_cast<float>(uWindowHeigth));
	this->gameData->guiView.setSize(static_cast<float>(uWindowWidth), static_cast<float>(uWindowHeigth));

	this->gameData->playerView.setCenter(static_cast<float>(uWindowWidth / 2), static_cast<float>(uWindowHeigth / 2));
	this->gameData->guiView.setCenter(static_cast<float>(uWindowWidth / 2), static_cast<float>(uWindowHeigth / 2));

	std::cout << "\tViews initialized.\n" << std::endl;



	this->gameData->bGameRunning = true;
	this->gameData->bSimulating = true;

}

SFMLGameEngine::~SFMLGameEngine()
{
}

sf::Vector2u SFMLGameEngine::getWindowSize()
{
	return sf::Vector2u(uWindowWidth, uWindowHeigth);
}

void SFMLGameEngine::m_renderingThread()
{
	while (this->gameData->bGameRunning)
	{
		m_updateRenderingClock();
		m_draw();
	}
}

void SFMLGameEngine::m_logicThread()
{
	while (this->gameData->bGameRunning)
	{
		m_updateLogicClock();	
		m_update();
	}
}

void SFMLGameEngine::Run()
{
	loadResources();

	this->gameData->bGameRunning = true;
	this->gameData->bSimulating = true;

	this->gameData->renderWindow.setActive(false);

	std::cout << "Creating threads..." << std::endl;

	std::thread renderThread(&SFMLGameEngine::m_renderingThread, this);
	std::thread updateThread(&SFMLGameEngine::m_logicThread, this);

	std::cout << "\tThreads created.\n" << std::endl;

	std::cout << "Engine started!" << std::endl;

	while (this->gameData->bGameRunning)
	{
		fEventsElapsedTime = eventsClk.restart().asSeconds();
		m_pollEvents();
	}

	std::cout << "Joining threads..." << std::endl;

	updateThread.join();
	renderThread.join();

	std::cout << "\tThreads joined.\n" << std::endl;

	this->gameData->renderWindow.close();
}

void SFMLGameEngine::m_pollEvents()
{
	sf::Event events;

	while (gameData->renderWindow.pollEvent(events)) {

		handleEvents(events);
	}

	handleInput();
}

void SFMLGameEngine::m_update()
{
	if (gameData->bSimulating) {
		handleLogic();
		//m_updateText();
	}
}

void SFMLGameEngine::m_draw()
{
	this->gameData->renderWindow.clear();

	handleDrawing();

	//drawing the fps counter


	//switching buffers
	this->gameData->renderWindow.display();
}

void SFMLGameEngine::m_updateLogicClock() {
	double elapsedTimeDiff = 0.0;

	//elapsed time update
	this->updateTime = this->updateClk.restart();
	this->fUpdateElapsedTime = this->updateTime.asSeconds();

	fUpdateCounter += fUpdateElapsedTime;
	if (this->fUpdateCounter >= 0.2f) {
		this->fUpdateCounter -= 0.2f;
	}

	if (this->fUpdateElapsedTime >= 0.2f) {
		this->fUpdateElapsedTime = 0.2f;
	}

	//lock the updates per second
	if (this->fUpdateElapsedTime < fMaxUpdatesPerSecond) {

		elapsedTimeDiff = fMaxUpdatesPerSecond - this->fUpdateElapsedTime;

		this->fUpdateElapsedTime = static_cast<float>(fMaxUpdatesPerSecond);
		std::this_thread::sleep_for(std::chrono::duration<float>(elapsedTimeDiff));
	}

}


void SFMLGameEngine::m_updateRenderingClock()
{
	double renderingTimeDiff = 0.0;

	//elapsed time update
	this->renderTime = this->renderClk.restart();
	this->fRenderElapsedTime = this->updateTime.asSeconds();

	fRenderCounter += fRenderElapsedTime;

	if (this->fRenderCounter >= 0.2f) {
		this->fRenderCounter -= 0.2f;
	}

	if (this->fRenderElapsedTime >= 0.5f) {
		this->fRenderElapsedTime = 0.5f;
	}

	//lock the updates per second
	if (this->fRenderElapsedTime < fFPSLimit) {

		renderingTimeDiff = fFPSLimit - this->fRenderElapsedTime;

		this->fRenderElapsedTime = static_cast<float>(fFPSLimit);
		std::this_thread::sleep_for(std::chrono::duration<float>(renderingTimeDiff));
	}
}

