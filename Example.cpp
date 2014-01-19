#include <iostream>
#include <Signals.h>

class Caller {
public:
    Signals::Signal<> mySignal;
};

class Callee {
public:
    Callee(int id) {
        this->id = id;
    }

    void getId() {
        std::cout << "Callee::mySlot() id = " << this->id << std::endl;
    }

private:
    int id;
};

int main() {
    Caller cr;
    Callee ce1(1), ce2(2);

    cr.mySignal.connect(std::bind(&Callee::getId, &ce1));
    cr.mySignal.connect(std::bind(&Callee::getId, &ce2));
    int handle = cr.mySignal.connect(std::bind(&Callee::getId, &ce2));
    cr.mySignal();

    cr.mySignal.disconnect(handle);
    cr.mySignal();

    cr.mySignal.disconnectAll();
    cr.mySignal();

    return 0;
}
