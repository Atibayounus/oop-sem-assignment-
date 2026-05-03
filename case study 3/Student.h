#include "Member.h"
class Student : public Member {
public:
    Student(int i, string n) : Member(i,n) {}
    int getMaxBooks(){ return 3; }
    int getDaysLimit(){ return 14; }
};
