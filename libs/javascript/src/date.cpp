#include <boost/javascript/date.hpp>
#include <boost/javascript/math.hpp>
#include <boost/javascript/number.hpp>
#include <ctime>
#include <boost/javascript/date_impl.hpp>
#include <boost/clipp/class.hpp>
#include <stdexcept>

#ifdef BOOST_MSVC
#include <sys/timeb.h>
#endif


using namespace boost::javascript;
using namespace boost::clipp;

const double msPerDay=86400000.;
const double msPerHour=3600000.;
const double msPerMinute=60000.;
const double msPerSecond=1000.;

const double hoursPerDay=24.;
const double minutesPerHour=60.;
const double secondsPerMinute=60.;

void date::init(context* c)
{
    if(is_type_defined<date>(c)) return;
    class_<date,object> cls("Date",c);
    cls.constructor(create_date, call_method);
    cls.constructor(arguments<long,long,long,long,long,long,long>(),construct_method).signature(
        "year",
        "month",
        arg("day")=1,
        arg("hours")=0,
        arg("minutes")=0,
        arg("seconds")=0,
        arg("ms")=0
    );
    cls.constructor(arguments<double>(),construct_method);
    cls.constructor(arguments<const std::string&>(),construct_method);
    cls.constructor(arguments<>(),construct_method);

    cls.converter(arguments<const date&>());

    cls.static_function("parse",&date::parse);
    cls.static_function("UTC",&date::UTC).signature(
        "year",
        "month",
        arg("day")=1,
        arg("hours")=0,
        arg("minutes")=0,
        arg("seconds")=0,
        arg("ms")=0
    );

    cls.function("defaultValue",&date::defaultValue).signature(arg("hint")=PreferredType::Hint::No);
    cls.function("toString",&date::toString);
    cls.function("toDateString",&date::toDateString);
    cls.function("toTimeString",&date::toTimeString);
    cls.function("toLocaleString",&date::toLocaleString);
    cls.function("toLocaleDateString",&date::toLocaleDateString);
    cls.function("toLocaleTimeString",&date::toLocaleTimeString);
    cls.function("valueOf",&date::valueOf);
    cls.function("getTime",&date::getTime);
    cls.function("toUTCString",&date::toUTCString);
    cls.function("getFullYear",&date::getFullYear);
    cls.function("getUTCFullYear",&date::getUTCFullYear);
    cls.function("getMonth",&date::getMonth);
    cls.function("getUTCMonth",&date::getUTCMonth);
    cls.function("getDate",&date::getDate);
    cls.function("getUTCDate",&date::getUTCDate);
    cls.function("getDay",&date::getDay);
    cls.function("getUTCDay",&date::getUTCDay);
    cls.function("getHours",&date::getHours);
    cls.function("getUTCHours",&date::getUTCHours);
    cls.function("getMinutes",&date::getMinutes);
    cls.function("getUTCMinutes",&date::getUTCMinutes);
    cls.function("getSeconds",&date::getSeconds);
    cls.function("getUTCSeconds",&date::getUTCSeconds);
    cls.function("getMilliseconds",&date::getMilliseconds);
    cls.function("getUTCMilliseconds",&date::getUTCMilliseconds);
    cls.function("getTimezoneOffset",&date::getTimezoneOffset);
    cls.function("setTime",&date::setTime);
    cls.function("setMilliseconds",&date::setMilliseconds);
    cls.function("setUTCMilliseconds",&date::setUTCMilliseconds);
    cls.function("setSeconds",&date::setSeconds);
    cls.function("setUTCSeconds",&date::setUTCSeconds);
    cls.function("setMinutes",&date::setMinutes);
    cls.function("setUTCMinutes",&date::setUTCMinutes);
    cls.function("setHours",&date::setHours);
    cls.function("setUTCHours",&date::setUTCHours);
    cls.function("setDate",&date::setDate);
    cls.function("setUTCDate",&date::setUTCDate);
    cls.function("setMonth",&date::setMonth);
    cls.function("setUTCMonth",&date::setUTCMonth);
    cls.function("setFullYear",&date::setFullYear);
    cls.function("setUTCFullYear",&date::setUTCFullYear);
};

date::date(long year,long month,long date,long hours,long minutes,long seconds,long ms)
{
    time_=UTC(year,month,date,hours,minutes,seconds,ms);
}

