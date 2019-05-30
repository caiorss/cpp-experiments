#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <ctime>
#include <boost/chrono.hpp>
#include <boost/asio.hpp>

namespace asio = boost::asio;

// using namespace std;

void timer_expired(std::string id)
{
   //std::cout << now_time << " " << id << " enter.\n!";
   std::cout << "timer running - id = " << id << std::endl;
   std::this_thread::sleep_for(std::chrono::seconds(3));
   // std::cout << now_time << " " << id << " leave.\n";
}

int main()
{
    auto service = asio::io_service{};
    auto strand  = asio::io_service::strand {service};

    // Send tasks to completion queue
    service.post([]{ std::cout << "eat\n"; });
    service.post([]{ std::cout << "drink\n"; });
    service.post([]{ std::cout << "and be merry!\n"; });

     asio::deadline_timer timer1(service, boost::posix_time::seconds(5));
     asio::deadline_timer timer2(service, boost::posix_time::seconds(5));
//    timer1.async_wait([](auto ... vn){  timer_expired("timer1"); });
//    timer2.async_wait([](auto ... vn){  timer_expired("timer2"); });
//    std::thread buttler([&]{  service.run(); });
//    buttler.join();

     timer1.async_wait(
                 // Completion handler
                 strand.wrap([](auto ... vn){ timer_expired("timer1"); })
                 );
     timer2.async_wait(
                 // The lambda is the completion handler
                 strand.wrap([](auto ... vn){ timer_expired("timer2"); })
                 );

    std::cout << "Done." << std::endl;

    return 0;
}
