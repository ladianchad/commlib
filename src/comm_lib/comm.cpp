#include "comm_lib/comm.hpp"
#include "comm_lib/error.hpp"


namespace comm_lib
{


std::map<const std::string, const std::shared_ptr<Comm>> CommSingleton::_comms = std::map<const std::string, const std::shared_ptr<Comm>>();
std::recursive_mutex CommSingleton::_m = std::recursive_mutex();

std::ostream&
operator<<(std::ostream& io, const Option& obj) {
  io << obj.toString();
  return io;
}

std::ostream&
operator<<(std::ostream& io, const std::shared_ptr<Option> & obj) {
  if(obj){
    io << obj->toString();
  }
  return io;
}

bool 
Option::getSysPoll() const {
  return this->_syspoll;
}

bool
Option::getAutoStart() const {
  return this->_auto_start;
}

bool
Option::getThreading() const {
  return this->_threading;
}

const std::string
Option::toString() const {
  std::string result;
  result += "use system polling : " + std::string(this->_syspoll ? "true" : "false") + "\n";
  result += "auto start : " + std::string(this->_auto_start ? "true" : "false") + "\n";
  result += "log level : " + std::to_string(this->_log_level) + "\n";
  result += "timeout : " + std::to_string(this->_timeout.count()) + " ms";
  return result;
}      

void
Comm::init() {
  if(*this->_init){
    std::cout<<"Already initialized. Skip."<<std::endl;
  } else {
    std::cout<<"Initialize sequence started.\n";
    std::cout<<"[Comm Type] : "<<this->getType()<<"\n";
    std::cout<<"[Option Information]"<<std::endl;
    std::cout<<this->getRawOption()<<std::endl;
    this->onInit();
    std::cout<<"Initialized.\n";
    *this->_init = true;
  }
};

void
Comm::start() {
  if(*this->_start){
    std::cout<<"Main Thread already started. Skip."<<std::endl;
  } else {
    std::cout<<"Start main thread."<<std::endl;
    this->onStart();
    std::cout<<"Main Thread started.\n";
    *this->_start = true;
  }
};

const std::shared_ptr<Option>
Comm::getRawOption() const {
  return nullptr;
}

size_t
Comm::write(const void * data, size_t size) {
  if(!*this->_init){
    throw WriteError("This method has not been initialized. Please call init() or use auto_start option.");
  }
  std::lock_guard<std::recursive_mutex> lock(*this->_w_m);
  const size_t result = this->_write(data, size);
  return result;
}

size_t
Comm::read(void * destination, size_t size, std::chrono::milliseconds timeout) {
  if(!*this->_init){
    throw ReadError("This method has not been initialized. Please call init() or use auto_start option.");
  }
  std::lock_guard<std::recursive_mutex> lock(*this->_r_m);
  return this->_read(destination, timeout, size);
}

size_t
Comm::read(void * destination, std::chrono::milliseconds timeout) {
  return this->read(destination, -1, timeout);
}

const std::string
Comm::getType() const {
  return "";
}

size_t 
Comm::_write(const void *, size_t) {
  throw CommError("This method not implemented.");
}

size_t 
Comm::_read(void *, std::chrono::milliseconds, size_t) {
  throw CommError("This method not implemented.");
}

} // namespace CommLib
