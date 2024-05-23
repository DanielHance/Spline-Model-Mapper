#ifndef SYS_AUTOPTR_H
#define SYS_AUTOPTR_H

#include "config.h"
#include "criticalpin.h"
#include "weakpin.h"
#include "zero.h"

namespace sys
{

#ifdef SYS_DEBUG
template <typename T> struct ref_lock;
template <typename T> struct ptr_lock;
#endif

template <typename T> struct ptr;
template <typename T> struct weak_ptr;
template <typename T> struct auto_ptr;
template <typename T> auto_ptr<T> make_auto();
template <typename T, typename U> auto_ptr<T> make_auto(U _u);

struct auto_ptr_data
{
  zero<void (*)(const void*)> m_deleter;
  weak_pin m_weakpin;
#ifdef SYS_DEBUG
  critical_pin m_criticalpin;
#endif
};

template <typename T>
struct auto_ptr
{
  auto_ptr();
  auto_ptr(const auto_ptr& _copy);
  auto_ptr& operator=(const auto_ptr& _other);
  ~auto_ptr();

  template <typename U> auto_ptr(const auto_ptr<U>& _other);
  template <typename U> auto_ptr& operator=(const auto_ptr<U>& _other);
  template <typename U> bool operator==(const auto_ptr<U>& _other) const;
  template <typename U> bool operator!=(const auto_ptr<U>& _other) const;

  template <typename U> bool operator==(const weak_ptr<U>& _other) const;
  template <typename U> bool operator!=(const weak_ptr<U>& _other) const;

  template <typename U> bool operator==(const ptr<U>& _other) const;
  template <typename U> bool operator!=(const ptr<U>& _other) const;

  void reset();
  weak_pin::lock weak_pin_lock() const;

#ifdef SYS_DEBUG
  critical_pin::lock critical_pin_lock() const;

  ref_lock<T> operator*() const;
  ptr_lock<T> operator->() const;
  ptr_lock<T> get() const;
#else
  T& operator*() const;
  T* operator->() const;
  T* get() const;
#endif

private:
  template <typename U> friend struct auto_ptr;
  template <typename U> friend auto_ptr<U> make_auto();
  template <typename U, typename V> friend auto_ptr<U> make_auto(V _v);

  mutable zero<T*> m_ptr;
  mutable zero<auto_ptr_data*> m_data;

  static void deleter(const void* _ptr);

  void bool_func() const { }
  typedef void (auto_ptr::*bool_type)() const;

public:
  operator bool_type() const;
};

}

/* Implementation */

#include "panic.h"
#include "lock.h"

