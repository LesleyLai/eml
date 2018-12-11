#define TOKEN_NORULE(token, text) TOKEN_TABLE_ENTRY(token, text, nullptr, nullptr, none)
#define TOKEN_BINARY_OP(token, text, precedence) TOKEN_TABLE_ENTRY(token, text, nullptr, parse_binary, precedence)

// TOKEN_TABLE_ENTRY(type, type_name, prefix, infix, precedence)
#define TOKEN_TABLE                                                     \
  TOKEN_TABLE_ENTRY(left_paren, "(", parse_grouping, nullptr, call)     \
  TOKEN_NORULE(right_paren, ")")           \
  TOKEN_NORULE(left_brace, "{")            \
  TOKEN_NORULE(right_brace, "}")           \
  TOKEN_NORULE(comma, ",")                 \
  TOKEN_TABLE_ENTRY(dot, ".", nullptr, nullptr, call)                   \
  TOKEN_TABLE_ENTRY(minus, "-", parse_unary, parse_binary, term)      \
  TOKEN_NORULE(minus_right_arrow, "->")    \
  TOKEN_BINARY_OP(plus, "+", term)             \
  TOKEN_NORULE(colon, ":")                 \
  TOKEN_NORULE(semicolon, ";")             \
  TOKEN_BINARY_OP(slash, "/", factor)          \
  TOKEN_TABLE_ENTRY(backslash, "\\", parse_lambda, nullptr, lambda)         \
  TOKEN_BINARY_OP(star, "*", factor)           \
  TOKEN_TABLE_ENTRY(bang, "!", parse_unary, nullptr, none)              \
  TOKEN_BINARY_OP(bang_equal, "!=", equality)  \
  TOKEN_NORULE(equal, "=")                 \
  TOKEN_BINARY_OP(double_equal, "==", equality) \
  TOKEN_NORULE(equal_right_arrow, "=>")    \
  TOKEN_BINARY_OP(greator, ">", comparison)    \
  TOKEN_NORULE(greator_greator, ">>")      \
  TOKEN_BINARY_OP(greater_equal, ">=", comparison) \
  TOKEN_BINARY_OP(less, "<", comparison)       \
  TOKEN_NORULE(less_less, "<<")            \
  TOKEN_BINARY_OP(less_equal, "<=", comparison) \
  TOKEN_TABLE_ENTRY(identifier, "identifier", parse_identifier, nullptr, none)   \
  TOKEN_NORULE(string_literal, "string")   \
  TOKEN_TABLE_ENTRY(number_literal, "number", parse_number, nullptr, none) \
  TOKEN_TABLE_ENTRY(keyword_and, "and", nullptr, nullptr, and)          \
  TOKEN_NORULE(keyword_async, "async")     \
  TOKEN_NORULE(keyword_await, "await")     \
  TOKEN_NORULE(keyword_case, "case")       \
  TOKEN_NORULE(keyword_class, "class")     \
  TOKEN_NORULE(keyword_def, "def")         \
  TOKEN_NORULE(keyword_else, "else")       \
  TOKEN_NORULE(keyword_extern, "extern")   \
  TOKEN_TABLE_ENTRY(keyword_false, "false", parse_literal, nullptr, none) \
  TOKEN_NORULE(keyword_for, "for")         \
  TOKEN_TABLE_ENTRY(keyword_if, "if", parse_branch, nullptr, none)      \
  TOKEN_NORULE(keyword_let, "let")         \
  TOKEN_NORULE(keyword_not, "not")         \
  TOKEN_TABLE_ENTRY(keyword_or, "or", nullptr, nullptr, or)             \
  TOKEN_NORULE(keyword_print, "print")     \
  TOKEN_NORULE(keyword_return, "return")   \
  TOKEN_NORULE(keyword_this, "this")       \
  TOKEN_TABLE_ENTRY(keyword_true, "true", parse_literal, nullptr, none) \
  TOKEN_TABLE_ENTRY(keyword_unit, "()", parse_literal, nullptr, none)   \
  TOKEN_NORULE(keyword_unsafe, "unsafe")   \
  TOKEN_NORULE(keyword_variant, "variant") \
  TOKEN_NORULE(error, "error")             \
  TOKEN_NORULE(eof, "eof")

TOKEN_TABLE
#undef TOKEN_TABLE
#undef TOKEN_NORULE
#undef TOKEN_BINARY_OP