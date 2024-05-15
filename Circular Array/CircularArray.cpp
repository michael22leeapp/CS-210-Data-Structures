// STUDENT ORGANIC WAGYU CODE
// Author: Michael Lee
// San Diego State University
// CS210/CS496: Data Structures
//
// File: CircularArray.h
// Defines the CircularArray collection class.

#define VERSION_CIRARRAY 0.1

// STUDENT CODE
#ifndef PROG1_CIRCULARARRAY_H
#define PROG1_CIRCULARARRAY_H
#define INITIAL_CAPACITY 10

#include "Queue.h"
#include "List.h"

namespace sdsu {
    template<typename T>
    class CircularArray : public sdsu::Queue<T>, public sdsu::List<T> {

    private:
        // Size represents how many T items the structure holds
        int curSize;

        // Capacity represents how many T items the structure CAN hold
        int curCapacity;

        //head and tail counters for the circular nature
        int headCount;
        int tailCount;

        // the array on the heap
        T *storage;

        bool isFull() {
            if (curSize == curCapacity) {
                return resize(true);
            } else{
                return false;
            }
        }

        void isHalf() {
            if (curSize == curCapacity / 2) {
                resize(false);
            }
        }

        bool resize(bool change) {
            T *temp = storage;
            int newCapacity;
            if (change){
                newCapacity = curCapacity << 1;
            } else{
                newCapacity = curCapacity >> 1;
            }
            storage = new T[newCapacity];
            for (int i = 0; i < curSize; i++) {
                storage[i] = temp[(i + headCount) % curCapacity];
            }
            tailCount = curSize - 1;
            headCount = 0;
            curCapacity = newCapacity;
            delete[] temp;
            return false;
        }

    public:
        // This is the default class constructor. It sets the current size to 0
        CircularArray() : curSize(0){
            //initialize a default capacity storage array on the heap
            curCapacity = INITIAL_CAPACITY;
            storage = new T[curCapacity];
            headCount = 0;
            tailCount = 0;
        }

        // The copy constructor!
        CircularArray(const CircularArray<T> &other) : CircularArray() {
            std::cout << "Copying . . .";
            curSize = other.curSize;
            curCapacity = other.curCapacity;
            storage = new T[other.curCapacity];
            headCount = other.headCount;
            tailCount = other.tailCount;
            for (int i = headCount; i < headCount + curSize; i++) {
                storage[i % curCapacity] = other.storage[i % curCapacity];
            }
        }

        ~CircularArray() override {
            delete[] storage;
        }

        void clear() override {
            // size == 0, and the capacity and dynamic array should
            // shrink back to its default size. There is a potential
            // for a memory leak here.
            delete[] storage;
            curSize = 0;
            curCapacity = INITIAL_CAPACITY;
            storage = new T[curCapacity];
            headCount = 0;
            tailCount = 0;
        }

        T dequeue() override {
            int *ptr = nullptr;
            if (isEmpty()){
                return *ptr;
            }
            return removeFirst();
        }

        bool enqueue(T t) override {
            return addLast(t);
        }

        bool isEmpty() const override {
            return size() == 0;
        }

        T& peek() override {
            return get(0);
        }

        bool addFirst(T t) override {
            if (isFull() == false) {
                //Check if first element
                if (headCount != 0 || tailCount != 0 || curSize != 0){
                    headCount = (headCount - 1 + curCapacity) % curCapacity;
                }
                storage[headCount] = t;
                curSize++;
                return true;
            } else {
                return false;
            }
        }

        bool addLast(T t) override {
            if (isFull() == false) {
                if (headCount != 0 || tailCount != 0 || curSize != 0){
                    tailCount = ((tailCount + 1) % curCapacity);
                }
                storage[tailCount] = t;
                curSize++;
                return true;
            } else {
                return false;
            }
        }

        T& get(int idx) override {
            if (idx < 0 || idx > curSize - 1) {
                throw std::out_of_range("CircularArray<T>::get(idx) : index out of range.");
            }
            return storage[(idx + headCount ) % curCapacity];
        }

        bool insert(int idx, T t) override {
            // if you figure out if you are closer to the head or tail, you can
            // minimize the amount of array manipulation you need to perform.
            if (idx < 0 || idx > curSize - 1) {
                throw std::out_of_range("CircularArray<T>::insert(idx,value) : index out of range.");
            }
            //idx closer to tail or in the middle
            if (idx >= curSize / 2) {
                for (int i = tailCount; i > tailCount - curSize + idx; i--) {
                    storage[(i + 1 + curCapacity) % curCapacity] = storage[(i + curCapacity) % curCapacity];
                }
                tailCount = (tailCount + 1) % curCapacity;

            } else { //idx closer to the head
                for (int i = headCount; i < idx + headCount + 1; i++) {
                    storage[(i - 1 + curCapacity) % curCapacity] = storage[i % curCapacity];
                }
                headCount = (headCount - 1 + curCapacity) % curCapacity;
            }
            storage[(idx + headCount) % curCapacity] = t;
            curSize++;
            return true;
        }

        T remove(int i) override {
            // Figure out if the target index is closer to the front or back
            // and then shuffle from that index (tail or head).
            if (i < 0 || i > curSize - 1)
                throw std::out_of_range("CircularArray<T>::remove(idx,value) : index out of range. ");

            int removedElem = storage[(i + headCount) % curCapacity];
            //idx closer to tail or in the middle
            if (i >= curSize / 2) {
                if(i != curSize - 1){
                    //Shuffle elements if index is not the tail
                    for (int i = tailCount; i > tailCount - curSize + i; i--) {
                        storage[(i - 1 + curCapacity) % curCapacity] = storage[i % curCapacity];
                    }
                }
                tailCount = (tailCount - 1 + curCapacity) % curCapacity;

            } else { //idx closer to the head
                if (i != 0){
                    //Shuffle elements if index is not the head
                    for (int i = headCount; i < i + headCount + 1; i++) {
                        storage[(i + 1) % curCapacity] = storage[i % curCapacity];
                    }
                }
                headCount = (headCount + 1) % curCapacity;
            }
            curSize--;
            isHalf();
            return removedElem;
        }

        T removeFirst() override {
            if (isEmpty() == false) {
                return remove(0);
            }
            return false;
        }

        T removeLast() override {
            if (isEmpty() == false) {
                return remove(curSize - 1);
            }
            return false;
        }

        void set(int idx, T value) override {
            if (idx < 0 || idx > curSize - 1) {
                throw std::out_of_range("CircularArray<T>::set(idx,value) : index out of range.");
            }
            storage[(idx + headCount) % curCapacity] = value;
        }

        int size() const override {
            return curSize;
        }
    };
}

#endif //PROG1_CIRCULARARRAY_H