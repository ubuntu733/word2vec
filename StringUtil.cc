//
// Created by alex on 17-7-23.
//

//************************************
// File: StringUtil.cc
// Autor: ShenChengEn - ubuntu733@gmail.com
// Description: ---
// create: 17-7-23 18:18
//************************************

#include "StringUtil.h"


namespace huoguo {

  const string QUOTE_SAVE_CHARS = "-_.;/?:@&=+$,";

  int StringToInt(const string& str) {
    int value = atoi(str.c_str());
    return value;
  }
  string StringFormat(const size_t maxSize, const char* fmt, ...) {
    char buffer[maxSize+1];
    va_list ap;
    va_start(ap, fmt);
    int writeNum = vsprintf(buffer, fmt, ap);
    if (writeNum <= 0) {
      return "";
    }
    va_end(ap);
    return string(buffer);
  }
/*  const char* StringFormat(const char *fmt, ...) {
    int size = 256;
    std::string str;
    va_list ap;
    while (1) {
      str.resize(size);
      va_start(ap, fmt);
      int n = vsnprintf((char *) str.c_str(), size, fmt, ap);
      va_end(ap);
      if (n > -1 && n < size) {
        str.resize(n);
        return str.c_str();
      }
      if (n > -1)
        size = n + 1;
      else
        size *= 2;
    }
    return str.c_str();
  }*/


  bool Split(const string& str, vector<string>& vec, const string& sep, bool saveEmpty) {
    if (str == ""){
      return false;
    }
    vec.clear();
    size_t start = 0;
    size_t strSize = str.size();
    while (start < strSize) {
      size_t pos = str.find_first_of(sep, start);
      if (pos == string::npos){
        if (saveEmpty || pos-start > 0) {
          vec.push_back(str.substr(start));
        }
        return true;
      }
      if (saveEmpty || pos-start > 0) {
        vec.push_back(str.substr(start, pos-start));
      }
      start = pos+sep.size();
    }
    return true;
  }

  size_t Split(const string& str, const string& sep, bool saveEmpty) {
    if (str == ""){
      return 0;
    }
    size_t start = 0;
    size_t strSize = str.size();
    size_t cnt = 0;
    while (start < strSize) {
      size_t pos = str.find_first_of(sep, start);
      if (pos == string::npos){
        if (saveEmpty || pos-start > 0) {
          cnt++;
        }
        break;
      }
      if (saveEmpty || pos-start > 0) {
        cnt++;
      }
      start = pos+sep.size();
    }
    return cnt;
  }

  void Upper(string& str) {
    size_t strSize = str.size();
    for (size_t i = 0; i < strSize; ++i) {
      str[i] = toupper(str[i]);
    }
  }

  void Lower(string& str) {
    size_t strSize = str.size();
    for (size_t i = 0; i < strSize; ++i) {
      str[i] = tolower(str[i]);
    }
  }

  string Ltrim(string& str, const char x) {
    size_t start = 0;
    size_t len = str.length();
    while (start < len && str[start] == x) {
      ++start;
    }
    return str.substr(start);
  }

  string Rtrim(string& str, const char x) {
    // var end type must be int not size_t
    // because when str is empty length()-1 set to size_t will be return mas number
    int end = str.length() - 1;
    while (end > (-1) && str[end] == x) {
      --end;
    }
    return str.substr(0, end+1);
  }

  string Trim(string& str, const char x) {
    str = Rtrim(str, x);
    return Ltrim(str, x);
  }

  string Trim(char* ptr, const char x) {
    string str = string(ptr);
    str = Rtrim(str, x);
    return Ltrim(str, x);
  }

  bool StartsWith(const string& str, const string& prefix) {
    size_t strSize = str.length();
    size_t prefixSize = prefix.length();
    if (strSize < prefixSize) {
      return false;
    }
    return str.compare(0, prefixSize, prefix) == 0;
  }

  bool StartsWith(const char* ptr, const string& prefix) {
    string str = string(ptr);
    return StartsWith(str, prefix);
  }

  bool EndsWith(const string& str, const string& suffix) {
    size_t strSize = str.length();
    size_t suffixSize = suffix.length();
    if (strSize < suffixSize) {
      return false;
    }
    return str.compare((strSize-suffixSize), suffixSize, suffix) == 0;
  }

  bool IsInStr(const string& str, char x) {
    return str.find(x) != string::npos;
  }

  uint16_t TwoCharToUint16(char high, char low) {
    return (((uint16_t)high & 0x00ff) << 8) | ((uint16_t)low & 0x00ff);
  }

  string GetCurrentTime(const string format) {
    time_t timeNow;
    time(&timeNow);
    size_t maxSize = 64;
    // string must be has enough storage space
    string timeStr = "";
    timeStr.resize(64);
    size_t len = strftime(const_cast<char*>(timeStr.c_str()), (size_t)maxSize, format.c_str(), localtime(&timeNow));
    timeStr.resize(len);
    return timeStr;
  }
  string &ReplaceAll(string &str, const string &oldValue, const string &newValue) {
    while (true) {
      string::size_type pos(0);
      if ((pos = str.find(oldValue)) != string::npos) {
        str.replace(pos, oldValue.length(), newValue);
      } else {
        break;
      }
    }
    return str;
  }
  int SplitString2vector(string s, string sep, vector<string> &result) {
    if (s.size() == 0)
      return 0;
    string::size_type pos_begin = s.find_first_not_of(sep);
    string::size_type pos = 0;
    string tmp;
    while (pos_begin != string::npos) {
      pos = s.find(sep, pos_begin);
      if (pos != string::npos) {
        tmp = s.substr(pos_begin, pos - pos_begin);
        pos_begin = pos + sep.length();
      } else {
        tmp = s.substr(pos_begin);
        pos_begin = pos;
      }

      if (!tmp.empty()) {
        result.push_back(tmp);
        tmp.clear();
      }
    }
    return 1;
  }

  bool Split(string sOriginal, char *pDividers,
                    string &rsFirstPart, string &rsSecondPart) {

    int iCharPos = sOriginal.find_first_of(pDividers);
    if (iCharPos == -1) {
      // if the character was not found
      rsFirstPart = sOriginal;
      rsSecondPart = "";
      return false;
    } else {
      // if the character was found
      rsFirstPart = sOriginal.substr(0, iCharPos);
      rsSecondPart = sOriginal.substr(iCharPos + 1,
                                      sOriginal.length() - iCharPos);
      return true;
    }
  }

}
