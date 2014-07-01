//
// Copyright (c) 2014 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//[custom_scope_cpp_11
//````C++11```
//<-
#include <memory>
#include <utility>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/type.hpp>
//->
#include <boost/di.hpp>

namespace di = boost::di;

namespace {

struct c1 { };

struct c2
{
    c2(std::shared_ptr<c1> sp) {
        if (sp) {
            std::clog << "in custom scope" << std::endl;
        } else {
            std::clog << "not in custom scope" << std::endl;
        }
    }
};

} // namespace

class custom_scope
{
public:
    typedef boost::mpl::int_<0> priority;

    class entry { };
    class exit { };

    template<typename TExpected, typename TGiven>
    class scope
    {
        class convertible
        {
        public:
            convertible(const std::shared_ptr<TExpected>& object) // non explicit
                : object_(object)
            { }

            template<typename I>
            std::shared_ptr<I> operator()(const boost::type<std::shared_ptr<I>>&) const {
                return object_;
            }

            std::shared_ptr<TExpected> object_;
        };

    public:
        typedef scope type;
        typedef convertible result_type;

        void call(const entry&) {
            in_scope_ = true;
        }

        void call(const exit&) {
            in_scope_ = false;
        }

        template<typename... Args>
        result_type create(Args&&... args) {
            if (in_scope_) {
                return std::make_shared<TGiven>(std::forward(args)...);
            }

            return std::shared_ptr<TGiven>();
        }

    private:
        bool in_scope_ = false;
    };
};

int main() {
    using injector_t = di::injector<
        di::scope<custom_scope>::bind<
            c1
        >
    >;;

    di::injector<injector_t> injector;

    injector.create<c2>(); // not in custom scope
    injector.call(custom_scope::entry());
    injector.create<c2>(); // in custom scope
    injector.call(custom_scope::exit());
    injector.create<c2>(); // not in custom scope
}

//`[table
//`[[Full code example: [@example/cpp_11/custom_scope.cpp custom_scope.cpp]]]]
//]

