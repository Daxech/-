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
    if (elements.empty())
    {
        throw std::out_of_range("Queue is empty");
    }
    elements.erase(elements.begin());
}

T & front() {
    if (elements.empty())
    {
        throw std::out_of_range("Queue is empty");
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
    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);

    std::cout << "Int Queue size: " << intQueue.size() << std::endl;
    std::cout << "Front element: " << intQueue.front() << std::endl;
    intQueue.pop();
    std::cout << "Front element after pop: " << intQueue.front() << std::endl;

    Queue < std::string> stringQueue;
    stringQueue.push("Hello");
    stringQueue.push("World");

    std::cout << "\nString Queue size: " << stringQueue.size() << std::endl;
    std::cout << "Front element: " << stringQueue.front() << std::endl;
    stringQueue.pop();
    std::cout << "Front element after pop: " << stringQueue.front() << std::endl;

    return 0;
}
