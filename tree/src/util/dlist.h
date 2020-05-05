/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>
#include <algorithm>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
      _isSorted = false;
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }//
      T& operator * () { return _node->_data; }//
      iterator& operator ++ () { _node = _node->_next; return *(this); }
      iterator operator ++ (int) { iterator ret = *(this); ++*(this);return ret; }
      iterator& operator -- () { _node = _node->_prev; return *(this); }
      iterator operator -- (int) { iterator ret = *(this); --*(this);return ret; }

      iterator& operator = (const iterator& i) { _node = i._node; return *(this); }

      bool operator != (const iterator& i) const {
         if (i._node != _node) return true;
         else return false; 
      }
      bool operator == (const iterator& i) const { 
         if (i._node == _node) return true;
         else return false;
      }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const {
      if (empty()) return end();
      return iterator(_head); 
   }
   iterator end() const { 
      if (empty()) return iterator(_head);
      return iterator(_dum); 
   }
   bool empty() const { 
      if (_head->_next == _head) return true;
      else return false; 
   }
   size_t size() const {  
      iterator i = begin();
      size_t k = 0;
      while(i != end())
      {
         ++i;
         ++k;
      }
      return k; 
   }

   void push_back(const T& x) { 
      if (empty())
      {
         _dum = _head;
         DListNode<T>* _e = new DListNode<T>(x,_dum,_dum);
         _head = _e;
         _dum->_next = _e;
         _dum->_prev = _e;
         _last = _e;
         //cerr << "node " << _e<<endl;
      } 
      else
      {
         DListNode<T>* _e = new DListNode<T>(x,_last,_dum);
         _last->_next = _e;
         _dum->_prev = _e;
         _last = _e;
         //cerr << "node " << _e<<endl;
      }
      _isSorted = false;
   }
   void pop_front() { 
      if (!empty())
      {
         _dum->_next = _head->_next;
         delete _head;
         _head = _dum->_next;
         _head->_prev = _dum;
      }
   }
   void pop_back() { 
      if (!empty())
      {
         _dum->_prev = _last->_prev;
         delete _last;
         _last = _dum->_prev;
         _last->_next = _dum;
      }
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
      if (empty()) return false;
      DListNode<T>* _pNext = pos._node->_next;
      DListNode<T>* _pPrev = pos._node->_prev;
      delete pos._node;
      _pPrev->_next = _pNext;
      _pNext->_prev = _pPrev;
      if (pos == _head) _head = _head->_next;
      if (pos == _last) _last = _last->_prev;
      return true; 
   }
   bool erase(const T& x) { 
      iterator i = find(x);
      if (i != end())
      {
         return erase(i);
      }
      else return false; 
   }

   iterator find(const T& x) { 
      iterator i = begin();
      while(i != end())
      {
         if( *(i) == x) return i;
         i++;
      }
      return end();
   }

   void clear() { while(!empty()) pop_front(); _isSorted = false;}  // delete all nodes except for the dummy node

   void sort() const { 
      if(_isSorted == true) return;
      iterator k = --end();
      ++end();
      if(!empty()) QuickSort(begin(),k);
      _isSorted = true;
   }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
   DListNode<T>* _dum;
   DListNode<T>* _last;

   iterator Partition(iterator _low, iterator _high) const
   {
      T x = *(_high);
      iterator i = --_low;
      ++_low;
      for (iterator j = _low; j != _high; j++)
      {
          if (j._node->_data <= x)
          {
            i++;
            T _i = *(i);
            *(i) = *(j);
            *(j) = _i;
          }
      }
      iterator _n = ++i;
      *(_high) = *(_n);
      *(_n) = x;
      return _n;
   }

   void QuickSort(iterator _low, iterator _high) const
   {
      if (compare(_low,_high))
      {
         iterator q = Partition(_low, _high);
         QuickSort(_low, --q);
         QuickSort(++q, _high);
      }
      else return;
   }

   bool compare(iterator _low, iterator _high) const
   {
      if (_low == _high) return false;
      if (--_low == _high) return false;
      return true;
   }
};

#endif // DLIST_H
