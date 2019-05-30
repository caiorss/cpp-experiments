#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

// If defined before including fmt, uses it as header-only library
#define FMT_HEADER_ONLY

// Basic functionality
#include <fmt/core.h>
// fmt string literals "name"_a, "product"_a
#include <fmt/format.h>
// Print to streams std::cout, std::cerr, std::ostream
#include <fmt/ostream.h>

#include <fmt/color.h>

// #include <fmt/color.h>

using namespace fmt::literals;

void printHeader(const char* header)
{
    fmt::print("\n{}\n", header);
    fmt::print("---------------------------------------------\n");
}

int main()
{
    printHeader(" ***** EXPERIMENT 1 - Boolean  *************");
    fmt::print(" true == {0} ; false == {1} \n", true, false);


    printHeader(" ***** EXPERIMENT 2 - Numeric Base  *********");

    fmt::print(" [BASES] => dec: {0:d} ; hex = 0x{0:X} "
               " ; oct = {0:o} ; bin = 0b{0:b} \n", 241 );

    printHeader(" ***** EXPERIMENT 3 - Positional arguments ****");

    fmt::print(" first = {0}, 2nd = {1}, 1st = {1}, 3rd = {2}\n",
                200, "hello", 5.615);

    printHeader("**** EXPERIMENT 4 - Named Arguments ********* ");

    fmt::print(" [A] Product => product = {0} id = {1} price = {2}\n"
               ,"oranges 1 kg", 200, 10.6758 );

    // Requires: #include <fmt/format.h>
    // using namespace fmt::literals;
    fmt::print(" [B] Product => product = {product} id = {id} price = {price:.2F}\n"
               , "product"_a = "oranges 1 kg", "id"_a = 200, "price"_a = 10.6758 );

    fmt::print(" [B] Product => product = {product} id = {id} price = {price:.2F}\n"
               , fmt::arg("product", "oranges 1 kg")
               , fmt::arg("id",  200)
               , fmt::arg("price", 10.6758 ));

    printHeader("************ Colored Output ******************");

    fmt::print( fmt::fg(fmt::color::aqua) | fmt::emphasis::bold,
               " [INFO] Voltage Leval  = {0:+.3F}\n", 10.6478);

    fmt::print( fmt::fg(fmt::color::red) | fmt::emphasis::underline,
               " [ERROR] Fatal Error, shutdown systems code  0x{0:X}\n", 2651);


    printHeader(" ***** EXPERIMENT 5 - Numeric Formatting ******");

    double x = 20.0;
    fmt::print("The square root of x = {}\n", std::sqrt(x));

    x = 28524.0;

    fmt::print(" log(x) = {:.2F}  (2 digit precision)\n", std::log(x));
    fmt::print(" log(x) = {:+.6F} (6 digit precision)\n", std::log(x));
    fmt::print(" 2000 * log(x) = {:+.6G} (6 digit precision)\n", 1e5 * std::log(x));
    fmt::print("  log(x) = {0:+.8E} ; sqrt(x) = {1:+8E} (8 digit precision)\n",
               std::log(x), std::sqrt(x));

    printHeader(" ***** EXPERIMENT 6 - Print numeric table ******");

    int i = 0;
    for(double x = 0.0; x <= 4.0; x += 0.5)
        fmt::print("{0:8d}{1:10.5F}{2:10.5F}\n", i++, x, std::exp(x));

    printHeader(" ***** EXPERIMENT 7 - Print table to file *******");

    // std::ofstream file("/tmp/table.txt");
    std::stringstream file; // Fake file

    i = 0;

    // Note: Requires <fmt/ostream.h>
    for(double x = -4.0; x <= 4.0; x += 1.0)
        fmt::print(file, "{0:8d}{1:10.5F}{2:10.5F}\n", i++, x, std::exp(x));

    fmt::print("File content = \n{0}", file.str());

    return 0;
}
