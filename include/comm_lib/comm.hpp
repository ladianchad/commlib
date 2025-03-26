#ifndef COMMLIB__COMM_HPP_
#define COMMLIB__COMM_HPP_

#include <mutex>
#include <memory>
#include <map>
#include <chrono>
#include <string>
#include <iostream>
#include <atomic>
#include <thread>

#include "error.hpp"
#include "define.hpp"

namespace comm_lib
{
  using namespace std::chrono_literals;
  using UnitData = uint8_t;

  /*
    @class Option
    @brief Options for setting communication methods.
    @details Abstract class to hold communication options.
  */
  class Option
  {
    public:
      virtual ~Option() = default;

      template<typename O, typename C>
      friend class CommBuilder;

      friend std::ostream& operator<<(std::ostream& io, const Option& obj);

      /*
        @brief virtual function to print all options.
        @return String with stringfied options all options.
      */
      virtual const std::string toString() const;  

      virtual const std::string getKey() const {
        return "";
      }

      bool getSysPoll() const;

      bool getAutoInit() const;

      bool getThreading() const;

      virtual bool operator==(const Option &) const {
        return true;
      }
  
      virtual bool operator!=(const Option &) const {
        return false;
      }

    protected:
      std::chrono::milliseconds _timeout;
      bool _syspoll = true;
      bool _auto_init = true;
      bool _threading = false;
      uint8_t _log_level = 0;
  };

  std::ostream& operator<<(std::ostream& io, const Option& obj);
  std::ostream& operator<<(std::ostream& io, const std::shared_ptr<Option> & obj);

  /*
    @class Comm
    @brief Commutioncation base interface for all methods.
  */
  class Comm
  {
  public:
    friend class Executer;
    
    virtual ~Comm() = default;
    
    void init();

    size_t write(const void * data, size_t size = -1);

    size_t read(void * destination, size_t size, std::chrono::milliseconds timeout = 0ms);

    size_t read(void * destination, std::chrono::milliseconds timeout = 0ms);

    virtual const std::string getType() const;

    bool operator==(const Comm & obj) const {
      return obj.getType() == this->getType();
    }

    bool operator!=(const Comm & obj) const {
      return !(obj == *this);
    }

  protected:    
    friend class CommSingleton;

    template<typename BO,  typename BC>
    friend class CommBuilder;

    virtual void onInit() {};

    virtual void onStart() {};

    Comm() : _r_m(std::make_shared<std::recursive_mutex>()), _w_m(std::make_shared<std::recursive_mutex>()), _init(std::make_shared<bool>(false)), _start(std::make_shared<bool>(false)) {};

    virtual size_t _write(const void *, size_t);

    virtual size_t _read(void *, std::chrono::milliseconds, size_t);

    virtual const std::shared_ptr<Option> getRawOption() const;

    std::shared_ptr<std::recursive_mutex> _r_m, _w_m;
    std::shared_ptr<bool> _init, _start;
    
    std::shared_ptr<std::thread> _read_thread;
  };


class CommSingleton
{        
  private:
    template<typename BO, typename BC>
    friend class CommBuilder;

    template<typename O = Option, typename C = Comm>
    static const std::shared_ptr<Comm> & create(const std::shared_ptr<O> & option) {
      std::lock_guard<std::recursive_mutex> lock(CommSingleton::_m);
      const std::string & key = option->getKey();
      if(CommSingleton::_comms.count(key) == 0){
        CommSingleton::_comms.emplace(key, std::shared_ptr<C>(new C(option)));
        return CommSingleton::_comms[key];
      }
      const std::shared_ptr<Comm> & result = CommSingleton::_comms[key];
      if(result->getRawOption() != option){
        throw CommError("Singleton error.");
      }
      return result;
    }
    CommSingleton() {};
    ~CommSingleton() {};

    static std::recursive_mutex _m;
    static std::map<const std::string, const std::shared_ptr<Comm>> _comms;
};

template<typename O = Option,  typename C = Comm>
class CommBuilder
{
  public:
    static CommBuilder Builder() {
      return CommBuilder<C>();
    }

    CommBuilder<C> setTimeout(std::chrono::milliseconds time) {
      this->_option->_timeout = time;
      return *this;
    }

    CommBuilder<C> setSysPoll(bool value){
      this->_option->_syspoll = value;
      return *this;
    }

    CommBuilder<C> setAutoInit(bool value){
      this->_option->_auto_init = value;
      return *this;
    }

    CommBuilder<C> setLogLevel(uint8_t value){
      this->_option->_log_level = value;
      return *this;
    }

    const std::shared_ptr<Comm> & build() const {
      const std::shared_ptr<Comm> & result = CommSingleton::create<O, C>(this->_option);
      if(this->_option->getAutoInit()){
        result->init();
        if(!this->_option->getSysPoll()){
          result->init();
          std::cout<<"Auto start is setted but syspoll is not setted. No thead will be created."<<std::endl;
        }
      } else if(this->_option->getSysPoll()) {
        std::cout<<"Syspoll is setted but auto start is not setted. System polling will start when you call start();"<<std::endl;
      }
      return CommSingleton::create<O, C>(this->_option);
    }

  protected :
    CommBuilder() : _option(std::make_shared<O>()) {}
    std::shared_ptr<O> _option;
  };
  
} // namespace CommLib

#endif