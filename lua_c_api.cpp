/**
 * References:
 *   + http://www.troubleshooters.com/codecorn/lua/lua_c_calls_lua.htm
 *   + https://www.cs.usfca.edu/~galles/cs420/lecture/LuaLectures/LuaAndC.html
 *   + http://www.lua.org/manual/5.2/manual.html#luaL_newstate
 *   + https://csl.name/post/lua-and-cpp/
 *   + http://lua-users.org/lists/lua-l/2011-08/msg00650.html
 *   +
 *
 *  Lua native library in C (CLang)
 *   + https://cs.brynmawr.edu/Courses/cs380/fall2011/luar-topics2.pdf
 */

#include <iostream>
#include <cmath>
#include <assert.h>

#include <lua.hpp>

#define ENABLE_ASSERT

#ifdef ENABLE_ASSERT
 #define M_ASSERT(expr) \
      { \
         if(!(expr)){ \
            std::cerr << "ASSERTION FAILURE: \n"; \
            std::cerr << " => Condition: " << #expr << "\n"; \
            std::cerr << " =>  Function: " << __FUNCTION__ << "\n"; \
            std::cerr << " =>      File: " << __FILE__ << ":" << __LINE__ << ":" << "\n"; \
            std::terminate(); \
         } \
      }
#else
  #define M_ASSERT(expr)
#endif

class LuaVM
{
    lua_State* m_vm;
public:
    LuaVM()
    {
        m_vm = ::luaL_newstate();
        M_ASSERT(m_vm != nullptr);

        // Load default lua libraries
        ::luaL_openlibs(m_vm);
        std::cerr << " [TRACE] Lua VM Created OK" << std::endl;
    }

    ~LuaVM()
    {
        ::lua_close(m_vm);
        std::cerr << " [TRACE] Shutdown VM OK. " << std::endl;
    }

    /// @brief Executed lua source code returning status code
    int exec(std::string const& code)
    {
        int status = luaL_dostring(m_vm, code.c_str());
        if(status != LUA_OK){
            std::cerr << " [LUA ERROR]: " << ::lua_tostring(m_vm, -1) << "\n";
           // Pop error message from stack
           lua_pop(m_vm, 1);
        }
        return status;
        //luaL_loadstring(m_vm, code) || lua_pcall(m_vm, 0, LUA_MULTRET, 0);
    }

    void showError()
    {
        if(::lua_gettop(m_vm) == 0)
            return;
        std::cerr << " [LUA ERROR]: " << ::lua_tostring(m_vm, -1) << "\n";
        // Pop error message from stack
        lua_pop(m_vm, 1);
    }

    /// @brief Add gloabal string
    /// Reference: https://stackoverflow.com/questions/10087226/
    void addString(const char* name, const char* value)
    {
        lua_pushglobaltable(m_vm);
        ::lua_pushstring(m_vm, name);  // Variable name
        ::lua_pushstring(m_vm, value); // Variable value
        ::lua_settable(m_vm, -3);
        lua_pop(m_vm, 2);

    }

    using LuaFunction = int (*) (lua_State*);

    void add_function(std::string const& name, LuaFunction func)
    {
        lua_register(m_vm, name.c_str(), func);
    }

    /// @brief Return stack size
    int size()
    {
        return ::lua_gettop(m_vm);
    }

    /// @brief Clean Lua Virtual Machine Stack
    void clean()
    {
        int nargs = ::lua_gettop(m_vm);
        lua_pop(m_vm, nargs);
    }

    lua_State* GetState() {
        return m_vm;
    }

private:

};

// Function that can be called from Lua virtual machine
int c_lua_function(lua_State* vm);


int main()
{
    std::puts("Running Lua VM");
    LuaVM vm;
    lua_State* st = vm.GetState();
    std::cout << " Stack size = " << vm.size() << "\n";

    vm.exec("print(' [LUA INFO] - hello world lua')");
    vm.exec("print(' [LUA] 3 * 4 + 10 - 2 = ', 3 * 4 + 10 - 2);");
    vm.exec("printn(' Call function that does not exist.');");

    vm.add_function("c_lua_function", &c_lua_function);
    vm.exec("print(' Result = ', c_lua_function(4, 5, 10))");
    std::cout << " Stack size = " << vm.size() << "\n";

    // Set globa lvariable in Lua VM
#if 0
    lua_pushstring(st, "LIB_NAME");  // Set variable name
    lua_pushstring(st, "mylibrary"); // Set variable value
    lua_settable(st, LUA_GLOBALSINDEX);
#endif

    std::cout << " === Create lua function and call it from C++" << std::endl;

    vm.exec(R"(
      function MyLuaFunction(x, y)
         print("Running LUA function MyLuaFunction")
         print(" x = " .. x)
         print(" y = " .. y)
         return 2 * x - 4 * y, x + 8 * y, x * y - 10
      end
           )");
    {
      lua_getglobal(st, "MyLuaFunction");
      lua_pushnumber(st, 10); // push argument 0 on Lua stack
      lua_pushnumber(st, 20); // push argument 1 on Lua stack
      if(lua_pcall(st, 2, 3, 0)){
          std::cerr << " [LUA CALL ERROR]: " << ::lua_tostring(st, -1) << "\n";
      } else { // Process results
          std::cout << " [TRACE CALL] " << " Stack size = " << vm.size() << "\n";
          lua_Number r0 = lua_tonumber(st, 0);
          lua_Number r1 = lua_tonumber(st, 1);
          lua_Number r2 = lua_tonumber(st, 2);
          std::cout << " Results r0 = "
                    << r0 << " ; r1 = " << r1 << " ; r2 = " << r2 << "\n";
      }

    }
    return 1;

    std::cout << " === Register global variable " << std::endl;

    vm.addString("LIB_NAME", " Dummy_library_v2.1");
    vm.exec("print('LIB_NAME = ' .. LIB_NAME)");
    std::cout << " Stack size = " << vm.size() << "\n";


    std::cout << " === Run arithmetic expression ====" << std::endl;

    vm.exec("return (3.45 * 8.5, 10.5 / 5.6, 0.51656)");
    std::cout << " Stack size = " << vm.size() << "\n";

    return 0;
}

// ---------------------------------------------------------------//

int c_lua_function(lua_State* vm)
{
    // Get number of arguments passed to the function
    int n = ::lua_gettop(vm);
    if( n !=3 && lua_isnumber(vm, 0) && lua_isnumber(vm, 1) && lua_isnumber(vm, 2))
    {
        ::lua_pushstring(vm, "Error: Incorrect number of arguments, expects numbers.");
        ::lua_error(vm);
    }
    lua_Number arg0 = lua_tonumber(vm, 0);
    lua_Number arg1 = lua_tonumber(vm, 1);
    lua_Number arg2 = lua_tonumber(vm, 2);

    std::cout << " [INFO] Call from " << __FUNCTION__ << "() "
              << " arg0 = " << arg0 << " ; arg1 = " << arg1 << " ; arg2 " << arg2 << "\n";

    // Result 0
    lua_Number res0 = arg0 * 2 + arg1 * 3;
    // Result 1
    lua_Number res1 = 5 * arg0 + 10 * arg1;
    // Push results into lua stack
    lua_pushnumber(vm, res0);
    lua_pushnumber(vm, res1);

    // Return number of results on the stack
    return 2;
}




