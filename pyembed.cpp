#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>

#include <pybind11/embed.h>

namespace py = pybind11;

class Runstat
{
    /// Sum of sequence processed
    double m_sum;
    /// Sum of squares processed
    double m_sumsq;
    /// Size of sequence processed
    size_t m_N;
public:
    Runstat();
    Runstat(Runstat const&)            = delete;
    Runstat& operator=(Runstat const&) = delete;
    ~Runstat();
    void   add(double x);
    void   reset();
    size_t size() const;
    double mean() const;
    /// Standard deviation
    double sdev() const;
};



// Embeded module within this file.
PYBIND11_EMBEDDED_MODULE(tbox, m)
{
    m.def("linear_function", [](double x, double a, double b)
          {
              return a * x + b;
          });
    m.def("test", []{
        std::cout << " [INFO] I am a test function." << std::endl;
    });

    py::class_<Runstat>(m, "Runstat")
        .def(py::init())
        .def("add",   &Runstat::add)
        .def("size",  &Runstat::size)
        .def("mean",  &Runstat::mean)
        .def("sdev",  &Runstat::sdev)
        .def("reset", &Runstat::reset);
}

/** @brief Read whole input stream to a string and returns it */
std::string istreamToString(std::istream& is){
    if(is.bad())
        throw std::runtime_error("Error: stream has errors.");
    std::stringstream ss;
    ss << is.rdbuf();
    return ss.str();
}

void py_exec_stream(std::istream& is)
{
    std::string code = istreamToString(is);
    std::cout << "\n  "
                 "******* BEGINNING OF CODE *********\n"
              << code
              << "\n  ******* END OF CODE ***************\n"
              << std::endl;

    py::exec(code);
}

// Embed Python Interpreter
int main()
{
    // Start interpreter keeping it alive in this scope
    auto interpreter = py::scoped_interpreter{};

    std::cout << " ==== EXPERIMENTR 1 == Run Python Code ==== " << std::endl;

    py::print(" [PY] Running Python interpreter ");

    py::exec(R"(
       print(" [PY] Evaluated code ")
       for i in range(1, 5): print("i = ", i)
     )");


    py::print(" ===>>> EXPERIMENTR 2 == Execute code with embedded module ==== ");

    py::exec(R"(
      import tbox
      tbox.test()
      print("linear_function(3, 5, 6) = ", tbox.linear_function(3, 5, 6))
      print("linear_function(4, 5, 6) = ", tbox.linear_function(4, 5, 6))
   )");

    py::print("\n ===>>> EXPERIMENT 3 == Load C++ class  ==== ");

    py::exec("from tbox import Runstat \n"
             "rs = Runstat(); \n"
             "rs.add(20.5); rs.add(10.0); rs.add(5); rs.add(8); rs.add(-10); \n"
             "print(' => mean = ', rs.mean()) \n"
             "print(' => sdev = ', rs.sdev()) \n"
             "print(' => size = ', rs.size()) \n"
             );

    try{
        py::print("\n ===>> EXPERIMENT 4 === Evaluate code from input stream ====");
        auto pycode = std::ifstream{"/home/archbox/projects/cppexperiments/pyembed_script.py"};
        py_exec_stream(pycode);
    } catch(pybind11::error_already_set const& ex)
    {
        std::cerr << " [ERROR] " << ex.what() << std::endl;
    }

    py::print("\n ===>>> EXPERIMENT 4 == Read code from stdin  ==== ");
    py::print(" Type a python code and then Ctrl + D when you are done.");
    py_exec_stream(std::cin);

    return EXIT_SUCCESS;
}

// --------- Member functions of class Runstat -------------//

Runstat::Runstat()
{
    std::cout << " [TRACE] Object created Ok." << std::endl;
    m_sum = 0.0;
    m_sumsq = 0.0;
    m_N = 0;
}

Runstat::~Runstat()
{
    std::cout << " [TRACE] Object deleted OK" << std::endl;
}

void
Runstat::add(double x)
{
    m_sum += x;
    m_sumsq += x * x;
    m_N++;
}

size_t
Runstat::size() const
{
    return m_N;
}

void
Runstat::reset()
{
    m_sum = 0.0;
    m_sumsq = 0.0;
    m_N = 0;
}

double
Runstat::mean() const
{
    return m_sum / m_N;
}

double
Runstat::sdev() const
{
    size_t N = m_N;
    return std::sqrt((m_sumsq - m_sum * m_sum / N) / (N - 1.0));
}


