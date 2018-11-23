#ifndef EML_AST_HPP
#define EML_AST_HPP

/**
 * @file ast.hpp
 * @brief This file contains the definition of the AST
 */

#include "value.hpp"
#include <memory>

namespace eml {

/**
 * @brief contains all the definitions related to the [Abstract
 * Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) of the
 * Embedded ML
 */
namespace ast {

struct Expr;

/// @brief Provides a wrapper of `std::make_unique` to its derived classes
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

class LiteralExpr;

template <detail::UnaryOpType optype> struct UnaryOpExprTemplate;
/// @brief AST Node for the unary negate operation (specializes @ref
/// UnaryOpExprTemplate)
using UnaryNegateExpr = UnaryOpExprTemplate<detail::UnaryOpType::negate>;
/// @brief AST Node for the unary not operation (specializes @ref
/// UnaryOpExprTemplate)
using UnaryNotExpr = UnaryOpExprTemplate<detail::UnaryOpType::not_op>;

template <detail::BinaryOpType optype> struct BinaryOpExprTemplate;
/// @brief AST Node for the binary plus operation (specializes @ref
/// BinaryOpExprTemplate)
using PlusOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::plus>;
/// @brief AST Node for the binary minus operation (specializes @ref
/// BinaryOpExprTemplate
using MinusOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::minus>;
/// @brief AST Node for the binary multiply operation (specializes @ref
/// BinaryOpExprTemplate
using MultOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::multiply>;
/// @brief AST Node for the binary divide operation (specializes @ref
/// BinaryOpExprTemplate
using DivOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::divide>;
/// @brief AST Node for the binary equal operation (specializes @ref
/// BinaryOpExprTemplate
using EqOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::equal>;
/// @brief AST Node for the binary not equal operation (specializes @ref
/// BinaryOpExprTemplate
using NeqOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::not_equal>;
/// @brief AST Node for the binary less operation (specializes @ref
/// BinaryOpExprTemplate
using LessOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::less>;
/// @brief AST Node for the binary less than or equal operation (specializes
/// @ref BinaryOpExprTemplate
using LeOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::less_equal>;
/// @brief AST Node for the binary greater operation (specializes @ref
/// BinaryOpExprTemplate
using GreaterOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::greater>;
/// @brief AST Node for the binary greater than or equal operation (specializes
/// @ref BinaryOpExprTemplate
using GeExpr = BinaryOpExprTemplate<detail::BinaryOpType::greater_equal>;

class ErrorExpr;

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

  virtual void operator()(const LiteralExpr& expr) = 0;
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

  [[noreturn]] void operator()(const ErrorExpr& /*expr*/)
  {
    // Unreachable
    std::clog << "Reach unreachable code\n";
    std::exit(1);
  }
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

  virtual void operator()(LiteralExpr& expr) = 0;
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

  [[noreturn]] void operator()(ErrorExpr& /*expr*/)
  {
    // Unreachable
    std::clog << "Reach unreachable code\n";
    std::exit(1);
  }
};

/**
 * @brief Base class for all the Expression AST Node
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
 * @brief A error node that represents with syntax error
 */
class ErrorExpr final : public Expr, public FactoryMixin<ErrorExpr> {
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
 * @brief A literal expression node of the AST is a Node contains a value
 */
class LiteralExpr final : public Expr, public FactoryMixin<LiteralExpr> {
  Value v_;

public:
  explicit LiteralExpr(Value v) : v_{v} {}
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
 * @brief Base class of all unary operations
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

/**
 * @brief The templates for all AST Nodes of unary operations
 *
 * Do not use this class directly, instead use one of the specializations
 * @see UnaryNegateExpr, UnaryNotExpr
 */
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

/**
 * @brief Base class for all Binary operations
 */
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

/**
 * @brief The templates for all AST Nodes of binary operations
 *
 * Do not use this class directly, instead use one of the specializations
 * @see PlusOpExpr, MinusOpExpr, MultOpExpr, DivOpExpr, EqOpExpr, NeqOpExpr,
 * LessOpExpr, LeOpExpr, GreaterOpExpr, GeExpr,
 */
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
