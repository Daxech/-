#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <typeinfo>
#include <ctime>

// Шаблонный класс Logger для записи логов
template<typename T>
class Logger
{
    private:
    std::ofstream logFile;
    public:
    Logger(const std::string& filename) {
        logFile.open(filename, std::ios::app);
        if (!logFile) {
            throw std::runtime_error("Failed to open log file");
        }
    }

    ~Logger() {
    if (logFile.is_open())
    {
        logFile.close();
    }
}

void log(const T& message)
{
    time_t now = time(0);
    char* dt = ctime(&now);
    logFile << dt << ": " << message << std::endl;
}
};

// Класс предмета
class Item
{
    protected:
    std::string name;
    std::string type;
    public:
    Item(const std::string& n, const std::string& t) : name(n), type(t) { }
    virtual ~Item() = default;
    virtual void use() = 0;
    std::string getName() const { return name; }
std::string getType() const { return type; }
};

// Класс оружия
class Weapon : public Item
{
private:
    int attackBonus;
public:
    Weapon(const std::string& n, int ab) : Item(n, "Weapon"), attackBonus(ab) { }
void use() override
{
    std::cout << "Equipped " << name << " (+" << attackBonus << " attack)" << std::endl;
}
int getAttackBonus() const { return attackBonus; }
};

// Класс зелья
class Potion : public Item
{
private:
    int healAmount;
public:
    Potion(const std::string& n, int ha) : Item(n, "Potion"), healAmount(ha) { }
void use() override
{
    std::cout << "Used " << name << " (heals " << healAmount << " HP)" << std::endl;
}
int getHealAmount() const { return healAmount; }
};

// Класс инвентаря
class Inventory
{
    private:
    std::vector<std::unique_ptr<Item>> items;
    public:
    void addItem(std::unique_ptr<Item> item)
    {
        items.push_back(std::move(item));
    }

    void removeItem(const std::string& itemName) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if ((* it)->getName() == itemName) {
                items.erase(it);
                return;
            }
        }
        throw std::runtime_error("Item not found in inventory");
    }

    void display() const {
        if (items.empty()) {
            std::cout << "Inventory is empty" << std::endl;
return;
        }
        std::cout << "Inventory:" << std::endl;
for (const auto&item : items) {
            std::cout << "- " << item->getName() << " (" << item->getType() << ")" << std::endl;
        }
    }

    Item* getItem(const std::string& itemName)
{
    for (auto & item : items) {
            if (item->getName() == itemName) {
                return item.get();
            }
        }
        return nullptr;
    }
};

// Базовый класс монстра
class Monster
{
    protected:
    std::string name;
    int health;
    int attack;
    int defense;
    public:
    Monster(const std::string& n, int h, int a, int d) 
        : name(n), health(h), attack(a), defense(d) { }
    virtual ~Monster() = default;

    virtual void attackTarget(Character& target) = 0;

    void takeDamage(int damage)
    {
        health -= damage;
        if (health < 0) health = 0;
    }

    bool isAlive() const { return health > 0; }

void displayInfo() const {
        std::cout << name << " [HP: " << health 
                  << ", ATK: " << attack 
                  << ", DEF: " << defense << "]" << std::endl;
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
};

// Классы конкретных монстров
class Goblin : public Monster
{
public:
    Goblin() : Monster("Goblin", 30, 8, 2) { }
void attackTarget(Character& target) override
{
    std::cout << "Goblin attacks with a crude club!" << std::endl;
    target.attackEnemy(*this);
}
};

class Dragon : public Monster
{
public:
    Dragon() : Monster("Dragon", 100, 20, 10) { }
void attackTarget(Character& target) override
{
    std::cout << "Dragon breathes fire!" << std::endl;
    target.attackEnemy(*this);
}
};

class Skeleton : public Monster
{
public:
    Skeleton() : Monster("Skeleton", 40, 10, 5) { }
void attackTarget(Character& target) override
{
    std::cout << "Skeleton attacks with a rusty sword!" << std::endl;
    target.attackEnemy(*this);
}
};

// Класс персонажа (обновленный)
class Character
{
    private:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int level;
    int experience;
    Inventory inventory;
    Logger<std::string> logger;

    public:
    Character(const std::string& n, int h, int a, int d) 
        : name(n), health(h), maxHealth(h), attack(a), defense(d), 
          level(1), experience(0), logger("game_log.txt")
    {
        logger.log("Character " + name + " created");
    }

    void attackEnemy(Monster& enemy)
    {
        int damage = attack - enemy.getDefense();
        if (damage > 0)
        {
            enemy.takeDamage(damage);
            std::string msg = name + " attacks " + enemy.getName() + " for " + std::to_string(damage) + " damage!";
            std::cout << msg << std::endl;
            logger.log(msg);

            if (!enemy.isAlive())
            {
                msg = enemy.getName() + " defeated!";
                std::cout << msg << std::endl;
                logger.log(msg);
                gainExperience(30);
            }
        }
        else
        {
            std::string msg = name + " attacks " + enemy.getName() + ", but it has no effect!";
            std::cout << msg << std::endl;
            logger.log(msg);
        }
    }

    void heal(int amount)
    {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        std::string msg = name + " heals for " + std::to_string(amount) + " HP!";
        std::cout << msg << std::endl;
        logger.log(msg);
    }

