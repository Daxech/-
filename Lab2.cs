#include <iostream>
#include <string>

class Weapon {
private:
    std::string name;
    int damage;
    int weight;

public:
    // Конструктор
    Weapon(const std::string& n, int d, int w)
        : name(n), damage(d), weight(w) {
        std::cout << "Weapon " << name << " created!\n";
    }

    // Деструктор
    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    // Метод для вывода информации
    void displayInfo() const {
        std::cout << "Weapon: " << name << ", Damage: " << damage 
                  << ", Weight: " << weight << std::endl;
    }
};

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Character " << name << " created!\n";
    }

    // Деструктор
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // Деструктор
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

int main() {
    // Создаем объекты оружия
    Weapon sword("Excalibur", 25, 5);
    Weapon bow("Longbow", 15, 3);
    Weapon axe("Battle Axe", 30, 8);

    // Создаем персонажей
    Character hero("Arthur", 100, 20, 15);
    Monster dragon("Smaug", 200, 30, 20);

    // Выводим информацию
    std::cout << "\nWeapon Info:\n";
    sword.displayInfo();
    bow.displayInfo();
    axe.displayInfo();

    std::cout << "\nCharacter Info:\n";
    hero.displayInfo();
    dragon.displayInfo();

    return 0;
}
