#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <thread>

// Класс Логгера (без изменений)
class Logger {
private:
    std::ofstream logFile;

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        char buf[80];
        std::tm tm;
        localtime_s(&tm, &in_time_t);
        strftime(buf, sizeof(buf), "[%Y-%m-%d %H:%M:%S]", &tm);
        return std::string(buf);
    }

public:
    Logger(const std::string& filename) {
        logFile.open(filename, std::ios::app);
        if (!logFile) {
            throw std::runtime_error("Cannot open log file");
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const std::string& message) {
        std::string logEntry = getCurrentTime() + " " + message;
        std::cout << logEntry << std::endl;
        logFile << logEntry << std::endl;
        logFile.flush();
    }
};

// Класс Персонажа (теперь конкретный, не абстрактный)
class Character {
protected:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int level;
    int experience;
    Logger& logger;

public:
    Character(const std::string& n, int h, int a, int d, Logger& logger)
        : name(n), health(h), maxHealth(h), attack(a), defense(d),
        level(1), experience(0), logger(logger) {
        logger.log("Character " + name + " appear");
    }

    virtual ~Character() = default;

    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.getDefense();
        if (damage > 0) {
            enemy.takeDamage(damage);
            std::string msg = name + " attack " + enemy.getName() + " to " + std::to_string(damage) + " damage";
            logger.log(msg);

            if (!enemy.isAlive()) {
                msg = enemy.getName() + " killed";
                logger.log(msg);
                gainExperience(30);
            }
        }
        else {
            logger.log(name + " attack " + enemy.getName() + ", but there is no damage");
        }
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
        logger.log(name + " get " + std::to_string(damage) + " damage");
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        logger.log(name + " heal " + std::to_string(amount) + " hp");
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {
            level++;
            experience -= 100;
            maxHealth += 20;
            attack += 5;
            defense += 3;
            health = maxHealth;
            logger.log(name + " got new level " + std::to_string(level) + "!");
        }
    }

    void displayInfo() const {
        std::cout << name << " [level: " << level << ", HP: " << health << "/" << maxHealth
            << ", Atk: " << attack << ", DEF: " << defense
            << ", Exp: " << experience << "/100]" << std::endl;
    }

    bool isAlive() const { return health > 0; }
    std::string getName() const { return name; }
    int getDefense() const { return defense; }
};

// Класс Монстра (наследуется от Character)
class Monster : public Character {
public:
    Monster(const std::string& n, int h, int a, int d, Logger& logger)
        : Character(n, h, a, d, logger) {
    }

    virtual ~Monster() = default;

    virtual void specialAttack(Character& target) = 0;
};

// Конкретные типы монстров
class Goblin : public Monster {
public:
    Goblin(Logger& logger) : Monster("Goblin", 30, 8, 2, logger) {}

    void specialAttack(Character& target) override {
        int damage = attack - target.getDefense() + 2; // Бонус к атаке
        if (damage > 0) {
            target.takeDamage(damage);
            logger.log(name + " Attack to " + std::to_string(damage) + " damage");
        }
    }
};

class Skeleton : public Monster {
public:
    Skeleton(Logger& logger) : Monster("Skeleton", 40, 10, 5, logger) {}

    void specialAttack(Character& target) override {
        int damage = attack - target.getDefense() + 3;
        if (damage > 0) {
            target.takeDamage(damage);
            logger.log(name + " Attack to " + std::to_string(damage) + " damage");
        }
    }
};

class Dragon : public Monster {
public:
    Dragon(Logger& logger) : Monster("Dragon", 100, 20, 10, logger) {}

    void specialAttack(Character& target) override {
        int damage = attack - target.getDefense() + 5;
        if (damage > 0) {
            target.takeDamage(damage);
            logger.log(name + " Attack to " + std::to_string(damage) + " damage");
        }
    }
};

// Функция битвы
void battle(Character& hero, Monster& monster) {
    while (hero.isAlive() && monster.isAlive()) {
        // Ход героя
        hero.attackEnemy(monster);
        if (!monster.isAlive()) break;

        // Ход монстра
        monster.specialAttack(hero);
        if (!hero.isAlive()) break;

        // Пауза между раундами
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    try {
        Logger gameLogger("game_log.txt");
        gameLogger.log("=== Start Game ===");

        Character hero("Hero", 100, 15, 5, gameLogger);

        // Создаем монстров
        std::vector<std::unique_ptr<Monster>> monsters;
        monsters.push_back(std::make_unique<Goblin>(gameLogger));
        monsters.push_back(std::make_unique<Skeleton>(gameLogger));
        monsters.push_back(std::make_unique<Dragon>(gameLogger));

        // Последовательные битвы
        for (auto& monster : monsters) {
            gameLogger.log("\n=== New Battle ===");
            hero.displayInfo();
            monster->displayInfo();
            std::cout << std::endl;

            battle(hero, *monster);

            if (!hero.isAlive()) {
                gameLogger.log("Hero is dead");
                break;
            }

            if (hero.isAlive()) {
                hero.heal(20);
                gameLogger.log("Hero is haling");
            }
        }

        if (hero.isAlive()) {
            gameLogger.log("\n=== Hero kill all mobs ===");
        }

        gameLogger.log("=== End Game ===");
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}