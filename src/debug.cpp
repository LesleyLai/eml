#include <sstream>
#include <string_view>

#include "ast.hpp"
#include "debug.hpp"

namespace eml {

namespace {
struct AstPrinter : AstConstVisitor {
public:
  explicit AstPrinter(AstPrintOption option) noexcept : print_option_{option} {}

  void operator()(const LiteralExpr& constant) override
  {
    ss_ << eml::to_string(constant.type(), constant.value(), PrintType::no);
  }

  void operator()(const IdentifierExpr& id) override
  {
    ss_ << id.name();
  }

  void unary_common(const UnaryOpExpr& expr, char op)
  {
    ss_ << '(' << op << ' ';
    expr.operand().accept(*this);
    ss_ << ')';
  }

  void operator()(const UnaryNegateExpr& expr) override
  {
    unary_common(expr, '-');
  }

  void operator()(const UnaryNotExpr& expr) override
  {
    unary_common(expr, '!');
  }

  void binary_common(const BinaryOpExpr& expr, std::string_view op)
  {
    ss_ << '(' << op << ' ';
    expr.lhs().accept(*this);
    ss_ << ' ';
    expr.rhs().accept(*this);
    ss_ << ')';
  }

  void operator()(const PlusOpExpr& expr) override
  {
    binary_common(expr, "+");
  }
  void operator()(const MinusOpExpr& expr) override
  {
    binary_common(expr, "-");
  }
  void operator()(const MultOpExpr& expr) override
  {
    binary_common(expr, "*");
  }
  void operator()(const DivOpExpr& expr) override
  {
    binary_common(expr, "/");
  }
  void operator()(const EqOpExpr& expr) override
  {
    binary_common(expr, "==");
  }
  void operator()(const NeqOpExpr& expr) override
  {
    binary_common(expr, "!=");
  }
  void operator()(const LessOpExpr& expr) override
  {
    binary_common(expr, "<");
  }
  void operator()(const LeOpExpr& expr) override
  {
    binary_common(expr, "<=");
  }
  void operator()(const GreaterOpExpr& expr) override
  {
    binary_common(expr, ">");
  }

  void operator()(const GeExpr& expr) override
  {
    binary_common(expr, ">=");
  }

  void operator()(const IfExpr& expr) override
  {
    ss_ << "(if ";
    expr.cond().accept(*this);
    if (print_option_ == AstPrintOption::pretty) {
      ss_ << "\n    ";
    } else {
      ss_ << ' ';
    }
    expr.If().accept(*this);
    ss_ << ' ';
    expr.Else().accept(*this);
    ss_ << ')';
  }

  void operator()(const LambdaExpr& expr) override
  {
    ss_ << "(lambda ";
    for (const auto& arg : expr.arguments()) {
      ss_ << arg << ' ';
    }

    if (print_option_ == AstPrintOption::pretty) {
      ss_ << "\n    ";
    }
    expr.expression().accept(*this);

    if (print_option_ == AstPrintOption::pretty) {
      ss_ << "\n    ";
    }
    ss_ << ")";
  }

  void operator()(const Definition& def) override
  {
    ss_ << "(let " << def.identifier() << ' ';
    if (print_option_ == AstPrintOption::pretty) {
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
  AstPrintOption print_option_;
};

} // namespace

std::string to_string(const eml::AstNode& node, AstPrintOption print_option)
{
  AstPrinter printer{print_option};
  node.accept(printer);
  return printer.to_string();
}

} // namespace eml
