// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_JAVASCRIPT_DATE_HPP_HOLT_15012004
#define BOOST_JAVASCRIPT_DATE_HPP_HOLT_15012004

#include <boost/javascript/config.hpp>
#include <boost/clipp/object.hpp>
#include <boost/javascript/primitive.hpp>
#include <valarray>

struct tm;

namespace boost { namespace javascript {

class BOOST_JAVASCRIPT_EXPORT_IMPORT date : public clipp::object  
{
public:
    date(long year,long month,long date = 1,long hours = 0,long minutes = 0,long seconds = 0,long ms = 0);
    date();
    date(double date);
    date(const std::string& date);
    date(const date& rhs) : object(),time_(rhs.time_) {}
    virtual ~date() {}

    static double UTC(long year,long month,long date = 1,long hours = 0,long minutes = 0,long seconds = 0,long ms = 0);
    static double parse(const std::string& date);
    static std::string create_date();

    clipp::valueP defaultValue(PreferredType::Hint hint=PreferredType::Hint::No);

    std::string toString() const;
    std::string toDateString() const;
    std::string toTimeString() const;
    std::string toLocaleString() const;
    std::string toLocaleDateString() const;
    std::string toLocaleTimeString() const;
    std::string toUTCString() const;
    double valueOf() const {return time_;}
    double getTime() const {return time_;}
    long getFullYear() const;
    long getUTCFullYear() const;
    long getMonth() const;
    long getUTCMonth() const;
    long getDate() const;
    long getUTCDate() const;
    long getDay() const;
    long getUTCDay() const;
    long getHours() const;
    long getUTCHours() const;
    long getMinutes() const;
    long getUTCMinutes() const;
    long getSeconds() const;
    long getUTCSeconds() const;
    long getMilliseconds() const;
    long getUTCMilliseconds() const;
    long getTimezoneOffset() const;

    double setTime(double time);
    double setMilliseconds(long ms);
    double setUTCMilliseconds(long ms);
    double setSeconds(long sec,std::valarray<long> const& args);
    double setUTCSeconds(long sec,std::valarray<long> const& args);
    double setMinutes(long min,std::valarray<long> const& args);
    double setUTCMinutes(long min,std::valarray<long> const& args);
    double setHours(long hours,std::valarray<long> const& args);
    double setUTCHours(long hours,std::valarray<long> const& args);
    double setDate(long date,std::valarray<long> const& args);
    double setUTCDate(long date,std::valarray<long> const& args);
    double setMonth(long month,std::valarray<long> const& args);
    double setUTCMonth(long month,std::valarray<long> const& args);
    double setFullYear(long year,std::valarray<long> const& args);
    double setUTCFullYear(long year,std::valarray<long> const& args);

    static void init(clipp::context* c);
public:
    static void fillTimeStruct(double t,tm& timeStruct);
    static long day(double t);
    static double timeWithinDay(double t);
    static double daysInYear(long y);
    static long dayFromYear(long y);
    static double timeFromYear(long y);
    static long yearFromTime(double t);
    static bool inLeapYear(double t);
    static long monthFromTime(double t);
    static long dayWithinYear(double t);
    static long dateFromTime(double t);
    static long weekDay(double t);
    static double daylightSavingTA(double t);
    static double localTZA();
    static double localTime(double t);
    static double utc(double t);
    static long hoursFromTime(double t);
    static long minFromTime(double t);
    static long secFromTime(double t);
    static long msFromTime(double t);
    static double makeTime(long hour,long min,long sec,long ms);
    static double makeDay(long year,long month,long date);
    static double makeDate(double day,double time);
    static double timeClip(double time);
private:
    double time_;
};

}} // namespace boost::clipp

#endif //BOOST_JAVASCRIPT_DATE_HPP_HOLT_15012004

