#include <vector>
#include "Book.h"
#include "MemberFactory.h"
#include "Logger.h"
#include "ReservationManager.h"
using namespace std;

class Library{
    vector<Book> books;
    ReservationManager rm;
public:
    void addBook(Book b){
        books.push_back(b);
    }

    void issueBook(string isbn, Member* m){
        for(auto &b:books){
            if(b.isbn==isbn){
                if(b.availableCopies==0){
                    rm.reserveBook(isbn,m);
                    cout<<"Reserved\n"; return;
                }
                b.availableCopies--;
                Logger::getInstance()->log("Issued "+b.title);
                return;
            }
        }
    }

    void returnBook(string isbn, Member* m,int daysLate){
        for(auto &b:books){
            if(b.isbn==isbn){
                b.availableCopies++;
                Logger::getInstance()->log("Returned "+b.title);

                if(rm.isReserved(isbn)) rm.notifyNext(isbn);

                if(dynamic_cast<Faculty*>(m)){
                    if(daysLate>30) cout<<"Notify HOD\n";
                } else {
                    if(daysLate>0) cout<<"Fine: "<<daysLate*5<<endl;
                }
                return;
            }
        }
    }
};
