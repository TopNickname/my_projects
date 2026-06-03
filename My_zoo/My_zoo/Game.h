#pragma once
#include "Zoo.h"
#include <deque>
#include <string>
#include <chrono>
#include <random>

class Game {
public:
	Game(int gridWidth, int gridHeight);
	virtual ~Game();

	void run();

	Animal* addAnimal(std::unique_ptr<Animal> animal) {
		return zoo.addAnimal(std::move(animal));
	}

	Zoo& Get_zoo() { return zoo; }

protected:
	virtual void update(float deltaTime);
	virtual void draw();
	virtual void handleInput(char key);

	void addMessage(const std::string& msg);
	void showInstructions() const;

	static std::string colorToAnsiFG(Animal::Color c);
	static std::string terrainToAnsiBG(Terrain t);
	static char shapeToChar(Animal::Shape s);

private:
	int FPS = 5;
	Zoo zoo;
	bool running = true;
	bool paused = false;

	std::deque<std::string> chatMessages;
	static constexpr size_t MAX_CHAT_LINES = 5;

	using Clock = std::chrono::steady_clock;
	Clock::time_point lastFrameTime;
	float updateInterval = 0.5f;
	float accumulator = 0.0f;

	std::mt19937 rng{ std::random_device{}() };

	static bool kbhit();
	static char getch();

	void restoreTerminal();
	bool altBufferActive = false;
};