//
// Created by alex on 17-7-23.
//

//************************************
// File: StringUtil.h
// Autor: ShenChengEn - ubuntu733@gmail.com
// Description: ---
// create: 17-7-23 18:18
//************************************

#ifndef HUOGUONLP_STRINGUTIL_H
#define HUOGUONLP_STRINGUTIL_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;
namespace huoguo {

  template<typename T>
  string toString(const T &t) {
    std::ostringstream oss;  //创建一个格式化输出流
    oss << t;             //把值传递如流中
    return oss.str();
    // return std::to_string(t);
  }

  template<typename T>
  string VectorToString(std::vector<T> vector, string div = ";") {
    if (vector.empty()) return "";
    string result = "";
    for (auto iter = vector.begin(); iter != vector.end(); iter++) {
      result += toString(*iter) + div;
    }
    if (div=="") {
      return result;
    }
    return result.substr(0, result.size() - 1);
  }

  template<typename T>
  string VectorVectorToString(std::vector<vector<T>> vector, string div = ";") {
    if (vector.empty()) return "";
    string result = "";
    for (auto iter = vector.begin(); iter != vector.end(); iter++) {
      result += VectorToString(*iter,div) + div;
    }
    if (div == "") return result;
    return result.substr(0, result.size() - 1);
  }
/*  bool StringFormat(string& str, const char* fmt, ...);


  const char* StringFormat(const char *fmt, ...);*/
  string StringFormat(const size_t maxSize, const char* fmt, ...);
  template<typename T>
  bool Join(T begin, T end, string& str, const string& connector);


  template<typename T>
  string Join(T begin, T end, const string& connector);


  bool Split(const string& str, vector<string>& vec, const string& sep, bool saveEmpty = false);

  string &ReplaceAll(string &str, const string &oldValue, const string &newValue);
  int SplitString2vector(string s, string sep, vector<string> &result);

  bool Split(string sOriginal, char *pDividers,
                    string &rsFirstPart, string &rsSecondPart);


  size_t Split(const string& str, const string& sep, bool saveEmpty = false);


  void Upper(string& str);


  void Lower(string& str);


  string Ltrim(string& str, const char x = ' ');


  string Rtrim(string &str, const char x = ' ');


  string Trim(string &str, const char x = ' ');
  string Trim(char* ptr, const char x = ' ');


  bool StartsWith(const string& str, const string& prefix);
  bool StartsWith(const char* ptr, const string& prefix);


  bool EndsWith(const string& str, const string& suffix);


  bool IsInStr(const string& str, char x);


  uint16_t TwoCharToUint16(char high, char low);


  string GetCurrentTime(const string format = "%Y-%m-%d %H:%M:%S");
}
#endif //HUOGUONLP_STRINGUTIL_H
