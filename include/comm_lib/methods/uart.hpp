#ifndef COMMLIB__INTERFACES__UART_HPP_
#define COMMLIB__INTERFACES__UART_HPP_

#include <termios.h>

#include "comm_lib/comm.hpp" 
#include "comm_lib/define.hpp"

namespace comm_lib
{

namespace methods
{

class UARTBaudRate
{
  public:
    UARTBaudRate() = delete;
    UARTBaudRate(const speed_t & speed) : _baudrate(speed) {};

    void operator=(const speed_t & speed) {
      this->_baudrate = speed;
    }

    const std::string toString() const;

  private:
    speed_t _baudrate;
};

class UARTOption : public Option
{
  public:
    friend class UARTBuilder;

    const UARTBaudRate getBaudRate() const {
      return this->_baudrate;
    }

    const std::string getKey() const {
      return this->_port;
    }

    const std::string toString() const;

  private:
    UARTBaudRate _baudrate = 0;
    std::string _port;
};

class UARTBuilder;

class UART : public Comm
{
  public:
    using Builder = UARTBuilder;
    COMLIB_COMM_ESSENTIALS_PUBLIC(UARTOption)

    ~UART() override final;

    const std::string getType() const override final {
      return "UART";
    }

  private:
    COMMLIB_ENABLE_SINGLTON
    COMLIB_COMM_ESSENTIALS_PRIVATE(UARTOption)

    UART(const std::shared_ptr<UARTOption> & option) : Comm(), _option(option) {}

    void onInit() override final;

    void onStart() override final;
  
    size_t _write(const void *, size_t) override final;

    size_t _read(void *, std::chrono::milliseconds, size_t) override final;
    
    int _fd = -1;
};

class UARTBuilder : public CommBuilder<UARTOption, UART>
{
public:
  COMMLIB_BUILDER_ESSENTIALS(UARTBuilder)

  UARTBuilder setBaudRate(const speed_t & baudrate);

  UARTBuilder setPort(const std::string & port);
};

} // namespace methods
  
} // namespace communication

#endif