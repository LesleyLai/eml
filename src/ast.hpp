#include <utility>

#ifndef EML_AST_HPP
#define EML_AST_HPP

/**
 * @file ast.hpp
 * @brief contains all the definitions related to the [Abstract
 * Syntax Tree](https://en.wikipedia.org/wiki/Abstract_syntax_tree) of the
 * Embedded ML
 */

#include "common.hpp"
#include "type.hpp"
#include "value.hpp"

#include <memory>
#include <optional>
#include <vector>

namespace eml {

struct AstNode;
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
  append,
  equal,
  not_equal,
  less,
  less_equal,
  greater,
  greater_equal,
};

} // namespace detail

class Definition;

class LiteralExpr;
class IdentifierExpr;
class IfExpr;
class LambdaExpr;

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
/// @brief AST Node for the binary append operation (specializes @ref
/// BinaryOpExprTemplate)
using AppendOpExpr = BinaryOpExprTemplate<detail::BinaryOpType::append>;
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

/**
 * @brief Abstract syntax tree visitor, const version
 */
struct AstConstVisitor {
  AstConstVisitor() = default;
  virtual ~AstConstVisitor() = default;
  AstConstVisitor(const AstConstVisitor&) = default;
  AstConstVisitor& operator=(const AstConstVisitor&) = default;
  AstConstVisitor(AstConstVisitor&&) = default;
  AstConstVisitor& operator=(AstConstVisitor&&) = default;

  virtual void operator()(const LiteralExpr& expr) = 0;
  virtual void operator()(const IdentifierExpr& expr) = 0;
  virtual void operator()(const UnaryNegateExpr& expr) = 0;
  virtual void operator()(const UnaryNotExpr& expr) = 0;
  virtual void operator()(const PlusOpExpr& expr) = 0;
  virtual void operator()(const MinusOpExpr& expr) = 0;
  virtual void operator()(const MultOpExpr& expr) = 0;
  virtual void operator()(const DivOpExpr& expr) = 0;
  virtual void operator()(const AppendOpExpr& expr) = 0;
  virtual void operator()(const EqOpExpr& expr) = 0;
  virtual void operator()(const NeqOpExpr& expr) = 0;
  virtual void operator()(const LessOpExpr& expr) = 0;
  virtual void operator()(const LeOpExpr& expr) = 0;
  virtual void operator()(const GreaterOpExpr& expr) = 0;
  virtual void operator()(const GeExpr& expr) = 0;
  virtual void operator()(const IfExpr& def) = 0;
  virtual void operator()(const LambdaExpr& expr) = 0;

  virtual void operator()(const Definition& def) = 0;
};

/**
 * @brief Abstract syntax tree visitor, none-const version
 */
struct AstVisitor {
  AstVisitor() = default;
  virtual ~AstVisitor() = default;
  AstVisitor(const AstVisitor&) = default;
  AstVisitor& operator=(const AstVisitor&) = default;
  AstVisitor(AstVisitor&&) = default;
  AstVisitor& operator=(AstVisitor&&) = default;

  virtual void operator()(LiteralExpr& expr) = 0;
  virtual void operator()(IdentifierExpr& expr) = 0;
  virtual void operator()(UnaryNegateExpr& expr) = 0;
  virtual void operator()(UnaryNotExpr& expr) = 0;
  virtual void operator()(PlusOpExpr& expr) = 0;
  virtual void operator()(MinusOpExpr& expr) = 0;
  virtual void operator()(MultOpExpr& expr) = 0;
  virtual void operator()(DivOpExpr& expr) = 0;
  virtual void operator()(AppendOpExpr& expr) = 0;
  virtual void operator()(EqOpExpr& expr) = 0;
  virtual void operator()(NeqOpExpr& expr) = 0;
  virtual void operator()(LessOpExpr& expr) = 0;
  virtual void operator()(LeOpExpr& expr) = 0;
  virtual void operator()(GreaterOpExpr& expr) = 0;
  virtual void operator()(GeExpr& expr) = 0;
  virtual void operator()(IfExpr& def) = 0;
  virtual void operator()(LambdaExpr& expr) = 0;

