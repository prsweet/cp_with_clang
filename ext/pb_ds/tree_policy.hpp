// -*- C++ -*-

// Copyright (C) 2005-2026 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License as published by the Free Software
// Foundation; either version 3, or (at your option) any later
// version.

// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

// Copyright (C) 2004 Ami Tavory and Vladimir Dreizin, IBM-HRL.

// Permission to use, copy, modify, sell, and distribute this software
// is hereby granted without fee, provided that the above copyright
// notice appears in all copies, and that both that copyright notice
// and this permission notice appear in supporting documentation. None
// of the above authors, nor IBM Haifa Research Laboratories, make any
// representation about the suitability of this software for any
// purpose. It is provided "as is" without express or implied
// warranty.

/**
 * @file tree_policy.hpp
 * Contains tree-related policies.
 */

#ifndef PB_DS_TREE_POLICY_HPP
#define PB_DS_TREE_POLICY_HPP

#if __has_include(<bits/c++config.h>)
# include <bits/c++config.h>
#endif
#include <iterator>
#include <memory>
#include <type_traits>

#if __has_include(<ext/pb_ds/detail/type_utils.hpp>)
# include <ext/pb_ds/detail/type_utils.hpp>
#else
namespace __gnu_pbds {
  namespace detail {
    template<typename _Alloc, typename T>
    struct rebind_traits : std::allocator_traits<_Alloc>::template rebind_traits<T>
    {
      using reference = T&;
      using const_reference = const T&;
    };
  }
}
#endif

#if __has_include(<ext/pb_ds/detail/branch_policy/branch_policy.hpp>)
# include <ext/pb_ds/detail/branch_policy/branch_policy.hpp>
#else
namespace __gnu_pbds {
  namespace detail {
    /// Primary template, base class for branch structure policies.
    template<typename Node_CItr, typename Node_Itr, typename _Alloc>
    struct branch_policy
    {
    protected:
      typedef typename Node_Itr::value_type             it_type;
      typedef typename std::iterator_traits<it_type>::value_type value_type;
      typedef typename value_type::first_type           key_type;

      typedef typename std::remove_const<value_type>::type rcvalue_type;
      typedef typename std::remove_const<key_type>::type   rckey_type;

      typedef rebind_traits<_Alloc, rcvalue_type>       rebind_v;
      typedef rebind_traits<_Alloc, rckey_type>         rebind_k;

      typedef typename rebind_v::reference              reference;
      typedef typename rebind_v::const_reference        const_reference;
      typedef typename rebind_v::const_pointer          const_pointer;

      typedef typename rebind_k::const_reference        key_const_reference;

      static inline key_const_reference
      extract_key(const_reference r_val)
      { return r_val.first; }

      virtual it_type
      end() = 0;

      it_type
      end_iterator() const
      { return const_cast<branch_policy*>(this)->end(); }

      virtual
      ~branch_policy() { }
    };

    /// Specialization for const iterators.
    template<typename Node_CItr, typename _Alloc>
    struct branch_policy<Node_CItr, Node_CItr, _Alloc>
    {
    protected:
      typedef typename Node_CItr::value_type               it_type;
      typedef typename std::iterator_traits<it_type>::value_type value_type;
      typedef typename std::remove_const<value_type>::type         rcvalue_type;
      typedef rebind_traits<_Alloc, rcvalue_type>       rebind_v;
      typedef typename rebind_v::reference              reference;
      typedef typename rebind_v::const_reference        const_reference;
      typedef typename rebind_v::const_pointer          const_pointer;

      typedef value_type                                key_type;
      typedef typename rebind_v::const_reference        key_const_reference;

      static inline key_const_reference
      extract_key(const_reference r_val)
      { return r_val; }

      virtual it_type
      end() const = 0;

      it_type
      end_iterator() const
      { return end(); }

      virtual
      ~branch_policy() { }
    };
  } // namespace detail
} // namespace __gnu_pbds
#endif

namespace __gnu_pbds
{
#define PB_DS_CLASS_T_DEC \
  template<typename Node_CItr, typename Node_Itr, typename Cmp_Fn, \
	   typename _Alloc>

#define PB_DS_CLASS_C_DEC \
  tree_order_statistics_node_update<Node_CItr, Node_Itr, Cmp_Fn, _Alloc>

#define PB_DS_BRANCH_POLICY_BASE \
  detail::branch_policy<Node_CItr, Node_Itr, _Alloc>

  /// Functor updating ranks of entrees.
  template<typename Node_CItr, typename Node_Itr, 
	   typename Cmp_Fn, typename _Alloc>
  class tree_order_statistics_node_update : private PB_DS_BRANCH_POLICY_BASE
  {
  private:
    typedef PB_DS_BRANCH_POLICY_BASE 		       	base_type;

  public:
    typedef Cmp_Fn 					cmp_fn;
    typedef _Alloc 					allocator_type;
    typedef typename allocator_type::size_type 		size_type;
    typedef typename base_type::key_type 		key_type;
    typedef typename base_type::key_const_reference 	key_const_reference;

