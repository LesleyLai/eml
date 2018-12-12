#include <sstream>
#include <string_view>

#include "ast.hpp"
#include "debug.hpp"

namespace eml {

namespace {
struct AstPrinter : ast::AstConstVisitor {
public:
  explicit AstPrinter(ast::PrintOption option) noexcept : print_option_{option}
  {
  }

  void operator()(const ast::LiteralExpr& constant) override
  {
    ss_ << eml::to_string(constant.value(), PrintType::no);
  }

  void operator()(const ast::IdentifierExpr& id) override
  {
    ss_ << id.name();
  }

  void unary_common(const ast::UnaryOpExpr& expr, char op)
  {
    ss_ << '(' << op << ' ';
    expr.operand().accept(*this);
    ss_ << ')';
  }

  void operator()(const ast::UnaryNegateExpr& expr) override
  {
    unary_common(expr, '-');
  }

  void operator()(const ast::UnaryNotExpr& expr) override
  {
    unary_common(expr, '!');
  }

  void binary_common(const ast::BinaryOpExpr& expr, std::string_view op)
  {
    ss_ << '(' << op << ' ';
    expr.lhs().accept(*this);
    ss_ << ' ';
    expr.rhs().accept(*this);
    ss_ << ')';
  }

  void operator()(const ast::PlusOpExpr& expr) override
  {
    binary_common(expr, "+");
  }
  void operator()(const ast::MinusOpExpr& expr) override
  {
    binary_common(expr, "-");
  }
  void operator()(const ast::MultOpExpr& expr) override
  {
    binary_common(expr, "*");
  }
  void operator()(const ast::DivOpExpr& expr) override
  {
    binary_common(expr, "/");
  }
  void operator()(const ast::EqOpExpr& expr) override
  {
    binary_common(expr, "==");
  }
  void operator()(const ast::NeqOpExpr& expr) override
  {
    binary_common(expr, "!=");
  }
  void operator()(const ast::LessOpExpr& expr) override
  {
    binary_common(expr, "<");
  }
  void operator()(const ast::LeOpExpr& expr) override
  {
    binary_common(expr, "<=");
  }
  void operator()(const ast::GreaterOpExpr& expr) override
  {
    binary_common(expr, ">");
  }

  void operator()(const ast::GeExpr& expr) override
  {
    binary_common(expr, ">=");
  }

  void operator()(const ast::IfExpr& expr) override
  {
    ss_ << "(if ";
    expr.cond().accept(*this);
    if (print_option_ == ast::PrintOption::pretty) {
      ss_ << "\n    ";
    } else {
      ss_ << ' ';
    }
    expr.If().accept(*this);
    ss_ << ' ';
    expr.Else().accept(*this);
    ss_ << ')';
  }

  void operator()(const ast::LambdaExpr& expr) override
  {
    ss_ << "(lambda ";
    for (const auto& arg : expr.arguments()) {
      ss_ << arg << ' ';
    }

    if (print_option_ == ast::PrintOption::pretty) {
      ss_ << "\n    ";
    }
    expr.expression().accept(*this);

    if (print_option_ == ast::PrintOption::pretty) {
      ss_ << "\n    ";
    }
    ss_ << ")";
  }

  void operator()(const ast::Definition& def) override
  {
    ss_ << "(let " << def.identifier() << ' ';
    if (print_option_ == ast::PrintOption::pretty) {
      ss_ << "\n  ";
    }
    def.to().accept(*this);
    ss_ << ')';
  }

  std::string to_string()
  {
    return ss_.str();
  }

private:
  std::stringstream ss_;
  ast::PrintOption print_option_;
}; // namespace
} // namespace

std::string to_string(const eml::ast::AstNode& node,
                      ast::PrintOption print_option)
{
  AstPrinter printer{print_option};
  node.accept(printer);
  return printer.to_string();
}

} // namespace eml
