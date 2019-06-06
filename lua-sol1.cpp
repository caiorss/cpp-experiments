/*  Sol2 Lua Binding  - Repository: https://github.com/ThePhD/sol2/
 *
 * Library Installation:
 *  $ mkdir -p ~/dev/include && cd dev && git clone https://github.com/ThePhD/sol2
 *  $ mv -v sol2/include/
 *
 * Video:
 *  ->>  https://www.youtube.com/watch?v=xQAmGBfKnas
 *
 *
 * Presentation:
 * -------------------------------------------------------
 *
 *  + https://thephd.github.io/presentations/sol2/Boston%20C++%20Meetup/2017.11.08%20-%20ThePhD%20-%20Wrapping%20Lua%20C%20in%20C++.pdf
 *
 *  + https://thephd.github.io/presentations/sol2/Boston%20C++%20Meetup/2017.11.08%20-%20ThePhD%20-%20Wrapping%20Lua%20C%20in%20C++.pdf
 *
 */

#include <sol/sol.hpp>
#include <cassert>

#ifdef __unix
  #include <unistd.h>
#endif

/** Check whether application is running on a UNIX terminal */
bool hasTTY()
{
    #ifdef __unix
      return ttyname(STDIN_FILENO) != nullptr;
    #else
      return false;
    #endif
}


class Counter
{
    size_t cnt;
public:
    Counter(): cnt(0)
    {
        std::cerr << " [TRACE] Object counter created" << std::endl;
    }
    ~Counter()
    {
        std::cerr << " [TRACE] Delete counter object" << std::endl;
    }

    size_t get() const
    {
        return cnt;
    }
    void inc()
    {
        std::cerr << " [TRACE] Called inc()= " << ++cnt << std::endl;
    }
    void dec()
    {
        std::cerr << " [TRACE] Called inc()= " << --cnt << std::endl;
    }
};


int main()
{
    sol::state lua;

    // Import base libraries, otherwise, it will not be possible to use print()
    // function
    lua.open_libraries();
#if 0
    lua.open_libraries(sol::lib::base, sol::lib::math
                     , sol::lib::string, sol::lib::io, sol::lib::package);
#endif

    int x = 0;

    // Global variables
    lua.set("size", 100);
    lua.set("weight", 500);
    lua["location"] = "Somewhere else";

    lua.set_function("beep", [&x]
                     {
                         ++x;
                         std::cout << "I was called" << std::endl;
                     });
    lua.script("beep()");
    assert(x == 1);

#if 1
    sol::object obj = lua.script("3 * 10 + 8;");
    if(obj.is<double>())
        std::cout << "Object is double => obj = " << obj.as<double>() << std::endl;
#endif

    std::cout << " ===== Read User Settings From Script ============" << std::endl;

    lua.script(R"(
             print(' ====== Printing text from LUA ===========');
             print('size ', size)
             print('location = ', location)
             path = "C:\\Windows\\System32"
     )");

    // Get configuration from user script
    std::string path = lua.get<std::string>("path");
    std::cout << " path := " << path << std::endl;

    std::string path2 = lua.get<std::string>("path2");
    std::cout << "path2 := " << path2 << std::endl;

    std::cout << " ===== Register User-Defined Types ===============\n\n";

    // Register class in Lua
    lua.new_usertype<Counter>("Counter",
                              // Member function name followed by member function pointer
                             "get", &Counter::get,
                             "inc", &Counter::inc
                             );

    lua.script(R"(
          -- Create counter object
          c = Counter.new()
          -- Call increment
          c:inc()
          c:inc()
          c:inc()
          print("conter value = ", c:get())
        )");

    if(hasTTY())
    {
        std::cout << " === Lua REPL - Read Print-Eval Loop =============\n\n";
        std::string line;

        sol::function lua_print = lua["print"];

        std::cout << std::boolalpha;
        std::cout << " hasTTY()? = " << hasTTY() << std::endl;

        // Check whether user has not typed Ctrtl + C or Ctrl + D (EOF) on UNIX
        while(std::cin.good())
        {
            std::cout << "Enter lua expression:>> ";
            std::getline(std::cin, line);
            sol::object value = lua.safe_script(line, sol::script_pass_on_error);
            lua_print.call(value);
            // std::cout << lua_print.call(value.get()) << std::endl;
        }
    }


    std::cout << " ===== End of Main() ==============" << std::endl;
    return 0;
}
