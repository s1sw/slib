#include "CppUnitTest.h"
#include <slib/Path.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
    TEST_CLASS(PathTests) {
public:

    TEST_METHOD(Basic) {
        Assert::IsTrue(slib::Path("C:/test.txt").IsAbsolute());
        Assert::IsFalse(slib::Path("../cat.txt").IsAbsolute());
    }

    TEST_METHOD(FileExtensions) {
        Assert::AreEqual(".txt", slib::Path("C:/test.txt").FileExtension().CStr());
        Assert::AreEqual(".txt", slib::Path("/test.txt").FileExtension().CStr());
        Assert::AreEqual(".txt", slib::Path("../cat.txt").FileExtension().CStr());
        Assert::AreEqual(".txt", slib::Path(".haha.so.many...dots.....txt").FileExtension().CStr());
        Assert::AreEqual("", slib::Path("no_extension").FileExtension().CStr());
        Assert::AreEqual(".", slib::Path("sorta_extension.").FileExtension().CStr());
        Assert::AreEqual(".extension_only", slib::Path(".extension_only").FileExtension().CStr());
    }

    TEST_METHOD(Stem) {
        Assert::AreEqual("hello", slib::Path("C:/hello.world").Stem().CStr());
        Assert::AreEqual("hello", slib::Path("hello").Stem().CStr());
        Assert::AreEqual("hello", slib::Path("hello.world").Stem().CStr());
    }

    TEST_METHOD(Normalize) {
        slib::Path p1("hello/./././././world");
        p1.Normalize();
        Assert::AreEqual("hello/world", p1.CStr());

        slib::Path p2("C:/hello/../world.txt");
        p2.Normalize();
        Assert::AreEqual("C:/world.txt", p2.CStr());

        slib::Path p3("/usr/../mnt/../home/someone/././.config/");
        p3.Normalize();
        Assert::AreEqual("/home/someone/.config", p3.CStr());

        slib::Path p4("C:\\Users\\someone\\AppData\\Roaming");
        p4.Normalize();
        Assert::AreEqual("C:/Users/someone/AppData/Roaming", p4.CStr());
    }

    TEST_METHOD(ParentPath) {
        Assert::AreEqual("/home/someone", slib::Path("/home/someone/.config").ParentPath().CStr());
        Assert::AreEqual("C:/", slib::Path("C:/test").ParentPath().CStr());

        Assert::AreEqual("C:/Users/someone/AppData", slib::Path("C:\\Users\\someone\\AppData\\Roaming").ParentPath().CStr());
    }

    TEST_METHOD(ReplaceSeparator) {
        slib::Path p1("C:/Users/someone");
        p1.ReplaceSeparator('\\');
        Assert::AreEqual("C:\\Users\\someone", p1.CStr());

        slib::Path p2("C:\\Users\\someone");
        p2.ReplaceSeparator('/');
        Assert::AreEqual("C:/Users/someone", p2.CStr());
    }
    };
}
