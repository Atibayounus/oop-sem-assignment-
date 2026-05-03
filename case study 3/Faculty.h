#include "Member.h"
class Faculty : public Member {
public:
    Faculty(int i, string n) : Member(i,n) {}
    int getMaxBooks(){ return 8; }
    int getDaysLimit(){ return 30; }
};
