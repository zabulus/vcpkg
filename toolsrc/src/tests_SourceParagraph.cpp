#include "CppUnitTest.h"
#include "vcpkg.h"

#pragma comment(lib,"version")
#pragma comment(lib,"winhttp")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(SourceParagraphTests)
    {
    public:
        TEST_METHOD(SourceParagraph_Construct_Minimum)
        {
            vcpkg::SourceParagraph pgh({
                {"Source", "zlib"},
                {"Version", "1.2.8"}
            });

            Assert::AreEqual("zlib", pgh.name.c_str());
            Assert::AreEqual("1.2.8", pgh.version.c_str());
            Assert::AreEqual("", pgh.maintainer.c_str());
            Assert::AreEqual("", pgh.description.c_str());
            Assert::AreEqual(size_t(0), pgh.depends.size());
        }

        TEST_METHOD(SourceParagraph_Construct_Maximum)
        {
            vcpkg::SourceParagraph pgh({
                {"Source", "s"},
                {"Version", "v"},
                {"Maintainer", "m"},
                {"Description", "d"},
                {"Build-Depends", "bd"}
            });
            Assert::AreEqual("s", pgh.name.c_str());
            Assert::AreEqual("v", pgh.version.c_str());
            Assert::AreEqual("m", pgh.maintainer.c_str());
            Assert::AreEqual("d", pgh.description.c_str());
            Assert::AreEqual(size_t(1), pgh.depends.size());
            Assert::AreEqual("bd", pgh.depends[0].name.c_str());
        }

        TEST_METHOD(SourceParagraph_Two_Depends)
        {
            vcpkg::SourceParagraph pgh({
                {"Source", "zlib"},
                {"Version", "1.2.8"},
                {"Build-Depends", "z, openssl"}
            });

            Assert::AreEqual(size_t(2), pgh.depends.size());
            Assert::AreEqual("z", pgh.depends[0].name.c_str());
            Assert::AreEqual("openssl", pgh.depends[1].name.c_str());
        }

        TEST_METHOD(SourceParagraph_Three_Depends)
        {
            vcpkg::SourceParagraph pgh({
                {"Source", "zlib"},
                {"Version", "1.2.8"},
                {"Build-Depends", "z, openssl, xyz"}
            });

            Assert::AreEqual(size_t(3), pgh.depends.size());
            Assert::AreEqual("z", pgh.depends[0].name.c_str());
            Assert::AreEqual("openssl", pgh.depends[1].name.c_str());
            Assert::AreEqual("xyz", pgh.depends[2].name.c_str());
        }

        TEST_METHOD(SourceParagraph_Construct_Qualified_Depends)
        {
            vcpkg::SourceParagraph pgh({
                { "Source", "zlib" },
                { "Version", "1.2.8" },
                { "Build-Depends", "libA [windows], libB [uwp]" }
            });

            Assert::AreEqual("zlib", pgh.name.c_str());
            Assert::AreEqual("1.2.8", pgh.version.c_str());
            Assert::AreEqual("", pgh.maintainer.c_str());
            Assert::AreEqual("", pgh.description.c_str());
            Assert::AreEqual(size_t(2), pgh.depends.size());
            Assert::AreEqual("libA", pgh.depends[0].name.c_str());
            Assert::AreEqual("windows", pgh.depends[0].qualifier.c_str());
            Assert::AreEqual("libB", pgh.depends[1].name.c_str());
            Assert::AreEqual("uwp", pgh.depends[1].qualifier.c_str());
        }
    };
}
