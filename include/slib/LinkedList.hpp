#pragma once

namespace slib {
    template <typename V>
    struct LinkedListElement {
        LinkedListElement() : next(nullptr), prev(nullptr) {

        }
        V v;
        LinkedListElement* next;
        LinkedListElement* prev;
    };

    template <typename V>
    class LinkedList {
    public:
        using Element = LinkedListElement<V>;

        Element* Add(const V& el) {
            Element* last = head;

            if (head != nullptr) {
                while (last->next != nullptr) {
                    last = last->next;
                }
            }

            Element* newEl = new Element;
            newEl->v = el;
            newEl->prev = last;

            if (last != nullptr) {
                last->next = newEl;
            } else {
                head = newEl;
            }

            return newEl;
        }

        void Remove(size_t idx) {
            Element* toRemove = head;
            for (size_t i = 0; i < idx; i++) {
                toRemove = toRemove->next;
            }

            if (toRemove->next) {
                toRemove->prev->next = toRemove->next;
                toRemove->next->prev = toRemove->prev;
            } else {
                toRemove->prev->next = nullptr;
            }

            delete toRemove;
        }

        void Remove(Element* toRemove) {
            if (toRemove->next) {

                if (toRemove->prev == nullptr) {
                    // head of list
                    head = toRemove->next;
                    head->prev = nullptr;
                } else {
                    toRemove->prev->next = toRemove->next;
                    toRemove->next->prev = toRemove->prev;
                }
            } else {
                toRemove->prev->next = nullptr;
            }

            delete toRemove;
        }

        Element* operator[](size_t idx) {
            Element* el = head;
            for (size_t i = 0; i < idx; i++) {
                el = el->next;
            }

            return el;
        }

        size_t GetIndexFor(Element* elPtr) {
            size_t counter = 0;

            Element* next = head;
            while (next != nullptr) {
                if (next == elPtr)
                    break;
                next = next->next;
                counter++;
            }

            return counter;
        }

        ~LinkedList() {
            Element* next = head;
            while (next != nullptr) {
                Element* curr = next;
                next = next->next;
                delete curr;
            }
        }

        Element* First() {
            return head;
        }
    private:
        Element* head;
    };
}