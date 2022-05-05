#pragma once

// wr22
#include <wr22/regex_parser/parser/regex.hpp>
#include <wr22/unicode/conversion.hpp>

// STL
#include <string_view>

namespace wr22::regex_explainer::explanation {

    /// The function outputs processed information about each node of the syntax tree.
    /// Thus, a complete description of the work of the written regular expression is created.
    std::ostream &operator<<(std::ostream &out, const SpannedPart &spanned_part) {
        auto span = spanned_part.span();
        spanned_part.part().visit(
                [&out, span](const part::Empty &) { fmt::print(out, "Empty [{}]", span); },
                [&out, span](const part::Literal &part) {
                    out << '\'';
                    {
                        auto it = std::ostream_iterator<char>(out);
                        wr22::unicode::to_utf8_write(it, part.character);
                    }
                    out << '\'';
                    fmt::print(out, " [{}]", span);
                },
                [&out, span](const part::Alternatives &part) {
                    fmt::print(out, "Alternatives [{}] {{ ", span);
                    bool first = true;
                    for (const auto &alt: part.alternatives) {
                        if (!first) {
                            out << ", ";
                        }
                        first = false;
                        out << alt;
                    }
                    out << " }";
                },
                [&out, span](const part::Sequence &part) {
                    fmt::print(out, "Sequence [{}] {{ ", span);
                    bool first = true;
                    for (const auto &item: part.items) {
                        if (!first) {
                            out << ", ";
                        }
                        first = false;
                        out << item;
                    }
                    out << " }";
                },
                [&out, span](const part::Group &part) {
                    fmt::print(
                            out,
                            "Group [{}] {{ capture: {}, inner: {} }}",
                            span,
                            part.capture,
                            *part.inner);
                },
                [&out, span](const part::Optional &part) {
                    fmt::print(out, "Optional [{}] {{ {} }}", span, *part.inner);
                },
                [&out, span](const part::Plus &part) {
                    fmt::print(out, "Plus [{}] {{ {} }}", span, *part.inner);
                },
                [&out, span](const part::Star &part) {
                    fmt::print(out, "Star [{}] {{ {} }}", span, *part.inner);
                },
                [&out, span]([[maybe_unused]] const part::Wildcard &part) {
                    fmt::print(out, "Wildcard [{}]", span);
                },
                [&out, span](const part::CharacterClass &part) {
                    fmt::print(
                            out,
                            "CharacterClass [{}]{} {{ ",
                            span,
                            part.data.inverted ? " (inverted)" : "");
                    bool first = true;
                    for (const auto &range: part.data.ranges) {
                        if (!first) {
                            out << ", ";
                        }
                        first = false;
                        out << range;
                    }
                    out << " }";
                });
        return out;
    }

}  // namespace wr22::regex_explainer::explanation
