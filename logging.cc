//
// Created by alex on 17-7-19.
//

//************************************
// File: logging.cc
// Autor: ShenChengEn - ubuntu733@gmail.com
// Description: ---
// create: 17-7-19 17:27
//************************************

#include "logging.h"

namespace huoguo{
  const static char *LOG_DIR = "/Users/chenchengen/CLionProjects/testWord2vec";
  const static char *LOG_FILE_NAME = "huoguo";
  Logger *Logger::instance = 0;

  Logger *Logger::getInstance() {
    //std::lock_guard<std::mutex> lock(mutex);
    if (instance == NULL)
      instance = new Logger();
    return instance;
  }

  Logger::Logger() {
    FLAGS_log_dir = LOG_DIR;
    google::InitGoogleLogging(LOG_FILE_NAME);
  }

  Logger::~Logger() {
    google::ShutdownGoogleLogging();
  }

  void Logger::write_log(int level, string msg, const char *filename, const int line) {
    if (level == 1) {
      LOG(INFO) << msg << "|" << filename << ":" << line;
    } else if (level == 2) {
      LOG(WARNING) << msg << "|" << filename << ":" << line;
    } else if (level == 3) {
      LOG(ERROR) << msg << "|" << filename << ":" << line;
    } else if (level == 4) {
      LOG(FATAL) << msg << "|" << filename << ":" << line;
    }

  }


}

