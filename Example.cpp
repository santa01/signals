/*
 * Copyright (c) 2013 Pavlo Lavrenenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <iostream>
#include <Signals.h>

class Object {
public:
    Object(int id) {
        this->id = id;
    }

    void getId(int param) {
        std::cout << "Object::getId(" << param << "): this->id = " << this->id << "\n";
    }

    void updateId(int&& newId, int param, const int& constRef) {
        this->id = newId;
        std::cout << "Object::updateId(" << param << ", " << constRef << "): this->id = " << this->id << "\n";
    }

private:
    int id;
};

int main(int /*argc*/, char** /*argv*/) {
    Signals::Signal<int> signal;
    Signals::Signal<int&&, int, const int&> signalRvalue;
    Object obj1(1), obj2(2), obj3(3);

    auto handle = signal.connect(Signals::Slot<int>(&Object::getId, &obj1, std::placeholders::_1));
    signal.connect(Signals::Slot<int>(&Object::getId, &obj2, std::placeholders::_1));
    signal.connect(Signals::Slot<int>(&Object::getId, &obj3, std::placeholders::_1));
    signalRvalue.connect(Signals::Slot<int&&, int, const int&>(&Object::updateId, &obj3, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    int param = 6;
    signal(param);

    signal.disconnect(handle);
    signal(7);

    signal.disconnectAll();
    signal(8);

    const int constParam = 3;
    signalRvalue(5, 4, constParam);

    return 0;
}
