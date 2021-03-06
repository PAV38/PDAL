// Boost.TypeErasure library
//
// Copyright 2012 Steven Watanabe
//
// Distributed under the Boost Software License Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// $Id: check_map.hpp 80883 2012-10-06 01:28:13Z steven_watanabe $

#ifndef BOOST_TYPE_ERASURE_DETAIL_CHECK_MAP_HPP_INCLUDED
#define BOOST_TYPE_ERASURE_DETAIL_CHECK_MAP_HPP_INCLUDED

#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/end.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_erasure/detail/get_placeholders.hpp>
#include <boost/type_erasure/deduced.hpp>
#include <boost/type_erasure/static_binding.hpp>

namespace pdalboost {} namespace boost = pdalboost; namespace pdalboost {
namespace type_erasure {
namespace detail {

template<class T>
struct is_deduced : pdalboost::mpl::false_ {};
template<class T>
struct is_deduced< ::pdalboost::type_erasure::deduced<T> > : pdalboost::mpl::true_ {};

// returns true if Map has a key for every non-deduced placeholder in Concept
template<class Concept, class Map>
struct check_map {
    typedef typename normalize_concept<Concept>::basic basic_components;
    
    // Every non-deduced placeholder referenced in this
    // map is indirectly deduced.
    typedef typename ::pdalboost::type_erasure::detail::get_placeholder_normalization_map<
        Concept>::type placeholder_subs;
    typedef typename ::pdalboost::mpl::fold<
        placeholder_subs,
        ::pdalboost::mpl::set0<>,
        ::pdalboost::mpl::insert<
            ::pdalboost::mpl::_1,
            ::pdalboost::mpl::second< ::pdalboost::mpl::_2>
        >
    >::type indirect_deduced_placeholders;

    typedef typename ::pdalboost::mpl::fold<
        basic_components,
        ::pdalboost::mpl::set0<>,
        ::pdalboost::type_erasure::detail::get_placeholders<
            ::pdalboost::mpl::_2,
            ::pdalboost::mpl::_1
        >
    >::type placeholders;
    typedef typename ::pdalboost::is_same<
        typename ::pdalboost::mpl::find_if<
            placeholders,
            ::pdalboost::mpl::not_<
                ::pdalboost::mpl::or_<
                    ::pdalboost::type_erasure::detail::is_deduced< ::pdalboost::mpl::_1>,
                    ::pdalboost::mpl::has_key<Map, ::pdalboost::mpl::_1>,
                    ::pdalboost::mpl::has_key<indirect_deduced_placeholders, ::pdalboost::mpl::_1>
                >
            >
        >::type,
        typename ::pdalboost::mpl::end<placeholders>::type
    >::type type;
};

template<class Concept, class Map>
struct check_map<Concept, ::pdalboost::type_erasure::static_binding<Map> > :
    check_map<Concept, Map>
{};

}
}
}

#endif