date::date(double date)
{
    time_=timeClip(date);
}

date::date(const std::string& date)
{
    time_=parse(date);
}

date::date()
{
#ifdef BOOST_MSVC
    struct _timeb timebuffer;
   _ftime( &timebuffer);
   time_=double(timebuffer.time)*msPerSecond+timebuffer.millitm;
#else
   time_=double(time(nullptr)*msPerSecond);
#endif
}

std::string date::create_date()
{
    return date().toString();
}

valueP date::defaultValue(PreferredType::Hint hint)
{
    valueP f1,f2;
    if(hint==PreferredType::Hint::Number) {
        f1=lookup("valueOf");
        f2=lookup("toString");
    }
    else {
        f1=lookup("toString");
        f2=lookup("valueOf");
    }
    valueP result;
    if(f1) result=f1();
    if(!result && f2) result=f2();
    return result;
}

double date::parse(const std::string& date)
{
    double value = boost::javascript::convertStringToDate(date.c_str());
    if(math::is_NaN(value)) throw std::runtime_error("Unable to parse date");
    return value;
}

double date::UTC(long year,long month,long date,long hours,long minutes,long seconds,long ms)
{
    if(0<=year && year<=99) year+=1900;
    return timeClip(makeDate(makeDay(year,month,date),makeTime(hours,minutes,seconds,ms)));
}

void date::fillTimeStruct(double t,tm& timeStruct)
{
    timeStruct.tm_year=yearFromTime(t)-1900;
    timeStruct.tm_mon=monthFromTime(t);
    timeStruct.tm_mday=dateFromTime(t);
    timeStruct.tm_yday=dayWithinYear(t);
    timeStruct.tm_wday=weekDay(t);
    timeStruct.tm_hour=hoursFromTime(t);
    timeStruct.tm_min=minFromTime(t);
    timeStruct.tm_sec=secFromTime(t);
    timeStruct.tm_isdst=0;
}

std::string date::toString() const
{
    if(math::is_NaN(time_)) return "Invalid Date";
    tm time;
    fillTimeStruct(localTime(time_),time);

    char dest[256];
    strftime(dest,255,"%#c",&time);
    return dest;
}

std::string date::toUTCString() const
{
    if(math::is_NaN(time_)) return "Invalid Date";
    tm time;
    fillTimeStruct(time_,time);

    char dest[256];
    strftime(dest,255,"%#cUTC",&time);
    return dest;
}

std::string date::toDateString() const
{
    if(math::is_NaN(time_)) return "Invalid Date";
    tm time;
    fillTimeStruct(localTime(time_),time);

    char dest[256];
    strftime(dest,255,"%#x",&time);
    return dest;
}

std::string date::toTimeString() const
{
    if(math::is_NaN(time_)) return "Invalid Date";
    tm time;
    fillTimeStruct(localTime(time_),time);

    char dest[256];
    strftime(dest,255,"%X",&time);
    return dest;
}

std::string date::toLocaleString() const
{
    if(math::is_NaN(time_)) return "Invalid Date";
    tm time;
    fillTimeStruct(localTime(time_),time);

    char dest[256];
    setlocale(LC_TIME,"");
    strftime(dest,255,"%#c",&time);
    setlocale(LC_TIME,"C");
    return dest;
}

std::string date::toLocaleDateString() const
{
    if(math::is_NaN(time_)) return "Invalid Date";
    tm time;
    fillTimeStruct(localTime(time_),time);

    char dest[256];
    setlocale(LC_TIME,"");
    strftime(dest,255,"%#x",&time);
    setlocale(LC_TIME,"C");
    return dest;
}

std::string date::toLocaleTimeString() const
{
    if(math::is_NaN(time_)) return "Invalid Date";
    tm time;
    fillTimeStruct(localTime(time_),time);

    char dest[256];
    setlocale(LC_TIME,"");
    strftime(dest,255,"%X",&time);
    setlocale(LC_TIME,"C");
    return dest;
}

long date::getFullYear() const
{
    return yearFromTime(localTime(time_));
}

long date::getUTCFullYear() const
{
    return yearFromTime(time_);
}

long date::getMonth() const
{
    return monthFromTime(localTime(time_));
}

long date::getUTCMonth() const
{
    return monthFromTime(time_);
}

long date::getDate() const
{
    return dateFromTime(localTime(time_));
}