namespace sys
{

template <typename T>
auto_ptr<T>::auto_ptr()
{ }

template <typename T>
auto_ptr<T>::auto_ptr(const auto_ptr& _copy)
  : m_ptr(_copy.m_ptr)
  , m_data(_copy.m_data)
{
  _copy.m_ptr = NULL;
  _copy.m_data = NULL;
}

template <typename T>
auto_ptr<T>& auto_ptr<T>::operator=(const auto_ptr& _other)
{
  if(get() == _other.get()) return *this;

  auto_ptr tmp(*this);

  m_ptr = _other.m_ptr;
  m_data = _other.m_data;

  _other.m_ptr = NULL;
  _other.m_data = NULL;

  return *this;
}

template <typename T>
void auto_ptr<T>::reset()
{
  if(*this)
  {
    m_data->m_deleter.get()(m_ptr);
    delete m_data.get();
  }

  m_ptr = NULL;
  m_data = NULL;
}

template <typename T>
auto_ptr<T>::~auto_ptr()
{
  reset();
}

template <typename T>
template <typename U>
auto_ptr<T>::auto_ptr(const auto_ptr<U>& _other)
  : m_ptr(_other.m_ptr)
  , m_data(_other.m_data)
{
  _other.m_ptr = NULL;
  _other.m_data = NULL;
}

template <typename T>
template <typename U>
auto_ptr<T>& auto_ptr<T>::operator=(const auto_ptr<U>& _other)
{
  if(get() == _other.get()) return *this;

  auto_ptr tmp(*this);

  m_ptr = _other.m_ptr;
  m_data = _other.m_data;

  _other.m_ptr = NULL;
  _other.m_data = NULL;

  return *this;
}

#ifdef SYS_DEBUG

template <typename T>
critical_pin::lock auto_ptr<T>::critical_pin_lock() const
{
  if(!*this)
  {
    return critical_pin::lock();
  }

  return critical_pin::lock(m_data->m_criticalpin);
}

template <typename T>
ref_lock<T> auto_ptr<T>::operator*() const
{
  if(!*this)
  {
    panic("Attempt to access NULL reference");
  }

  return ref_lock<T>(m_ptr, critical_pin_lock());
}

template <typename T>
ptr_lock<T> auto_ptr<T>::operator->() const
{
  if(!*this)
  {
    panic("Attempt to access NULL pointer");
  }

  return ptr_lock<T>(m_ptr, critical_pin_lock());
}

template <typename T>
ptr_lock<T> auto_ptr<T>::get() const
{
  if(!*this)
  {
    return ptr_lock<T>(NULL, critical_pin::lock());
  }

  return ptr_lock<T>(m_ptr, critical_pin_lock());
}

#else

template <typename T>
T& auto_ptr<T>::operator*() const
{
  return *m_ptr;
}

template <typename T>
T* auto_ptr<T>::operator->() const
{
  return m_ptr;
}

template <typename T>
T* auto_ptr<T>::get() const
{
  return m_ptr;
}

#endif

template <typename T>
void auto_ptr<T>::deleter(const void* _ptr)
{
  const T* ptr = static_cast<const T*>(_ptr);
  delete ptr;
}

template <typename T>
template <typename U>
bool auto_ptr<T>::operator==(const auto_ptr<U>& _other) const
{
  return get() == _other.get();
}

template <typename T>
template <typename U>
bool auto_ptr<T>::operator!=(const auto_ptr<U>& _other) const
{
  return get() != _other.get();
}

template <typename T>
template <typename U>
bool auto_ptr<T>::operator==(const ptr<U>& _other) const
{
  return get() == _other.get();
}

template <typename T>
template <typename U>
bool auto_ptr<T>::operator!=(const ptr<U>& _other) const
{
  return get() != _other.get();
}

template <typename T>
template <typename U>
bool auto_ptr<T>::operator==(const weak_ptr<U>& _other) const
{
  return get() == _other.get();
}

template <typename T>
template <typename U>
bool auto_ptr<T>::operator!=(const weak_ptr<U>& _other) const
{
  return get() != _other.get();
}

template <typename T>
auto_ptr<T>::operator bool_type() const
{
  // TODO: Issue on MSVC
/*
  if(m_ptr && m_data)
  {
    return &auto_ptr::bool_func;
  }

  return NULL;
*/

  if(m_ptr)
  {
    if(m_data)
    {
      return &auto_ptr::bool_func;
    }
  }

  return NULL;
}

template <typename T>
weak_pin::lock auto_ptr<T>::weak_pin_lock() const
{
  if(!*this)
  {
    return weak_pin::lock();
  }

  return weak_pin::lock(m_data->m_weakpin);
}

template <typename T>
auto_ptr<T> make_auto()
{
  auto_ptr<T> rtn;

  rtn.m_ptr = new T();
  rtn.m_data = new auto_ptr_data();
  rtn.m_data->m_deleter = auto_ptr<T>::deleter;

  return rtn;
}

template <typename T, typename U>
auto_ptr<T> make_auto(U _u)
{
  auto_ptr<T> rtn;

  rtn.m_ptr = new T(_u);
  rtn.m_data = new auto_ptr_data();
  rtn.m_data->m_deleter = auto_ptr<T>::deleter;

  return rtn;
}

template <typename T, typename U>
bool operator==(const auto_ptr<T>& _a, const U* _b)
{
  return _a.get() == _b;
}

template <typename T, typename U>
bool operator!=(const auto_ptr<T>& _a, const U* _b)
{
  return _a.get() != _b;
}

template <typename T, typename U>
bool operator==(const U* _a, const auto_ptr<T>& _b)
{
  return _a == _b.get();
}

template <typename T, typename U>
bool operator!=(const U* _a, const auto_ptr<T>& _b)
{
  return _a != _b.get();
}

}

#endif

