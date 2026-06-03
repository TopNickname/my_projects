#include "Game.h"
#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <cctype>
#include <random>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#endif

// ===================== Терминальный ввод =====================
void Game::enableRawMode() {
#ifdef _WIN32
#else
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
}

void Game::disableRawMode() {
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
#endif
}

char Game::readChar() {
#ifdef _WIN32
    return _getch();
#else
    if (havePending) {
        havePending = false;
        return pendingChar;
    }
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0)
        return c;
    return '\0';
#endif
}

bool Game::isKeyPressed() {
#ifdef _WIN32
    return _kbhit() != 0;
#else
    if (havePending)
        return true;
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
        pendingChar = c;
        havePending = true;
        return true;
    }
    return false;
#endif
}

// ===================== Преобразования ANSI =====================
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

// ===================== Конструктор / Деструктор =====================
Game::Game(int gridWidth, int gridHeight)
    : zoo(gridWidth, gridHeight)
{
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
    disableRawMode();
    if (altBufferActive) {
        std::cout << "\033[?1049l" << std::flush;
        altBufferActive = false;
    }
}

// ===================== Инструкции =====================
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
After pressing a number, you can:
  - type a name and press Enter → then enter X, Y coordinates.
  - press Enter on empty name → random name & random free position.
Press Escape at any input stage to cancel.

