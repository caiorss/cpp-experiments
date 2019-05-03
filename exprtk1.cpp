//  http://www.partow.net/programming/exprtk/
//  https://github.com/ArashPartow/exprtk
//  https://github.com/ArashPartow/exprtk-extras
//  https://github.com/ArashPartow/exprtk-extras/blob/master/exprtk_bsm_benchmark.cpp
//----------------------------------
//  Other math expression libraries:
//
//  + CalcStar
//    - https://www.codeproject.com/Tips/747223/CalcStar-A-Cplusplus-Math-Expression-Evaluator
//
//  + MuParser
//   - https://beltoforion.de/article.php?a=muparser&hl=en&s=idExample#idExample

#include <iostream>
#include <cmath>
#include <exprtk.hpp>


template <typename T>
void trig_function();

template <typename T>
void custom_function();

// Simple Example 10 - Square-Root Via Newton-Raphson Method
template <typename T>
void newton_sqrt();

// Simple Example 14 - Standard Deviation Via Vector Processing Methods
template <typename T>
void stddev_example();

// Simple Example 15 - Black-Scholes-Merton Option Pricing Model
template <typename T>
void black_scholes_merton_model();

//-----------------------------------------------

int main()
{
    trig_function<double>();

    custom_function<double>();

    stddev_example<double>();

    black_scholes_merton_model<double>();

   // newton_sqrt<double>();

    return 0;
}

// ===========================================================//

template <typename T>
void trig_function()
{
   typedef exprtk::symbol_table<T> symbol_table_t;
   typedef exprtk::expression<T>     expression_t;
   typedef exprtk::parser<T>             parser_t;

   std::string expression_string = "clamp(-1.0,sin(2 * pi * x) + cos(x / 2 * pi),+1.0)";

   T x;

   symbol_table_t symbol_table;
   symbol_table.add_variable("x",x);
   symbol_table.add_constants();

   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;
   parser.compile(expression_string,expression);

   for (x = T(0); x <= T(+10); x += T(2.0))
   {
      T y = expression.value();
      printf("%19.15f\t%19.15f\n",x,y);
   }
}



template <typename T>
struct myfunc : public exprtk::ifunction<T>
{
   using exprtk::ifunction<T>::operator();

   myfunc()
   : exprtk::ifunction<T>(2)
   { exprtk::disable_has_side_effects(*this); }

   inline T operator()(const T& v1, const T& v2)
   {
      return T(1) + (v1 * v2) / T(3);
   }
};

template <typename T>
inline T myotherfunc(T v0, T v1, T v2)
{
   return std::abs(v0 - v1) * v2;
}

template <typename T>
void custom_function()
{
   typedef exprtk::symbol_table<T> symbol_table_t;
   typedef exprtk::expression<T>     expression_t;
   typedef exprtk::parser<T>             parser_t;

   std::string expression_string =
                  "myfunc(sin(x / pi), otherfunc(3 * y, x / 2, x * y))";

   T x = T(1);
   T y = T(2);
   myfunc<T> mf;

   symbol_table_t symbol_table;
   symbol_table.add_variable("x",x);
   symbol_table.add_variable("y",y);
   symbol_table.add_function("myfunc",mf);
   symbol_table.add_function("otherfunc",myotherfunc);
   symbol_table.add_constants();

   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;
   parser.compile(expression_string,expression);

   T result = expression.value();
   printf("Result: %10.5f\n",result);
}

// Simple Example 14 - Standard Deviation Via Vector Processing Methods
template <typename T>
void stddev_example()
{
   typedef exprtk::expression<T> expression_t;
   typedef exprtk::parser<T>         parser_t;

   std::string stddev_program =
                  " var x[25] := {                     "
                  "                 1,  2,  3,  4,  5, "
                  "                 6,  7,  8,  9, 10, "
                  "                11, 12, 13, 14, 15, "
                  "                16, 17, 18, 19, 20, "
                  "                21, 22, 23, 24, 25  "
                  "              };                    "
                  "                                    "
                  " sqrt(sum([x - avg(x)]^2) / x[])    ";

   expression_t expression;

   parser_t parser;
   parser.compile(stddev_program,expression);

   T stddev = expression.value();

   printf("stddev(1..25) = %10.6f\n",stddev);
}


// Simple Example 15 - Black-Scholes-Merton Option Pricing Model
template <typename T>
void black_scholes_merton_model()
{
   typedef exprtk::symbol_table<T> symbol_table_t;
   typedef exprtk::expression<T>     expression_t;
   typedef exprtk::parser<T>             parser_t;

   std::string bsm_model_program =
                  " var d1 := (log(s / x) + (r + v^2 / 2) * t) / (v * sqrt(t)); "
                  " var d2 := d1 - v * sqrt(t);                                 "
                  "                                                             "
                  " if (callput_flag == 'call')                                 "
                  "   s * ncdf(d1) - x * e^(-r * t) * ncdf(d2);                 "
                  " else if (callput_flag == 'put')                             "
                  "   x * e^(-r * t) * ncdf(-d2) - s * ncdf(-d1);               "
                  "                                                             ";

   T s = T(60.00); // Stock price
   T x = T(65.00); // Strike price
   T t = T( 0.25); // Years to maturity
   T r = T( 0.08); // Risk free rate
   T v = T( 0.30); // Volatility

   std::string callput_flag;

   static const T e = exprtk::details::numeric::constant::e;

   symbol_table_t symbol_table;
   symbol_table.add_variable("s",s);
   symbol_table.add_variable("x",x);
   symbol_table.add_variable("t",t);
   symbol_table.add_variable("r",r);
   symbol_table.add_variable("v",v);
   symbol_table.add_constant("e",e);
   symbol_table.add_stringvar("callput_flag",callput_flag);

   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;
   parser.compile(bsm_model_program,expression);

   {
      callput_flag = "call";
      T bsm = expression.value();

      printf("BSM(%s,%5.3f,%5.3f,%5.3f,%5.3f,%5.3f) = %10.6f\n",
             callput_flag.c_str(),
             s,x,t,r,v,
             bsm);
   }

   {
      callput_flag = "put";
      T bsm = expression.value();

      printf("BSM(%s,%5.3f,%5.3f,%5.3f,%5.3f,%5.3f) = %10.6f\n",
             callput_flag.c_str(),
             s,x,t,r,v,
             bsm);
   }
}



