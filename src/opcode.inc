/**
 * @file opcode.inc
 * @brief This file contains a table of the opcodes for the Embedded ML vm
 */

#define OPCODE_TABLE                                                  \
  OPCODE(return, -1)                                                  \
  OPCODE(push_f64, 1) /*Pushes a float_64 to the stack*/              \
  OPCODE(pop, -1)  /*Pops the top value of the stack*/                \
                                                                      \
  OPCODE(true, 1)  /*Pushes true to the stack*/                       \
  OPCODE(false, 1) /*Pushes false to the stack*/                      \
  OPCODE(unit, 1)  /*Pushes unit to the stack*/                       \
                                                                      \
   /*Unary Arithmatics*/                                              \
  OPCODE(negate, 0)                                                   \
  OPCODE(not, 0)                                                      \
                                                                      \
   /*Binary Arithmatics*/                                             \
  OPCODE(add, -1)                                                     \
  OPCODE(subtract, -1)                                                \
  OPCODE(multiply, -1)                                                \
  OPCODE(divide, -1)                                                  \
                                                                      \
   /*Comparisons*/                                                    \
  OPCODE(equal, -1)                                                   \
  OPCODE(not_equal, -1)                                               \
  OPCODE(less, -1)                                                    \
  OPCODE(less_equal, -1)                                              \
  OPCODE(greater, -1)                                                 \
  OPCODE(greater_equal, -1)                                           \

OPCODE_TABLE
#undef OPCODE_TABLE
