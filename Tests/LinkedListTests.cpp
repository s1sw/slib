#include "CppUnitTest.h"
#include <slib/LinkedList.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
    TEST_CLASS(LinkedListTests) {
public:

    TEST_METHOD(Basic) {
        slib::LinkedList<int> list;

        list.add(5);
        list.add(15);
        list.add(20);
        list.add(25);

        Assert::AreEqual(5, list[0]->v);
        Assert::AreEqual(25, list[3]->v);
    }
    };
}
