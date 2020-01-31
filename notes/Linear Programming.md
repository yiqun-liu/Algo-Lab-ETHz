# CGAL: Linear Programming

## Formulation

**Linear Program**

- **n** variables: x<sub>1</sub>, x<sub>1</sub>, ... x<sub>n</sub>
- **m** linear constraints: 
  - a<sub>r,1</sub> x<sub>1</sub> + a<sub>r,2</sub> x<sub>2</sub> + ... + a<sub>r,n</sub> x<sub>n</sub> &le; b<sub>r</sub>,  (0 &le; r &le; m)
  - the constraints might be of various types (&le;, &ge; or =), but must always be linear.
- variable bounds (optional): l<sub>i</sub> &le; x<sub>i</sub> &le; u<sub>i</sub>,  (0 &le; i &le; n)
- objective function (optional): Maximize / Minimize: c<sub>0</sub> + c<sub>1</sub> x<sub>1</sub> + c<sub>2</sub> x<sub>2</sub> + ... + c<sub>n</sub> x<sub>n</sub>



**Solution to Linear Program**: several possibilities

- **infeasible** (no assignment satisfies the set of constraints)
- feasible (constraints satisfied)
  - objective value is bounded: there is an **optimal** solution
  - objective value is **unbounded**



## When to Use

Two conditions:

1. Constraints are linear
2. at least one of **n** or **m** is small



## Data Type

**CGAL Data Type for LP**

- `CGAL::Gmpz`: arbitrary precision **integer**

- `CGAL::Gmpq`: arbitrary precision **rational**

type conversion between `CGAL` data type and `double`:

```c++
// Assume we have
double value = ...;

// Assignment
// double -> CGAL data type: implicit conversion
CGAL::Gmpz exact = value; 

// CGAL -> double: explicit conversion needed
value = CGAL::to_double(exact); // option 1: call static converter
value = exact.to_double();      // option 2

// Comparision: implicit conversion
if (value < precise)
    do_something();
```





To solve LP problem with `CGAL`, we need to specify two types

- input type: in what data type we specify the linear program (types of coefficients)
  - `int`, `long`, `CGAL::Gmpz`, `CGAL::Gmpq`
- solver type: decides how CGAL solver internally do calculations to solve the problem
  - `CGAL::Gmpz`, `CGAL::Gmpq` (used when and only when `CGAL::Gmpq` is the input type)
  - Solver do internal calculations with `CGAL::Quotient<T>` where `T` is the solver type



In sample codes, LP types are all specified by `typedef`:

```c++
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>

// choose input type (type of input coefficients)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
```

which was later referenced in

```c++
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
// ...
Solution s = CGAL::solve_linear_program(lp, ET()); // decides type of s
```



## Steps to use

### Construct

```c++
Program lp (CGAL::SMALLER, true, 0, false, 0);
```

Parameters

- 1<sup>st</sup>: type of constraints
  - `CGAL::SMALLER`: &le;
  - `CGAL::EQUAL`: =
  - `CGAL::LARGER`: &ge;
- 2<sup>nd</sup> & 3<sup>rd</sup>: whether to enable all-variable **lower bound** and what the bound is
- 4<sup>th</sup> & 5<sup>th</sup>: whether to enable all-variable **upper bound** and what the bound is



### Variable bounds

As we have seen, variable bounds can be set when `Program` is constructed in the coarsest granularity. It can also be set one by one with similar parameters:

```c++
// lp is the name of `Program` object
lp.set_l(variable_index, is_finite, value); // lower bound
lp.set_u(variable_index, is_finite, value); // upper bound
```

For example:

```c++
lp.set_u(0, true, 4); // x0 <= 4
```



Here, we reference variable by **0-based index**. The linear program is adjusted dynamically to the indices we provide. When there are limited number of variables, it is convenient to rename variables:

```c++
const int X = 0; const int Y = 1; // renaming variable index
lp.set_u(X, true, 4); // x <= 4
lp.set_l(Y, true, 2); // l >= 2
```



The same is true when dealing with linear constraints - we refer to constraints by 0-based index, as we will see.



### Specify Constraints

We are essentially assign values to a linear systems. We do so by assigning to the "elements" in such "matrix" one by one:

```c++
lp.set_a(variable_index, constraints_index, coefficient_value); // (column, row, value)
lp.set_b(constraints_index, right_hand_side_value); // (row, constant)
```

For example:

```c++
const int X = 0; const int Y = 1; // renaming variable index (or column name)
lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 7); // constraints 0: x + y <= 7
lp.set_a(X, 1, -1); lp.set_a(Y, 1, 2); lp.set_b(1, 4); // constraints 1: -x + 2y <= 4
```



Same as `set_l` and `set_u` to variables, there is a member function`set_r` which can overwrite constraints relation (&le;, = or &ge;) at row-level (rows are officially referenced as "entries")

```c++
lp.set_r(constraints_index, comparision_type); // (row, relation)
```

For example:

```c++
lp.set_r(0, CGAL::EQUAL);
// row 0 is now c0+c1*x+...cn*x = b, no matter what is defined in Constructor
```



### Specify Objective

Similarly, we need to do assignment to all coefficients in the objective function

```c++
lp.set_c(variable_index, value); // coefficients for x_i
lp.set_c0(variable_index, value); // the constant terms(shift) in objective
```

`CGAL` **minimizes** objective function. Negate it if you want to maximize it.



### Solve

```c++
Program lp (CGAL::SMALLER, true, 0, false, 0);
// ...

// solve the program, using ET as the exact type
Solution s = CGAL::solve_linear_program(lp, ET());
```



### Process the Solution

**Judge solution type**:

- infeasible: `s.is_infeasible()`
- feasible
  - optimal: `s.is_optimal()`
  - unbounded: `s.is_unbounded()`



**Fetch objective value**: (should be done after checking `s.is_optimal()`)

The objective value is in type `CGAL::Quotient<ET>`. It can be compared with `double` directly. To do further processing, we usually fetch its numerator and denominator, which is in "solver type" `ET`:

```c++
Quotient<ET> quotient = s.objective_value();

ET num = s.objective_value_numerator();   // OR: num = quotient.numerator();
ET den = s.objective_value_denominator(); // OR: den = quotient.denominator();
```



`CGAL::to_double()` function, which we also use broadly in computation geometric problems, is also very handy here:

```c++
double obj = CGAL::to_double(s.objective_value());
```

Or, we can call member function `.to_double()` defined for `CGAL::Gmpz`,  `CGAL::Gmpq` (But it is not defined for `CGAL::Quotient<ET>`).

```c++
double num = s.objective_value_numerator().to_double();
double den = s.objective_value_denominator().to_double();
double obj = num / den; // not recommended: division is done in double type
						// thus less robust than CGAL::to_double(s.objective_value());

// this is NOT valid
double obj = s.objective_value().to_double();
```



## Other Information



### Nonnegative Solver

- When to use: when all variables are nonnegative and has no upper bounds
  - the solve Ignores all other bounds (specified with the constructor or with `set_l` or `set_u`).
- Effect: may or may not be more efficient than the general solver

```c++
Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
```



### Cycling

- When to use: when solver stuck in infinite loop
- Effect: slower down the solver, use conservatively

```c++
CGAL::Quadratic_program_options options;
options.set_pricing_strategy(CGAL::QP_BLAND);

// Add an additional parameter `options` to avoid infinite loop
Solution s = CGAL::solve_linear_program(lp, ET(), options);
```



### Bugs

Do not copy or assign objects of type `CGAL::Quadratic_program_solution<ET>`

Avoid passing this object, or pass its pointer instead.