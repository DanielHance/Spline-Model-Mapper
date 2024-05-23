#ifndef SYS_LINKEDLIST_H
#define SYS_LINKEDLIST_H

#include "autoptr.h"
#include "ptr.h"
#include "config.h"
#include "base.h"

namespace sys
{

#ifdef SYS_DEBUG
template <typename T> struct ref_lock;
template <typename T> struct ptr_lock;
#endif

template <typename T>
struct linked_list : base
{
private:
  struct node : base
  {
    node();
    node(const T& _value);

    T m_value;
    auto_ptr<node> m_next;
    ptr<node> m_prev;
  };

public:
  struct iterator
  {
    iterator& operator++();
    bool operator!=(const iterator& _other) const;
    bool operator==(const iterator& _other) const;

#ifdef SYS_DEBUG
    ptr_lock<T> operator->() const;
    ref_lock<T> operator*() const;
#else
    T* operator->() const;
    T& operator*() const;
#endif

  private:
    template <typename U>
    friend struct linked_list;

    mutable ptr<node> m_node;
  };

  linked_list();
  linked_list(const linked_list& _copy);
  linked_list& operator=(const linked_list& _assign);
  ~linked_list();

  ptr<T> push_back(const T& _value);
  //ptr<T> emplace_back();
  iterator emplace_back();
  void clear();
  iterator begin() const;
  iterator end() const;
  iterator erase(const iterator& _it);

private:
  auto_ptr<node> m_first;
  ptr<node> m_last;

};

/* Implementation */

#include "panic.h"

template <typename T>
linked_list<T>::node::node()
{ }

template <typename T>
linked_list<T>::node::node(const T& _value) :
  m_value(_value)
{ }

#define REF_LIST_ATTACH           \
  if(m_last)                      \
  {                               \
    strong->m_prev = m_last;      \
    m_last->m_next = strong;      \
    m_last = weak;                \
  }                               \
  else                            \
  {                               \
    m_first = strong;             \
    m_last = weak;                \
  }

template <typename T>
ptr<T> linked_list<T>::push_back(const T& _value)
{
  auto_ptr<node> strong = make_auto<typename linked_list<T>::node>(_value);
  ptr<node> weak(strong);

  REF_LIST_ATTACH

  ptr<T> rtn(&weak->m_value
#ifdef SYS_DEBUG
  , weak->critical_pin_lock()
#endif
  );

  return rtn;
}

/*
template <typename T>
ptr<T> linked_list<T>::emplace_back()
{
  auto_ptr<node> strong = make_auto<typename linked_list<T>::node>();
  ptr<node> weak(strong);

  REF_LIST_ATTACH

  ptr<T> rtn(&weak->m_value
#ifdef SYS_DEBUG
  , weak->critical_pin_lock()
#endif
  );

  return rtn;
}
*/

template <typename T>
typename linked_list<T>::iterator linked_list<T>::emplace_back()
{
  auto_ptr<node> strong = make_auto<typename linked_list<T>::node>();
  ptr<node> weak(strong);

  REF_LIST_ATTACH

  iterator rtn;
  rtn.m_node = weak;

  return rtn;
}

template <typename T>
linked_list<T>::linked_list()
{ }

template <typename T>
linked_list<T>::linked_list(const linked_list& _copy)
{
  for(typename linked_list::iterator it = _copy.begin();
    it != _copy.end(); ++it)
  {
    push_back(*it);
  }
}

template <typename T>
linked_list<T>& linked_list<T>::operator=(const linked_list& _assign)
{
  auto_ptr<node> firstsink(m_first);
  m_last.reset();

  for(typename linked_list::iterator it = _assign.begin();
    it != _assign.end(); ++it)
  {
    push_back(*it);
  }

  return *this;
}

template <typename T>
linked_list<T>::~linked_list()
{
  clear();
}

template <typename T>
void linked_list<T>::clear()
{
  m_last.reset();
  m_first.reset();
}

template <typename T>
typename linked_list<T>::iterator& linked_list<T>::iterator::operator++()
{
  if(!m_node)
  {
    sys::panic("Invalid iterator or past end");
  }

  ptr<node> next = m_node->m_next;

  m_node = next;

  return *this;
}

template <typename T>
bool linked_list<T>::iterator::operator!=(const iterator& _other) const
{
  bool rtn = m_node == _other.m_node;

  return !rtn;
}

template <typename T>
bool linked_list<T>::iterator::operator==(const iterator& _other) const
{
  bool rtn = m_node == _other.m_node;

  return rtn;
}

#ifdef SYS_DEBUG
template <typename T>
ptr_lock<T> linked_list<T>::iterator::operator->() const
{
  return ptr_lock<T>(&m_node->m_value, m_node.critical_pin_lock());
}

template <typename T>
ref_lock<T> linked_list<T>::iterator::operator*() const
{
  return ref_lock<T>(&m_node->m_value, m_node.critical_pin_lock());
}
#else
template <typename T>
T* linked_list<T>::iterator::operator->() const
{
  return &m_node->m_value;
}

template <typename T>
T& linked_list<T>::iterator::operator*() const
{
  return m_node->m_value;
}
#endif

template <typename T>
typename linked_list<T>::iterator linked_list<T>::begin() const
{
  if(!m_first)
  {
    return end();
  }

  iterator rtn;
  rtn.m_node = m_first;

  return rtn;
}

template <typename T>
typename linked_list<T>::iterator linked_list<T>::end() const
{
  iterator rtn;

  return rtn;
}

template <typename T>
typename linked_list<T>::iterator linked_list<T>::erase(const iterator& _it)
{
  ptr<node> curr = _it.m_node;

  if(curr->m_prev && curr->m_next)
  {
    curr->m_prev->m_next = curr->m_next;
    curr->m_next->m_prev = curr->m_prev;
  }
  else if(curr->m_prev)
  {
    curr->m_prev->m_next.reset();
  }
  else if(curr->m_next)
  {
    curr->m_next->m_prev.reset();
  }

  if(!curr->m_prev)
  {
    m_first = curr->m_next;
  }

  if(!curr->m_next)
  {
    m_last = curr->m_prev;
  }

  iterator rtn(_it);
  ++rtn;

  _it.m_node.reset();

  return rtn;
}

}

#endif

