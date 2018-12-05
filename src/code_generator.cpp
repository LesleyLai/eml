#include "ast.hpp"
#include "compiler.hpp"

namespace eml {

namespace {

struct CodeGenerator;

// Emit different push instructions depends on they of an expression
struct TypeDispatcher {
  CodeGenerator& generator;
  Value v;

  void operator()(const NumberType& /*t*/);

  void operator()(const BoolType& /*t*/);

  void operator()(const UnitType& /*t*/);
};

struct CodeGenerator : ast::AstConstVisitor {
  explicit CodeGenerator(Bytecode& chunk, const Compiler& compiler)
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
    TypeDispatcher visitor{*this, constant.value()};
    std::visit(visitor, *constant.type());
  }

  void operator()([[maybe_unused]] const ast::IdentifierExpr& id) override
  {
    EML_ASSERT(id.type() != std::nullopt,
               "Identifier expression passed to the code generator are "
               "garanteed to have a type");
    EML_ASSERT(id.value() != std::nullopt,
               "Identifier expression passed to the code generator are "
               "garanteed to have a value");
    TypeDispatcher visitor{*this, *id.value()};
    std::visit(visitor, *id.type());
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

  void operator()(const ast::Definition& /*def*/) override
  {
    // no-op
  }

  Bytecode& chunk_; // Not null
  const Compiler& compiler_;
};

void TypeDispatcher::operator()(const NumberType&)
{
  const double number = v.unsafe_as_number();
  const auto offset = generator.chunk_.add_constant(eml::Value{number});

  generator.emit_code(eml::op_push_f64);
  generator.emit_code(std::byte{*offset});
}

void TypeDispatcher::operator()(const BoolType&)
{
  if (v.unsafe_as_boolean()) {
    generator.emit_code(eml::op_true);
  } else {
    generator.emit_code(eml::op_false);
  }
}

void TypeDispatcher::operator()(const UnitType&)
{
  generator.emit_code(eml::op_unit);
}

} // anonymous namespace

auto Compiler::bytecode_from_ast(const ast::AstNode& expr) const -> Bytecode
{
  Bytecode code;
  CodeGenerator code_generator{code, *this};
  expr.accept(code_generator);
  return code;
}

} // namespace eml
