#include "CppUnitTest.h"
#include <slib/List.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
    TEST_CLASS(ListTests)
    {
    private:
        slib::List<int> makeList() {
            slib::List<int> l;
            l.add(1);
            l.add(5);
            l.add(7);

            return l;
        }

        class ComplexType {
        public:
            ComplexType(int i) : i(i) {}
            int i;
            float f;
            bool b;
        };
    public:
        
        TEST_METHOD(Basic) {
            slib::List<int> intList;

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

            Assert::AreEqual(list2.numElements(), (size_t)3);
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

        TEST_METHOD(ComplexTypeTest) {
            slib::List<ComplexType> complexList;

            complexList.add(ComplexType{ 15 });
            complexList.add(ComplexType{ 20 });
            complexList.add(ComplexType{ 25 });

            Assert::AreEqual(20, complexList[1].i);

            complexList.removeFromEnd(1);
            Assert::AreEqual((size_t)2, complexList.numElements());
        }
    };
}
