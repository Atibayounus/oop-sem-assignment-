#ifndef MEMBER_H
#define MEMBER_H
#include <iostream>
using namespace std;

class Member {
protected:
    int id;
    string name;

public:
    Member(int i, string n) { id = i; name = n; }

    virtual int getMaxBooks() = 0;
    virtual int getDaysLimit() = 0;

    virtual void update(string msg) {
        cout << "Notification for " << name << ": " << msg << endl;
    }

    int getId() { return id; }
    string getName() { return name; }
};
#endif
