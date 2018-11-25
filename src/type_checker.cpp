#include "type_checker.hpp"

#include <iomanip>
#include <sstream>

namespace eml {

struct Func1Type {
  Type arg_type;
  Type result_type;
};

struct Func2Type {
  Type arg1_type;
  Type arg2_type;
  Type result_type;
};

namespace {
struct TypeChecker : ast::ExprVisitor {
  bool has_error = false;
  bool panic_mode = false;
  std::vector<CompilationError> errors;

  void operator()(ast::LiteralExpr& constant) override
  {
    // no-op
    EML_ASSERT(constant.type() != std::nullopt,
               "All literal should have a type assigned from the parser");
  }

  void unary_common(ast::UnaryOpExpr& expr, std::string_view op,
                    const Func1Type& allowed_type)
  {
    if (expr.type()) {
      return;
    }

    expr.operand().accept(*this);
    if (expr.operand().type() == allowed_type.arg_type) {
      expr.set_type(allowed_type.result_type);
    } else {
      if (!panic_mode) {
        std::stringstream ss;
        ss << "Unmatched types around of unary operator " << op << '\n';
        ss << std::left << "Requires " << op << " " << std::setw(8)
           << allowed_type.arg_type << '\n';
        ss << std::left << "Has      " << op << " " << std::setw(8)
           << *expr.operand().type() << '\n';
        error(ss.str());
      }
    }
  }

  void operator()(ast::UnaryNegateExpr& expr) override
  {
    unary_common(expr, "-", Func1Type{NumberType{}, NumberType{}});
  }

  void operator()(ast::UnaryNotExpr& expr) override
  {
    unary_common(expr, "!", Func1Type{BoolType{}, BoolType{}});
  }

  void error(std::string message)
  {
    if (panic_mode) {
      return;
    }
    has_error = true;
    panic_mode = true;

    errors.emplace_back(std::in_place_type<TypeError>, message);
  }

  void binary_common(ast::BinaryOpExpr& expr, std::string_view op,
                     const Func2Type& allowed_type)
  {
    if (expr.type()) {
      return;
    }

    expr.lhs().accept(*this);
    expr.rhs().accept(*this);
    if (expr.lhs().type() == allowed_type.arg1_type &&
        expr.rhs().type() == allowed_type.arg2_type) {
      expr.set_type(allowed_type.result_type);
    } else {
      if (!panic_mode) {
        std::stringstream ss;
        ss << "Unmatched types around binary operator" << op << '\n';
        ss << std::left << "Requires " << std::setw(8) << allowed_type.arg1_type
           << std::setw(3) << op << std::setw(8) << allowed_type.arg2_type
           << '\n';
        ss << "Has      " << std::setw(8) << *expr.lhs().type() << std::setw(3)
           << op << std::setw(8) << *expr.rhs().type() << '\n';
        error(ss.str());
      }
    }
  }

  void equality_common(ast::BinaryOpExpr& expr, std::string_view op)
  {
    expr.lhs().accept(*this);
    expr.rhs().accept(*this);
    if (expr.lhs().type() == expr.rhs().type()) {
      expr.set_type(BoolType{});
    } else {
      if (!panic_mode) {
        std::stringstream ss;
        ss << "Unmatched types around comparison operator" << op << '\n';
        ss << "Requires "
           << "T " << op << " T\n";
        ss << "where T: EqualityComparable\n";
        ss << "Has " << *expr.lhs().type() << " " << op << " "
           << *expr.rhs().type() << '\n';
        error(ss.str());
      }
    }
  }

  void operator()(ast::PlusOpExpr& expr) override
  {
    binary_common(expr, "+",
                  Func2Type{NumberType{}, NumberType{}, NumberType{}});
  }
  void operator()(ast::MinusOpExpr& expr) override
  {
    binary_common(expr, "-",
                  Func2Type{NumberType{}, NumberType{}, NumberType{}});
  }
  void operator()(ast::MultOpExpr& expr) override
  {
    binary_common(expr, "*",
                  Func2Type{NumberType{}, NumberType{}, NumberType{}});
  }
  void operator()(ast::DivOpExpr& expr) override
  {
    binary_common(expr, "/",
                  Func2Type{NumberType{}, NumberType{}, NumberType{}});
  }
  void operator()(ast::EqOpExpr& expr) override
  {
    equality_common(expr, "==");
  }
  void operator()(ast::NeqOpExpr& expr) override
  {
    equality_common(expr, "!=");
  }
  void operator()(ast::LessOpExpr& expr) override
  {
    binary_common(expr, "<", Func2Type{NumberType{}, NumberType{}, BoolType{}});
  }
  void operator()(ast::LeOpExpr& expr) override
  {
    binary_common(expr,
                  "<=", Func2Type{NumberType{}, NumberType{}, BoolType{}});
  }
  void operator()(ast::GreaterOpExpr& expr) override
  {
    binary_common(expr, ">", Func2Type{NumberType{}, NumberType{}, BoolType{}});
  }
  void operator()(ast::GeExpr& expr) override
  {
    binary_common(expr,
                  ">=", Func2Type{NumberType{}, NumberType{}, BoolType{}});
  }
}; // namespace
} // namespace

TypeCheckResult type_check(ast::Expr_ptr& ptr)
{
  TypeChecker type_checker{};
  ptr->accept(type_checker);
  if (!type_checker.has_error) {
    return std::move(ptr);
  } else {
    return unexpected{std::move(type_checker.errors)};
  }
}

} // namespace eml
