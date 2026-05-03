#include "Member.h"
class ExternalMember : public Member {
public:
    ExternalMember(int i, string n) : Member(i,n) {}
    int getMaxBooks(){ return 2; }
    int getDaysLimit(){ return 7; }
};
