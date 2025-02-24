#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <iomanip>
#include <stdexcept>

class CosmicObject {
protected:
    std::string name;
public:
    CosmicObject(const std::string& name) : name(name) {}
    virtual void display() const {
        std::cout << "Name: " << name << std::endl;
    }
    virtual ~CosmicObject() = default; // Виртуальный деструктор для корректного удаления производных объектов
};

class Planet : public CosmicObject {
private:
    std::string discoveryDate;
    double radius;
public:
    Planet(const std::string& name, const std::string& discoveryDate, double radius)
        : CosmicObject(name), discoveryDate(discoveryDate), radius(radius) {}
    void display() const override {
        CosmicObject::display();
        std::cout << "Discovery Date: " << discoveryDate << std::endl;
        std::cout << "Radius: " << radius << " km" << std::endl;
    }
};

class Spaceship : public CosmicObject {
private:
    std::string countryOfOrigin;
public:
    Spaceship(const std::string& name, const std::string& countryOfOrigin)
        : CosmicObject(name), countryOfOrigin(countryOfOrigin) {}
    void display() const override {
        CosmicObject::display();
        std::cout << "Country of Origin: " << countryOfOrigin << std::endl;
    }
};

std::string parseQuotedString(std::istringstream& iss) {
    std::string quotedString;
    std::getline(iss, quotedString, '"');
    std::getline(iss, quotedString, '"');
    return quotedString;
}

bool isValidDate(const std::string& date) {
    if (date.size() != 10 || date[4] != '.' || date[7] != '.') {
        return false;
    }
    for (size_t i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) {
            return false;
        }
    }
    return true;
}

std::unique_ptr<CosmicObject> createObject(const std::string& description) {
    std::istringstream iss(description);
    std::string type;
    iss >> type;

    if (type == "Planet") {
        std::string name = parseQuotedString(iss);
        std::string discoveryDate = parseQuotedString(iss);
        if (!isValidDate(discoveryDate)) {
            std::cerr << "Invalid discovery date format. Expected format: yyyy.mm.dd" << std::endl;
            return nullptr;
        }
        double radius;
        iss >> radius;
        if (iss.fail()) {
            std::cerr << "Invalid radius value." << std::endl;
            return nullptr;
        }
        return std::make_unique<Planet>(name, discoveryDate, radius);
    }
    else if (type == "Spaceship") {
        std::string name = parseQuotedString(iss);
        std::string countryOfOrigin = parseQuotedString(iss);
        return std::make_unique<Spaceship>(name, countryOfOrigin);
    }

    std::cerr << "Unknown object type: " << type << std::endl;
    return nullptr;
}

int main() {
    std::string input;
    std::cout << "Enter object description: ";
    std::getline(std::cin, input);
    std::unique_ptr<CosmicObject> obj = createObject(input);
    if (obj) {
        obj->display();
    }
    else {
        std::cerr << "Invalid object description." << std::endl;
    }
    return 0;
}