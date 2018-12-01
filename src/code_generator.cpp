#include "compiler.hpp"

namespace eml {

namespace {
struct CodeGenerator : ast::AstConstVisitor {
  explicit CodeGenerator(Bytecode& chunk, Compiler& compiler)
      : chunk_{chunk}, compiler_{compiler}
  {
  }

  void emit_code(eml::opcode code)
  {
    chunk_.write(code, eml::line_num{0});
  }

  void emit_code(std::byte byte)
  {
    chunk_.write(byte, eml::line_num{0});
  }

  void operator()(const ast::LiteralExpr& constant) override
  {
    switch (constant.value().type) {
    case Value::type::Unit:
      emit_code(eml::op_unit);
      break;
    case Value::type::Boolean:
      if (constant.value().unsafe_as_boolean()) {
        emit_code(eml::op_true);
      } else {
        emit_code(eml::op_false);
      }
      break;
    case Value::type::Number:
      const double number = constant.value().unsafe_as_number();
      const auto offset = chunk_.add_constant(eml::Value{number});

      emit_code(eml::op_push_f64);
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

  void operator()(const ast::Definition& def) override
  {
    const auto identifier = def.identifier();
    const auto& to = def.to();

    // TODO(Lesley Lai): implement constant folding
    try {
      const auto& v = dynamic_cast<const ast::LiteralExpr&>(to);

      compiler_.add_global(identifier, *def.binding_type(), v.value());
    } catch (std::exception& e) {
      std::clog << e.what() << '\n';
      std::clog << "Constant folding is not implemented yet!!!\n";
    }
  }

private:
  Bytecode& chunk_; // Not null
  Compiler& compiler_;
};
} // anonymous namespace

auto Compiler::bytecode_from_ast(const ast::AstNode& expr) -> Bytecode
{
  Bytecode code;
  CodeGenerator code_generator{code, *this};
  expr.accept(code_generator);
  return code;
}

} // namespace eml
