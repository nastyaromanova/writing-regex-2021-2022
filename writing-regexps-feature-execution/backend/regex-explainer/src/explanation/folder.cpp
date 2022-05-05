#pragma once

// wr22
#include <wr22/regex_parser/parser/regex.hpp>
#include <wr22/regex_explainer/explanation/folder.hpp>
#include <wr22/unicode/conversion.hpp>

// STL
#include <string_view>

namespace wr22::regex_explainer::explanation {

    /// The function returns the part of the current regex that needed to be show
    /// during the explanation. The entire selected part of the regular expression is converted
    /// to the type 'std::u32string_view' and returned to the output stream.
    std::u32string_view get_regex_part () {}

    /// Function that returns the sample for the node from AST as the std::string_view.
    ///
    /// Function "std::string::to_utf8" converts a char32_t to a string. For more information
    /// follow the unicode directory.
    std::string_view get_sample (const regex_parser::regex::part::Literal& vertex) {
        std::string_view pattern1 = " matches the character ",
                         pattern2 = " with index ";
        std::string_view current_sample =
                wr22::unicode::to_utf8(vertex.character) + pattern1
                + wr22::unicode::to_utf8(vertex.character) + pattern2
                + int(vertex.character);
        return current_sample;
    }

    std::u32string_view get_sample (const regex_parser::regex::part::Alternatives& vertex);

    std::string_view get_sample (const regex_parser::regex::part::Sequence& vertex);

    std::string_view get_sample (const regex_parser::regex::part::Group& vertex);

    std::string_view get_sample (const regex_parser::regex::part::Optional& vertex);

    std::string_view get_sample (const regex_parser::regex::part::Plus& vertex);

    std::string_view get_sample (const regex_parser::regex::part::Star& vertex);

    std::string_view get_sample (const regex_parser::regex::part::Wildcard& vertex);

    std::string_view get_sample (const regex_parser::regex::part::CharacterClass& vertex);

    std::string_view construct_explanation(const regex_parser::regex::SpannedPart& ast_vertex) {

    }

}  // namespace wr22::regex_explainer::explanation
