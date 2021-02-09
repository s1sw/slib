#include "CppUnitTest.h"
#include <slib/StaticAllocList.hpp>
#include <slib/BinarySearch.hpp>
#include <slib/Iterator.hpp>
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
    namespace VisualStudio {
        namespace CppUnitTestFramework {
            template<>
            static std::wstring ToString<slib::StaticAllocList<int>::Iterator>(const slib::StaticAllocList<int>::Iterator& it) { return std::to_wstring(it.getIndex()); }
        }
    }
}

namespace Tests {
    TEST_CLASS(BinarySearchTests) {
private:
    slib::StaticAllocList<int> makeList() {
        return slib::StaticAllocList<int> { 10, { 1, 3, 5, 7, 11, 13, 17, 19, 23, 29 }};
    }

    slib::StaticAllocList<int> genRandomList(int length) {
        slib::StaticAllocList<int> sal(length);
        for (int i = 0; i < length; i++) {
            sal.add(i);
        }

        return sal;
    }
public:

    TEST_METHOD(SimpleSearches) {
        auto list = makeList();

        auto eleven = slib::binarySearch(11, list);
        auto thirteen = slib::binarySearch(13, list);
        Assert::AreEqual(11, *eleven);
        Assert::AreEqual(13, *thirteen);
    }

    TEST_METHOD(RandomisedSearch) {
        auto randList = genRandomList(500000);

        auto targetIt = randList.begin();
        slib::advance(targetIt, 15650);
        int targetVal = *targetIt;

        auto found = slib::binarySearch(targetVal, randList);

        Assert::AreNotEqual(randList.end(), found);
        Assert::AreEqual(targetVal, *found);
    }
    };
}
