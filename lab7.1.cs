#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

// Базовый класс Entity
class Entity
{
    public:
    virtual ~Entity() = default;
    virtual void display() const = 0;
    virtual std::string serialize() const = 0;
};

// Класс Player
class Player : public Entity
{
private:
    std::string name;
int health;
int level;

public:
    Player(const std::string& name, int health, int level)
        : name(name), health(health), level(level) {
}

void display() const override {
        std::cout << "Player: " << name
            << ", Health: " << health
            << ", Level: " << level << std::endl;
    }

    std::string serialize() const override {
        return "Player " + name + " " + std::to_string(health) + " " + std::to_string(level);
    }

    // Геттеры для загрузки
    static Player* deserialize(const std::string& data)
{
    size_t pos = data.find(' ');
    if (pos == std::string::npos || data.substr(0, pos) != "Player") {
        return nullptr;
    }

    size_t nextPos = data.find(' ', pos + 1);
    std::string name = data.substr(pos + 1, nextPos - pos - 1);

    pos = nextPos;
    nextPos = data.find(' ', pos + 1);
    int health = std::stoi(data.substr(pos + 1, nextPos - pos - 1));

    pos = nextPos;
    int level = std::stoi(data.substr(pos + 1));

    return new Player(name, health, level);
}
};

// Шаблонный класс GameManager
template<typename T>
class GameManager
{
    private:
    std::vector<T> entities;

    public:
    ~GameManager()
    {
        for (auto entity : entities)
        {
            delete entity;
        }
    }

    void addEntity(T entity)
    {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->display();
}
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
if (!file)
{
    throw std::runtime_error("Failed to open file for writing.");
}

for (const auto&entity : entities) {
            file << entity->serialize() << "\n";
        }
    }

    void loadFromFile(const std::string& filename)
{
    // Очищаем текущие entities
    for (auto entity : entities) {
            delete entity;
        }
        entities.clear();

std::ifstream file(filename);
if (!file)
{
    throw std::runtime_error("Failed to open file for reading.");
}

std::string line;
while (std::getline(file, line))
{
    if (line.empty()) continue;

    // В этом примере обрабатываем только Player
    // Можно расширить для других типов Entity
    Player* player = Player::deserialize(line);
    if (player)
    {
        addEntity(player);
    }
}
    }
};

int main()
{
    try
    {
        GameManager<Entity*> manager;

        // Создаем несколько персонажей
        manager.addEntity(new Player("Hero", 100, 1));
        manager.addEntity(new Player("Mage", 80, 3));
        manager.addEntity(new Player("Warrior", 120, 2));

        // Сохраняем в файл
        manager.saveToFile("game_save.txt");
        std::cout << "Game saved to file.\n";

        // Загружаем из файла в новый менеджер
        GameManager<Entity*> loadedManager;
        loadedManager.loadFromFile("game_save.txt");
        std::cout << "Game loaded from file:\n";

        // Отображаем загруженных персонажей
        loadedManager.displayAll();

    }
    catch (const std::exception&e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
    }