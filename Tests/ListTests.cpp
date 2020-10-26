#include "pch.h"
#include "CppUnitTest.h"
#include <List.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
    TEST_CLASS(ListTests)
    {
    private:
        slib::List<int> makeList() {
            slib::List<int> l;
            l.Add(1);
            l.Add(5);
            l.Add(7);

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
        
        TEST_METHOD(Basic)
        {
            slib::List<int> intList;

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

            Assert::AreEqual(list2.NumElements(), (size_t)3);
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

            complexList.Add(ComplexType{ 15 });
            complexList.Add(ComplexType{ 20 });
            complexList.Add(ComplexType{ 25 });

            Assert::AreEqual(20, complexList[1].i);

            complexList.RemoveFromEnd(1);
            Assert::AreEqual((size_t)2, complexList.NumElements());
        }
    };
}
