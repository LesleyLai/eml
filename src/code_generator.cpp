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
  friend TypeDispatcher;

  explicit CodeGenerator(Bytecode& chunk, const Compiler& compiler)
      : chunk_{chunk}, compiler_{compiler}
  {
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
    chunk_.write(op, line_num{0});
  }

  void operator()(const ast::UnaryNegateExpr& expr) override
  {
    unary_common(expr, op_negate_f64);
  }

  void operator()(const ast::UnaryNotExpr& expr) override
  {
    unary_common(expr, op_not);
  }

  void binary_common(const ast::BinaryOpExpr& expr, opcode op)
  {
    expr.lhs().accept(*this);
    expr.rhs().accept(*this);
    chunk_.write(op, line_num{0});
  }

  void operator()(const ast::PlusOpExpr& expr) override
  {
    binary_common(expr, op_add_f64);
  }
  void operator()(const ast::MinusOpExpr& expr) override
  {
    binary_common(expr, op_subtract_f64);
  }
  void operator()(const ast::MultOpExpr& expr) override
  {
    binary_common(expr, op_multiply_f64);
  }
  void operator()(const ast::DivOpExpr& expr) override
  {
    binary_common(expr, op_divide_f64);
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
    binary_common(expr, op_less_f64);
  }
  void operator()(const ast::LeOpExpr& expr) override
  {
    binary_common(expr, op_less_equal_f64);
  }
  void operator()(const ast::GreaterOpExpr& expr) override
  {
    binary_common(expr, op_greater_f64);
  }
  void operator()(const ast::GeExpr& expr) override
  {
    binary_common(expr, op_greater_equal_f64);
  }

  // Emits [instruction] followed by a placeholder for a jump offset. The
  // placeholder can be patched by calling [jumpPatch]. Returns the index of the
  // placeholder.
  auto write_jump(eml::opcode jump_instruction, line_num linum)
      -> std::ptrdiff_t
  {
    chunk_.write(jump_instruction, linum);
    const auto jump = chunk_.write(std::byte{}, linum);
    return jump;
  }

  // Replaces the placeholder argument for a previous jump
  // instruction with an offset that jumps to the current end of bytecode.
  void jump_patch(std::ptrdiff_t index)
  {
    const auto jump_to = chunk_.next_instruction_index();
    chunk_.write_at(static_cast<std::byte>(jump_to - index - 1), index);
  }

  void operator()(const ast::IfExpr& expr) override
  {
    EML_ASSERT(expr.cond().type() == BoolType{},
               "Type of condition must be boolean");
    EML_ASSERT(expr.If().type() != std::nullopt, "Branches must have a type");
    EML_ASSERT(expr.If().type() == expr.Else().type(),
               "Type of different branches must match");

    expr.cond().accept(*this);
    const auto else_jump_pos = write_jump(eml::op_jmp_false, line_num{0});

    expr.If().accept(*this);

    const auto if_jump_pos = write_jump(eml::op_jmp, line_num{0});

    jump_patch(else_jump_pos);

    expr.Else().accept(*this);

    jump_patch(if_jump_pos);
  }

  void operator()(const ast::Definition& /*def*/) override {} // no-op

  Bytecode& chunk_; // Not null
  const Compiler& compiler_;
};

void TypeDispatcher::operator()(const NumberType&)
{
  const double number = v.unsafe_as_number();
  const auto offset = generator.chunk_.add_constant(eml::Value{number});

  generator.chunk_.write(eml::op_push_f64, line_num{0});
  generator.chunk_.write(std::byte{*offset}, line_num{0});
}

void TypeDispatcher::operator()(const BoolType&)
{
  if (v.unsafe_as_boolean()) {
    generator.chunk_.write(eml::op_true, line_num{0});
  } else {
    generator.chunk_.write(eml::op_false, line_num{0});
  }
}

void TypeDispatcher::operator()(const UnitType&)
{
  generator.chunk_.write(eml::op_unit, line_num{0});
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
