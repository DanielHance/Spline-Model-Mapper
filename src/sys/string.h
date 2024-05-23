#ifndef SYS_STRING_H
#define SYS_STRING_H

#include "base.h"
#include "vector.h"
#include "panic.h"
#include "config.h"

#include <string.h>

namespace sys
{

//struct string;
//inline string operator+(const string& _a, const string& _b);

struct string : base
{
  string();

  string& operator+=(const string& _value);
  string operator+(const string& _value) const;

  string(const char* _value);
  string& operator=(const char* _value);
  string& operator+=(const char* _value);
  string operator+(const char* _value) const;

  string(char _value);
  string& operator+=(char _value);
  string operator+(char _value) const;

  /***********************************************************
   * MACRO
   ***********************************************************/
  #define SYS_STRING_APPEND(T, S)                \
  string(T _value)                               \
  {                                              \
    m_data.push_back(0);                         \
    *this += _value;                             \
  }                                              \
                                                 \
  string& operator=(T _value)                    \
  {                                              \
    char buffer[128] = {0};                      \
    snprintf(buffer, sizeof(buffer), S, _value); \
    *this = buffer;                              \
                                                 \
    return *this;                                \
  }                                              \
                                                 \
  string& operator+=(T _value)                   \
  {                                              \
    char buffer[128] = {0};                      \
    snprintf(buffer, sizeof(buffer), S, _value); \
    *this += buffer;                             \
                                                 \
    return *this;                                \
  }                                              \
                                                 \
  string operator+(T _value) const               \
  {                                              \
    string rtn(*this);                           \
    rtn += _value;                               \
                                                 \
    return rtn;                                  \
  }                                              \

  SYS_STRING_APPEND(signed short, "%hi")
  SYS_STRING_APPEND(unsigned short, "%hu")
  SYS_STRING_APPEND(int, "%i")
  SYS_STRING_APPEND(unsigned int, "%u")
  SYS_STRING_APPEND(float, "%f")
  SYS_STRING_APPEND(double, "%f")
  SYS_STRING_APPEND(long double, "%Lf")
  SYS_STRING_APPEND(signed long, "%li")
  //SYS_STRING_APPEND(unsigned long, "%lu")
  SYS_STRING_APPEND(size_t, "%zu")

  bool operator==(const string& _other) const
  {
    if(strcmp(m_data.unsafe_raw(), _other.m_data.unsafe_raw()) == 0)
    {
      return true;
    }

    return false;
  }

  bool operator!=(const string& _other) const
  {
    return !(*this == _other);
  }

  bool operator<(const string& _other) const
  {
    if(strcmp(m_data.unsafe_raw(), _other.m_data.unsafe_raw()) < 0)
    {
      return true;
    }

    return false;
  }

  bool operator>(const string& _other) const
  {
    if(strcmp(m_data.unsafe_raw(), _other.m_data.unsafe_raw()) > 0)
    {
      return true;
    }

    return false;
  }

  bool operator<=(const string& _other) const
  {
    if(strcmp(m_data.unsafe_raw(), _other.m_data.unsafe_raw()) <= 0)
    {
      return true;
    }

    return false;
  }

  bool operator>=(const string& _other) const
  {
    if(strcmp(m_data.unsafe_raw(), _other.m_data.unsafe_raw()) >= 0)
    {
      return true;
    }

    return false;
  }

  char& operator[](
#ifdef SYS_DEBUG
    const index_lock& _index
#else
    size_t _index
#endif
  ) {
    if(_index >= length())
    {
      printf("Character index out of bounds %i, %i", (int)_index, (int)length());
      panic("Character index out of bounds");
    }

    return m_data[_index];
  }

  const char& operator[](
#ifdef SYS_DEBUG
    const index_lock& _index
#else
    size_t _index
#endif
  ) const {
    if(_index >= length())
    {
      panic("Character (const) index out of bounds");
    }

    return m_data[_index];
  }

  size_t length() const
  {
    return m_data.size() - 1;
  }

  void resize(size_t _newSize)
  {
    m_data.resize(_newSize + 1);
    m_data[_newSize] = 0;
  }

  void clear()
  {
    m_data.resize(1);
    m_data[0] = 0;
  }

