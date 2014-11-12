#ifndef HALIDE_BRANCH_UTILS_H
#define HALIDE_BRANCH_UTILS_H

/** \file
 * Defines several IR mutators and visitors that detect and modify the branching structure
 * of the IR. These are mostly used by specialize_branched_loops.
 */

#include "IR.h"
#include "Bounds.h"
#include "Scope.h"

namespace Halide {
namespace Internal {

/**
 * Returns true if [stmt] or [expr] branches linearly in the variable [var] or any of
 * the variables in the scope [free_vars]. By which we mean that the expression or
 * statement contains a branch whose condition depends linearly on the named variable
 * or any of the variables in the scope [free_vars]. The optional scope argument
 * [bound_vars] should contain entries for each bound variable that may appear in [stmt]
 * or [expr], and have its value set to true if the variable is bound to a linear expression
 * of the free variables, and false otherwise.
 */
// @(
EXPORT bool branches_linearly_in_var(Stmt stmt, const std::string &var,
                                     bool branch_on_minmax = false);

EXPORT bool branches_linearly_in_var(Expr expr, const std::string &var,
                                     bool branch_on_minmax = false);

EXPORT bool branches_linearly_in_var(Stmt stmt, const std::string &var,
                                     const Scope<bool> &bound_vars,
                                     bool branch_on_minmax = false);

EXPORT bool branches_linearly_in_var(Expr expr, const std::string &var,
                                     const Scope<bool> &bound_vars,
                                     bool branch_on_minmax = false);

EXPORT bool branches_linearly_in_vars(Stmt stmt,
                                      const Scope<int> &free_vars,
                                      bool branch_on_minmax = false);

EXPORT bool branches_linearly_in_vars(Expr expr,
                                      const Scope<int> &free_vars,
                                      bool branch_on_minmax = false);

EXPORT bool branches_linearly_in_vars(Stmt stmt,
                                      const Scope<int> &free_vars,
                                      const Scope<bool> &bound_vars,
                                      bool branch_on_minmax = false);

EXPORT bool branches_linearly_in_vars(Expr expr,
                                      const Scope<int> &free_vars,
                                      const Scope<bool> &bound_vars,
                                      bool branch_on_minmax = false);
// @}

/**
 * Normalize the branching conditions in IfThenElse and Select
 * nodes. By which we mean, reduce the condition to a simple
 * inequality expression if possible. Equality/inequality conditions
 * are converted into compound expressions involving inequalities and
 * all logical connectives are removed from the conditions. We end up
 * with a nested tree of branches, which can then be pruned by
 * [prune_branches]. Here is an example:
 *
 *   if (x <= 10 && x != 5) {
 *      ...
 *   }
 *
 * will get mutated into:
 *
 *   if (x <= 10) {
 *     if (x < 5) {
 *       ...
 *     } else if (x > 5) {
 *       ...
 *     }
 *  }
 *
 */
// @{
EXPORT Stmt normalize_branch_conditions(Stmt stmt, const Scope<Expr> &scope, const int branching_limit = 10);
EXPORT Expr normalize_branch_conditions(Expr expr, const Scope<Expr> &scope, const int branching_limit = 10);
// @}

/**
 * Prune the branches in [stmt] or [expr] relative to the variable
 * [var], considering the bounds provided by [bounds].  Branching
 * conditions are used to modify the bounds on [var], and thus we can
 * potentially reduce some of the nested branching structure. Here is
 * an example:
 *
 *   if (x < 0) {
 *      if (x < 1) {
 *          ...
 *      }
 *   }
 *
 * Will be reduced to:
 *
 *   if (x < 0) {
 *     ...
 *   }
 *
 * The final argument [vars] is a scope containing all the free variables.
 */
// @{
EXPORT Stmt prune_branches(Stmt stmt, const std::string &var, const Scope<Expr> &scope,
                           const Scope<Interval> &bounds, const Scope<int> &vars);

EXPORT Expr prune_branches(Expr expr, const std::string &var, const Scope<Expr> &scope,
                           const Scope<Interval> &bounds, const Scope<int> &vars);
// @}

}
}

#endif