    void gainExperience(int exp)
    {
        experience += exp;
        if (experience >= 100)
        {
            level++;
            experience -= 100;
            maxHealth += 20;
            attack += 5;
            defense += 3;
            health = maxHealth;
            std::string msg = name + " leveled up to level " + std::to_string(level) + "!";
            std::cout << msg << std::endl;
            logger.log(msg);
        }
    }

    void useItem(const std::string& itemName) {
        try {
            Item* item = inventory.getItem(itemName);
            if (!item) {
                throw std::runtime_error("Item not found");
            }

if (item->getType() == "Potion")
{
    Potion* potion = dynamic_cast<Potion*>(item);
    heal(potion->getHealAmount());
    inventory.removeItem(itemName);
}
else if (item->getType() == "Weapon")
{
    Weapon* weapon = dynamic_cast<Weapon*>(item);
    attack += weapon->getAttackBonus();
    std::cout << "Attack increased by " << weapon->getAttackBonus() << std::endl;
}
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
logger.log("Error using item: " + std::string(e.what()));
        }
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health << "/" << maxHealth
                  << ", Attack: " << attack << ", Defense: " << defense
                  << ", Level: " << level << ", Experience: " << experience << std::endl;
    }

    void saveToFile(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Failed to open file for saving");
    }

    file << name << "\n" << health << "\n" << maxHealth << "\n"
         << attack << "\n" << defense << "\n"
         << level << "\n" << experience << "\n";

    logger.log("Game saved to " + filename);
}

void loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Failed to open file for loading");
    }

    file >> name >> health >> maxHealth >> attack >> defense >> level >> experience;

    logger.log("Game loaded from " + filename);
}

Inventory & getInventory() { return inventory; }
std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
};

// Класс игры
class Game
{
    private:
    Character player;
    Logger<std::string> logger;
    public:
    Game(const std::string& playerName) : player(playerName, 100, 10, 5), logger("game_log.txt") { }

    void start()
    {
        std::cout << "Game started! Welcome, " << player.getName() << "!" << std::endl;
        logger.log("Game started with player " + player.getName());

        // Добавляем начальные предметы
        player.getInventory().addItem(std::make_unique<Potion>("Health Potion", 30));
        player.getInventory().addItem(std::make_unique<Weapon>("Steel Sword", 5));
    }

    void battle()
    {
        try
        {
            // Создаем случайного монстра
            std::unique_ptr<Monster> monster;
            int choice = rand() % 3;

            switch (choice)
            {
                case 0: monster = std::make_unique<Goblin>(); break;
                case 1: monster = std::make_unique<Dragon>(); break;
                case 2: monster = std::make_unique<Skeleton>(); break;
            }

            std::cout << "A wild " << monster->getName() << " appears!" << std::endl;
            logger.log("Battle started with " + monster->getName());

            while (player.getHealth() > 0 && monster->isAlive())
            {
                // Ход игрока
                std::cout << "\nYour turn:" << std::endl;
                player.displayInfo();
                monster->displayInfo();

                std::cout << "1. Attack\n2. Use item\nChoose action: ";
                int action;
                std::cin >> action;

                if (action == 1)
                {
                    player.attackEnemy(*monster);
                }
                else if (action == 2)
                {
                    player.getInventory().display();
                    std::cout << "Enter item name to use: ";
                    std::string itemName;
                    std::cin >> itemName;
                    player.useItem(itemName);
                }

                // Ход монстра, если он еще жив
                if (monster->isAlive())
                {
                    std::cout << "\nEnemy turn:" << std::endl;
                    monster->attackTarget(player);

                    if (player.getHealth() <= 0)
                    {
                        throw std::runtime_error("Player has been defeated!");
                    }
                }
            }
        }
        catch (const std::exception&e) {
            std::cerr << "Battle error: " << e.what() << std::endl;
            logger.log("Battle error: " + std::string(e.what()));
        }
        }

        void saveGame()
        {
            try
            {
                player.saveToFile("savegame.txt");
                std::cout << "Game saved successfully!" << std::endl;
            }
            catch (const std::exception&e) {
                std::cerr << "Save error: " << e.what() << std::endl;
                logger.log("Save error: " + std::string(e.what()));
            }
            }

            void loadGame()
            {
                try
                {
                    player.loadFromFile("savegame.txt");
                    std::cout << "Game loaded successfully!" << std::endl;
                }
                catch (const std::exception&e) {
                    std::cerr << "Load error: " << e.what() << std::endl;
                    logger.log("Load error: " + std::string(e.what()));
                }
                }
            }
            ;

            int main()
            {
                try
                {
                    Game game("Hero");
                    game.start();

                    while (true)
                    {
                        std::cout << "\nMain Menu:\n1. Battle\n2. Save Game\n3. Load Game\n4. Exit\nChoose option: ";
                        int choice;
                        std::cin >> choice;

                        switch (choice)
                        {
                            case 1: game.battle(); break;
                            case 2: game.saveGame(); break;
                            case 3: game.loadGame(); break;
                            case 4: return 0;
                            default: std::cout << "Invalid choice!" << std::endl;
                        }
                    }
                }
                catch (const std::exception&e) {
                    std::cerr << "Fatal error: " << e.what() << std::endl;
                    return 1;
                }

                return 0;
                }