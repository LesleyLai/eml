#ifndef EML_AST_HPP
#define EML_AST_HPP

#include "value.hpp"
#include <memory>

namespace eml {

namespace ast {

struct Expr;

template <typename Derived> struct FactoryMixin {
  /**
   * @brief A factory member function that creates a `std::unique_ptr` of itself
   */
  template <typename... Args>
  static auto create(Args&&... args) -> std::unique_ptr<Derived>
  {
    return std::make_unique<Derived>(std::forward<Args>(args)...);
  }
};

namespace detail {
enum class UnaryOpType {
  negate,
  not_op,
};

enum class BinaryOpType {
  plus,
  minus,
  multiply,
  divide,
  equal,
  not_equal,
  less,
  less_equal,
  greater,
  greater_equal,
};

}; // namespace detail

class ConstExpr;

template <detail::UnaryOpType optype> struct UnaryOpExprTemplate;
using UnaryNegateExpr = UnaryOpExprTemplate<detail::UnaryOpType::negate>;
using UnaryNotExpr = UnaryOpExprTemplate<detail::UnaryOpType::not_op>;

template <detail::BinaryOpType optype> struct BinaryOpExprTemplate;
using PlusOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::plus>;
using MinusOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::minus>;
using MultOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::multiply>;
using DivOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::divide>;
using EqOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::equal>;
using NeqOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::not_equal>;
using LessOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::less>;
using LeOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::less_equal>;
using GreaterOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::greater>;
using GeExpr = BinaryOpExprTemplate<detail::BinaryOpType::greater_equal>;

/**
 * @brief Expression visitor, const version
 */
struct ExprConstVisitor {
  ExprConstVisitor() = default;
  virtual ~ExprConstVisitor() = default;
  ExprConstVisitor(const ExprConstVisitor&) = default;
  ExprConstVisitor& operator=(const ExprConstVisitor&) = default;
  ExprConstVisitor(ExprConstVisitor&&) = default;
  ExprConstVisitor& operator=(ExprConstVisitor&&) = default;

  virtual void operator()(const ConstExpr& expr) = 0;
  virtual void operator()(const UnaryNegateExpr& expr) = 0;
  virtual void operator()(const UnaryNotExpr& expr) = 0;
  virtual void operator()(const PlusOpExpr& expr) = 0;
  virtual void operator()(const MinusOpExpr& expr) = 0;
  virtual void operator()(const MultOpExpr& expr) = 0;
  virtual void operator()(const DivOpExpr& expr) = 0;
  virtual void operator()(const EqOpExpr& expr) = 0;
  virtual void operator()(const NeqOpExpr& expr) = 0;
  virtual void operator()(const LessOpExpr& expr) = 0;
  virtual void operator()(const LeOpExpr& expr) = 0;
  virtual void operator()(const GreaterOpExpr& expr) = 0;
  virtual void operator()(const GeExpr& expr) = 0;
};

/**
 * @brief Expression visitor, none-const version
 */
struct ExprVisitor {
  ExprVisitor() = default;
  virtual ~ExprVisitor() = default;
  ExprVisitor(const ExprVisitor&) = default;
  ExprVisitor& operator=(const ExprVisitor&) = default;
  ExprVisitor(ExprVisitor&&) = default;
  ExprVisitor& operator=(ExprVisitor&&) = default;

  virtual void operator()(ConstExpr& expr) = 0;
  virtual void operator()(UnaryNegateExpr& expr) = 0;
  virtual void operator()(UnaryNotExpr& expr) = 0;
  virtual void operator()(PlusOpExpr& expr) = 0;
  virtual void operator()(MinusOpExpr& expr) = 0;
  virtual void operator()(MultOpExpr& expr) = 0;
  virtual void operator()(DivOpExpr& expr) = 0;
  virtual void operator()(EqOpExpr& expr) = 0;
  virtual void operator()(NeqOpExpr& expr) = 0;
  virtual void operator()(LessOpExpr& expr) = 0;
  virtual void operator()(LeOpExpr& expr) = 0;
  virtual void operator()(GreaterOpExpr& expr) = 0;
  virtual void operator()(GeExpr& expr) = 0;
};

/**
 * @brief Expression AST Node
 */
struct Expr {
  Expr() = default;
  virtual ~Expr() = default;
  Expr(const Expr&) = delete;
  Expr& operator=(const Expr&) = delete;
  Expr(Expr&&) = default;
  Expr& operator=(Expr&&) = default;

  virtual void accept(ExprConstVisitor& visitor) const = 0;
  virtual void accept(ExprVisitor& visitor) = 0;
};

using Expr_ptr = std::unique_ptr<Expr>;

/**
 * @brief A expr expression node of the AST
 */
class ConstExpr final : public Expr, public FactoryMixin<ConstExpr> {
  Value v_;

public:
  explicit ConstExpr(Value v) : v_{v} {}
  Value v() const
  {
    return v_;
  }

  void accept(ExprVisitor& visitor) override
  {
    visitor(*this);
  }

  void accept(ExprConstVisitor& visitor) const override
  {
    visitor(*this);
  }
};

/**
 * @brief The Unary_operation struct
 */
class UnaryOp : public Expr {
  Expr_ptr operand_;

public:
  explicit UnaryOp(Expr_ptr operand) : operand_{std::move(operand)} {}

  Expr& operand() const
  {
    return *operand_;
  }
};

template <detail::UnaryOpType optype>
struct UnaryOpExprTemplate final : UnaryOp,
                                   FactoryMixin<UnaryOpExprTemplate<optype>> {
  explicit UnaryOpExprTemplate(Expr_ptr operand) : UnaryOp{std::move(operand)}
  {
  }

  void accept(ExprVisitor& visitor) override
  {
    visitor(*this);
  }

  void accept(ExprConstVisitor& visitor) const override
  {
    visitor(*this);
  }
};

class BinaryOpExpr : public Expr {
  Expr_ptr lhs_;
  Expr_ptr rhs_;

public:
  explicit BinaryOpExpr(Expr_ptr lhs, Expr_ptr rhs)
      : lhs_{std::move(lhs)}, rhs_{std::move(rhs)}
  {
  }

  Expr& lhs() const
  {
    return *lhs_;
  }

  Expr& rhs() const
  {
    return *rhs_;
  }
};

template <detail::BinaryOpType optype>
struct BinaryOpExprTemplate final : BinaryOpExpr,
                                    FactoryMixin<BinaryOpExprTemplate<optype>> {
  explicit BinaryOpExprTemplate(Expr_ptr lhs, Expr_ptr rhs)
      : BinaryOpExpr{std::move(lhs), std::move(rhs)}
  {
  }

  void accept(ExprVisitor& visitor) final
  {
    visitor(*this);
  }

  void accept(ExprConstVisitor& visitor) const override
  {
    visitor(*this);
  }
};

} // namespace ast
} // namespace eml

#endif // EML_AST_HPP
