#include "CppUnitTest.h"
#include <slib/StaticAllocList.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
    TEST_CLASS(StaticAllocListTests) {
private:
    slib::StaticAllocList<int> makeList() {
        slib::StaticAllocList<int> l(3);
        l.Add(1);
        l.Add(5);
        l.Add(7);

        return l;
    }
public:

    TEST_METHOD(Basic) {
        slib::StaticAllocList<int> intList(3);

        intList.Add(1);
        intList.Add(2);
        intList.Add(3);

        Assert::AreEqual(intList[0], 1);
        Assert::AreEqual(intList[1], 2);
        Assert::AreEqual(intList[2], 3);
        Assert::AreEqual(intList.NumElements(), (size_t)3);

        intList.RemoveAt(1);
        Assert::AreEqual(intList[1], 3);
        Assert::AreEqual(intList.NumElements(), (size_t)2);
    }

    TEST_METHOD(FunctionPassing) {
        auto list2 = makeList();

        Assert::AreEqual((size_t)3, list2.NumElements());
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
