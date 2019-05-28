#include <iostream>
#include <string>
#include <functional>

#include <boost/coroutine2/all.hpp>

namespace bcr = boost::coroutines2;

void simpleCoroutine(bcr::coroutine<void>::push_type& yield)
{
    std::cout << " => Step1 \n";
    yield();
    std::cout << " => Step2 \n";
    yield();
    std::cout << " => Step3 \n";
    yield();
    std::cout << " => Finished iterations \n";

}

// Generates factorial sequence from 1! to 8!
void factorialSequenceCoroutine(bcr::coroutine<int>::push_type& yield)
{
    int prod = 1;
    for(int i = 1; i < 6; i++)
    {
        prod = prod * i;
        yield(prod);
    }
    std::cout << " [INFO] No more values can be returned." << "\n";
}


int main()
{
    std::cout << " [TRACE] Starting main()" << std::endl;

    std::puts("==== EXPERIMENT 1 ==============================");
    {

        std::cout << "Call 1" << "\n";
        // Create coroutine object.
        // Note a coroutine is executed when created until first yield()
        bcr::coroutine<void>::pull_type instance(simpleCoroutine);

        std::cout << "Call 2" << "\n";
        instance();
        std::cout << "Call 3" << "\n";
        instance();
        std::cout << "Call 4" << "\n";
        instance();
        std::cout << "Call 5" << "\n";
    }

    std::puts("==== EXPERIMENT 2 ==============================");
    {
        // Note a coroutine is executed when created until first yield()
        auto instance = bcr::coroutine<int>::pull_type{&factorialSequenceCoroutine};

        // Get current value of the coroutine instance (object)
        std::cout << " Value 0 = " << instance.get() << "\n";
        // Execute next step, until the next yield statement.
        instance();
        std::cout << " Value 1 = " << instance.get() << "\n";
        instance();
        std::cout << " Value 2 = " << instance.get() << "\n";
        instance();
        std::cout << " Value 4 = " << instance.get() << "\n";
        instance();
        std::cout << " Value 5 = " << instance.get() << "\n";
        instance();
        std::cout << " Value 6 = " << instance.get() << "\n";
        instance();

        // DO NOT TRY IT: assertions failure and program crashes.
        // std::cout << " Value 7 = " << instance.get() << "\n";
        // instance();
    }

    std::puts("==== EXPERIMENT 3 == Loop over a coroutine sequence =======");
    {
        // Note a coroutine is executed when created until first yield()
        auto instance = bcr::coroutine<int>::pull_type{&factorialSequenceCoroutine};

        for(auto it: instance)
            std::cout << " it = " << it << std::endl;
    }


    std::cout << " [TRACE] Finishing main() " << std::endl;
    return EXIT_SUCCESS;
}
