// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_DATE_IMPL_HPP_HOLT_15012004
#define BOOST_JAVASCRIPT_DATE_IMPL_HPP_HOLT_15012004

namespace boost { namespace javascript {

double convertStringToDate(const char* date);
double localTZA();
double daylightSavingTA(double t);

}} //namespace boost::javascript

#endif //BOOST_JAVASCRIPT_DATE_IMPL_HPP_HOLT_15012004

