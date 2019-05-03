/**
  * API: http://fmtlib.net/latest/syntax.html
  *
  *
  */
#include <iostream>
#include <fstream>
#include <cmath>

// Basic functionality
#include <fmt/core.h>

// fmt string literals "name"_a, "product"_a
#include <fmt/format.h>

// Print to streams std::cout, std::cerr, std::ostream
#include <fmt/ostream.h>

// #include <fmt/color.h>

using namespace fmt::literals;

int main()
{

    fmt::print(" true == {0} ; false == {1} \n", true, false);

    double x = 20.0;
    fmt::print("The square root of x = {}\n", std::sqrt(x));

    fmt::print(" ==== Print with 6 digits precision \n");
    fmt::print(" sqrt(x) = {:.6f} \n", std::sqrt(x));


    fmt::print(" ==== Print with 6 digits precision => [f] \n");
    fmt::print(" -log(x) = {:+10.6f} \n", -std::log(x));
    fmt::print(" -log(x) = {:10.6f} \n", -std::log(x));


    fmt::print(" === Print with 8 digits precision => G \n");
    x = 206163;
    fmt::print("  log(x) = {0} ; sqrt(x) = {1} \n",
               std::log(x), std::sqrt(x));

    fmt::print("  log(x) = {1:10.6E} ; sqrt(x) = {1} \n",
               std::log(x), std::sqrt(x));

    fmt::print(" [BASES] => dec: {0:d} ; hex = 0x{0:X} "
               " ; oct = {0:o} ; bin = 0b{0:b} \n", 241 );

    fmt::print(" Product => product = {0} id = {1} price = {2}"
               ,"oranges 1 kg", 200, 10.6758 );

    // Requires: #include <fmt/format.h>
    // using namespace fmt::literals;
    fmt::print(" Product => product = {product} id = {id} price = {price:.2F}"
               , "product"_a = "oranges 1 kg", "id"_a = 200, "price"_a = 10.6758 );


    fmt::print("\n\n========= Print Table to console =================\n");

    int i = 0;
    for(double x = 0.0; x <= 4.0; x += 0.5)
        fmt::print("{0:8d}{1:10.5F}{2:10.5F}\n", i++, x, std::exp(x));

    fmt::print("\n\n========= Print Table to file =================\n");

    std::ofstream file("/tmp/table.txt");

    i = 0;

    // Note: Requires <fmt/ostream.h>
    for(double x = -4.0; x <= 4.0; x += 0.5)
        fmt::print(file, "{0:8d}{1:10.5F}{2:10.5F}\n", i++, x, std::exp(x));


    return 0;
}
