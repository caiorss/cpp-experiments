#include <iostream>
#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

TEST_CASE("testing the factorial function")
{
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}

inline
bool AlmostEqual(double a, double b, double tol)
{
    return std::fabs(a - b) < tol * std::fmax(std::abs(a), std::fabs(b));
}

/* Newton method for approximating square root.
 *
 * Root of F(x) = 0 =>  x[n+1] = x[n] - F(x[n]) / dF(x[n])
 *
 * Where the dF(X) is the derivate of F(x).
 *   + Sove G(x) = x^2 - a = 0
 *   + dG(x) = 2x
 *
 *   x[n+1] = x[n] - (x[n]^2 - a) / (2x[n])
 *   x[n+1] = (x[n]^2 + a) / (2 x[n])
 *
 * Recurring equation:
 *   x[n+1] = 1/2 (x[n] + a / x[n])
 *
 * Continue while the following condition is false:
 *
 *   iteratins <= maximum number of iterations
 *   || x[n+1] - x[n] || <= tolerance * max( || x[n1+1] ||, || x[n] ||)
 *
 */
double suquare_root_newton(double a, double tol)
{
    double x_prev = a, x_next = a;
    size_t itermax = 100, i = 0;
    for(i = 0; i < itermax; i++)
    {
        x_prev = x_next;
        x_next = 1.0 / 2.0 * (x_prev + a / x_prev);
        if(AlmostEqual(x_prev, x_next, tol))
            return x_next;
    }
    return NAN;
}


// See: Flot point comparsion
// => https://github.com/onqtam/doctest/blob/master/doc/markdown/assertions.md
TEST_CASE("Square root using Newton's method")
{
    double digit4 = 0.0001;
    CHECK(suquare_root_newton(10.0, digit4) == doctest::Approx(3.1622).epsilon(digit4));
    CHECK(suquare_root_newton(26.0, digit4) == doctest::Approx(5.099020));
    CHECK(suquare_root_newton(96.0, digit4) == doctest::Approx(9.797959));
}