long date::getUTCDate() const
{
    return dateFromTime(time_);
}

long date::getDay() const
{
    return weekDay(localTime(time_));
}

long date::getUTCDay() const
{
    return weekDay(time_);
}

long date::getHours() const
{
    return hoursFromTime(localTime(time_));
}

long date::getUTCHours() const
{
    return hoursFromTime(time_);
}

long date::getMinutes() const
{
    return minFromTime(localTime(time_));
}

long date::getUTCMinutes() const
{
    return minFromTime(time_);
}

long date::getSeconds() const
{
    return secFromTime(localTime(time_));
}

long date::getUTCSeconds() const
{
    return secFromTime(time_);
}

long date::getMilliseconds() const
{
    return msFromTime(localTime(time_));
}

long date::getUTCMilliseconds() const
{
    return msFromTime(time_);
}

long date::getTimezoneOffset() const
{
    return long((time_-localTime(time_))/msPerMinute);
}

double date::setTime(double time)
{
    return time_=timeClip(time);
}

double date::setMilliseconds(long ms)
{
    double t=localTime(time_);
    long hours=hoursFromTime(t);
    long minutes=minFromTime(t);
    long seconds=secFromTime(t);
    return time_=utc(makeDate(day(t),makeTime(hours,minutes,seconds,ms)));
}

double date::setUTCMilliseconds(long ms)
{
    double t=time_;
    long hours=hoursFromTime(t);
    long minutes=minFromTime(t);
    long seconds=secFromTime(t);
    return time_=makeDate(day(t),makeTime(hours,minutes,seconds,ms));
}

double date::setSeconds(long sec,std::valarray<long> const& args)
{
    double t=localTime(time_);
    long hours=hoursFromTime(t);
    long minutes=minFromTime(t);
    long ms = args.size()>0? args[0]: msFromTime(t);
    return time_=utc(makeDate(day(t),makeTime(hours,minutes,sec,ms)));
}

double date::setUTCSeconds(long sec,std::valarray<long> const& args)
{
    double t=time_;
    long hours=hoursFromTime(t);
    long minutes=minFromTime(t);
    long ms = args.size()>0? args[0]: msFromTime(t);
    return time_=makeDate(day(t),makeTime(hours,minutes,sec,ms));
}

double date::setMinutes(long min,std::valarray<long> const& args)
{
    double t=localTime(time_);
    long hours=hoursFromTime(t);
    long seconds=args.size()>0? args[0] : secFromTime(t);
    long ms = args.size()>1? args[1]: msFromTime(t);
    return time_=utc(makeDate(day(t),makeTime(hours,min,seconds,ms)));
}

double date::setUTCMinutes(long min,std::valarray<long> const& args)
{
    double t=time_;
    long hours=hoursFromTime(t);
    long seconds=args.size()>0? args[0] : secFromTime(t);
    long ms = args.size()>1? args[1]: msFromTime(t);
    return time_=makeDate(day(t),makeTime(hours,min,seconds,ms));
}

double date::setHours(long hours,std::valarray<long> const& args)
{
    double t=localTime(time_);
    long minutes=args.size()>0? args[0] : minFromTime(t);
    long seconds=args.size()>1? args[1] : secFromTime(t);
    long ms = args.size()>2? args[2]: msFromTime(t);
    return time_=utc(makeDate(day(t),makeTime(hours,minutes,seconds,ms)));
}

double date::setUTCHours(long hours,std::valarray<long> const& args)
{
    double t=time_;
    long minutes=args.size()>0? args[0] : minFromTime(t);
    long seconds=args.size()>1? args[1] : secFromTime(t);
    long ms = args.size()>2? args[2]: msFromTime(t);
    return time_=makeDate(day(t),makeTime(hours,minutes,seconds,ms));
}

double date::setDate(long date,std::valarray<long> const& args)
{
    double t=localTime(time_);
    long year = yearFromTime(t);
    long month = monthFromTime(t);
    long hours=args.size()>0?args[0] : hoursFromTime(t);
    long minutes=args.size()>1? args[1] : minFromTime(t);
    long seconds=args.size()>2? args[2] : secFromTime(t);
    long ms = args.size()>3? args[3]: msFromTime(t);    
    return time_=utc(makeDate(makeDay(year,month,date),makeTime(hours,minutes,seconds,ms)));
}

