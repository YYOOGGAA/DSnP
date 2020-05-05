/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
   // TODO: design your own class!!
	friend class BSTree<T>;
    friend class BSTree<T>::iterator;

    BSTreeNode(const T& d, BSTreeNode<T>* l = 0, BSTreeNode<T>* r = 0, BSTreeNode<T>* p = 0):
       _data(d), _childl(l), _childr(r), _parent(p) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
    T               _data;
    BSTreeNode<T>*  _childl;
    BSTreeNode<T>*  _childr;
    BSTreeNode<T>*  _parent;
};


template <class T>
class BSTree
{
   // TODO: design your own class!!
public:
   BSTree() {
      _dum = new BSTreeNode<T>(T());
      _root = _dum;
      _size = 0;
   }
   ~BSTree() { clear(); delete _dum; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class BSTree;

   public:
      iterator(BSTreeNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }

      iterator& operator ++ () { 
      	 if (_node->_childr != 0) _node = TreeMin(_node->_childr);
      	 else {BSTreeNode<T>* y = _node->_parent;
      	    while (y != 0 && _node == y->_childr)
      	    {
      	    	_node = y;
      	    	y = y->_parent;
      	    }
      	    _node = y;
      	 }
      	 return *(this);
      }
      iterator operator ++ (int) { iterator ret = *(this); ++*(this);return ret;}

      iterator& operator -- () { 
         if (_node->_childl != 0) _node = TreeMax(_node->_childl);
         else
         {
            BSTreeNode<T>* y = _node->_parent;
            while (y != 0 && _node == y->_childl)
      	    {
      	 	   _node = y;
      	 	   y = y->_parent;
      	    }
      	    _node = y;
         }
      	 return *(this);
      }
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
      BSTreeNode<T>* _node;
      BSTreeNode<T>* TreeMin (BSTreeNode<T>* _r) const
      {
      	 BSTreeNode<T>* _n = _r;
         while (_n->_childl != 0)
         {
            _n = _n->_childl;
         }
         return _n;
      }
      BSTreeNode<T>* TreeMax(BSTreeNode<T>* _r) const
      {
      	 BSTreeNode<T>* _n = _r;
         while (_n->_childr != 0)
         {
      	    _n = _n->_childr;
         }
         return _n;
      }
   };

   // TODO: implement these functions
   iterator begin() const { 
   	  if (empty()) return end();
   	  BSTreeNode<T>* _min = FindMin(_root);
      return iterator(_min);
   }
   iterator end() const { return iterator(_dum);}
   bool empty() const {
      if (_root == _dum) {return true;}
      return false; 
   }
   size_t size() const { return _size;}

   void insert(const T& z){
      if (empty()) 
      {
      	 _root = new BSTreeNode<T>(z,0,0,_dum);
      	 _dum->_childl = _root;
      }
      else
      {
         BSTreeNode<T>* y = 0;
         BSTreeNode<T>* x = _root;
         while (x != 0)
         {
            y = x;
            if (z < x->_data) x = x->_childl;
            else x = x->_childr;
         }
         BSTreeNode<T>* _n = new BSTreeNode<T>(z,0,0,y);
         if (z < y->_data) y->_childl = _n;
         else y->_childr = _n;
      }
      _size++;
   }
   void pop_front() { 
      if (!empty())
      {
      	_delete(begin());
      }
   }
   void pop_back() { 
      if (!empty())
      {
      	 BSTreeNode<T>*  _max = FindMax(_root);
      	 _delete(iterator(_max));
      }
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
      if (empty()) return false;
      _delete(pos);
      return true;
   }
   bool erase(const T& x) {
      iterator pos = find(x);
      if (pos != end())
      {
      	return erase(pos);
      } 
      return false;
   }

   iterator find(const T& k) { 
      if (empty()) return end();
      BSTreeNode<T>* x =_root;
      while (x != 0 && k != x->_data)
      {
      	if (k < x->_data) x = x->_childl;
      	else x = x->_childr;
      }
      if (x) return iterator(x);
      return end();
   }

   void clear() { while(!empty()) pop_front();_size = 0;}  // delete all nodes except for the dummy node

   void sort() const {}
   void print() const {}


private:
   // [NOTE] DO NOT ADD or REMOVE any data member 
   BSTreeNode<T>*  _root;
   BSTreeNode<T>*  _dum;
   size_t _size;

   // [OPTIONAL TODO] helper functions; called by public member functions
   BSTreeNode<T>* FindMax(BSTreeNode<T>* _r) const
   {
      BSTreeNode<T>* _n = _r;
      while (_n->_childr != 0 && _n->_childr != _dum)
      {
      	 _n = _n->_childr;
      }
      return _n;
   }
   BSTreeNode<T>* FindMin (BSTreeNode<T>* _r) const
   {
      BSTreeNode<T>* _n = _r;
      while (_n->_childl != 0)
      {
         _n = _n->_childl;
      }
      return _n;
    }

   void _delete(iterator pos)
   {
   	  if (pos == end()) return;
   	  BSTreeNode<T>* z = pos._node;
      if (z->_childl == 0)
      {
      	transplant(z,z->_childr);
      	if (_root == 0) _root = _dum;
      }
      else if (z->_childr == 0) 
      {
      	 transplant(z,z->_childl);
      }
      else 
      {
      	 BSTreeNode<T>* y = FindMin(z->_childr);
      	 if (y->_parent != z)
      	 {
      	 	transplant(y,y->_childr);
      	 	y->_childr = z->_childr;
      	 	y->_childr->_parent = y;
      	 }
      	 transplant(z,y);
      	 y->_childl = z->_childl;
      	 y->_childl->_parent = y;
      }
      delete pos._node;
      _size--;
   }

   void transplant(BSTreeNode<T>* u, BSTreeNode<T>* v)
   {
   	  if (u->_parent == _dum) _root = v;
   	  if (u == u->_parent->_childl)
   	  	 u->_parent->_childl = v;
   	  else u->_parent->_childr = v;
   	  if (v != 0) v->_parent = u->_parent;
   }
};

#endif // BST_H
