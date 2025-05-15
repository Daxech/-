#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <string>

using namespace std;

// Исключение для некорректного уровня доступа
class InvalidAccessLevelException : public exception {
public:
    const char* what() const noexcept override {
        return "Invalid access level! Must be between 1 and 3.";
    }
};

// Исключение для пустого имени
class EmptyNameException : public exception {
public:
    const char* what() const noexcept override {
        return "Name cannot be empty!";
    }
};

class User {
protected:
    string name;
    int id;
    int accessLevel; // 1 - student, 2 - teacher, 3 - admin

    void validate() const {
        if (accessLevel < 1 || accessLevel > 3) {
            throw InvalidAccessLevelException();
        }
        if (name.empty()) {
            throw EmptyNameException();
        }
    }

public:
    User(const string& name, int id, int accessLevel)
        : name(name), id(id), accessLevel(accessLevel) {
        validate();
    }

    virtual ~User() {}

    // Геттеры
    string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    // Сеттеры
    void setName(const string& newName) {
        if (newName.empty()) throw EmptyNameException();
        name = newName;
    }
    void setId(int newId) { id = newId; }
    void setAccessLevel(int level) {
        if (level < 1 || level > 3) throw InvalidAccessLevelException();
        accessLevel = level;
    }

    virtual void displayInfo() const {
        cout << "ID: " << id << ", Name: " << name
            << ", Access Level: " << accessLevel;
    }

    virtual string getType() const = 0;
};

class Student : public User {
private:
    string group;

public:
    Student(const string& name, int id, const string& group)
        : User(name, id, 1), group(group) {
    }

    string getGroup() const { return group; }
    void setGroup(const string& newGroup) { group = newGroup; }

    void displayInfo() const override {
        User::displayInfo();
        cout << ", Type: Student, Group: " << group << endl;
    }

    string getType() const override { return "Student"; }
};

class Teacher : public User {
private:
    string department;

public:
    Teacher(const string& name, int id, const string& department)
        : User(name, id, 2), department(department) {
    }

    string getDepartment() const { return department; }
    void setDepartment(const string& newDept) { department = newDept; }

    void displayInfo() const override {
        User::displayInfo();
        cout << ", Type: Teacher, Department: " << department << endl;
    }

    string getType() const override { return "Teacher"; }
};

class Administrator : public User {
private:
    string position;

public:
    Administrator(const string& name, int id, const string& position)
        : User(name, id, 3), position(position) {
    }

    string getPosition() const { return position; }
    void setPosition(const string& newPos) { position = newPos; }

    void displayInfo() const override {
        User::displayInfo();
        cout << ", Type: Administrator, Position: " << position << endl;
    }

    string getType() const override { return "Administrator"; }
};

class Resource {
private:
    string name;
    int requiredAccessLevel;

public:
    Resource(const string& name, int requiredAccessLevel)
        : name(name), requiredAccessLevel(requiredAccessLevel) {
        if (requiredAccessLevel < 1 || requiredAccessLevel > 3) {
            throw InvalidAccessLevelException();
        }
    }

    string getName() const { return name; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    void displayInfo() const {
        cout << "Resource: " << name << ", Required Access Level: "
            << requiredAccessLevel << endl;
    }
};

template<typename T>
class AccessControlSystem {
private:
    vector<unique_ptr<User>> users;
    vector<T> resources;

public:
    void addUser(unique_ptr<User> user) {
        users.push_back(move(user));
    }

    void addResource(const T& resource) {
        resources.push_back(resource);
    }

    bool checkAccess(int userId, const string& resourceName) const {
        auto userIt = find_if(users.begin(), users.end(),
            [userId](const unique_ptr<User>& u) { return u->getId() == userId; });

        auto resIt = find_if(resources.begin(), resources.end(),
            [resourceName](const T& r) { return r.getName() == resourceName; });

        if (userIt == users.end() || resIt == resources.end()) {
            return false;
        }

        return resIt->checkAccess(**userIt);
    }

    void displayAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    void displayAllResources() const {
        for (const auto& resource : resources) {
            resource.displayInfo();
        }
    }

    User* findUserById(int id) const {
        auto it = find_if(users.begin(), users.end(),
            [id](const unique_ptr<User>& u) { return u->getId() == id; });
        return it != users.end() ? it->get() : nullptr;
    }

    User* findUserByName(const string& name) const {
        auto it = find_if(users.begin(), users.end(),
            [name](const unique_ptr<User>& u) { return u->getName() == name; });
        return it != users.end() ? it->get() : nullptr;
    }