double date::setUTCDate(long date,std::valarray<long> const& args)
{
    double t=time_;
    long year = yearFromTime(t);
    long month = monthFromTime(t);
    long hours=args.size()>0?args[0] : hoursFromTime(t);
    long minutes=args.size()>1? args[1] : minFromTime(t);
    long seconds=args.size()>2? args[2] : secFromTime(t);
    long ms = args.size()>3? args[3]: msFromTime(t);    
    return time_=makeDate(makeDay(year,month,date),makeTime(hours,minutes,seconds,ms));
}

double date::setMonth(long month,std::valarray<long> const& args)
{
    double t=localTime(time_);
    long year = yearFromTime(t);
    long date =args.size()>0?args[0] : dateFromTime(t);
    long hours=args.size()>1?args[1] : hoursFromTime(t);
    long minutes=args.size()>2? args[2] : minFromTime(t);
    long seconds=args.size()>3? args[3] : secFromTime(t);
    long ms = args.size()>4? args[4]: msFromTime(t);    
    return time_=utc(makeDate(makeDay(year,month,date),makeTime(hours,minutes,seconds,ms)));
}

double date::setUTCMonth(long month,std::valarray<long> const& args)
{
    double t=time_;
    long year = yearFromTime(t);
    long date =args.size()>0?args[0] : dateFromTime(t);
    long hours=args.size()>1?args[1] : hoursFromTime(t);
    long minutes=args.size()>2? args[2] : minFromTime(t);
    long seconds=args.size()>3? args[3] : secFromTime(t);
    long ms = args.size()>4? args[4]: msFromTime(t);    
    return time_=makeDate(makeDay(year,month,date),makeTime(hours,minutes,seconds,ms));
}

double date::setFullYear(long year,std::valarray<long> const& args)
{
    double t=localTime(time_);
    long month = args.size()>0?args[0] : monthFromTime(t);
    long date =args.size()>1?args[1] : dateFromTime(t);
    long hours=args.size()>2?args[2] : hoursFromTime(t);
    long minutes=args.size()>3? args[3] : minFromTime(t);
    long seconds=args.size()>4? args[4] : secFromTime(t);
    long ms = args.size()>5? args[5]: msFromTime(t);    
    return time_=utc(makeDate(makeDay(year,month,date),makeTime(hours,minutes,seconds,ms)));
}

double date::setUTCFullYear(long year,std::valarray<long> const& args)
{
    double t=time_;
    long month = args.size()>0?args[0] : monthFromTime(t);
    long date =args.size()>1?args[1] : dateFromTime(t);
    long hours=args.size()>2?args[2] : hoursFromTime(t);
    long minutes=args.size()>3? args[3] : minFromTime(t);
    long seconds=args.size()>4? args[4] : secFromTime(t);
    long ms = args.size()>5? args[5]: msFromTime(t);    
    return time_=makeDate(makeDay(year,month,date),makeTime(hours,minutes,seconds,ms));
}

long date::day(double t)
{
    return long(math::floor(t/msPerDay));
}

double date::timeWithinDay(double t)
{
    return number(t) % double(msPerDay);
}

double date::daysInYear(long y)
{
    if(y%400==0) return 366;
    else if(y%100==0 && y%400!=0) return 365;
    else if(y%4==0 && y%100!=0) return 366;
    else return 365;
}

long date::dayFromYear(long y)
{
    return long(365.*(double(y)-1970.)+math::floor((double(y)-1969.)/4.)-math::floor((double(y)-1901.)/100.)+math::floor((double(y)-1601.)/400.));
}

double date::timeFromYear(long y)
{
    return msPerDay*dayFromYear(y);
}

long date::yearFromTime(double t)
{
    //Make an educated guess.
    long year=day(t)/365+1970;
    double t_test=timeFromYear(year);
    //Corrigate up/down
    while(t_test>t) {
        year--;
        t_test=timeFromYear(year);
    }
    while(t_test<t) {
        year++;
        t_test=timeFromYear(year);
    }
    return year-1;
}

bool date::inLeapYear(double t)
{
    return daysInYear(yearFromTime(t))==366;
}

