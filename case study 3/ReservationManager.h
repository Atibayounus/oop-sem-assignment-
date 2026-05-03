#include <queue>
#include <map>
#include "Member.h"
using namespace std;

class ReservationManager{
    map<string, queue<Member*> > waitlist;
public:
    void reserveBook(string isbn, Member* m){
        waitlist[isbn].push(m);
    }
    void notifyNext(string isbn){
        if(!waitlist[isbn].empty()){
            Member* m=waitlist[isbn].front();
            waitlist[isbn].pop();
            m->update("Book available: "+isbn);
        }
    }
    bool isReserved(string isbn){
        return !waitlist[isbn].empty();
    }
};
