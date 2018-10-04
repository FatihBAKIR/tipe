// The Art of C++
// Copyright (c) 2016-2018 Daniel Frey

#ifndef TAO_OPTIONAL_EXTERNAL_OPTIONAL_HPP
#define TAO_OPTIONAL_EXTERNAL_OPTIONAL_HPP

#ifndef TAO_OPTIONAL_USE_STD_OPTIONAL
#if( __cplusplus >= 201703L ) || ( defined( _MSVC_LANG ) && ( _MSVC_LANG >= 201703L ) )
#define TAO_OPTIONAL_USE_STD_OPTIONAL
#endif
#endif

#ifndef TAO_OPTIONAL_USE_STD_OPTIONAL
#ifndef TAO_OPTIONAL_USE_TS_OPTIONAL
#if( __cplusplus >= 201402L ) && defined( __GNUC__ ) && ( ( __GNUC__ > 4 ) || ( __GNUC__ == 4 ) && ( __GNUC_MINOR__ >= 9 ) )
#define TAO_OPTIONAL_USE_TS_OPTIONAL
#endif
#endif
#endif

#if defined( TAO_OPTIONAL_USE_STD_OPTIONAL )
#include <optional>
#elif defined( TAO_OPTIONAL_USE_TS_OPTIONAL )
#include <experimental/optional>
#else
#include "akrzemi1/optional.hpp"
#endif

namespace tao
{

#if defined( TAO_OPTIONAL_USE_STD_OPTIONAL )
   using std::in_place;
   using std::make_optional;
   using std::nullopt;
   using std::optional;
#else
   using std::experimental::in_place;
   using std::experimental::make_optional;
   using std::experimental::nullopt;
   using std::experimental::optional;
#endif

}  // namespace tao

#endif
