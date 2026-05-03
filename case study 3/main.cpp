#include "Library.h"
int main(){
    Library lib;
    Member* m=MemberFactory::createMember("student",1,"Ali");
    lib.addBook(Book("OOP","111",3));
    lib.issueBook("111",m);
    lib.returnBook("111",m,5);
}
