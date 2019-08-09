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

#ifndef SIGNALS_H
#define SIGNALS_H

#include <functional>
#include <list>

namespace Signals {

template<typename... Types>
class Slot {
public:
    template<typename F, typename... Args>
    Slot(F&& f, Args&&... args):
            callable(std::bind(f, args...)) {
    }

    void operator ()(Types... args) {
        this->callable(std::forward<Types>(args)...);
    }

private:
    std::function<void(Types...)> callable;
};

template<typename... Types>
class Signal {
public:
    using SlotType = Slot<Types...>;
    using SlotIterator = typename std::list<SlotType>::iterator;

    class SlotHandle {
    private:
        SlotHandle() = delete;
        SlotHandle(const SlotIterator& iterator):
                iterator(iterator) {
        }

        friend class Signal;
        SlotIterator iterator;
    };

    void operator ()(Types... args) {
        for (auto& slot: this->slots) {
            slot(std::forward<Types>(args)...);
        }
    }

    SlotHandle connect(const SlotType& slot) {
        this->slots.push_back(slot);
        return SlotHandle(--this->slots.end());
    }

    void disconnect(const SlotHandle& handle) {
        this->slots.erase(handle.iterator);
    }

    void disconnectAll() {
        this->slots.clear();
    }

private:
    std::list<SlotType> slots;
};

}  // namespace Signals

#endif  // SIGNALS_H
