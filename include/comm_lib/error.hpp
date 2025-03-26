#ifndef COMMLIB__ERROR_HPP_
#define COMMLIB__ERROR_HPP_

#include <exception>
#include <string>

namespace comm_lib
{
  /*
    @class CommError
    @brief Base error for all communicaiton error.
  */
  class CommError : public std::exception
  {

    public:
      CommError(std::string msg) : _message(msg) {}

      /*
        @brief Describe communication error.
      */
      const char* what() const noexcept override {
        return this->_message.c_str();
      }

    private:
      const std::string _message;
  };

  using WriteError = CommError ;
  using ReadError = CommError;
  using CallbackError = CommError;

} // namespace CommLib

#endif