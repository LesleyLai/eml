// TOKEN_TABLE_ENTRY(type, type_name, prefix, infix, precedence)
#define TOKEN_TABLE                                                     \
  TOKEN_TABLE_ENTRY(left_paren, "(", parse_grouping, nullptr, call)     \
  TOKEN_TABLE_ENTRY(right_paren, ")", nullptr, nullptr, none)           \
  TOKEN_TABLE_ENTRY(left_brace, "{", nullptr, nullptr, none)            \
  TOKEN_TABLE_ENTRY(right_brace, "}", nullptr, nullptr, none)           \
  TOKEN_TABLE_ENTRY(comma, ",", nullptr, nullptr, none)                 \
  TOKEN_TABLE_ENTRY(dot, ".", nullptr, nullptr, call)                   \
  TOKEN_TABLE_ENTRY(minus, "-", parse_unary, parse_binary, term)        \
  TOKEN_TABLE_ENTRY(minus_right_arrow, "->", nullptr, nullptr, none)    \
  TOKEN_TABLE_ENTRY(plus, "+", nullptr, parse_binary, term)             \
  TOKEN_TABLE_ENTRY(colon, ":", nullptr, nullptr, none)                 \
  TOKEN_TABLE_ENTRY(semicolon, ";", nullptr, nullptr, none)             \
  TOKEN_TABLE_ENTRY(slash, "/", nullptr, parse_binary, factor)          \
  TOKEN_TABLE_ENTRY(star, "*", nullptr, parse_binary, factor)           \
  TOKEN_TABLE_ENTRY(bang, "!", parse_unary, nullptr, none)              \
  TOKEN_TABLE_ENTRY(bang_equal, "!=", nullptr, parse_binary, equality)  \
  TOKEN_TABLE_ENTRY(equal, "=", nullptr, nullptr, none)                 \
  TOKEN_TABLE_ENTRY(double_equal, "==", nullptr, parse_binary, equality) \
  TOKEN_TABLE_ENTRY(equal_right_arrow, "=>", nullptr, nullptr, none)    \
  TOKEN_TABLE_ENTRY(greator, ">", nullptr, parse_binary, comparison)    \
  TOKEN_TABLE_ENTRY(greator_greator, ">>", nullptr, nullptr, none)      \
  TOKEN_TABLE_ENTRY(greater_equal, ">=", nullptr, parse_binary, comparison) \
  TOKEN_TABLE_ENTRY(less, "<", nullptr, parse_binary, comparison)       \
  TOKEN_TABLE_ENTRY(less_less, "<<", nullptr, nullptr, none)            \
  TOKEN_TABLE_ENTRY(less_equal, "<=", nullptr, parse_binary, comparison) \
  TOKEN_TABLE_ENTRY(identifier, "identifier", nullptr, nullptr, none)   \
  TOKEN_TABLE_ENTRY(string_literal, "string", nullptr, nullptr, none)   \
  TOKEN_TABLE_ENTRY(number_literal, "number", parse_number, nullptr, none) \
  TOKEN_TABLE_ENTRY(keyword_and, "and", nullptr, nullptr, and)          \
  TOKEN_TABLE_ENTRY(keyword_case, "case", nullptr, nullptr, none)       \
  TOKEN_TABLE_ENTRY(keyword_class, "class", nullptr, nullptr, none)     \
  TOKEN_TABLE_ENTRY(keyword_def, "def", nullptr, nullptr, none)         \
  TOKEN_TABLE_ENTRY(keyword_else, "else", nullptr, nullptr, none)       \
  TOKEN_TABLE_ENTRY(keyword_extern, "extern", nullptr, nullptr, none)   \
  TOKEN_TABLE_ENTRY(keyword_false, "false", parse_literal, nullptr, none) \
  TOKEN_TABLE_ENTRY(keyword_for, "for", nullptr, nullptr, none)         \
  TOKEN_TABLE_ENTRY(keyword_if, "if", nullptr, nullptr, none)           \
  TOKEN_TABLE_ENTRY(keyword_let, "let", nullptr, nullptr, none)         \
  TOKEN_TABLE_ENTRY(keyword_not, "not", nullptr, nullptr, none)         \
  TOKEN_TABLE_ENTRY(keyword_or, "or", nullptr, nullptr, or)             \
  TOKEN_TABLE_ENTRY(keyword_print, "print", nullptr, nullptr, none)     \
  TOKEN_TABLE_ENTRY(keyword_return, "return", nullptr, nullptr, none)   \
  TOKEN_TABLE_ENTRY(keyword_this, "this", nullptr, nullptr, none)       \
  TOKEN_TABLE_ENTRY(keyword_true, "true", parse_literal, nullptr, none) \
  TOKEN_TABLE_ENTRY(keyword_variant, "variant", nullptr, nullptr, none) \
  TOKEN_TABLE_ENTRY(keyword_unit, "()", parse_literal, nullptr, none)   \
  TOKEN_TABLE_ENTRY(error, "error", nullptr, nullptr, none)             \
  TOKEN_TABLE_ENTRY(eof, "eof", nullptr, nullptr, none)

TOKEN_TABLE
#undef TOKEN_TABLE
