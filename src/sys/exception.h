#ifndef SYS_EXCEPTION_H
#define SYS_EXCEPTION_H

#include "string.h"

namespace sys
{

struct exception
{
  exception();
  exception(const string& _message);
  virtual ~exception();

  const string& what() const;
  void panic() const;

private:
  string m_message;

};

}

/* Implementation */

#include "io.h"
#include "panic.h"
#include "config.h"

namespace sys
{

inline exception::exception()
{ }

inline exception::exception(const string& _message)
  : m_message(_message)
{
#ifdef SYS_DEBUG
  out(string("[sys] throw Exception: ") + _message + '\n');
#endif
}

inline exception::~exception()
{ }

inline const string& exception::what() const
{
  return m_message;
}

inline void exception::panic() const
{
  sys::panic(m_message);
}

}

#endif

