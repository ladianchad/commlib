#include <comm_lib/comm.hpp>
#include <comm_lib/error.hpp>
#include <comm_lib/methods/uart.hpp>
#include <iostream>
#include <chrono>

void Callback(const char data) {
  std::cout<<data<<std::endl;
}

int main(int argc, char const *argv[])
{
  using namespace comm_lib;
  using namespace std::chrono_literals;
  try
  {
    std::shared_ptr<Comm> comm = methods::UART::Builder()
      .setTimeout(1s)
      .setBaudRate(100)
      .setAutoStart(true)
      .setSysPoll(false)
      .setLogLevel(2)
      .setPort("/dev/ttys009")
      .build();
    const char data[20] = "HELLO WORLD\n";
    std::cout<< comm->write(data, sizeof(data)) <<std::endl;
    char buff[20] = {};
    std::cout<< comm->read(buff, 3) << std::endl;
    std::cout<<buff<<std::endl;
  }
  catch(const CommError& e)
  {
    std::cerr << e.what() << '\n';
  }
  
  return 0;
}
