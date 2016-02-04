#include <boost/javascript/array.hpp>
#include <boost/javascript/reference.hpp>
#include <boost/javascript/undefined.hpp>
#include <boost/clipp/class.hpp>
#include <boost/clipp/container_wrapper.ipp>
#include <boost/clipp/detail/operator_invoker.hpp>
#include <boost/next_prior.hpp>

using namespace boost::javascript;
using namespace boost::clipp;

array::array(unsigned int length)
{
    container_type& c=*this;
    c.resize(length);
}

array::array(const array& rhs)
:   base(rhs)
{
}

array::array(const container_base& container)
:   base(container)
{
}

array::array(const container_type& container)
:   base(container)
{
}

array::array(std::valarray<valueP> const &rhs)
{
    if(rhs.size()>0) create(rhs[0]->get_context());
    for(size_t i=0;i<rhs.size();++i) {
        operator[](i)=rhs[i]->duplicate();
    }
}

valueP array::defaultValue(PreferredType::Hint hint)
{
    valueP f1,f2;
    if(hint==PreferredType::Hint::Number || hint==PreferredType::Hint::No) {
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

valueP array::lookup(const std::string& identifier,valueP parent)
{
    valueP result=base::lookup(identifier,parent);
    if(result) return result;
    try {
        unsigned index = lexical_cast<int>(identifier);
        return (*this)[index].duplicate();
    }
    catch(...) {
        return nullptr;
    }
}

reference& array::operator[](unsigned int index)
{
    container_type& c=*this;
    if(index>=c.size()) {
        c.resize(index+1);
    }
    if(!c[index]) {
        c[index]=new reference;
        c[index]->create(get_context());
    }
    return *c[index];
}

valueP array::operator[](const std::string& identifier)
{
    valueP result=base::lookup(identifier);
    if(result) return result;
    unsigned index;
    try {
        index = lexical_cast<unsigned>(identifier);
    }
    catch(...) {
        return nullptr;
    }
    return wrap_struct<reference&>::wrap(operator[](index),get_context());
}

unsigned int array::length() const
{
    container_type const& c=*this;
    return c.size();
}

void array::init(context* c)
{
    if(is_type_defined<array>(c)) return;
    class_<array,base> cls("Array",c);

    cls.constructor(arguments<unsigned int>(),construct_method);
    cls[constructor<std::valarray<valueP> const &>()];

    cls.converter(arguments<const container_base&>());
    cls.converter(arguments<const container_type&>());

    cls[self[unsigned()]];
    cls[self[std::string()]];
    cls.read("length",&array::length);
    
    cls.function("toString",&array::toString);
    cls.static_function("concat",&array::concat);
    cls.function("join",&array::join).signature(arg("separator")=",");
    cls.function("pop",&array::pop);
    cls.function("push",&array::push);
    cls.function("reverse",&array::reverse);
    cls.function("shift",&array::shift);
    cls.function("slice",&array::slice).signature("start",arg("end") = std::numeric_limits<int>::max());
    cls.function("sort",&array::sort).signature(arg("compareFn")=new undefined());
    cls.function("splice",&array::splice);
    cls.function("unshift",&array::unshift);

    cls.function("defaultValue",&array::defaultValue).signature(arg("hint")=PreferredType::Hint::No);
}

std::string array::toString() const
{
    return join();
}

std::string array::toLocaleString() const
{
    array* self=const_cast<array*>(this);
    std::string text;
    for(unsigned i=0;i<length();++i) {
        valueP caller = (*self)[i]["toLocaleString"];
        if(caller) {
            unwrap<std::string> local_text=unwrap<std::string>(caller());
            if(local_text.ok()) text+=local_text();
        }
        if(i!=0) text+=",";
    }
    return text;}

arrayP array::concat(std::valarray<valueP> const& items)
{
    array* new_array=new array(items.size());
    for(unsigned i=0;i<items.size();++i)
    (*new_array)[i]=items[i];
    return new_array;
}

std::string array::join(std::string const& separator) const
{
    array* self=const_cast<array*>(this);
    std::string text;
    for(unsigned i=0;i<length();++i) {
        valueP caller = (*self)[i]["toString"];
        if(caller) {
            unwrap<std::string> local_text=unwrap<std::string>(caller());
            if(local_text.ok()) text+=local_text();
        }
        if(i!=0) text+=separator;
    }
    return text;
}

valueP array::pop()
{
    if(length()==0) return new undefined();
    valueP result = (*this)[length()-1].duplicate();
    long new_length=length()-1;
    container_type& c=*this;
    c.erase(prior(c.end()));
    return result;
}

int array::push(std::valarray<valueP> const &items)
{
    for(unsigned i=0;i<items.size();++i) {
        (*this)[length()]=items[i];
    }
    return items.size();
}

arrayP array::reverse() 
{
    container_type& c=*this;
    std::reverse(c.begin(),c.end());
    return this;
}

valueP array::shift()
{
    if(length()==0) return new undefined();
    valueP result=(*this)[0].duplicate();
    container_type& c=*this;
    for(container_type::iterator it=boost::next(c.begin());it!=c.end();++it) {
        std::swap(*(it-1),*(it));
    }
    c.erase(prior(c.end()));
    return result;
}

arrayP array::slice(int start,int end)
{
    if(start<0) start=std::max(int(length()+start),0);
    else start=std::min(int(length()),start);

    if(end<0) end=std::max(int(length()+end),0);
    else end=std::min(int(length()),end);
    if(end<start) return new array(0);
    array* a=new array(end-start+1);
    a->create(get_context());
    for(int i=start;i<=end;++i) {
        (*a)[i-start]=(*this)[i].duplicate();
    }
    return a;
}

struct less_compareFn : public std::binary_function<referenceP, referenceP, bool> {
    less_compareFn(valueP compareFn) : compareFn_(compareFn) {}
    bool operator()(referenceP const& x, referenceP const& y) const {
        try {
            if(!x && y) return false;
            if(x && !y) return true;
            if(!x && !y) return false;
            int result = unwrap<int>(compareFn_(x->duplicate(),y->duplicate()))();
            return (result<0);
        }
        catch(...) {
            return false;
        }
    }
private:
    mutable valueP compareFn_;
};

struct less_noCompareFn : public std::binary_function<referenceP, referenceP, bool> {
    bool operator()(referenceP const& x, referenceP const& y) const {
        try {            
            if(!x && y) return false;
            if(x && !y) return true;
            if(!x && !y) return false;
            return unwrap<bool>(invoke_operator<'<'>(x->duplicate(),y->duplicate()))();
        }
        catch(...) {
            return false;
        }
    }
};

arrayP array::sort(valueP compareFn)
{
    if(compareFn && !undefined::is_undefined(compareFn)) {
        container_type& c=*this;
        std::sort(c.begin(),c.end(),less_compareFn(compareFn));   
    }
    else {
        container_type& c=*this;
        std::sort(c.begin(),c.end(),less_noCompareFn());
    }
    return this;
}

arrayP array::splice(int start,int deleteCount,std::valarray<valueP> const &items)
{
    if(start<0) start = std::max(int(length()+start),0);
    deleteCount=std::min(std::max(deleteCount,0),int(length()-start));
    array* a=new array(deleteCount);
    a->create(get_context());
    unsigned i;
    for(i=0;i<unsigned(deleteCount);++i) {
        if(unsigned(start+i)<=length()) (*a)[i]=(*this)[start+i].duplicate();
    }
    if(unsigned(deleteCount)>items.size()) {
        int deleteItems=deleteCount-items.size();
        int deleteStart=start + items.size();
        container_type& c=*this;
        unsigned deleteEnd=deleteStart+deleteItems;
        if(deleteEnd<length()) {
            for(unsigned i=deleteEnd;i<length();++i) {
                std::swap(c[deleteItems+i-deleteEnd],c[i]);
            }
            deleteStart+=length()-deleteEnd;
        }
        c.erase(c.begin()+deleteStart,c.end());
    }
    if(unsigned(deleteCount)<items.size()) {
        container_type& c=*this;
        int item_count=items.size()-deleteCount;
        int itemsStart=start+deleteCount;
        for (int i = 0; i < item_count;++i)
        {
            if (itemsStart + 1 > c.size())
            {
                c.resize(itemsStart + 1);
            }
            c.insert(c.begin() + itemsStart + 1, 0);
        }
    }
    for(i=0;i<items.size();++i) {
        (*this)[start+i]=items[i];
    }
    return a;
}

int array::unshift(std::valarray<valueP> const &items)
{
    container_type& c=*this;
    for(int i=items.size()-1;i>=0;--i) {
        c.insert(c.begin(),0);
        (*this)[0]=items[i];
    }
    return length();
}
