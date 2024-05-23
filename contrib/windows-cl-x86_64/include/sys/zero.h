#ifndef SYS_ZERO_H
#define SYS_ZERO_H

namespace sys
{

/***************************************************************************
 * zero
 *
 * Provides a way for member variables to be initialized to zero implicitly
 * rather than via initializer lists. Acts as a template wrapper class which
 * forwards through the contained internal variable but also initializing to
 * zero in its constructor.
 ***************************************************************************/
template <typename T>
struct zero
{
  zero();

  zero(const T& _value);
  zero& operator=(const T& _value);

  operator T&();
  operator const T&() const;

  T* operator&();
  const T* operator&() const;

  T operator->();
  const T operator->() const;

  T& get();
  const T& get() const;

private:
  T m_value;
};

}

/* Implementation */

namespace sys
{

template <typename T>
zero<T>::zero() :
  m_value(0)
{ }

template <typename T>
zero<T>::zero(const T& _value) :
  m_value(_value)
{ }

template <typename T>
zero<T>& zero<T>::operator=(const T& _value)
{
  m_value = _value;

  return *this;
}

template <typename T>
zero<T>::operator T&()
{
  return m_value;
}

template <typename T>
zero<T>::operator const T&() const
{
  return m_value;
}

template <typename T>
T* zero<T>::operator&()
{
  return &m_value;
}

template <typename T>
const T* zero<T>::operator&() const
{
  return &m_value;
}

template <typename T>
T zero<T>::operator->()
{
  return m_value;
}

template <typename T>
const T zero<T>::operator->() const
{
  return m_value;
}

template <typename T>
T& zero<T>::get()
{
  return m_value;
}

template <typename T>
const T& zero<T>::get() const
{
  return m_value;
}

}

#endif

