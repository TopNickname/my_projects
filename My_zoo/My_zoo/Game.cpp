#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <algorithm>
#include <random>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#endif

// ---- Утилиты ввода ----

bool Game::kbhit() {
#ifdef _WIN32
	return _kbhit() != 0;
#else
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if (ch != EOF) {
		ungetc(ch, stdin);
		return true;
	}
	return false;
#endif
}

char Game::getch() {
#ifdef _WIN32
	return _getch();
#else
	char ch;
	std::cin.get(ch);
	return ch;
#endif
}

// ---- Преобразование в ANSI ----

std::string Game::colorToAnsiFG(Animal::Color c) {
	switch (c) {
	case Animal::Color::red:    return "\033[31m";
	case Animal::Color::green:  return "\033[32m";
	case Animal::Color::blue:   return "\033[34m";
	case Animal::Color::yellow: return "\033[33m";
	case Animal::Color::brown:  return "\033[33;2m";
	case Animal::Color::purple: return "\033[35m";
	case Animal::Color::orange: return "\033[38;5;214m";
	case Animal::Color::cyan:   return "\033[36m";
	case Animal::Color::black:  return "\033[30m";
	case Animal::Color::white:  return "\033[37m";
	case Animal::Color::pink:   return "\033[38;5;213m";
	case Animal::Color::lime:   return "\033[38;5;118m";
	case Animal::Color::teal:   return "\033[38;5;30m";
	case Animal::Color::maroon: return "\033[38;5;88m";
	case Animal::Color::navy:   return "\033[38;5;18m";
	case Animal::Color::olive:  return "\033[38;5;100m";
	case Animal::Color::silver: return "\033[38;5;145m";
	case Animal::Color::gold:   return "\033[38;5;220m";
	default:                    return "\033[0m";
	}
}

std::string Game::terrainToAnsiBG(Terrain t) {
	switch (t) {
	case Terrain::Land:  return "\033[42m";
	case Terrain::Water: return "\033[44m";
	default:             return "\033[40m";
	}
}

char Game::shapeToChar(Animal::Shape s) {
	return Animal::shapeToChar(s);
}

// ---- Конструктор / Деструктор ----

Game::Game(int gridWidth, int gridHeight)
	: zoo(gridWidth, gridHeight)
{
	// водоём по центру
	for (int y = gridHeight / 3; y < 2 * gridHeight / 3; ++y) {
		for (int x = gridWidth / 3; x < 2 * gridWidth / 3; ++x) {
			zoo.getGrid().setTerrain(Vectori2{ x,y }, Terrain::Water);
		}
	}
	lastFrameTime = Clock::now();
}

Game::~Game() {
	restoreTerminal();
}

void Game::restoreTerminal() {
	if (altBufferActive) {
		std::cout << "\033[?1049l" << std::flush;
		altBufferActive = false;
	}
}

// ---- Инструкции ----

void Game::showInstructions() const {
	std::cout << R"(
=== Zoo Simulator ===
Controls:
  p / P   - pause / resume
  q / Q   - quit
  1       - add Cat
  2       - add Bear
  3       - add Hamster
  4       - add Shark
  5       - add Just a Fish
  6       - add Duck
  7       - add Creeper
------------------------
Press Enter to start...
)" << std::endl;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ---- Главный цикл ----

void Game::run() {
	showInstructions();

	// Вход в альтернативный буфер
	std::cout << "\033[?1049h" << std::flush;
	altBufferActive = true;

	while (running) {
		auto now = Clock::now();
		float dt = std::chrono::duration<float>(now - lastFrameTime).count();
		lastFrameTime = now;

		while (kbhit()) {
			char key = getch();
			handleInput(key);
		}

		if (!paused) {
			accumulator += dt;
			while (accumulator >= updateInterval) {
				update(updateInterval);
				accumulator -= updateInterval;
			}
		}

		draw();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS));
	}

	restoreTerminal();
}

// ---- Обработка ввода ----

void Game::handleInput(char key) {
	switch (key) {
	case 'p': case 'P':
		paused = !paused;
		addMessage(paused ? "Game paused" : "Game resumed");
		break;
	case 'q': case 'Q':
		running = false;
		addMessage("Quitting...");
		break;
	case '1': addAnimal(std::make_unique<Cat>()); addMessage("Cat added"); break;
	case '2': addAnimal(std::make_unique<Bear>()); addMessage("Bear added"); break;
	case '3': addAnimal(std::make_unique<Hamster>()); addMessage("Hamster added"); break;
	case '4': addAnimal(std::make_unique<Shark>()); addMessage("Shark added"); break;
	case '5': addAnimal(std::make_unique<Just_a_fish>()); addMessage("Just a fish added"); break;
	case '6': addAnimal(std::make_unique<Duck>()); addMessage("Duck added"); break;
	case '7': addAnimal(std::make_unique<Creeper>()); addMessage("Creeper added"); break;
	default: break;
	}
}

// ---- Обновление состояния ----

void Game::update(float /*deltaTime*/) {
	const auto& animals = zoo.getAnimals();
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	std::uniform_int_distribution<int> dirDist(-1, 1);

	for (const auto& pair : animals) {
		auto& animal = *pair.second;

		if (dist(rng) < 0.4f) {
			int dx = dirDist(rng);
			int dy = dirDist(rng);
			if (dx != 0 || dy != 0) {
				Vectori2 newPos(animal.getPos().x() + dx, animal.getPos().y() + dy);
				zoo.moveAnimal(animal, newPos);
			}
		}

		if (dist(rng) < 0.1f) {
			std::string sound = animal.getSound();
			if (!sound.empty() && sound != "...") {
				std::string msg = animal.getSound() + " (id:" + std::to_string(animal.getID()) + ")";
				addMessage(msg);
			}
		}
	}
}

// ---- Отрисовка ----

void Game::draw() {
	std::cout << "\033[H" << std::flush;

	const Grid& grid = zoo.getGrid();
	int w = grid.getWidth();
	int h = grid.getHeight();
	const auto& animals = zoo.getAnimals();

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			bool animalHere = false;
			for (const auto& pair : animals) {
				const auto& a = *pair.second;
				if (a.getPos().x() == x && a.getPos().y() == y) {
					std::cout << colorToAnsiFG(a.getColor())
						<< shapeToChar(a.getShape())
						<< "\033[0m";
					animalHere = true;
					break;
				}
			}
			if (!animalHere) {
				Terrain t = grid.getTerrain(Vectori2{ x,y });
				char terrainChar = (t == Terrain::Water) ? '~' : '.';
				std::cout << terrainToAnsiBG(t)
					<< terrainChar
					<< "\033[0m";
			}
		}
		std::cout << "\n";
	}

	std::cout << std::string(w, '=') << "\n";
	int linesToShow = std::min(chatMessages.size(), MAX_CHAT_LINES);
	for (size_t i = chatMessages.size() - linesToShow; i < chatMessages.size(); ++i) {
		std::cout << chatMessages[i] << "\n";
	}
	std::cout << std::flush;
}

// ---- Чат ----

void Game::addMessage(const std::string& msg) {
	chatMessages.push_back(msg);
	if (chatMessages.size() > MAX_CHAT_LINES * 2) {
		chatMessages.pop_front();
	}
}