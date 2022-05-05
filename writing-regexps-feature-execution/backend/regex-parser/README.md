# regex-parser

This is the library for parsing regular expressions. It takes a regular expression as a UTF-32
string and parses it into its syntax tree, throwing exceptions if the regular expression cannot
be parsed (has invalid syntax). The syntax tree consists of nodes, each representing a particular
element in the regex. Each node contains full information about the component of the regex it
is representing (e.g.  the characters in a character class `[0-9a-f]` or the type of the group
capture `(abc)` vs `(?:abc)` vs `(?<name>abc)`) and the location of this element in the regex string.

## Usage instructions

The function that does parsing is [`wr22::regex_parser::parser::parse_regex`][fn.parse_regex].
A brief example of its usage is as follows:

```cpp
// Assume `parse_regex` is already in the scope.
auto regex = U"a(b|c)+";
auto syntax_tree = parse_regex(regex);
std::cout << syntax_tree << std::endl;
```

`parse_regex` accepts a [`std::u32string_view`][std::u32string_view] as an argument and returns
a [`wr22::regex_parser::regex::SpannedPart`][t.spanned_part].  The characters comprising a
regular expression are represented as `char32_t`, so that any Unicode code point can fit into
this representation.

The `SpannedPart` returned from `parse_regex()` represents the syntax tree of the parsed
regular expression.  `SpannedPart` consists of two items:

- A [`Span`][t.span], which indicates what range of characters in the regex is covered by
  this node. See the API reference for details.
- A [`Part`][t.part], which represents one of the supported types of nodes.

`Part` is an algebraic, or variant data type (see [`Adt`][t.adt] or [`std::variant`][std::variant]
for an explanation). A `Part` represents a node in syntax tree. There are different types of
syntax tree nodes, which correspond to the [variants][v.part] of `Part`:

- Empty node (`part::Empty`). Represents an empty regular expression or the content
  of an empty group, e.g. the inner part of `()`.
- Character literal (`part::Literal`). Represents a single Unicode character (code point)
  that is matched literally, e.g. `a`, or `Ǽ`.
- Sequence of nodes (`part::Sequence`). Represents a sequence of smaller elements of a regex that
  will be matched one after another. For example, `abc` is represented as a sequence of three
  `Literal`s.
- Alternatives list (`part::Alternatives`). Represents a list of smaller elements of a regex at least
  some of which should match. The alternatives are separated by `|` in a regex. E.g. `a|b|cd` contains
  three alternatives: a `Literal` `a`, a `Literal` `b` and a `Sequence` of `Literal`s `c` and `d`.
- Group (`part::Group`). Represents a subexpression in parentheses, e.g. `(abc)`. A group might
  be capturing or non-capturing, and, if capturing, might capture the matched substring by name or by index.
  For additional and more detailed information, see the API reference.
- Quantifiers (`part::Optional`, `part::Star`, `part::Plus`). These nodes represent a quantifier over
  a subexpression (e.g. `(foo)?`, `.*` or `[a-z]+`).
- Wildcard (`part::Wildcard`). Represents a "dot" whildcard matching any single character (`.`).

More variants will be eventually added.

Each of the listed types is a type a syntax tree node might have. Either of these types may be
contained in a `Part`, since these types are `Part`'s variants. To check what type of a syntax
tree node is contained in a given `Part` and to access the stored value of this type, the method
[`Part::visit`][m.part.visit] exists (see the API reference for a usage example).

For a more detailed reference on the functions and data types available in this library, we
ask the reader to take a look at the [API reference][api].

## Library status
Currently, the library is not ready to be seriously used as a building block. Some prototyping
can be done now, but the library's interface may currently change without a warning, including
breaking changes. The library code resides on the `feature/regex-parser-cpp` branch and will
be merged into `main` when it becomes ready to be used (the library's interface might still
change from time to time).

Utilities such as `Adt` might split off into a separate utility library if it becomes necessary
to use them from other code as well. This might change their namespaces and the header files
that need to be included.

The level of regex support is as follows.

**Supported features**:

- Literal characters
- Groups (capture by index or by name (3 flavors) or none at all)
- Alternative lists (`a|bb|ccc`)
- Quantifiers `?`, `+` and `*` (greedy only)
- Wildcards (`.`)

**Unsupported features**:

- Character classes (`[a-z]`)
- Start of line / end of line (`^`, `$`)
- Repetitions (e.g. `(abc){3}` or `x{5,10}`)
- Escape sequences (e.g. `\n` or `\d`)
- Special character escaping
- Extended character classes (`[[:digit:]]`)
- Lazy or possessive quantifiers (e.g. `*?` or `*+`).
- Lookaround
- Backreferences
- Extensions for recursion
- Other features not explicitly mentioned

**Compatibility notes**:

- Group names are currently represented as UTF-8 encoded `std::string`s.
  It is unclear if we should change it.
- Group names are not checked for being valid (e.g. `:0::-` is accepted as a group name).
  This should probably stay as it is, and an additional verification might be done in a
  post-processing step using language-specific settings.

[api]: https://writing-regexps-2021-22.github.io/docs/regex-parser/index.html
[fn.parse_regex]: https://writing-regexps-2021-22.github.io/docs/regex-parser/namespacewr22_1_1regex__parser_1_1parser.html#a0dc595a19e81abed1c444fda1bbe6aee
[m.part.visit]: https://writing-regexps-2021-22.github.io/docs/regex-parser/classwr22_1_1regex__parser_1_1utils_1_1Adt.html#a07d5c8e3b851046fa584fe4d8ec311ea
[std::string]: https://en.cppreference.com/w/cpp/string/basic_string
[std::string_view]: https://en.cppreference.com/w/cpp/string/basic_string_view
[std::u32string_view]: https://en.cppreference.com/w/cpp/string/basic_string_view
[std::variant]: https://en.cppreference.com/w/cpp/utility/variant
[t.adt]: https://writing-regexps-2021-22.github.io/docs/regex-parser/classwr22_1_1regex__parser_1_1utils_1_1Adt.html
[t.part]: https://writing-regexps-2021-22.github.io/docs/regex-parser/classwr22_1_1regex__parser_1_1regex_1_1Part.html
[t.span]: https://writing-regexps-2021-22.github.io/docs/regex-parser/classwr22_1_1regex__parser_1_1span_1_1Span.html
[t.spanned_part]: https://writing-regexps-2021-22.github.io/docs/regex-parser/classwr22_1_1regex__parser_1_1regex_1_1SpannedPart.html
[tool.cmake]: https://cmake.org
[tool.ninja]: https://ninja-build.org
[tool.ycm]: https://github.com/ycm-core/YouCompleteMe
[v.part]: https://writing-regexps-2021-22.github.io/docs/regex-parser/namespacewr22_1_1regex__parser_1_1regex_1_1part.html
