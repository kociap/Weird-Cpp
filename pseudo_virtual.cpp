#include <iostream>

class Base;
class Derived;
void _Base_vdtor(Base*);
void _Derived_vdtor(Base*);
void _Base_v_print(Base*);
void _Derived_v_print(Base*);

class Base {
protected:
    void* vfnpt[2];

    friend void _Base_vdtor(Base* b) {
        b->~Base();
    }

    friend void _Base_v_print(Base* b) {
        std::cout << "base\n";
    }

public:
    Base() {
        vfnpt[0] = (void*)&_Base_vdtor;
        vfnpt[1] = (void*)&_Base_v_print;
    }
 
    ~Base() {
        if(vfnpt[0]) {
            void* dtor = vfnpt[0];
            vfnpt[0] = nullptr;
            ((void(*)(Base*))dtor)(this);
            return;
        }
        std::cout << "base dtor\n";
    }

    void print() {
        return ((void(*)(Base*))vfnpt[1])(this);
    }
};

class Derived: public Base {
    friend void _Derived_vdtor(Base* b) {
        ((Derived*)b)->~Derived();
    }

    friend void _Derived_v_print(Base* b) {
        std::cout << "derived\n";
    }

public:
    Derived() {
        vfnpt[0] = (void*)&_Derived_vdtor;
        vfnpt[1] = (void*)&_Derived_v_print;
    }

    ~Derived() {
        std::cout << "derived dtor\n";
    }

    void print() {
        ((void(*)(Base*))vfnpt[1])(this);
    }
};

int main() {
    Base* base = new Derived;
    base->print();
    delete base;
    return 0;
}
