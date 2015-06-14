#include <iostream>
#include <functional>
#include <Signals.h>

using namespace std;
using namespace Signals;

class Object {
public:
    Object(int id) {
        this->id = id;
    }

    void getId(int param) {
        std::cout << "Object::getId(" << param << "): id = " << this->id << std::endl;
    }

private:
    int id;
};

int main() {
    Signal<int> signal;
    Object obj1(1), obj2(2);

    signal.connect(Slot<int>(&Object::getId, &obj1, placeholders::_1));
    signal.connect(Slot<int>(&Object::getId, &obj2, placeholders::_1));
    auto handle = signal.connect(Slot<int>(&Object::getId, &obj2, placeholders::_1));
    signal(6);

    signal.disconnect(handle);
    signal(7);

    signal.disconnectAll();
    signal(8);

    return 0;
}
