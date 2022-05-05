#pragma once

// stl
#include <cstddef>
#include <stdexcept>
#include <ostream>

// nlohmann
#include <nlohmann/json.hpp>

namespace wr22::regex_parser::span {

/// The exception thrown on an attempt to construct an invalid span.
///
/// See the documentation for `Span` for additional information.
struct InvalidSpan : public std::runtime_error {
    InvalidSpan(size_t begin, size_t end);

    size_t begin;
    size_t end;
};

/// Character position range in the input string.
///
/// The range is encoded by two numbers: `begin`, the position (0-based index) of the first
/// character in the range, and `end`, the past-the-end position, or the 0-based index of the last
/// character in the range **plus 1**. This is to be consistent with the behavior of C++ iterators
/// and `begin()`/`end()` functions on STL containers. Please note, however, that the
/// `begin()`/`end()` methods here are just accessors that are not used for iteration, they return
/// plain indices which have no iterator semantics.
///
/// Invalid spans (`begin > end`) are not allowed and their construction will result in an error.
/// See the documentation for the relevant methods for details.
class Span {
public:
    /// Construct an empty span that "starts" at a given position.
    ///
    /// The resulting span will have `begin = position` and `end = position`.
    static Span make_empty(size_t position);

    /// Construct a span that captures only one position.
    ///
    /// The resulting span will have `begin = position` and `end = position + 1`.
    /// @throws InvalidSpan if `position + 1` overflows `size_t`. Note that the error message might
    /// not be precise enough.
    static Span make_single_position(size_t position);

    /// Construct a span with given values of `begin` and `end` without any transformations.
    ///
    /// @throws InvalidSpan if `end < begin`.
    static Span make_from_positions(size_t begin, size_t end);

    /// Construct a span with a given value of `begin` and a given length.
    ///
    /// The length is determined by the number of characters covered by this span, and, since
    /// `begin` and `end` form a half-interval, it equals `end - begin`.
    ///
    /// @throws InvalidSpan if `begin + length` overflows `size_t`. Note that
    /// the error message might not be precise enough.
    static Span make_with_length(size_t begin, size_t length);

    /// Construct a new span that equals the current span extended to the right by a specified number of positions.
    /// Please note that this method creates a new span and does not modify the existing one.
    ///
    /// Formally, `[begin..end].extend_right(n) = [begin..(end+n)]`.
    ///
    /// @throws InvalidSpan if `end + length` overflows `size_t`. Note that
    /// the error message might not be precise enough.
    Span extend_right(size_t num_positions) const;

    /// Get the length of the span (the number of characters covered).
    size_t length() const;

    /// Get the `begin` position of the span.
    size_t begin() const;

    /// Get the `end` position of the span.
    size_t end() const;

    bool operator==(const Span& other) const = default;
    bool operator!=(const Span& other) const = default;

private:
    /// General-form constructor.
    ///
    /// Constructs a range from the `begin` and `end` values without any transformation.
    /// This constructor is private because named static methods (`make_*`) are preferrable to avoid
    /// code misinterpretations and mistakes.
    /// @throws InvalidSpan if `end < begin`
    explicit Span(size_t begin, size_t end);

    size_t m_begin;
    size_t m_end;
};

std::ostream& operator<<(std::ostream& out, Span span);
void to_json(nlohmann::json& j, Span span);

}  // namespace wr22::regex_parser::span
