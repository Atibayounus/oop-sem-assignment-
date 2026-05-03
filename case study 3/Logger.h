#include <iostream>
#include <ctime>
using namespace std;

class Logger {
private:
    static Logger* instance;
    Logger(){}
public:
    static Logger* getInstance(){
        if(!instance) instance=new Logger();
        return instance;
    }
    void log(string msg){
        time_t now=time(0);
        string dt=ctime(&now);
        cout<<"["<<dt.substr(0,dt.length()-1)<<"] "<<msg<<endl;
    }
};
Logger* Logger::instance=NULL;
