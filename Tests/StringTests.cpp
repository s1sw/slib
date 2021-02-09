#include "CppUnitTest.h"
#include <slib/String.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
    TEST_CLASS(StringTests) {
public:

    TEST_METHOD(Basic) {
        slib::String str;

        Assert::AreEqual((size_t)0, str.byteLength());

        str = "hello world!";

        Assert::AreEqual((size_t)12, str.byteLength());
        Assert::AreEqual('e', str[1]);
        Assert::AreEqual("hello world!", str.cStr());
    }

    TEST_METHOD(Concatenation) {
        slib::String a("a");
        slib::String b("b");

        slib::String concatenated = a + b;

        Assert::AreEqual('a', concatenated[0]);
        Assert::AreEqual('b', concatenated[1]);
    }

#define STR_A "This is our first \"long\" string..."
#define STR_B "And this is our second."
    TEST_METHOD(LongConcatenation) {
        slib::String a(STR_A);
        slib::String b(STR_B);

        slib::String concatenated = a + b;

        Assert::AreEqual(STR_A STR_B, concatenated.cStr());
    }

    TEST_METHOD(InPlaceConcatenation) {
        slib::String a("a");

        a += "b";

        Assert::AreEqual("ab", a.cStr());
    }

    TEST_METHOD(InPlaceLongConcatenation) {
        slib::String a(STR_A);

        a += STR_B;

        Assert::AreEqual(STR_A STR_B, a.cStr());
    }

    TEST_METHOD(SubstringTest) {
        slib::String a(STR_A);

        Assert::AreEqual("is our", a.substring(5, 6).cStr());
        Assert::AreEqual(STR_A, a.substring(0).cStr());
        Assert::AreEqual("is our first \"long\" string...", a.substring(5).cStr());
    }

#undef STR_A
#undef STR_B
    };
}
