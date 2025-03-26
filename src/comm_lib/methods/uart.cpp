#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>

#include "comm_lib/comm.hpp"
#include "comm_lib/methods/uart.hpp"

namespace comm_lib
{

namespace methods
{
  
const std::string
UARTBaudRate::toString() const {
  return std::to_string(this->_baudrate);
}

const std::string
UARTOption::toString() const {
  std::string result = "";
  result += "port : " + this->getKey() + "\n";
  result += "baudrate : " + this->getBaudRate().toString() + "\n";
  result += Option::toString();
  return result;
}

UART::~UART() {
  ::close(this->_fd);
}

void
UART::onInit() {
  if(this->getOption()->getBaudRate().toString() == "") {
    throw CommError("Baudrate is not setted!!");
  }
  if(this->getOption()->getKey() == "") {
    throw CommError("Serial Port is not setted!!");
  }
  this->_fd = ::open(this->_option->getKey().c_str(), O_RDWR);
  if(this->_fd < 0){
    throw CommError(std::string("failed open serial port | error : ") + strerror(errno));
  }
  if(flock(this->_fd, LOCK_EX | LOCK_NB) == -1) {
    throw CommError(std::string("port already use by other process | error :") + strerror(errno));
  }
  struct termios tty;
  if(tcgetattr(this->_fd, &tty) != 0)
  {
    throw CommError(std::string("failed to get serial port info | error :") + strerror(errno));
  }
  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= CREAD | CLOCAL;
  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR;
  tty.c_cc[VTIME] = 10;
  tty.c_cc[VMIN] = 0;
  if (tcsetattr(this->_fd, TCSANOW, &tty) != 0)
  {
    throw CommError(std::string("failed set attr | error :") + strerror(errno));
  }
};

void 
UART::onStart() { 

};

size_t
UART::_write(const void * data, size_t size) {
  return ::write(this->_fd, data, size);
}

size_t
UART::_read(void * destination, std::chrono::milliseconds, size_t size) {
  return ::read(this->_fd, destination, size);
}

UARTBuilder
UARTBuilder::setBaudRate(const speed_t & baudrate) {
  this->_option->_baudrate = baudrate;
  return *this;
}

UARTBuilder
UARTBuilder::setPort(const std::string & port) {
  this->_option->_port = port;
  return *this;
}

} // namespace methods
} // namespace comm_lib
