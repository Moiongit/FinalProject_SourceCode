#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

// Strategy Pattern for Bill Calculation
class BillStrategy {
public:
    virtual double calculateBill(int nights, double rate) = 0;
    virtual ~BillStrategy() {}
};

class StandardBill : public BillStrategy {
public:
    double calculateBill(int nights, double rate) override {
        return nights * rate;
    }
};

class DiscountBill : public BillStrategy {
public:
    double calculateBill(int nights, double rate) override {
        if (nights >= 5)
            return nights * rate * 0.9; // 10% discount
        return nights * rate;
    }
};

// Room class
class Room {
private:
    int roomNumber;
    bool available;
    double rate;

public:
    Room(int num, double r) : roomNumber(num), available(true), rate(r) {}

    int getRoomNumber() const { return roomNumber; }
    bool isAvailable() const { return available; }
    double getRate() const { return rate; }

    void book() { available = false; }
    void release() { available = true; }
    void setRate(double r) { rate = r; }
};

// Base User class
class User {
protected:
    string username, password;

public:
    User(string u, string p) : username(u), password(p) {}
    virtual void menu() = 0;

    string getUsername() const { return username; }
    bool checkPassword(const string& p) const { return password == p; }
    virtual ~User() {}
};

class Customer : public User {
public:
    Customer(string u, string p) : User(u, p) {}
    void menu() override {
        cout << "Welcome, " << username << "! (Customer)\n";
    }
};

class Admin : public User {
public:
    Admin(string u, string p) : User(u, p) {}
    void menu() override {
        cout << "Welcome, " << username << "! (Admin)\n";
    }
};

// Reservation class
class Reservation {
private:
    string customerName;
    int roomNumber;
    int nights;
    double totalBill;

public:
    Reservation(string name, int room, int n, double bill)
        : customerName(name), roomNumber(room), nights(n), totalBill(bill) {}

    void show() const {
        cout << "Customer: " << customerName
             << ", Room: " << roomNumber
             << ", Nights: " << nights
             << ", Bill: $" << totalBill << endl;
    }

    int getRoomNumber() const { return roomNumber; }
    string getCustomerName() const { return customerName; }
};

// --- [FEEDBACK FEATURE WITH RATING] ---
struct Feedback {
    string customerName;
    int roomNumber;
    string comment;
    int rating; // New rating field
};

// System class
class HotelSystem {
private:
    vector<Room> rooms;
    vector<User*> users;
    vector<Reservation> reservations;
    vector<Feedback> feedbacks;
    BillStrategy* billStrategy;
    User* currentUser = nullptr;

    Room* findRoom(int num) {
        for (auto& room : rooms)
            if (room.getRoomNumber() == num)
                return &room;
        return nullptr;
    }

    int getInt(const string& prompt) {
        int val;
        cout << prompt;
        while (!(cin >> val)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n" << prompt;
        }
        cin.ignore(1000, '\n');
        return val;
    }

    double getDouble(const string& prompt) {
        double val;
        cout << prompt;
        while (!(cin >> val) || val < 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n" << prompt;
        }
        cin.ignore(1000, '\n');
        return val;
    }

public:
    HotelSystem() {
        rooms.emplace_back(101, 100.0);
        rooms.emplace_back(102, 120.0);
        rooms.emplace_back(103, 150.0);
        users.push_back(new Admin("admin", "admin123"));
        billStrategy = new StandardBill();
    }

    ~HotelSystem() {
        delete billStrategy;
        for (auto user : users)
            delete user;
    }

    // --- [FEEDBACK FEATURE] ---
    void leaveFeedback() {
        cout << "\nLeave Feedback:\n";
        int roomNum = getInt("Enter room number: ");
        bool hasReservation = false;
        for (const auto& res : reservations) {
            if (res.getCustomerName() == currentUser->getUsername() &&
                res.getRoomNumber() == roomNum) {
                hasReservation = true;
                break;
            }
        }
        if (!hasReservation) {
            cout << "You have no reservation for this room.\n";
            return;
        }
        string comment;
        int rating;
        cout << "Enter your feedback: ";
        getline(cin, comment);
        rating = getInt("Enter rating (1-5): ");
        feedbacks.push_back({currentUser->getUsername(), roomNum, comment, rating});
        cout << "Thank you for your feedback!\n";
    }

    void viewFeedback() {
        cout << "\nCustomer Feedback:\n";
        if (feedbacks.empty()) {
            cout << "No feedback available.\n";
            return;
        }
        for (const auto& f : feedbacks) {
            cout << "Room " << f.roomNumber << " | " << f.customerName
                 << ": " << f.comment << " (Rating: " << f.rating << "/5)\n";
        }
    }
};

int main() {
    HotelSystem system;
    // Note: system.mainMenu() would be the main menu, assumed to exist
    return 0;
}