Animals on wrong terrain slowly lose HP and die.
'%' means many animals in one cell.
Names are shown in chat messages.
------------------------
Press Enter to start...
)" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ===================== Главный цикл =====================
void Game::run() {
    showInstructions();

    std::cout << "\033[?1049h" << std::flush;
    altBufferActive = true;
    enableRawMode();

    while (running) {
        auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - lastFrameTime).count();
        lastFrameTime = now;

        while (isKeyPressed()) {
            char key = readChar();
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

// ===================== Обработка ввода =====================
void Game::handleInput(char key) {
    // ------------------ Режимы ввода при добавлении ------------------
    if (addState != AddState::Normal) {
        switch (addState) {
        case AddState::EnterName:
            if (key == 27) { // Escape
                addState = AddState::Normal;
                addMessage("Add cancelled");
                return;
            }
            if (key == '\r' || key == '\n') {
                if (inputBuffer.empty()) {
                    // Пустая строка – случайное имя и случайная позиция
                    std::string species;
                    switch (chosenAnimalType) {
                    case '1': species = "Cat"; break;
                    case '2': species = "Bear"; break;
                    case '3': species = "Hamster"; break;
                    case '4': species = "Shark"; break;
                    case '5': species = "Fish"; break;
                    case '6': species = "Duck"; break;
                    case '7': species = "Creeper"; break;
                    default: species = "Unknown";
                    }
                    std::uniform_int_distribution<int> randid(1000, 9999);
                    std::string randomName = species + "_" + std::to_string(randid(rng));
                    nameBuffer = randomName;

                    // Создаём животное и помещаем на случайную свободную клетку
                    std::unique_ptr<Animal> animal;
                    switch (chosenAnimalType) {
                    case '1': animal = std::make_unique<Cat>(); break;
                    case '2': animal = std::make_unique<Bear>(); break;
                    case '3': animal = std::make_unique<Hamster>(); break;
                    case '4': animal = std::make_unique<Shark>(); break;
                    case '5': animal = std::make_unique<Just_a_fish>(); break;
                    case '6': animal = std::make_unique<Duck>(); break;
                    case '7': animal = std::make_unique<Creeper>(); break;
                    default: return;
                    }
                    animal->setName(nameBuffer);
                    Vectori2 freePos = zoo.getRandomFreePosition();
                    animal->setPos(freePos);
                    Animal* a = zoo.addAnimal(std::move(animal));
                    if (a)
                        addMessage(species + " '" + nameBuffer + "' added at (" +
                            std::to_string(freePos.x()) + "," + std::to_string(freePos.y()) + ")");
                    else
                        addMessage("Failed to add animal");
                    addState = AddState::Normal;
                    inputBuffer.clear();
                    return;
                }
                else {
                    // Имя введено – переходим к координатам
                    nameBuffer = inputBuffer;
                    addState = AddState::EnterX;
                    addMessage("Enter X coordinate (0-" + std::to_string(zoo.getGrid().getWidth() - 1) + "):");
                    inputBuffer.clear();
                }
            }
            else if (key == 127 || key == '\b') {
                if (!inputBuffer.empty()) inputBuffer.pop_back();
            }
            else if (std::isprint(static_cast<unsigned char>(key))) {
                inputBuffer += key;
            }
            break;

        case AddState::EnterX:
            if (key == 27) {
                addState = AddState::Normal;
                addMessage("Add cancelled");
                return;
            }
            if (key == '\r' || key == '\n') {
                if (inputBuffer.empty()) {
                    addMessage("Please enter X coordinate");
                    return;
                }
                try {
                    int x = std::stoi(inputBuffer);
                    if (x < 0 || x >= zoo.getGrid().getWidth()) {
                        addMessage("X out of bounds. Enter again:");
                        inputBuffer.clear();
                        return;
                    }
                    pendingX = x;
                    addState = AddState::EnterY;
                    addMessage("Enter Y coordinate (0-" + std::to_string(zoo.getGrid().getHeight() - 1) + "):");
                    inputBuffer.clear();
                }
                catch (...) {
                    addMessage("Invalid number. Enter X:");
                    inputBuffer.clear();
                }
            }
            else if (key == 127 || key == '\b') {
                if (!inputBuffer.empty()) inputBuffer.pop_back();
            }
            else if (std::isdigit(static_cast<unsigned char>(key)) || key == '-') {
                inputBuffer += key;
            }
            break;

        case AddState::EnterY:
            if (key == 27) {
                addState = AddState::Normal;
                addMessage("Add cancelled");
                return;
            }
            if (key == '\r' || key == '\n') {
                if (inputBuffer.empty()) {
                    addMessage("Please enter Y coordinate");
                    return;
                }
                try {
                    int y = std::stoi(inputBuffer);
                    if (y < 0 || y >= zoo.getGrid().getHeight()) {
                        addMessage("Y out of bounds. Enter again:");
                        inputBuffer.clear();
                        return;
                    }
                    int x = pendingX;
                    std::unique_ptr<Animal> animal;
                    std::string species;
                    switch (chosenAnimalType) {
                    case '1': animal = std::make_unique<Cat>();    species = "Cat"; break;
                    case '2': animal = std::make_unique<Bear>();   species = "Bear"; break;
                    case '3': animal = std::make_unique<Hamster>(); species = "Hamster"; break;
                    case '4': animal = std::make_unique<Shark>();  species = "Shark"; break;
                    case '5': animal = std::make_unique<Just_a_fish>(); species = "Fish"; break;
                    case '6': animal = std::make_unique<Duck>();   species = "Duck"; break;
                    case '7': animal = std::make_unique<Creeper>(); species = "Creeper"; break;
                    default: return;
                    }
                    animal->setName(nameBuffer);
                    animal->setPos(Vectori2(x, y));
                    Animal* a = zoo.addAnimal(std::move(animal));
                    if (a)
                        addMessage(species + " '" + nameBuffer + "' added at (" +
                            std::to_string(x) + "," + std::to_string(y) + ")");
                    else
                        addMessage("Failed to add animal");
                }
                catch (...) {
                    addMessage("Invalid number. Enter Y:");
                    inputBuffer.clear();
                    return;
                }
                addState = AddState::Normal;
                inputBuffer.clear();
            }
            else if (key == 127 || key == '\b') {
                if (!inputBuffer.empty()) inputBuffer.pop_back();
            }
            else if (std::isdigit(static_cast<unsigned char>(key)) || key == '-') {
                inputBuffer += key;
            }
            break;
        }
        return;
    }

    // ------------------ Обычный режим ------------------
    switch (key) {
    case 'p': case 'P':
        paused = !paused;
        addMessage(paused ? "Game paused" : "Game resumed");
        break;
    case 'q': case 'Q':
        running = false;
        addMessage("Quitting...");
        break;
    case '1': case '2': case '3': case '4': case '5': case '6': case '7':
        addState = AddState::EnterName;
        chosenAnimalType = key;
        inputBuffer.clear();
        addMessage("Enter name (or Enter for random):");
        break;
    default:
        break;
    }
}

// ===================== Обновление состояния =====================
void Game::update(float deltaTime) {
    const auto& animals = zoo.getAnimals();
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    std::uniform_int_distribution<int> dirDist(-1, 1);

    std::vector<int> toRemove;

    for (const auto& pair : animals) {
        auto& animal = *pair.second;
        int id = animal.getID();

        Terrain currentTerrain = zoo.getGrid().getTerrain(animal.getPos());
        if (!animal.can_traverse(currentTerrain)) {
            animal.setHP(animal.getHP() - 2.0f * deltaTime);
            if (animal.getHP() <= 0.0) {
                addMessage(animal.getName() + " (" + animal.species() + ") died in wrong terrain!");
                toRemove.push_back(id);
                continue;
            }
        }

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
                std::string msg = animal.getName() + " (" + animal.species() + ") " + sound;
                addMessage(msg);
            }
        }
    }

    for (int id : toRemove) {
        zoo.removeAnimal(id);
    }
}