    typedef size_type 					metadata_type;
    typedef Node_CItr 	       			node_const_iterator;
    typedef Node_Itr 					node_iterator;
    typedef typename node_const_iterator::value_type 	const_iterator;
    typedef typename node_iterator::value_type 		iterator;

    /// Finds an entry by __order. Returns a const_iterator to the
    /// entry with the __order order, or a const_iterator to the
    /// container object's end if order is at least the size of the
    /// container object.
    inline const_iterator
    find_by_order(size_type) const;

    /// Finds an entry by __order. Returns an iterator to the entry
    /// with the __order order, or an iterator to the container
    /// object's end if order is at least the size of the container
    /// object.
    inline iterator
    find_by_order(size_type);

    /// Returns the order of a key within a sequence. For example, if
    /// r_key is the smallest key, this method will return 0; if r_key
    /// is a key between the smallest and next key, this method will
    /// return 1; if r_key is a key larger than the largest key, this
    /// method will return the size of r_c.
    inline size_type
    order_of_key(key_const_reference) const;

  private:
    /// Const reference to the container's value-type.
    typedef typename base_type::const_reference 	const_reference;

    /// Const pointer to the container's value-type.
    typedef typename base_type::const_pointer 		const_pointer;

    /// Const metadata reference.
    typedef const metadata_type&                        metadata_const_reference;

    /// Metadata reference.
    typedef metadata_type&                              metadata_reference;

    /// Returns the node_const_iterator associated with the tree's root node.
    virtual node_const_iterator
    node_begin() const = 0;

    /// Returns the node_iterator associated with the tree's root node.
    virtual node_iterator
    node_begin() = 0;

    /// Returns the node_const_iterator associated with a just-after leaf node.
    virtual node_const_iterator
    node_end() const = 0;

    /// Returns the node_iterator associated with a just-after leaf node.
    virtual node_iterator
    node_end() = 0;

    /// Access to the cmp_fn object.
    virtual cmp_fn& 
    get_cmp_fn() = 0;

  protected:
    /// Updates the rank of a node through a node_iterator node_it;
    /// end_nd_it is the end node iterator.
    inline void
    operator()(node_iterator, node_const_iterator) const;

    virtual
    ~tree_order_statistics_node_update();
  };

#if __has_include(<ext/pb_ds/detail/tree_policy/order_statistics_imp.hpp>)
# include <ext/pb_ds/detail/tree_policy/order_statistics_imp.hpp>
#else
PB_DS_CLASS_T_DEC
inline typename PB_DS_CLASS_C_DEC::iterator
PB_DS_CLASS_C_DEC::
find_by_order(size_type order)
{
  node_iterator it = node_begin();
  node_iterator end_it = node_end();

  while (it != end_it)
    {
      node_iterator l_it = it.get_l_child();
      const size_type o = (l_it == end_it)? 0 : l_it.get_metadata();

      if (order == o)
	return *it;
      else if (order < o)
	it = l_it;
      else
        {
	  order -= o + 1;
	  it = it.get_r_child();
        }
    }

  return base_type::end_iterator();
}

PB_DS_CLASS_T_DEC
inline typename PB_DS_CLASS_C_DEC::const_iterator
PB_DS_CLASS_C_DEC::
find_by_order(size_type order) const
{ return const_cast<PB_DS_CLASS_C_DEC*>(this)->find_by_order(order); }

PB_DS_CLASS_T_DEC
inline typename PB_DS_CLASS_C_DEC::size_type
PB_DS_CLASS_C_DEC::
order_of_key(key_const_reference r_key) const
{
  node_const_iterator it = node_begin();
  node_const_iterator end_it = node_end();

  const cmp_fn& r_cmp_fn = const_cast<PB_DS_CLASS_C_DEC*>(this)->get_cmp_fn();
  size_type ord = 0;
  while (it != end_it)
    {
      node_const_iterator l_it = it.get_l_child();

      if (r_cmp_fn(r_key, this->extract_key(*(*it))))
	it = l_it;
      else if (r_cmp_fn(this->extract_key(*(*it)), r_key))
        {
	  ord += (l_it == end_it)? 1 : 1 + l_it.get_metadata();
	  it = it.get_r_child();
        }
      else
        {
	  ord += (l_it == end_it)? 0 : l_it.get_metadata();
	  it = end_it;
        }
    }
  return ord;
}

PB_DS_CLASS_T_DEC
inline void
PB_DS_CLASS_C_DEC::
operator()(node_iterator node_it, node_const_iterator end_nd_it) const
{
  node_iterator l_it = node_it.get_l_child();
  const size_type l_rank = (l_it == end_nd_it) ? 0 : l_it.get_metadata();

  node_iterator r_it = node_it.get_r_child();
  const size_type r_rank = (r_it == end_nd_it) ? 0 : r_it.get_metadata();

  const_cast<metadata_reference>(node_it.get_metadata())= 1 + l_rank + r_rank;
}

PB_DS_CLASS_T_DEC
PB_DS_CLASS_C_DEC::
~tree_order_statistics_node_update()
{ }
#endif

#undef PB_DS_CLASS_T_DEC
#undef PB_DS_CLASS_C_DEC
#undef PB_DS_BRANCH_POLICY_BASE

} // namespace __gnu_pbds

#endif 
