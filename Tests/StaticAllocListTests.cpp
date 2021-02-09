#include "CppUnitTest.h"
#include <slib/StaticAllocList.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
    TEST_CLASS(StaticAllocListTests) {
private:
    slib::StaticAllocList<int> makeList() {
        slib::StaticAllocList<int> l(3);
        l.add(1);
        l.add(5);
        l.add(7);

        return l;
    }
public:

    TEST_METHOD(Basic) {
        slib::StaticAllocList<int> intList(3);

        intList.add(1);
        intList.add(2);
        intList.add(3);

        Assert::AreEqual(intList[0], 1);
        Assert::AreEqual(intList[1], 2);
        Assert::AreEqual(intList[2], 3);
        Assert::AreEqual(intList.numElements(), (size_t)3);

        intList.removeAt(1);
        Assert::AreEqual(intList[1], 3);
        Assert::AreEqual(intList.numElements(), (size_t)2);
    }

    TEST_METHOD(FunctionPassing) {
        auto list2 = makeList();

        Assert::AreEqual((size_t)3, list2.numElements());
        Assert::AreEqual(list2[1], 5);
    }

    TEST_METHOD(Iterators) {
        auto list = makeList();

        int expectedVals[3] = { 1, 5, 7 };
        int i = 0;

        for (int j : list) {
            Assert::AreEqual(j, expectedVals[i]);
            i++;
        }
    }
    };
}