  virtual void operator()(Definition& def) = 0;
};

struct AstNode {
  explicit AstNode(std::optional<Type> type = std::nullopt) : type_{type} {}
  virtual ~AstNode() = default;
  AstNode(const AstNode&) = delete;
  AstNode& operator=(const AstNode&) = delete;
  AstNode(AstNode&&) = default;
  AstNode& operator=(AstNode&&) = default;

  virtual void accept(AstConstVisitor& visitor) const = 0;
  virtual void accept(AstVisitor& visitor) = 0;

  /**
   * @brief Gets the type of the AST node
   * @warning If the expression does not have a type, the result is undefined
   */
  [[nodiscard]] auto type() const -> const Type&
  {
    EML_ASSERT(has_type(), "Must have a type");
    return *type_;
  }

  /**
   * @brief Returns true if the ast node have a type assigned
   */
  [[nodiscard]] auto has_type() const -> bool
  {
    return type_.has_value();
  }

  /**
   * @brief Sets the type of the AST node
   */
  void set_type(Type type)
  {
    type_ = type;
  }

private:
  std::optional<Type> type_ = std::nullopt;
};

namespace detail {
struct Let {
  std::string_view identifier;
  std::unique_ptr<Expr> to;
  std::optional<Type> type;
};
} // namespace detail

/**
 * @brief A definition is a top-level construct `let x = e` or `let x: T = e`
 */
class Definition : public AstNode, public FactoryMixin<Definition> {
public:
  Definition(std::string_view identifier, std::unique_ptr<Expr> to,
             std::optional<Type> type = {})
      : AstNode{UnitType{}}, binding_{identifier, std::move(to), type}
  {
  }

  [[nodiscard]] auto identifier() const -> std::string_view
  {
    return binding_.identifier;
  }

  void set_binding_type(Type type)
  {
    binding_.type = std::move(type);
  }

  /**
   * @brief Gets the type a definition bind to
   */
  [[nodiscard]] auto binding_type() const -> const std::optional<Type>&
  {
    return binding_.type;
  }

  [[nodiscard]] auto to() const -> Expr&
  {
    return *binding_.to;
  }

  void accept(AstVisitor& visitor) override
  {
    visitor(*this);
  }

  void accept(AstConstVisitor& visitor) const override
  {
    visitor(*this);
  }

private:
  detail::Let binding_;
};

/**
 * @brief Base class for all the Expression AST Node
 */
struct [[nodiscard]] Expr : AstNode {
public:
  Expr() = default;
  explicit Expr(Type type) : AstNode{type} {}
};

using Expr_ptr = std::unique_ptr<Expr>;

/**
 * @brief A literal expression node of the AST is a Node contains a value
 */
class LiteralExpr final : public Expr, public FactoryMixin<LiteralExpr> {
  Value v_;

  auto deduce_literal_type(const Value& v) -> Type
  {
    if (v.is_boolean()) {
      return BoolType{};
    } else if (v.is_number()) {
      return NumberType{};
    } else if (v.is_unit()) {
      return UnitType{};
    }

    EML_UNREACHABLE();
  }

public:
  explicit LiteralExpr(Value v) : Expr{deduce_literal_type(v)}, v_{v} {}

  LiteralExpr(Value v, Type t) : Expr{t}, v_{std::move(v)} {}
  auto value() const -> Value
  {
    return v_;
  }

  void accept(AstVisitor& visitor) override
  {
    visitor(*this);
  }

  void accept(AstConstVisitor& visitor) const override
  {
    visitor(*this);
  }
};

/**
 * @brief The IdentifierExpr is a wrapper for an identifier of an value.
 */
class IdentifierExpr final : public Expr, public FactoryMixin<IdentifierExpr> {
public:
  explicit IdentifierExpr(std::string name) : name_{std::move(name)} {}

  void accept(AstVisitor& visitor) override
  {
    visitor(*this);
  }

  void accept(AstConstVisitor& visitor) const override
  {
    visitor(*this);
  }