  const char* unsafe_raw() const
  {
    return m_data.unsafe_raw();
  }

  string substring(size_t _begin, size_t _count) const
  {
    if(_begin + _count > length())
    {
      sys::panic("Substring out of bounds");
    }

    string rtn;

    for(size_t i = _begin; i < _begin + _count; ++i)
    {
      rtn += m_data[i];
    }

    return rtn;
  }

  string substring(size_t _begin) const
  {
    if(_begin > length())
    {
      sys::panic("Substring begins out of bounds");
    }

    return substring(_begin, length() - _begin);
  }

private:
  vector<char> m_data;

};

}

/* Implementation */

#define SYS_TAKE_RAW                 \
  size_t len = strlen(_value);       \
  m_data.resize(len + 1);            \
                                     \
  for(size_t ci = 0; ci < len; ++ci) \
  {                                  \
    m_data[ci] = _value[ci];         \
  }                                  \
                                     \
  m_data[m_data.size() - 1] = 0;     \

namespace sys
{

inline string::string()
{
  m_data.push_back(0);
}

/***********************************************************
 * char
 ***********************************************************/
inline string::string(char _value)
{
  m_data.push_back(_value);
  m_data.push_back(0);
}

inline string& string::operator+=(char _value)
{
  m_data[m_data.size() - 1] = _value;
  m_data.push_back(0);

  return *this;
}

inline string string::operator+(char _value) const
{
  string rtn(*this);
  rtn += _value;

  return rtn;
}

/***********************************************************
 * string
 ***********************************************************/
inline string& string::operator+=(const string& _value)
{
  size_t len = _value.length();
  size_t cs = length();

  m_data.resize(cs + len + 1);

  for(size_t ci = 0; ci < len; ++ci)
  {
    m_data[cs + ci] = _value[ci];
  }

  m_data[m_data.size() - 1] = 0;

  return *this;
}

inline string string::operator+(const string& _value) const
{
  string rtn(*this);
  rtn += _value;

  return rtn;
}

/***********************************************************
 * const char*
 ***********************************************************/
inline string::string(const char* _value)
{
  SYS_TAKE_RAW
}

inline string& string::operator=(const char* _value)
{
  SYS_TAKE_RAW

  return *this;
}

inline string& string::operator+=(const char* _value)
{
  size_t len = strlen(_value);
  size_t cs = length();

  m_data.resize(cs + len + 1);

  for(size_t ci = 0; ci < len; ++ci)
  {
    m_data[cs + ci] = _value[ci];
  }

  m_data[m_data.size() - 1] = 0;

  return *this;
}

inline string string::operator+(const char* _value) const
{
  string rtn(*this);
  rtn += _value;

  return rtn;
}

inline void panic(const string& _message)
{
  printf("Panic: %s\n", _message.unsafe_raw());
  abort();
}

inline void out(const string& _message)
{
  fprintf(stdout, "%s", _message.unsafe_raw());
}

inline void err(const string& _message)
{
  fprintf(stderr, "%s", _message.unsafe_raw());
}

/*
inline string operator+(const string& _a, const string& _b)
{
  string rtn(_a);
  rtn += _b;

  return rtn;
}
*/

namespace internal
{

inline char* string_buffer()
{
  static char rtn[SYS_STRING_BUFFER] = {0};

  return rtn;
}

}

inline string to_string(int _value)
{
  char* buffer = internal::string_buffer();

  snprintf(buffer, sizeof(buffer[0]) * SYS_STRING_BUFFER, "%i", _value);

  return buffer;
}

inline string to_string(double _value)
{
  char* buffer = internal::string_buffer();

  snprintf(buffer, sizeof(buffer[0]) * SYS_STRING_BUFFER, "%f", _value);

  return buffer;
}

inline string operator+(const char* _a, const string& _b)
{
  string rtn(_a);
  rtn += _b;

  return rtn;
}

inline string operator+(char _a, const string& _b)
{
  string rtn(_a);
  rtn += _b;

  return rtn;
}

inline string operator+(int _a, const string& _b)
{
  string rtn(_a);
  rtn += _b;

  return rtn;
}


}

#endif

