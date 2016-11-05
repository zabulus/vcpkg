#include "CppUnitTest.h"
#include "vcpkg.h"

#pragma comment(lib,"version")
#pragma comment(lib,"winhttp")

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
namespace VisualStudio {
namespace CppUnitTestFramework
{
template <>
inline std::wstring ToString<vcpkg::package_spec_parse_result>(const vcpkg::package_spec_parse_result& t)
{
    return ToString(static_cast<uint32_t>(t));
}
}
}
}

namespace UnitTest1
{
TEST_CLASS(ControlParsing)
{
public:
    TEST_METHOD(parse_paragraphs_empty)
    {
        const char* str = "";
        auto pghs = vcpkg::parse_paragraphs(str);
        Assert::IsTrue(pghs.empty());
    }

    TEST_METHOD(parse_paragraphs_one_field)
    {
        const char* str = "f1: v1";
        auto pghs = vcpkg::parse_paragraphs(str);
        Assert::AreEqual(size_t(1), pghs.size());
        Assert::AreEqual(size_t(1), pghs[0].size());
        Assert::AreEqual("v1", pghs[0]["f1"].c_str());
    }

    TEST_METHOD(parse_paragraphs_one_pgh)
    {
        const char* str =
            "f1: v1\n"
            "f2: v2";
        auto pghs = vcpkg::parse_paragraphs(str);
        Assert::AreEqual(size_t(1), pghs.size());
        Assert::AreEqual(size_t(2), pghs[0].size());
        Assert::AreEqual("v1", pghs[0]["f1"].c_str());
        Assert::AreEqual("v2", pghs[0]["f2"].c_str());
    }

    TEST_METHOD(parse_paragraphs_two_pgh)
    {
        const char* str =
            "f1: v1\n"
            "f2: v2\n"
            "\n"
            "f3: v3\n"
            "f4: v4";
        auto pghs = vcpkg::parse_paragraphs(str);
        Assert::AreEqual(size_t(2), pghs.size());
        Assert::AreEqual(size_t(2), pghs[0].size());
        Assert::AreEqual("v1", pghs[0]["f1"].c_str());
        Assert::AreEqual("v2", pghs[0]["f2"].c_str());
        Assert::AreEqual(size_t(2), pghs[1].size());
        Assert::AreEqual("v3", pghs[1]["f3"].c_str());
        Assert::AreEqual("v4", pghs[1]["f4"].c_str());
    }

    TEST_METHOD(parse_paragraphs_field_names)
    {
        const char* str =
            "1:\n"
            "f:\n"
            "F:\n"
            "0:\n"
            "F-2:\n";
        auto pghs = vcpkg::parse_paragraphs(str);
        Assert::AreEqual(size_t(1), pghs.size());
        Assert::AreEqual(size_t(5), pghs[0].size());
    }

    TEST_METHOD(parse_paragraphs_multiple_blank_lines)
    {
        const char* str =
            "f1: v1\n"
            "f2: v2\n"
            "\n"
            "\n"
            "f3: v3\n"
            "f4: v4";
        auto pghs = vcpkg::parse_paragraphs(str);
        Assert::AreEqual(size_t(2), pghs.size());
    }

    TEST_METHOD(parse_paragraphs_empty_fields)
    {
        const char* str =
            "f1:\n"
            "f2: ";
        auto pghs = vcpkg::parse_paragraphs(str);
        Assert::AreEqual(size_t(1), pghs.size());
        Assert::AreEqual(size_t(2), pghs[0].size());
        Assert::AreEqual("", pghs[0]["f1"].c_str());
        Assert::AreEqual("", pghs[0]["f2"].c_str());
        Assert::AreEqual(size_t(2), pghs[0].size());
    }

    TEST_METHOD(parse_paragraphs_multiline_fields)
    {
        const char* str =
            "f1: simple\n"
            " f1\r\n"
            "f2:\r\n"
            " f2\r\n"
            " continue\r\n";
        auto pghs = vcpkg::parse_paragraphs(str);
        Assert::AreEqual(size_t(1), pghs.size());
        Assert::AreEqual("simple\n f1", pghs[0]["f1"].c_str());
        Assert::AreEqual("\n f2\n continue", pghs[0]["f2"].c_str());
    }

    TEST_METHOD(parse_paragraphs_crlfs)
    {
        const char* str =
            "f1: v1\r\n"
            "f2: v2\r\n"
            "\r\n"
            "f3: v3\r\n"
            "f4: v4";
        auto pghs = vcpkg::parse_paragraphs(str);
        Assert::AreEqual(size_t(2), pghs.size());
        Assert::AreEqual(size_t(2), pghs[0].size());
        Assert::AreEqual("v1", pghs[0]["f1"].c_str());
        Assert::AreEqual("v2", pghs[0]["f2"].c_str());
        Assert::AreEqual(size_t(2), pghs[1].size());
        Assert::AreEqual("v3", pghs[1]["f3"].c_str());
        Assert::AreEqual("v4", pghs[1]["f4"].c_str());
    }

    TEST_METHOD(package_spec_parse)
    {
        vcpkg::expected<vcpkg::package_spec> spec = vcpkg::package_spec::from_string("zlib", vcpkg::triplet::X86_WINDOWS);
        Assert::AreEqual(vcpkg::package_spec_parse_result::SUCCESS, vcpkg::to_package_spec_parse_result(spec.error_code()));
        Assert::AreEqual("zlib", spec.get()->name().c_str());
        Assert::AreEqual(vcpkg::triplet::X86_WINDOWS.canonical_name(), spec.get()->target_triplet().canonical_name());
    }

    TEST_METHOD(package_spec_parse_with_arch)
    {
        vcpkg::expected<vcpkg::package_spec> spec = vcpkg::package_spec::from_string("zlib:x64-uwp", vcpkg::triplet::X86_WINDOWS);
        Assert::AreEqual(vcpkg::package_spec_parse_result::SUCCESS, vcpkg::to_package_spec_parse_result(spec.error_code()));
        Assert::AreEqual("zlib", spec.get()->name().c_str());
        Assert::AreEqual(vcpkg::triplet::X64_UWP.canonical_name(), spec.get()->target_triplet().canonical_name());
    }

    TEST_METHOD(package_spec_parse_with_multiple_colon)
    {
        auto ec = vcpkg::package_spec::from_string("zlib:x86-uwp:", vcpkg::triplet::X86_WINDOWS).error_code();
        Assert::AreEqual(vcpkg::package_spec_parse_result::TOO_MANY_COLONS, vcpkg::to_package_spec_parse_result(ec));
    }

    TEST_METHOD(utf8_to_utf16)
    {
        auto str = vcpkg::Strings::utf8_to_utf16("abc");
        Assert::AreEqual(L"abc", str.c_str());
    }

    TEST_METHOD(utf8_to_utf16_with_whitespace)
    {
        auto str = vcpkg::Strings::utf8_to_utf16("abc -x86-windows");
        Assert::AreEqual(L"abc -x86-windows", str.c_str());
    }
};

}
