#include <boost/javascript/config.hpp>
#include <boost/javascript/date.hpp>
#include <boost/javascript/math.hpp>
#include <boost/javascript/date_impl.hpp>

//Windows implementation

#if defined(BOOST_MSVC)

#include <windows.h>
#pragma comment(lib,"oleaut32.lib")

const double msPerDay=86400000.;
const double msPerHour=3600000.;
const double msPerMinute=60000.;
const double msPerSecond=1000.;

const double hoursPerDay=24.;
const double minutesPerHour=60.;
const double secondsPerMinute=60.;

bool timeBefore(double t,long month,long weekday,long weekOfMonth,long hour)
{
    long current_month=boost::javascript::date::monthFromTime(t)+1;
    if(current_month<month) return true;
    if(current_month>month) return false;

    long date=boost::javascript::date::dateFromTime(t);
    long firstWeekday=boost::javascript::date::day(t-(date-1)*msPerDay);

    long transitDate=1;
    if(weekday>firstWeekday) transitDate+=(7-(weekday-firstWeekday));
    else transitDate+=(firstWeekday-weekday);
    transitDate+=7*(weekOfMonth-1);

    if(date<transitDate) return true;
    if(date>transitDate) return false;

    double timeOfTransit=hour*msPerHour;
    if(boost::javascript::date::timeWithinDay(t)<timeOfTransit) return true;
    return false;
}


double boost::javascript::convertStringToDate(const char* date)
{
    OLECHAR strIn[256];
    int i=0;
    while(date[i]!=0 && i<255) {
        strIn[i]=OLECHAR(date[i]);
        ++i;
    }
    strIn[i]=0;

    DATE dateOut;
    
    HRESULT res = VarDateFromStr(strIn, 0, LOCALE_NOUSEROVERRIDE, &dateOut);
    if(res!=S_OK) res=VarDateFromStr(strIn, 0, 0, &dateOut);
    if(res!=S_OK) return boost::javascript::math::NaN();

    double days;
    double time;
    if(dateOut<0) {
        days = - boost::javascript::math::floor(-dateOut);
        time = days - dateOut;
    }
    else {
        days = boost::javascript::math::floor(dateOut);
        time = dateOut - days;
    }
    //Set the correct day offset
    double dec1899=boost::javascript::date::UTC(1899,12,30);
    days += boost::javascript::date::day(dec1899);
    
    long hours = long(time*=hoursPerDay);
    time-=hours;
    long minutes = long(time*=minutesPerHour);
    time-=minutes;
    long seconds = long(time*=secondsPerMinute);
    time-=seconds;
    long ms = long(time*=msPerSecond);

    return days * msPerDay + boost::javascript::date::makeTime(hours,minutes,seconds,ms);
}


double boost::javascript::localTZA()
{
    TIME_ZONE_INFORMATION time_zone_info;
    if(GetTimeZoneInformation(&time_zone_info)) return -time_zone_info.Bias*msPerMinute;
    else return 0;
}

double boost::javascript::daylightSavingTA(double t)
{    
    TIME_ZONE_INFORMATION time_zone_info;
    if(GetTimeZoneInformation(&time_zone_info) && 
       time_zone_info.DaylightDate.wMonth!=0 &&
       time_zone_info.DaylightDate.wYear==0) 
    {
        //Norhern hemisphere
        if(time_zone_info.StandardDate.wMonth > time_zone_info.DaylightDate.wMonth) {
            //Guaranteed DaylightSavingTime
            if(
                timeBefore(
                    t,
                    time_zone_info.DaylightDate.wMonth,
                    time_zone_info.DaylightDate.wDayOfWeek,
                    time_zone_info.DaylightDate.wDay,
                    time_zone_info.DaylightDate.wHour
                )
              ) return 0.0;
            else if(
                timeBefore(
                    t,
                    time_zone_info.StandardDate.wMonth,
                    time_zone_info.StandardDate.wDayOfWeek,
                    time_zone_info.StandardDate.wDay,
                    time_zone_info.StandardDate.wHour
                )
              ) return -time_zone_info.DaylightBias*msPerMinute;
            else return 0.0;
        }
        //Southern hemisphere
        else {
            //Guaranteed DaylightSavingTime
            if(
                timeBefore(
                    t,
                    time_zone_info.StandardDate.wMonth,
                    time_zone_info.StandardDate.wDayOfWeek,
                    time_zone_info.StandardDate.wDay,
                    time_zone_info.StandardDate.wHour
                )
              ) return -time_zone_info.DaylightBias*msPerMinute;
            else if(
                timeBefore(
                    t,
                    time_zone_info.DaylightDate.wMonth,
                    time_zone_info.DaylightDate.wDayOfWeek,
                    time_zone_info.DaylightDate.wDay,
                    time_zone_info.DaylightDate.wHour
                )
              ) return 0.0;
            else return -time_zone_info.DaylightBias*msPerMinute;
        }
    }
    return 0.0;
}

#else 

bool timeBefore(double t,long month,long weekday,long weekOfMonth,long hour)
{
    return false;
}


double boost::javascript::convertStringToDate(const char* date)
{
    return boost::javascript::math::NaN();
}


double boost::javascript::localTZA()
{
    return 0;
}

double boost::javascript::daylightSavingTA(double t)
{
    return 0;
}

#endif
