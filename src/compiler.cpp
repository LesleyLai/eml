#include "compiler.hpp"
#include "parser.hpp"

namespace eml {

namespace {
struct Compiler : ast::ExprConstVisitor {
  explicit Compiler(chunk& chunk) : chunk_{&chunk} {}

  void emit_code(eml::opcode code)
  {
    chunk_->write(code, eml::line_num{0});
  }

  void emit_code(std::byte byte)
  {
    chunk_->write(byte, eml::line_num{0});
  }

  void operator()(const ast::LiteralExpr& constant) override
  {
    switch (constant.v().type) {
    case Value::type::Unit:
      emit_code(eml::op_unit);
      break;
    case Value::type::Boolean:
      if (constant.v().unsafe_as_boolean()) {
        emit_code(eml::op_true);
      } else {
        emit_code(eml::op_false);
      }
      break;
    case Value::type::Number:
      const double number = constant.v().unsafe_as_number();
      const auto offset = chunk_->add_constant(eml::Value{number});

      emit_code(eml::op_push);
      emit_code(std::byte{*offset});
      break;
    }
  }

  void unary_common(const ast::UnaryOpExpr& expr, opcode op)
  {
    expr.operand().accept(*this);
    emit_code(op);
  }

  void operator()(const ast::UnaryNegateExpr& expr) override
  {
    unary_common(expr, op_negate);
  }

  void operator()(const ast::UnaryNotExpr& expr) override
  {
    unary_common(expr, op_not);
  }

  void binary_common(const ast::BinaryOpExpr& expr, opcode op)
  {
    expr.lhs().accept(*this);
    expr.rhs().accept(*this);
    emit_code(op);
  }

  void operator()(const ast::PlusOpExpr& expr) override
  {
    binary_common(expr, op_add);
  }
  void operator()(const ast::MinusOpExpr& expr) override
  {
    binary_common(expr, op_subtract);
  }
  void operator()(const ast::MultOpExpr& expr) override
  {
    binary_common(expr, op_multiply);
  }
  void operator()(const ast::DivOpExpr& expr) override
  {
    binary_common(expr, op_divide);
  }
  void operator()(const ast::EqOpExpr& expr) override
  {
    binary_common(expr, op_equal);
  }
  void operator()(const ast::NeqOpExpr& expr) override
  {
    binary_common(expr, op_not_equal);
  }
  void operator()(const ast::LessOpExpr& expr) override
  {
    binary_common(expr, op_less);
  }
  void operator()(const ast::LeOpExpr& expr) override
  {
    binary_common(expr, op_less_equal);
  }
  void operator()(const ast::GreaterOpExpr& expr) override
  {
    binary_common(expr, op_greater);
  }
  void operator()(const ast::GeExpr& expr) override
  {
    binary_common(expr, op_greater_equal);
  }

private:
  chunk* chunk_; // Not null
};
} // anonymous namespace

auto bytecode_from_ast(const ast::Expr& expr) -> chunk
{
  chunk code;
  Compiler compiler{code};
  expr.accept(compiler);
  return code;
}

} // namespace eml
