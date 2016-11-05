#pragma once

#include <unordered_map>
#include "SourceParagraph.h"
#include "triplet.h"
#include "package_spec.h"

namespace vcpkg
{
    struct version_dependency
    {
        std::string name;
        std::string version;
    };

    struct BinaryParagraph
    {
        BinaryParagraph();
        explicit BinaryParagraph(std::unordered_map<std::string, std::string> fields);
        BinaryParagraph(const SourceParagraph& spgh, const triplet& target_triplet);

        std::string displayname() const;

        std::string fullstem() const;

        std::string dir() const;

        package_spec spec;
        std::string version;
        std::string description;
        std::string maintainer;
        std::string triplet_hash;
        std::vector<version_dependency> depends;
    };

    std::vector<version_dependency> expand_versioned_dependencies(const std::vector<std::string>& depends);

    std::ostream& operator<<(std::ostream& os, const BinaryParagraph& pgh);
    std::ostream& operator<<(std::ostream& os, const version_dependency& pgh);
}
