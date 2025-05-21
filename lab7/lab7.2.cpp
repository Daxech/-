#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <random>

using namespace std;

// Класс персонажа
class Character
{
    private:
    string name;
    int health;
    int attack;
    int defense;
    mutex mtx;

    public:
    Character(const string& name, int health, int attack, int defense)
        : name(name), health(health), attack(attack), defense(defense) { }

    void takeDamage(int damage)
    {
        lock_guard < mutex > lock (mtx) ;
        int actualDamage = max(1, damage - defense);
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    bool isAlive()
    {
        lock_guard < mutex > lock (mtx) ;
        return health > 0;
    }

    int getAttack()
    {
        lock_guard < mutex > lock (mtx) ;
        return attack;
    }

    void displayInfo()
    {
        lock_guard < mutex > lock (mtx) ;
        cout << name << " [HP: " << health << ", ATK: " << attack << ", DEF: " << defense << "]" << endl;
    }

    string getName()
    {
        lock_guard < mutex > lock (mtx) ;
        return name;
    }
};

// Класс монстра
class Monster
{
    private:
    string name;
    int health;
    int attack;
    int defense;
    mutex mtx;

    public:
    Monster(const string& name, int health, int attack, int defense)
        : name(name), health(health), attack(attack), defense(defense) { }

    void takeDamage(int damage)
    {
        lock_guard < mutex > lock (mtx) ;
        int actualDamage = max(1, damage - defense);
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    bool isAlive()
    {
        lock_guard < mutex > lock (mtx) ;
        return health > 0;
    }

    int getAttack()
    {
        lock_guard < mutex > lock (mtx) ;
        return attack;
    }

    void displayInfo()
    {
        lock_guard < mutex > lock (mtx) ;
        cout << name << " [HP: " << health << ", ATK: " << attack << ", DEF: " << defense << "]" << endl;
    }

    string getName()
    {
        lock_guard < mutex > lock (mtx) ;
        return name;
    }
};

vector<Monster> monsters;
mutex monstersMutex;

// Функция для генерации монстров
void generateMonsters()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> healthDist(30, 70);
    uniform_int_distribution<> attackDist(5, 20);
    uniform_int_distribution<> defenseDist(0, 10);

    while (true)
    {
        this_thread::sleep_for(chrono::seconds(3));

        lock_guard < mutex > lock (monstersMutex) ;
        monsters.push_back(Monster("Goblin", healthDist(gen), attackDist(gen), defenseDist(gen)));
        cout << "New monster generated!" << endl;
    }
}

// Функция для боя между персонажем и монстром
void fight(Character& hero, Monster& monster)
{
    while (hero.isAlive() && monster.isAlive())
    {
        // Герой атакует монстра
        monster.takeDamage(hero.getAttack());
        cout << hero.getName() << " attacks " << monster.getName() << "!" << endl;

        if (!monster.isAlive())
        {
            cout << monster.getName() << " defeated!" << endl;
            break;
        }

        // Монстр атакует героя
        hero.takeDamage(monster.getAttack());
        cout << monster.getName() << " attacks " << hero.getName() << "!" << endl;

        if (!hero.isAlive())
        {
            cout << hero.getName() << " has been defeated!" << endl;
            break;
        }

        this_thread::sleep_for(chrono::seconds(1));
    }
}

// Функция для обработки боев
void battleSystem(Character& hero)
{
    while (hero.isAlive())
    {
        Monster* currentMonster = nullptr;

        {
            lock_guard < mutex > lock (monstersMutex) ;
            if (!monsters.empty())
            {
                currentMonster = &monsters.back();
                monsters.pop_back();
                cout << "Starting battle with " << currentMonster->getName() << "!" << endl;
            }
        }

        if (currentMonster)
        {
            fight(hero, *currentMonster);
        }

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main()
{
    // Создаем героя
    Character hero("Hero", 100, 20, 10);

    // Запускаем генератор монстров в отдельном потоке
    thread generator(generateMonsters);
    generator.detach();

    // Запускаем систему боев в отдельном потоке
    thread battleThread(battleSystem, ref (hero));

    // Главный цикл для отображения информации
    while (hero.isAlive())
    {
        system("clear"); 

        cout << "=== Hero Status ===" << endl;
        hero.displayInfo();

        {
            lock_guard < mutex > lock (monstersMutex) ;
            cout << "\n=== Monsters in queue ===" << endl;
            if (monsters.empty())
            {
                cout << "No monsters waiting..." << endl;
            }
            else
            {
                for (auto & monster : monsters)
                {
                    monster.displayInfo();
                }
            }
        }

        this_thread::sleep_for(chrono::seconds(1));
    }

    battleThread.join();
    cout << "Game Over!" << endl;

    return 0;
}
