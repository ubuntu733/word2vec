//
// Created by alex on 17-7-19.
//

//************************************
// File: logging.h
// Autor: ShenChengEn - ubuntu733@gmail.com
// Description: ---
// create: 17-7-19 17:27
//************************************

#ifndef CONTEXT_LOGGING_H
#define CONTEXT_LOGGING_H

#include "StringUtil.h"
#include <glog/logging.h>
#include <mutex>
namespace huoguo {
  using std::mutex;

  class Logger {
  public:
    static Logger *getInstance();

    ~Logger();

    void write_log(int level, string msg, const char *filename, const int line);

  private:
    static Logger *instance;
    mutex lock;

    Logger();
  };

#define INFO(msg) do { \
  std::lock_guard<std::mutex> lock(mutex); \
  Logger::getInstance()->write_log(1,msg,__FILE__,__LINE__); \
} while (0);
#define WARNING(msg) do { \
  std::lock_guard<std::mutex> lock(mutex); \
  Logger::getInstance()->write_log(2,msg,__FILE__,__LINE__); \
} while (0);
#define ERROR(msg) do { \
  std::lock_guard<std::mutex> lock(mutex); \
  Logger::getInstance()->write_log(3,msg,__FILE__,__LINE__); \
} while (0);
#define FATAL(msg) do { \
  std::lock_guard<std::mutex> lock(mutex); \
  Logger::getInstance()->write_log(4,msg,__FILE__,__LINE__); \
} while (0);
#define SCHECK(exp, msg) do { \
  std::lock_guard<std::mutex> lock(mutex); \
  if (!exp) \
    FATAL(msg); \
  } while (0);
}
#endif //CONTEXT_LOGGING_H
