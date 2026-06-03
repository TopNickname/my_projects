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

private:
	void enableRawMode();
	void disableRawMode();
	char readChar();
	bool isKeyPressed();

	enum class AddState { Normal, EnterName, EnterX, EnterY };
	AddState addState = AddState::Normal;
	int  chosenAnimalType = 0;
	std::string inputBuffer;
	std::string nameBuffer;   // сохраняет введённое имя
	int pendingX = 0;         // сохраняет X-координату

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

	void restoreTerminal();
	bool altBufferActive = false;

#ifdef _WIN32
#else
	struct termios orig_termios;
	char pendingChar = 0;
	bool havePending = false;
#endif
};