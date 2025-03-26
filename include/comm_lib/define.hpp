#ifndef COMMLIB__DEFIND_HPP_
#define COMMLIB__DEFIND_HPP_

#define COMMLIB_ENABLE_SINGLTON friend class comm_lib::CommSingleton;


#define __COMMLIB_BUILDER_ESSENTIAL__BUILDER(BULDER_CLASS) static BULDER_CLASS Builder() {      \
                                                  return BULDER_CLASS();           \
                                                }            

#define __COMMLIB_BUILDER_ESSENTIAL__TIMEOUT(BULDER_CLASS) BULDER_CLASS setTimeout(std::chrono::milliseconds time) { \
                                                  this->_option->_timeout = time; \
                                                  return *this; \
                                                }
                                                
#define __COMMLIB_BUILDER_ESSENTIAL__SYSPOLL(BULDER_CLASS) BULDER_CLASS setSysPoll(bool value){ \
                                                  this->_option->_syspoll = value; \
                                                  return *this; \
                                                }

#define __COMMLIB_BUILDER_ESSENTIAL__AUTOINIT(BULDER_CLASS) BULDER_CLASS setAutoInit(bool value){ \
                                                  this->_option->_auto_init = value; \
                                                  return *this; \
                                                }
                                            
#define __COMMLIB_BUILDER_ESSENTIAL__LOGLEVEL(BULDER_CLASS) BULDER_CLASS setLogLevel(uint8_t value){ \
                                                  this->_option->_log_level = value; \
                                                  return *this; \
                                                }

#define COMMLIB_BUILDER_ESSENTIALS(BULDER_CLASS) \
  __COMMLIB_BUILDER_ESSENTIAL__BUILDER(BULDER_CLASS) \
  __COMMLIB_BUILDER_ESSENTIAL__TIMEOUT(BULDER_CLASS) \
  __COMMLIB_BUILDER_ESSENTIAL__SYSPOLL(BULDER_CLASS) \
  __COMMLIB_BUILDER_ESSENTIAL__AUTOINIT(BULDER_CLASS) \
  __COMMLIB_BUILDER_ESSENTIAL__LOGLEVEL(BULDER_CLASS)


#define __COMMLIB_COMM_ESSENTIAL__GET_OPTION(OPTION_CLASS) const std::shared_ptr<OPTION_CLASS> getOption() const { \
  return this->_option; \
}

#define COMLIB_COMM_ESSENTIALS_PUBLIC(OPTION_CLASS) \
  __COMMLIB_COMM_ESSENTIAL__GET_OPTION(OPTION_CLASS)

#define __COMMLIB_COMM_ESSENTIAL__OPTION(OPTION_CLASS) const std::shared_ptr<OPTION_CLASS> _option;
#define __COMMLIB_COMM_ESSENTIAL__GET_RAW_OPTION const std::shared_ptr<Option> getRawOption() const override final { \
  return this->_option; \
}

#define COMLIB_COMM_ESSENTIALS_PRIVATE(OPTION_CLASS) \
  __COMMLIB_COMM_ESSENTIAL__OPTION(OPTION_CLASS) \
  __COMMLIB_COMM_ESSENTIAL__GET_RAW_OPTION
#endif





                                          