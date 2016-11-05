#include "CppUnitTest.h"
#include "vcpkg.h"

#pragma comment(lib,"version")
#pragma comment(lib,"winhttp")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
TEST_CLASS(BinaryParagraphTests)
{
public:

    TEST_METHOD(BinaryParagraph_Construct_Minimum)
    {
        vcpkg::BinaryParagraph pgh({
            { "Package", "zlib" },
            { "Version", "1.2.8" },
            { "Architecture", "x86-windows" },
            { "Multi-Arch", "same" },
        });

        Assert::AreEqual("zlib", pgh.spec.name().c_str());
        Assert::AreEqual("1.2.8", pgh.version.c_str());
        Assert::AreEqual("", pgh.maintainer.c_str());
        Assert::AreEqual("", pgh.description.c_str());
        Assert::AreEqual("x86-windows", pgh.spec.target_triplet().canonical_name().c_str());
        Assert::AreEqual(size_t(0), pgh.depends.size());
    }

    TEST_METHOD(BinaryParagraph_Construct_Maximum)
    {
        vcpkg::BinaryParagraph pgh({
            { "Package", "s" },
            { "Version", "v" },
            { "Architecture", "x86-windows" },
            { "Multi-Arch", "same" },
            { "Maintainer", "m" },
            { "Description", "d" },
            { "Depends", "bd" }
        });
        Assert::AreEqual("s", pgh.spec.name().c_str());
        Assert::AreEqual("v", pgh.version.c_str());
        Assert::AreEqual("m", pgh.maintainer.c_str());
        Assert::AreEqual("d", pgh.description.c_str());
        Assert::AreEqual(size_t(1), pgh.depends.size());
        Assert::AreEqual("bd", pgh.depends[0].c_str());
    }

    TEST_METHOD(BinaryParagraph_Three_Depends)
    {
        vcpkg::BinaryParagraph pgh({
            { "Package", "zlib" },
            { "Version", "1.2.8" },
            { "Architecture", "x86-windows" },
            { "Multi-Arch", "same" },
            { "Depends", "a, b, c" },
        });

        Assert::AreEqual(size_t(3), pgh.depends.size());
        Assert::AreEqual("a", pgh.depends[0].c_str());
        Assert::AreEqual("b", pgh.depends[1].c_str());
        Assert::AreEqual("c", pgh.depends[2].c_str());
    }

    TEST_METHOD(BinaryParagraph_serialize_min)
    {
        std::stringstream ss;
        vcpkg::BinaryParagraph pgh({
            { "Package", "zlib" },
            { "Version", "1.2.8" },
            { "Architecture", "x86-windows" },
            { "Multi-Arch", "same" },
        });
        ss << pgh;
        auto pghs = vcpkg::parse_paragraphs(ss.str());
        Assert::AreEqual(size_t(1), pghs.size());
        Assert::AreEqual(size_t(4), pghs[0].size());
        Assert::AreEqual("zlib", pghs[0]["Package"].c_str());
        Assert::AreEqual("1.2.8", pghs[0]["Version"].c_str());
        Assert::AreEqual("x86-windows", pghs[0]["Architecture"].c_str());
        Assert::AreEqual("same", pghs[0]["Multi-Arch"].c_str());
    }

    TEST_METHOD(BinaryParagraph_serialize_max)
    {
        std::stringstream ss;
        vcpkg::BinaryParagraph pgh({
            { "Package", "zlib" },
            { "Version", "1.2.8" },
            { "Architecture", "x86-windows" },
            { "Description", "first line\n second line" },
            { "Maintainer", "abc <abc@abc.abc>" },
            { "Depends", "dep" },
            { "Multi-Arch", "same" },
        });
        ss << pgh;
        auto pghs = vcpkg::parse_paragraphs(ss.str());
        Assert::AreEqual(size_t(1), pghs.size());
        Assert::AreEqual(size_t(7), pghs[0].size());
        Assert::AreEqual("zlib", pghs[0]["Package"].c_str());
        Assert::AreEqual("1.2.8", pghs[0]["Version"].c_str());
        Assert::AreEqual("x86-windows", pghs[0]["Architecture"].c_str());
        Assert::AreEqual("same", pghs[0]["Multi-Arch"].c_str());
        Assert::AreEqual("first line\n second line", pghs[0]["Description"].c_str());
        Assert::AreEqual("dep", pghs[0]["Depends"].c_str());
    }

    TEST_METHOD(BinaryParagraph_serialize_multiple_deps)
    {
        std::stringstream ss;
        vcpkg::BinaryParagraph pgh({
            { "Package", "zlib" },
            { "Version", "1.2.8" },
            { "Architecture", "x86-windows" },
            { "Multi-Arch", "same" },
            { "Depends", "a, b, c" },
        });
        ss << pgh;
        auto pghs = vcpkg::parse_paragraphs(ss.str());
        Assert::AreEqual(size_t(1), pghs.size());
        Assert::AreEqual("a, b, c", pghs[0]["Depends"].c_str());
    }
};

}
