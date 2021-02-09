#include "CppUnitTest.h"
#include <slib/Path.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests {
    TEST_CLASS(PathTests) {
public:

    TEST_METHOD(Basic) {
        Assert::IsTrue(slib::Path("C:/test.txt").isAbsolute());
        Assert::IsFalse(slib::Path("../cat.txt").isAbsolute());
    }

    TEST_METHOD(FileExtensions) {
        Assert::AreEqual(".txt", slib::Path("C:/test.txt").fileExtension().cStr());
        Assert::AreEqual(".txt", slib::Path("/test.txt").fileExtension().cStr());
        Assert::AreEqual(".txt", slib::Path("../cat.txt").fileExtension().cStr());
        Assert::AreEqual(".txt", slib::Path(".haha.so.many...dots.....txt").fileExtension().cStr());
        Assert::AreEqual("", slib::Path("no_extension").fileExtension().cStr());
        Assert::AreEqual(".", slib::Path("sorta_extension.").fileExtension().cStr());
        Assert::AreEqual(".extension_only", slib::Path(".extension_only").fileExtension().cStr());
    }

    TEST_METHOD(stem) {
        Assert::AreEqual("hello", slib::Path("C:/hello.world").stem().cStr());
        Assert::AreEqual("hello", slib::Path("hello").stem().cStr());
        Assert::AreEqual("hello", slib::Path("hello.world").stem().cStr());
    }

    TEST_METHOD(normalize) {
        slib::Path p1("hello/./././././world");
        p1.normalize();
        Assert::AreEqual("hello/world", p1.cStr());

        slib::Path p2("C:/hello/../world.txt");
        p2.normalize();
        Assert::AreEqual("C:/world.txt", p2.cStr());

        slib::Path p3("/usr/../mnt/../home/someone/././.config/");
        p3.normalize();
        Assert::AreEqual("/home/someone/.config", p3.cStr());

        slib::Path p4("C:\\Users\\someone\\AppData\\Roaming");
        p4.normalize();
        Assert::AreEqual("C:/Users/someone/AppData/Roaming", p4.cStr());
    }

    TEST_METHOD(ParentPath) {
        Assert::AreEqual("/home/someone", slib::Path("/home/someone/.config").parentPath().cStr());
        Assert::AreEqual("C:/", slib::Path("C:/test").parentPath().cStr());

        Assert::AreEqual("C:/Users/someone/AppData", slib::Path("C:\\Users\\someone\\AppData\\Roaming").parentPath().cStr());
    }

    TEST_METHOD(ReplaceSeparator) {
        slib::Path p1("C:/Users/someone");
        p1.replaceSeparator('\\');
        Assert::AreEqual("C:\\Users\\someone", p1.cStr());

        slib::Path p2("C:\\Users\\someone");
        p2.replaceSeparator('/');
        Assert::AreEqual("C:/Users/someone", p2.cStr());
    }
    };
}
