//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[text_visitor_cpp_11
//````C++11```
//<-
#include <iostream>
#include <memory>
#include <boost/mpl/size.hpp>
#include <boost/mpl/int.hpp>
#include <boost/units/detail/utility.hpp>
//->
#include <boost/di.hpp>

//<-
namespace mpl   = boost::mpl;
namespace utils = boost::units::detail;
//->
namespace di    = boost::di;

/*<Define classes as usual>*/
struct i0 { virtual ~i0() { }; };
struct c0 : i0 { };
struct c1 { c1(std::shared_ptr<i0>) { } };
struct c2 { c2(int, double, char) { } };
struct c3 { c3(std::shared_ptr<c1>, std::shared_ptr<c2>) { } };

/*<<Definition of text visitor>>*/
class text_visitor
{
public:
    /*<<Definition of the visitor call operator or requirement>>*/
    template<typename T>
    void operator()(const T&) const {
        int size = mpl::size<typename T::call_stack>::value;
        while(--size) {
            std::cout << "\t";
        }
        std::cout << utils::demangle(typeid(typename T::type).name()) << std::endl;
    }
};

int main() {
    /*<<make an injector>>*/
    auto injector = di::make_injector(
        di::bind_int<42>()
      , di::deduce<c0>()
    );

    /*<<iterate through created objects>>*/
    injector.visit<c3>(text_visitor());

    /*< output [pre
    c3
        std::shared_ptr<c1>
            std::shared_ptr<i0>
        std::shared_ptr<c2>
            int
            double
            char
    ]>*/
    return 0;
}

//]