long date::monthFromTime(double t)
{
    int leapDay=inLeapYear(t)?1:0;
    long dayInYear=dayWithinYear(t);

    if(0<=dayInYear && dayInYear<31) return 0;
    if(31<=dayInYear && dayInYear<59+leapDay) return 1;
    if(59+leapDay<=dayInYear && dayInYear<90+leapDay) return 2;
    if(90+leapDay<=dayInYear && dayInYear<120+leapDay) return 3;
    if(120+leapDay<=dayInYear && dayInYear<151+leapDay) return 4;
    if(151+leapDay<=dayInYear && dayInYear<181+leapDay) return 5;
    if(181+leapDay<=dayInYear && dayInYear<212+leapDay) return 6;
    if(212+leapDay<=dayInYear && dayInYear<243+leapDay) return 7;
    if(243+leapDay<=dayInYear && dayInYear<273+leapDay) return 8;
    if(273+leapDay<=dayInYear && dayInYear<304+leapDay) return 9;
    if(304+leapDay<=dayInYear && dayInYear<334+leapDay) return 10;
    if(334+leapDay<=dayInYear && dayInYear<365+leapDay) return 11;
    return 0;
}

long date::dayWithinYear(double t)
{
    return day(t)-dayFromYear(yearFromTime(t));
}

long date::dateFromTime(double t)
{
    long day=dayWithinYear(t);
    long month=monthFromTime(t);
    int leapDay=inLeapYear(t)?1:0;

    switch(month) {
    case 0: return dayWithinYear(t)+1;
    case 1: return dayWithinYear(t)-30;
    case 2: return dayWithinYear(t)-58-leapDay;
    case 3: return dayWithinYear(t)-89-leapDay;
    case 4: return dayWithinYear(t)-119-leapDay;
    case 5: return dayWithinYear(t)-150-leapDay;
    case 6: return dayWithinYear(t)-180-leapDay;
    case 7: return dayWithinYear(t)-211-leapDay;
    case 8: return dayWithinYear(t)-242-leapDay;
    case 9: return dayWithinYear(t)-272-leapDay;
    case 10: return dayWithinYear(t)-303-leapDay;
    case 11: return dayWithinYear(t)-333-leapDay;
    default:
        return -1;
    }
}

long date::weekDay(double t)
{
    return (day(t)+4)%7;
}

double date::daylightSavingTA(double t)
{
    return boost::javascript::daylightSavingTA(t);
}

double date::localTZA()
{
    return boost::javascript::localTZA();
}

double date::localTime(double t)
{
    return t+localTZA()+daylightSavingTA(t);
}

double date::utc(double t)
{
    return t-localTZA()-daylightSavingTA(t-localTZA());
}

long date::hoursFromTime(double t)
{
    return long(math::floor(t/msPerHour)%number(hoursPerDay));
}

long date::minFromTime(double t)
{
    return long(math::floor(t/msPerMinute)%number(minutesPerHour));
}

long date::secFromTime(double t)
{
    return long(math::floor(t/msPerSecond)%number(secondsPerMinute));
}

long date::msFromTime(double t)
{
    return long(t%number(msPerSecond));
}

double date::makeTime(long hour,long min,long sec,long ms)
{
    return hour*msPerHour+min*msPerMinute+sec*msPerSecond+ms;
}

double date::makeDay(long year,long month,long date)
{
    long totalYear = long(year+math::floor(month/12.));
    long totalMonth= month%12;
    double tTotal=timeFromYear(year);
    long totalDays=0;
    switch(totalMonth) {
    case 11:
        totalDays+=30;
    case 10:
        totalDays+=31;
    case 9:
        totalDays+=30;
    case 8:
        totalDays+=31;
    case 7:
        totalDays+=31;
    case 6:
        totalDays+=30;
    case 5:
        totalDays+=31;
    case 4:
        totalDays+=30;
    case 3:
        totalDays+=31;
    case 2:
        totalDays+= daysInYear(totalYear)==366 ? 29 : 28;
    case 1:
        totalDays+= 31;
    }
    totalDays+=date-1;
    tTotal+=totalDays*msPerDay;
    if(yearFromTime(tTotal)==totalYear &&
       monthFromTime(tTotal)==totalMonth &&
       dateFromTime(tTotal)==date) return tTotal;
    else return math::NaN();
}

double date::makeDate(double day,double time)
{
    if(math::is_NaN(day)) return math::NaN();
    return day*msPerDay+time;
}

double date::timeClip(double time)
{
    if(math::abs(time)>8.64e15) return math::NaN();
    return math::floor(time);
}
