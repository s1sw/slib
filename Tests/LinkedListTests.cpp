#include "CppUnitTest.h"
#include <slib/LinkedList.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
    TEST_CLASS(LinkedListTests) {
public:

    TEST_METHOD(Basic) {
        slib::LinkedList<int> list;

        list.Add(5);
        list.Add(15);
        list.Add(20);
        list.Add(25);

        Assert::AreEqual(5, list[0]->v);
        Assert::AreEqual(25, list[3]->v);
    }
    };
}