// ===================== Отрисовка =====================
void Game::draw() {
    std::cout << "\033[H";

    const Grid& grid = zoo.getGrid();
    int w = grid.getWidth();
    int h = grid.getHeight();
    const auto& animals = zoo.getAnimals();

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            std::vector<const Animal*> here;
            for (const auto& pair : animals) {
                const auto& a = *pair.second;
                if (a.getPos().x() == x && a.getPos().y() == y)
                    here.push_back(&a);
            }

            if (here.empty()) {
                Terrain t = grid.getTerrain(Vectori2{ x,y });
                char terrainChar = (t == Terrain::Water) ? '~' : '.';
                std::cout << terrainToAnsiBG(t)
                    << terrainChar
                    << "\033[0m";
            }
            else if (here.size() == 1) {
                const Animal& a = *here.front();
                Terrain t = grid.getTerrain(a.getPos());
                std::cout << terrainToAnsiBG(t)
                    << colorToAnsiFG(a.getColor())
                    << Animal::shapeToChar(a.getShape())
                    << "\033[0m";
            }
            else {
                std::cout << "\033[37;41m%\033[0m";
            }
        }
        std::cout << "\n";
    }

    std::cout << std::string(w, '=') << "\n";

    // --- Блок чата: всегда выводим MAX_CHAT_LINES строк с очисткой ---
    int totalMessages = static_cast<int>(chatMessages.size());
    int linesToShow = std::min(totalMessages, static_cast<int>(MAX_CHAT_LINES));
    for (int line = 0; line < static_cast<int>(MAX_CHAT_LINES); ++line) {
        int msgIdx = totalMessages - linesToShow + line;
        if (msgIdx >= 0 && msgIdx < totalMessages) {
            std::cout << chatMessages[msgIdx];
        }
        std::cout << "\033[K\n";   // очистить до конца строки и перевод
    }

    // Подсказка при вводе
    if (addState != AddState::Normal) {
        std::cout << "> ";
        if (addState == AddState::EnterName) {
            std::cout << "Name: " << inputBuffer;
        }
        else if (addState == AddState::EnterX) {
            std::cout << "X: " << inputBuffer;
        }
        else if (addState == AddState::EnterY) {
            std::cout << "Y: " << inputBuffer;
        }
        std::cout << "_\n";
    }

    // Очистка оставшейся части экрана
    std::cout << "\033[J" << std::flush;
}

// ===================== Чат =====================
void Game::addMessage(const std::string& msg) {
    chatMessages.push_back(msg);
    if (chatMessages.size() > MAX_CHAT_LINES * 2) {
        chatMessages.pop_front();
    }
}