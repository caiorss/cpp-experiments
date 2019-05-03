#include <iostream>
#include <vector>
#include <range/v3/all.hpp>

namespace r = ranges;
namespace view = ranges::view;

template<typename T>
void printValue(T&& x)
{
    std::cout << " x = " << x << std::endl;
}

template<typename Element>
std::ostream&
operator<<(std::ostream& os, std::vector<Element> const& xs )
{
    os << "[" << xs.size() << "]( ";
    for(const auto&  x: xs)
       os << x << " ";
    return os << " )";
}

int main()
{

    std::cout << "   +------------------------------------+\n"
              << "   | Range Algorithms Experiments       |\n"
              << "   +------------------------------------+\n";

    auto printLambda = [](auto x){
        std::cout << " x = " << x << std::endl;
    };

    // std::cout << view::iota(2, 10) << std::endl;

    //-------------------------------------------//
    std::puts("\n === EXPERIMENT 1 A - for_each ===");
    {

        std::vector<int> xs = {8, 9, 20, 25};
        std::cout << " => Print numbers" << std::endl;
        ranges::for_each(xs, printLambda);

        std::deque<std::string> words = {"c++", "c++17", "C++20", "asm", "ADA"};
        std::cout << " => Print words" << std::endl;
        ranges::for_each(xs, printLambda);
    }

    //-------------------------------------------//
    std::puts("\n === EXPERIMENT 1 B - for_each ===");
    {
        std::string astr = "bolts";
        ranges::for_each(astr, printLambda);
    }

    std::puts("\n === EXPERIMENT 2 - sort ===");
    {
        std::vector<double> xs2 = {10.45, -30.0, 45.0, 8.2, 100.0, 10.6};
        std::cout << " Reverse vector" << std::endl;
        std::cout << " BEFORE xs2 = " << xs2 << std::endl;
        ranges::sort(xs2);
        std::cout << " AFTER xs2 = " << xs2 << std::endl;
    }

    std::puts("\n === EXPERIMENT 3 - fill ===");
    {
        std::vector<int> xs2(10, 0);

        std::cout << " BEFORE  A xs2 = " << view::all(xs2 )<< std::endl;
        std::cout << " BEFOREB B xs2 = " << xs2 << std::endl;

        ranges::fill(xs2, 10);
        std::cout << " AFTER 1 => x2 = " << xs2 << std::endl;

        ranges::fill(xs2, 5);
        std::cout << " AFTER 2 => x2 = " << xs2 << std::endl;
    }

    std::puts("\n === EXPERIMENT 4 - reverse ===");
    {
        std::deque<std::string> words = {"c++", "c++17", "C++20", "asm", "ADA"};

        std::cout << " BEFORE => words = " << view::all(words) << std::endl;

        ranges::reverse(words);
        std::cout << " AFTER => words = " << view::all(words) << std::endl;
    }

    std::puts("\n === EXPERIMENT 5 - remove_if ===");
    {
        std::vector<int> xvec = {1, 2, 5, 10, 1, 4, 1, 2, 8, 20, 100, 10, 1};

        std::cout << " BEFORE => xvec = " << xvec << std::endl;

        // Erase remove idiom
        xvec.erase(ranges::remove_if(xvec, [](int x){ return x == 1; }), xvec.end());
        std::cout << " AFTER  => xvec = " << xvec << std::endl;
    }

    std::puts("\n === EXPERIMENT 6 - find ===");
    {
        std::vector<int> xvec = {1, 2, 5, 10, 1, 4, 1, 2};

        auto it = ranges::find(xvec, 10);
        if(it != xvec.end()){
             std::cout << " [OK] Found value == 10 => *it = " << *it << std::endl;
             std::cout << " Position = "
                       << ranges::distance(xvec.begin(), it)
                       << std::endl;
        }
        else
            std::cout << " [FAIL] Not found  value " << std::endl;
    }

    std::puts("\n === EXPERIMENT 7 - accumulator ===");
    {
        std::vector<int> xvec = {1, 2, 5, 10, 1, 4, 1, 2};

        int result = ranges::accumulate(xvec, 0);
        std::cout << " => ranges::accumulate(xvec, 0) = "
                  << result << std::endl;

    }


    std::cout << "\n";
    std::cout << "  +------------------------------------+\n"
              << "   | Range View Experiments             |\n"
              << "   +------------------------------------+\n";

    std::puts("\n === EXPERIMENT 1A - view::all");
    {
        std::vector<char> xs = {'x', 'y', 'm', 'k', 'm'};

        std::cout << " [1] => std::view(xs) = "
                  << view::all(xs) << std::endl;

        std::cout << "\n [2] => xs = ";
        for(auto&& x: view::all(xs))
            std::cout << x << " ";
        std::cout << std::endl;
    }

    std::puts("\n === EXPERIMENT 1B - view::reverse ===");
    {
        std::vector<char> xs = {'x', 'y', 'm', 'k', 'm'};

        for(auto const& x: view::reverse(xs))
            std::cout << " " << x;

        std::cout << std::endl;
        std::cout << " AFTER xs = " << xs << std::endl;
    }

    std::puts("\n === EXPERIMENT 1C - view::transform ===");
    {
        std::vector<int> xs = {100, 5, 20, 9, 10, 6};
        auto a_lambda = [](auto x){ return 5 * x -10; };

        for(auto const& x: xs | view::transform(a_lambda))
            std::cout << " " << x;

        std::cout << std::endl;
        //std::cout << " AFTER xs = " << xs << std::endl;
    }

    std::puts("\n === EXPERIMENT 2A - Range adaptors pipeline ===");
    {
        auto a_lambda = [](auto x){ return 5 * x -10; };

        std::cout << " => Iota view = ";
        for(auto const& x: view::iota(1) | view::take(8))
            std::cout << " " << x;
        std::cout << std::endl;
    }

    std::puts("\n === EXPERIMENT 2B - Range adaptors pipeline ===");
    {
        std::cout << " => Iota view [B] = \n";
        auto aview =
                  view::iota(1)
                | view::take(5)
                | view::transform([](int x){ return 5 * x + 6; });

        std::cout << " [INFO] aview = " << aview << std::endl;
        std::cout << " [INFO] aview | reverse = "
                  << (aview | view::reverse)
                  << std::endl;

        std::cout << "\n Iteration 1 => ";
        ranges::for_each(aview, [](int a){ std::cout << a << " "; });

        std::cout << "\n Iteration 2 => ";
        for(auto const& x: aview | view::reverse)
            std::cout << x << " ";
        std::cout << std::endl;

    }

    std::puts("\n === EXPERIMENT 3A - enumerate ===");
    {
        std::deque<std::string> words = { "c++", "c++17", "C++20", "asm" };

        std::cout << " ==== Loop 1 ==== " << std::endl;
        for(auto const& x: view::enumerate(words))
            std::cout << " => n = " << x.first
                      << " ; w = " << x.second << std::endl;

        std::cout << " ==== Loop 2 ==== " << std::endl;
        for(auto const& x: words | view::enumerate)
            std::cout << " => n = " << x.first
                      << " ; w = " << x.second << std::endl;
    }


    std::puts("\n === EXPERIMENT 4 - ranges::accumulate withe iterator ===");
    {
        auto aview = view::iota(2)
                   | view::transform([](double x){return 3.0 * x - 5; })
                   | view::take(15);

        std::cout << " aview = " << aview << std::endl;
        std::cout << " accumulate(aview) = " << ranges::accumulate(aview, 0.0)
                  << std::endl;
    }

    std::puts("\n === EXPERIMENT 5 - Copy Range to destination ===");
    {
        std::vector<int> output;
        auto aview = view::iota(5)
                   | view::transform([](int n){ return 6 * n - 10;})
                   | view::take(10);

        std::cout << " BEFORE => output = " << output << std::endl;

        ranges::copy(aview, ranges::back_inserter(output));
        std::cout << " AFTER => output = " << output << std::endl;
    }


    // view::iota(1) | view::take(5) | ranges::for_each(printValue);

    return 0;
}
