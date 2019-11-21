#include "MillionParticles.h"



MillionParticles::MillionParticles() :
	SFMLGameEngine(1920, 1080, "1 Million Particles", 120, 60)
{
	for (int i = 0; i < 1000000; ++i) {
		particles.append(sf::Vertex(sf::Vector2f((rand() % getWindowSize().x), (rand() % getWindowSize().y)), sf::Color(100, 80, 60)));
		velocities.push_back(sf::Vector2f(0, 0));
	}

	fpsCounter.setFont(arial);
	fpsCounter.setCharacterSize(15);
	fpsCounter.setPosition(this->gameData->renderWindow.mapPixelToCoords(sf::Vector2i(5, 5)));
}


MillionParticles::~MillionParticles()
{
}

void MillionParticles::loadResources()
{
	arial.loadFromFile("Data\\Fonts\\arial.ttf");
}

void MillionParticles::handleEvents(sf::Event & ev)
{
	switch (ev.type)
	{
	case sf::Event::Closed:
		gameData->bGameRunning = false;
		break;
	case sf::Event::KeyReleased: {
		switch (ev.key.code) {
		case sf::Keyboard::Escape:
			gameData->bGameRunning = false;
			break;
		case sf::Keyboard::Space:
			this->gameData->bSimulating = !this->gameData->bSimulating;
			break;
		default:
			break;
		}
	}
	default:
		break;
	}
}

void MillionParticles::handleInput()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(this->gameData->renderWindow);
		sf::Vector2f worldPos = this->gameData->renderWindow.mapPixelToCoords(mousePos);

		//worldPos.x - position.x, worldPos.y - position.y
		for (int i = 0; i < particles.getVertexCount(); ++i) {
			sf::Vector2f velocityVec(worldPos.x - particles[i].position.x, worldPos.y - particles[i].position.y);
			float vecLength = sqrt((velocityVec.x * velocityVec.x) + (velocityVec.y * velocityVec.y));

			velocityVec.x /= powf(vecLength, 1.1);
			velocityVec.y /= powf(vecLength, 1.1);

			velocities[i].x += velocityVec.x;
			velocities[i].y += velocityVec.y;

			particles[i].color.r = 100 + static_cast<int>((abs(velocities[i].x) + abs(velocities[i].y)) / 2.0f);
			particles[i].color.g = 50 + static_cast<int>(abs(velocities[i].x));
			particles[i].color.b = 0 + static_cast<int>(abs(velocities[i].y));

		}

	}
	//Moving the camera
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		gameData->playerView.move(-gameData->playerView.getSize().x * fEventsElapsedTime, 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		gameData->playerView.move(gameData->playerView.getSize().x * fEventsElapsedTime, 0.0f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		gameData->playerView.move(0.0f, -gameData->playerView.getSize().x * fEventsElapsedTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		gameData->playerView.move(0.0f, gameData->playerView.getSize().x * fEventsElapsedTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
		gameData->playerView.zoom(1.0f + fEventsElapsedTime);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
		gameData->playerView.zoom(1.0f - fEventsElapsedTime);
	}

}

void MillionParticles::handleLogic()
{
	for (int i = 0; i < particles.getVertexCount(); ++i) {
		particles[i].position.x += velocities[i].x * fUpdateElapsedTime;
		particles[i].position.y += velocities[i].y * fUpdateElapsedTime;
	}

	m_textUpdate();
}

void MillionParticles::handleDrawing()
{
	this->gameData->renderWindow.setView(this->gameData->playerView);
	this->gameData->renderWindow.draw(particles);

	this->gameData->renderWindow.setView(this->gameData->guiView);
	std::unique_lock<std::mutex> fpsLock(fpsCounterMutex);
	this->gameData->renderWindow.draw(fpsCounter);
	fpsLock.unlock();

}

void MillionParticles::m_textUpdate()
{
	float fUpdatesPerSecond = 1.0f / fUpdateElapsedTime;
	float fFramesPerSecond = 1.0f / fRenderElapsedTime;

	std::string str = "FPS: ";
	str += std::to_string(fFramesPerSecond);

	str += "\nUpdates per second: ";
	str += std::to_string(fUpdatesPerSecond);

	str += "\n\nESC - Exit\nSpacebar - Resume/Pause Simulation\nWASD - Move view around\nQE - Zoom in/out\nLMB - accelerate particles";


	std::unique_lock<std::mutex> fpsLock(fpsCounterMutex);
	fpsCounter.setString(str);
	fpsLock.unlock();
}
