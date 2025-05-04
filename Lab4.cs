using System;
#include <iostream>
#include <string>

class Weapon
{
    private:
    std::string name;
    int damage;

    public:
    Weapon(const std::string& n, int d) : name(n), damage(d) { }

    Weapon operator +(const Weapon& other) const {
        std::string newName = name + " + " + other.name;
    int newDamage = damage + other.damage;
        return Weapon(newName, newDamage);
    }

    bool operator >(const Weapon& other) const {
        return damage > other.damage;
    }

    friend std::ostream & operator <<(std::ostream& os, const Weapon& weapon)
{
    os << "Weapon: " << weapon.name << ", Damage: " << weapon.damage;
    return os;
}
};

int main()
{
    Weapon sword("Sword", 30);
    Weapon bow("Bow", 25);
    Weapon axe("Axe", 40);

    Weapon combined = sword + bow;
    std::cout << combined << std::endl;  

    std::cout << "Is sword stronger than bow? " << (sword > bow ? "Yes" : "No") << std::endl;  
    std::cout << "Is bow stronger than axe? " << (bow > axe ? "Yes" : "No") << std::endl;    

    Weapon superWeapon = sword + bow + axe;
    std::cout << superWeapon << std::endl;  

    return 0;
}