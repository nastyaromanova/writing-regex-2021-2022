#pragma once

// wr22
#include <wr22/regex_parser/parser/regex.hpp>
#include <wr22/unicode/conversion.hpp>

// STL
#include <string_view>

namespace wr22::regex_explainer::explanation {

    /// It is a function that returns the sample for the node from AST as the std::string_view.
    /// The function is overloaded and returns a different pattern of type "std::string_view"
    /// depending on the type of the argument it takes.
    ///
    /// Function "std::string::to_utf8" converts a char32_t to a string. For more information
    /// follow the unicode directory.

    /// There is no function for type "Empty" because it does not have any meaningful information.

    /// The function describes which literal matched and what index it has in the ASCII table.
    /// The function is case sensitive.
    std::u32string_view get_sample (const regex_parser::regex::part::Literal& vertex);

    /// The function lists all alternatives that needed to be mached and describes each one in detail.
    std::u32string_view get_sample (const regex_parser::regex::part::Alternatives& vertex);

    /// The function lists all items of the sequence list and match there explanation one after another.
    std::u32string_view get_sample (const regex_parser::regex::part::Sequence& vertex);

    /// TO BE DONE
    std::u32string_view get_sample (const regex_parser::regex::part::Group& vertex);

    /// Next three functions return the explanation of some quantifier.
    /// They are always the same and do not require any modifications depending on the situation.
    std::u32string_view get_sample (const regex_parser::regex::part::Optional& vertex);
    std::u32string_view get_sample (const regex_parser::regex::part::Plus& vertex);
    std::u32string_view get_sample (const regex_parser::regex::part::Star& vertex);

    /// The function returns the explanation for some regex basic that is always the same
    /// and does not require any modifications  depending on the situation.
    std::u32string_view get_sample (const regex_parser::regex::part::Wildcard& vertex);

    /// The function lists the symbols from the certain set that need to be matched.
    /// The function is case sensitive.
    std::u32string_view get_sample (const regex_parser::regex::part::CharacterClass& vertex);

    /// Constructs an explanation for the regular expression for which the AST has been built.
    ///
    /// If an error was thrown during the construction of the AST, then the function reports
    /// that a mistake was made in writing the regular expression and does not do anything more.
    std::u32string_view construct_explanation(const regex_parser::regex::SpannedPart& ast_vertex);

}  // namespace wr22::regex_explainer::explanation
