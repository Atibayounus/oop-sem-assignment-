#include "Student.h"
#include "Faculty.h"
#include "ExternalMember.h"

class MemberFactory {
public:
    static Member* createMember(string type,int id,string name){
        if(type=="student") return new Student(id,name);
        if(type=="faculty") return new Faculty(id,name);
        if(type=="external") return new ExternalMember(id,name);
        return NULL;
    }
};