  std::string name() const
  {
    return name_;
  }

  /**
   * @brief Gets a value if it is already setted
   */
  std::optional<Value> value() const
  {
    return value_;
  }

  /**
   * @brief Sets the value of the Identifier bind to
   */
  void set_value(const std::optional<Value>& value)
  {
    value_ = value;
  }

private:
  std::string name_;
  std::optional<Value> value_;
};

/**
 * @brief The IfExpr represent a branch if ... [else if ...] else ...
 */
class IfExpr final : public Expr, public FactoryMixin<IfExpr> {
public:
  IfExpr(Expr_ptr cond, Expr_ptr If, Expr_ptr Else)
      : cond_{std::move(cond)}, if_(std::move(If)), else_(std::move(Else))
  {
  }

  /**
   * @brief Gets the condition of an if expression
   */
  auto cond() const noexcept -> const Expr&
  {
    return *cond_;
  }

  /**
   * @brief Gets the expression that if branch evaluate to
   */
  auto If() const noexcept -> const Expr&
  {
    return *if_;
  }

  /**
   * @brief Gets the expression that else branch evaluate to
   */
  auto Else() const noexcept -> const Expr&
  {
    return *else_;
  }

  /**
   * @brief Gets the condition of an if expression
   */
  auto cond() noexcept -> Expr&
  {
    return *cond_;
  }

  /**
   * @brief Gets the expression that if branch evaluate to
   */
  auto If() noexcept -> Expr&
  {
    return *if_;
  }

  /**
   * @brief Gets the expression that else branch evaluate to
   */
  auto Else() noexcept -> Expr&
  {
    return *else_;
  }

  void accept(AstVisitor& visitor) override
  {
    visitor(*this);
  }

  void accept(AstConstVisitor& visitor) const override
  {
    visitor(*this);
  }

private:
  Expr_ptr cond_;
  Expr_ptr if_;
  Expr_ptr else_;
};

/**
 * @brief Represents the AST node of a funcion definition
 */
class LambdaExpr final : public Expr, public FactoryMixin<LambdaExpr> {
public:
  LambdaExpr(std::vector<std::string> arguments, Expr_ptr expression)
      : args_{std::move(arguments)}, exprs_{std::move(expression)}
  {
    EML_ASSERT(exprs_ != nullptr,
               "Cannot create a lambda that evaluate to nothing");
  }

  void accept(AstVisitor& visitor) override
  {
    visitor(*this);
  }

  void accept(AstConstVisitor& visitor) const override
  {
    visitor(*this);
  }

  [[nodiscard]] auto arguments() const noexcept
      -> const std::vector<std::string>&
  {
    return args_;
  }

  [[nodiscard]] auto expression() const noexcept -> const Expr&
  {
    return *exprs_;
  }

private:
  std::vector<std::string> args_;
  Expr_ptr exprs_;
};

/**
 * @brief Base class of all unary operations
 */
class UnaryOpExpr : public Expr {
  Expr_ptr operand_;

public:
  explicit UnaryOpExpr(Expr_ptr operand) : operand_{std::move(operand)}
  {
    EML_ASSERT(operand_ != nullptr,
               "Operand of unary operation cannot be nullptr");
  }

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
struct UnaryOpExprTemplate final : UnaryOpExpr,
                                   FactoryMixin<UnaryOpExprTemplate<optype>> {
  explicit UnaryOpExprTemplate(Expr_ptr operand)
      : UnaryOpExpr{std::move(operand)}
  {
  }

  void accept(AstVisitor& visitor) override
  {
    visitor(*this);
  }

  void accept(AstConstVisitor& visitor) const override
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
    EML_ASSERT(lhs_ != nullptr, "Operand of unary operation cannot be nullptr");
    EML_ASSERT(rhs_ != nullptr, "Operand of unary operation cannot be nullptr");
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

  void accept(AstVisitor& visitor) final
  {
    visitor(*this);
  }

  void accept(AstConstVisitor& visitor) const override
  {
    visitor(*this);
  }
};

} // namespace eml

#endif // EML_AST_HPP