    void sortUsersByAccessLevel() {
        sort(users.begin(), users.end(),
            [](const unique_ptr<User>& a, const unique_ptr<User>& b) {
                return a->getAccessLevel() < b->getAccessLevel();
            });
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (!file) {
            throw runtime_error("Cannot open file for writing!");
        }

        for (const auto& user : users) {
            file << user->getType() << ","
                << user->getName() << ","
                << user->getId() << ","
                << user->getAccessLevel();

            if (auto s = dynamic_cast<Student*>(user.get())) {
                file << "," << s->getGroup();
            }
            else if (auto t = dynamic_cast<Teacher*>(user.get())) {
                file << "," << t->getDepartment();
            }
            else if (auto a = dynamic_cast<Administrator*>(user.get())) {
                file << "," << a->getPosition();
            }

            file << "\n";
        }

        file << "RESOURCES\n";
        for (const auto& resource : resources) {
            file << resource.getName() << ","
                << resource.getRequiredAccessLevel() << "\n";
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            throw runtime_error("Cannot open file for reading!");
        }

        users.clear();
        resources.clear();

        string line;
        bool readingResources = false;

        while (getline(file, line)) {
            if (line.empty()) continue;
            if (line == "RESOURCES") {
                readingResources = true;
                continue;
            }

            if (!readingResources) {
                // Чтение пользователей
                size_t pos = 0;
                string token;
                vector<string> tokens;

                while ((pos = line.find(',')) != string::npos) {
                    token = line.substr(0, pos);
                    tokens.push_back(token);
                    line.erase(0, pos + 1);
                }
                tokens.push_back(line);

                if (tokens.size() < 4) continue;

                string type = tokens[0];
                string name = tokens[1];
                int id = stoi(tokens[2]);
                int accessLevel = stoi(tokens[3]);

                if (type == "Student" && tokens.size() > 4) {
                    users.push_back(make_unique<Student>(name, id, tokens[4]));
                }
                else if (type == "Teacher" && tokens.size() > 4) {
                    users.push_back(make_unique<Teacher>(name, id, tokens[4]));
                }
                else if (type == "Administrator" && tokens.size() > 4) {
                    users.push_back(make_unique<Administrator>(name, id, tokens[4]));
                }
            }
            else {
                // Чтение ресурсов
                size_t pos = line.find(',');
                if (pos == string::npos) continue;

                string name = line.substr(0, pos);
                int requiredLevel = stoi(line.substr(pos + 1));
                resources.emplace_back(name, requiredLevel);
            }
        }
    }
};

int main() {
    try {
        AccessControlSystem<Resource> system;

        // Добавление пользователей
        system.addUser(make_unique<Student>("Ivan Petrov", 1, "CS-101"));
        system.addUser(make_unique<Teacher>("Sergey Smirnov", 2, "Computer Science"));
        system.addUser(make_unique<Administrator>("Olga Ivanova", 3, "Head of Department"));

        // Добавление ресурсов
        system.addResource(Resource("Lecture Hall 101", 1));
        system.addResource(Resource("Computer Lab 205", 2));
        system.addResource(Resource("Server Room", 3));

        // Демонстрация полиморфизма
        cout << "All users:\n";
        system.displayAllUsers();

        cout << "\nAll resources:\n";
        system.displayAllResources();

        // Проверка доступа
        cout << "\nAccess checks:\n";
        cout << "Student access to Lecture Hall 101: "
            << (system.checkAccess(1, "Lecture Hall 101") ? "Granted" : "Denied") << endl;
        cout << "Student access to Server Room: "
            << (system.checkAccess(1, "Server Room") ? "Granted" : "Denied") << endl;
        cout << "Admin access to Server Room: "
            << (system.checkAccess(3, "Server Room") ? "Granted" : "Denied") << endl;

        // Поиск пользователя
        cout << "\nSearch results:\n";
        if (auto user = system.findUserByName("Sergey Smirnov")) {
            cout << "Found user: ";
            user->displayInfo();
        }

        // Сортировка пользователей
        system.sortUsersByAccessLevel();
        cout << "\n\nUsers sorted by access level:\n";
        system.displayAllUsers();

        // Работа с файлами
        system.saveToFile("university_access.txt");

        AccessControlSystem<Resource> newSystem;
        newSystem.loadFromFile("university_access.txt");
        cout << "\nLoaded from file:\n";
        newSystem.displayAllUsers();
        newSystem.displayAllResources();

    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}