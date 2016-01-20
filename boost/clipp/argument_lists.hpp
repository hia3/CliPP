// Copyright Peder Holt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_CLIPP_ARGUMENT_LISTS_HPP_HOLT_23112003
#define BOOST_CLIPP_ARGUMENT_LISTS_HPP_HOLT_23112003

#include <boost/clipp/value.hpp>

namespace boost { namespace clipp { 


class empty_argument_list
{
public:
    empty_argument_list()  {}
    size_t size() const {return 0;}
    valueP operator[](size_t index) const {return nullptr;}
};

template<typename MemberT>
class member_argument_list
{
public:
    member_argument_list(MemberT& member,const value::Params& params,const valueP& parent)
    :  member_(member)
    ,  parent_(parent)
    ,  params_(params)
    {}
    size_t size() const {
        int input_size  =   params_.size()+1;
        int arity       =   member_.get_arity();
        if(input_size<arity) {
            for(int i=input_size;i<arity;++i) {
                if(member_.get_default_value(i-1)) ++input_size;
                else break;
            }
        }
        return input_size;
    }
    valueP operator[](size_t index) const 
    {
        if(index==0) return parent_;
        else if(index <= params_.size()) return params_[index-1];
        else return member_.get_default_value(index-1);
    }
private:
    MemberT& member_;
    const valueP& parent_;
    const value::Params& params_;
};

template<typename MemberT>
class static_argument_list
{
public:
    static_argument_list(MemberT& member,const value::Params& params,const valueP& parent) 
    :   member_(member)
    ,   params_(params) {}
    size_t size() const {
        int input_size  =   params_.size();
        int arity       =   member_.get_arity();
        if(input_size<arity) {
            for(int i=input_size;i<arity;++i) {
                if(member_.get_default_value(i)) ++input_size;
                else break;
            }
        }
        if(arity==0 && input_size>0) throw invalid_number_of_arguments(arity,input_size);
        return input_size;
    }
    valueP operator[](size_t index) const {
        if(index < params_.size()) return params_[index];
        else return member_.get_default_value(index);
    }
private:
    MemberT& member_;
    const value::Params& params_;
};

}} //namespace boost::clipp

#endif //BOOST_CLIPP_ARGUMENT_LISTS_HPP_HOLT_23112003

