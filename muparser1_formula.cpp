#include <iostream>
#include <string>
#include <cmath>

#include <muParser.h>

// Defined for UNIX-like: Linux, Mac OSX, QNX, ...
#if __unix__
  #include <unistd.h>
#endif

bool isTTY_terminal()
{
   #if __unix__
      return ::ttyname(STDIN_FILENO) != nullptr;
   #else
      return false;
   #endif
}

double future_value(double PV, double rate, double nper)
{
    return PV * std::pow(1 + rate / 100.0, nper);
}

int main()
{

    mu::Parser p1;

    std::puts("\n====== EXPERIMENT 1 - Simple math expression =======");
    p1.SetExpr("2^1 + 2^3 + 2^4");
    std::cout << " [*] p1-A value: "
              << p1.GetExpr() << " = " << p1.Eval() << std::endl;

    p1.SetExpr("3.5 * 10 - sin(3.1415) * 2.0 + sqrt(10) / 100.0 + 2^3");
    std::cout << " [*] p1-B value: "
              << p1.GetExpr() << " = " << p1.Eval() << std::endl;

    std::puts("\n====== EXPERIMENT 2 - Expression with variables =======");
    p1.DefineConst("pi", 3.1415);
    double x = 10.0, y = 4.5;
    p1.DefineVar("x", &x);
    p1.DefineVar("y", &y);
    p1.SetExpr(" 3 * pi + sin(pi) + 4 * x + y - 5");
    std::cout << " [*] p1-C value: "
              << p1.GetExpr() << " = " << p1.Eval() << std::endl;

    std::puts("\n====== EXPERIMENT 3 - Expression with custom function =======");

    p1.DefineFun("fv", &future_value);

    p1.DefineFun("payoff", [](double S, double K){
        return std::max(S - K, 0.0);
    });

    p1.SetExpr("fv(100, 2, 8) + payoff(100, 90)");
    std::cout << " [*] p1-D value: " << p1.GetExpr() << " = " << p1.Eval() << std::endl;


    std::puts("\n====== EXPERIMENT 4 - Parser error handling =======");

    // When an error happens it throws an exception: mu::ParserError
    try{
        p1.SetExpr("10.2 * 5.0 + a * 3");
        double value = p1.Eval();
        std::cout << " [*] p1-E value: " << value << std::endl;
    } catch (mu::ParserError const& ex)
    {
        std::cerr << " [ERROR] p1-C Parser error: " << ex.GetMsg() << std::endl;
    }


    std::puts("\n====== EXPERIMENT 5 - Calutor Interactive Shell ======");

    if(isTTY_terminal())
    {
        std::cout << " === Calculator Started OK. =====" << std::endl;

        mu::Parser p2;
        double ans = 0.0;
        p2.DefineVar("ans", &ans);
        std::string line;

        while(std::cin.good())
        {
            std::cout << " EXPR => ";
            std::getline(std::cin, line);

            if(line == "")
                continue;

            if(line == "quit")
                break;

            p2.SetExpr(line);
            try {
                ans = p2.Eval();
                std::cout << " => ans = " << ans << "\n\n";
            } catch(mu::ParserError const& ex)
            {
                std::cerr << " [ERROR] Parser error " << ex.GetMsg() << std::endl;
            }
        }

    }

#if _WIN32
    std::cout << "Enter RETURN to exit. " << std::endl;
    std::cin.get();
#endif
    return 0;
}
