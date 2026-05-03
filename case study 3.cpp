#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/* SIMPLE TIME STAMP */
string getTime() {
    return "SYSTEM_TIME";
}

/* LOGGER (SINGLETON)  */
class Logger {
    ofstream file;

    Logger() {
        file.open("audit.txt", ios::app);
    }

public:
    static Logger& get() {
        static Logger obj;
        return obj;
    }

    void log(string msg) {
        file << "[" << getTime() << "] " << msg << endl;
    }
};

/* OBSERVER  */
class Book;
class IObserver {
public:
    virtual void update(Book* b, string msg) = 0;
};

/* BOOK  */
class Book {
public:
    string isbn, title, author, edition, category;
    int total, available;
    string reservedBy;

    IObserver* observers[10];
    int obsCount;

    Book() { obsCount = 0; }

    Book(string i, string t, string a, string e, string c, int cpy) {
        isbn = i;
        title = t;
        author = a;
        edition = e;
        category = c;
        total = cpy;
        available = cpy;
        reservedBy = "";
        obsCount = 0;
    }

    bool operator==(const Book& b) {
        return isbn == b.isbn;
    }

    void attach(IObserver* o) {
        if (obsCount < 10)
            observers[obsCount++] = o;
    }

    void notify(string msg) {
        for (int i = 0; i < obsCount; i++)
            observers[i]->update(this, msg);
    }

    void changeAvailability(int delta) {
        int prev = available;
        available += delta;

        if (prev == 0 && available > 0)
            notify("Book available again");
    }

    string toLine() {
        return isbn + "|" + title + "|" + author + "|" + edition + "|" +
               category + "|" + to_string(total) + "|" +
               to_string(available) + "|" + reservedBy;
    }

    static Book fromLine(string line) {
        Book b;
        string p[8];
        int j = 0, k = 0;

        for (int i = 0; i < line.size(); i++) {
            if (line[i] == '|') {
                p[j++] = line.substr(k, i - k);
                k = i + 1;
            }
        }
        p[j] = line.substr(k);

        b.isbn = p[0];
        b.title = p[1];
        b.author = p[2];
        b.edition = p[3];
        b.category = p[4];
        b.total = stoi(p[5]);
        b.available = stoi(p[6]);
        b.reservedBy = p[7];

        return b;
    }
};

/* MEMBER  */
class Member : public IObserver {
protected:
    static int counter;

public:
    string id, name, type;
    string issued[5];
    string issueDate[5];
    int count;

    Member(string n, string t) {
        name = n;
        type = t;
        id = t.substr(0,1) + to_string(++counter);
        count = 0;
    }

    virtual int maxBooks() = 0;
    virtual int fineRate() = 0;

    bool canBorrow() {
        return count < maxBooks();
    }

    void add(string isbn, string date) {
        issued[count] = isbn;
        issueDate[count] = date;
        count++;
    }

    void remove(string isbn) {
        for (int i = 0; i < count; i++) {
            if (issued[i] == isbn) {
                issued[i] = issued[count - 1];
                issueDate[i] = issueDate[count - 1];
                count--;
                return;
            }
        }
    }

    void update(Book* b, string msg) {
        cout << "NOTIFY " << name << ": " << msg << " (" << b->title << ")\n";
    }
};

int Member::counter = 0;

/* MEMBER TYPES  */
class Student : public Member {
public:
    Student(string n) : Member(n, "Student") {}
    int maxBooks() { return 3; }
    int fineRate() { return 5; }
};

class Faculty : public Member {
public:
    Faculty(string n) : Member(n, "Faculty") {}
    int maxBooks() { return 8; }
    int fineRate() { return 0; }
};

class External : public Member {
public:
    External(string n) : Member(n, "External") {}
    int maxBooks() { return 2; }
    int fineRate() { return 5; }
};

/* FACTORY */
class Factory {
public:
    static Member* create(string type, string name) {
        if (type == "Student") return new Student(name);
        if (type == "Faculty") return new Faculty(name);
        if (type == "External") return new External(name);
        return NULL;
    }
};

/*  LOADER  */
class Loader {
public:
    void saveBooks(Book b[], int n) {
        ofstream f("books.txt");
        for (int i = 0; i < n; i++)
            f << b[i].toLine() << endl;
    }

    void loadBooks(Book b[], int &n) {
        ifstream f("books.txt");
        string line;
        n = 0;

        while (getline(f, line)) {
            if (!line.empty())
                b[n++] = Book::fromLine(line);
        }
    }
};

/* LIBRARY  */
class Library {
    Book books[20];
    Member* members[20];

    int bCount, mCount;
    Loader loader;

public:
    Library() {
        bCount = 0;
        mCount = 0;
        loader.loadBooks(books, bCount);
    }

    ~Library() {
        loader.saveBooks(books, bCount);
    }

    Book* findBook(string isbn) {
        for (int i = 0; i < bCount; i++)
            if (books[i].isbn == isbn)
                return &books[i];
        return NULL;
    }

    Member* findMember(string id) {
        for (int i = 0; i < mCount; i++)
            if (members[i]->id == id)
                return members[i];
        return NULL;
    }

    void addBook(Book b) {
        books[bCount++] = b;
        Logger::get().log("Book added " + b.isbn);
    }

    void removeBook(string isbn) {
        for (int i = 0; i < bCount; i++) {
            if (books[i].isbn == isbn) {
                books[i] = books[bCount - 1];
                bCount--;
                Logger::get().log("Book removed " + isbn);
                return;
            }
        }
    }

    void addMember(Member* m) {
        members[mCount++] = m;
        Logger::get().log("Member added " + m->id);
    }

    string today() {
        return "DAY1"; // date system 
    }

    void issue(string mid, string isbn) {
        Member* m = findMember(mid);
        Book* b = findBook(isbn);

        if (m && b) {
            if (m->canBorrow() && b->available > 0) {
                b->available--;
                m->add(isbn, today());
                cout << "Issued\n";
                Logger::get().log("Issued " + isbn);
            } else {
                b->reservedBy = mid;
                b->attach(m);
                cout << "Reserved\n";
            }
        }
    }

    void ret(string mid, string isbn, int daysLate) {
        Member* m = findMember(mid);
        Book* b = findBook(isbn);

        if (m && b) {
            m->remove(isbn);
            b->available++;

            if (daysLate > 0 && m->fineRate() > 0)
                cout << "Fine Rs " << daysLate * m->fineRate() << endl;

            if (b->reservedBy != "") {
                cout << "Notify " << b->reservedBy << endl;
                b->reservedBy = "";
            }

            Logger::get().log("Returned " + isbn);
        }
    }

    void showBooks() {
        for (int i = 0; i < bCount; i++)
            cout << books[i].isbn << " | " << books[i].title << endl;
    }
};

/* MAIN */
int main() {
    Library lib;

    lib.addBook(Book("1","C++","Bjarne","4th","CS",2));
    lib.addBook(Book("2","DSA","CLRS","3rd","CS",1));

    Member* s = Factory::create("Student","Ali");
    Member* f = Factory::create("Faculty","Khan");

    lib.addMember(s);
    lib.addMember(f);

    lib.issue(s->id,"1");
    lib.issue(f->id,"2");

    lib.ret(f->id,"2",10);

    cout << "\nBooks:\n";
    lib.showBooks();

    return 0;
}
