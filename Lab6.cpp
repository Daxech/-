using System;

#include <iostream>
#include <vector>
#include <stdexcept>

template<typename T>
class Queue
{
    private:
    std::vector<T> elements;

    public:
    void push(const T& item) {
        elements.push_back(item);
    }

void pop()
{
    if (empty())
    {
        throw std::out_of_range("Queue is empty - cannot pop!");
    }
    elements.erase(elements.begin());
}

T & front() {
    if (empty())
    {
        throw std::out_of_range("Queue is empty - no front element!");
    }
    return elements.front();
}

bool empty() const {
        return elements.empty();
    }

    size_t size() const {
        return elements.size();
    }
};

int main()
{
    Queue<int> queue;

    try
    {
        queue.pop(); 
    }
    catch (const std::out_of_range&e) {
        std::cerr << "[Error] " << e.what() << std::endl;
    }

    try
    {
        std::cout << queue.front() << std::endl; 
    }
    catch (const std::out_of_range&e) {
        std::cerr << "[Error] " << e.what() << std::endl;
    }

    try
    {
        queue.push(10);
        queue.push(20);
        std::cout << "Front element: " << queue.front() << std::endl; 
        queue.pop();
        std::cout << "Front after pop: " << queue.front() << std::endl; 
    }
    catch (const std::exception&e) {
        std::cerr << "[Unexpected Error] " << e.what() << std::endl;
    }

    return 0;
    }
