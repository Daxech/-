using System;

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Inventory
{
    private:
    std::unique_ptr<std::string[]> items;
    size_t capacity;  
    size_t size;      

    public:
    Inventory(size_t initialCapacity = 10)
        : capacity(initialCapacity), size(0)
    {
        items = std::make_unique < std::string[]> (capacity);
    }

    void addItem(const std::string& item) {
        if (size >= capacity) {
            resize(capacity* 2);
}
items[size++] = item;
    }

    void displayInventory() const {
        std::cout << "Inventory (Capacity: " << capacity << ", Items: " << size << "):\n";
for (size_t i = 0; i < size; ++i)
{
    std::cout << "- " << items[i] << "\n";
}
    }

private:
    void resize(size_t newCapacity)
{
    auto newItems = std::make_unique < std::string[]> (newCapacity);
    for (size_t i = 0; i < size; ++i)
    {
        newItems[i] = items[i];  
    }
    items = std::move(newItems); 
    capacity = newCapacity;
}
};

int main()
{
    Inventory inv;

    inv.addItem("Sword");
    inv.addItem("Health Potion");
    inv.addItem("Bow");
    inv.addItem("Mana Potion");

    inv.displayInventory();

    inv.addItem("Shield");
    inv.addItem("Arrow");
    inv.addItem("Fire Scroll");

    inv.displayInventory();

    return 0;
}
