#ifndef BOOK_H
#define BOOK_H
#include <iostream>
using namespace std;

class Book {
public:
    string title, isbn;
    int totalCopies;
    int availableCopies;

    Book(string t = "", string i = "", int c = 0) {
        title = t;
        isbn = i;
        totalCopies = c;
        availableCopies = c;
    }

    bool operator==(const Book& b) {
        return isbn == b.isbn;
    }
};
#endif
