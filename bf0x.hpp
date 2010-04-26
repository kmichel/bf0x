#ifndef INCLUDED_BF0X_HPP
#define INCLUDED_BF0X_HPP

#include <iostream>

#ifdef DEBUG
#  define BF0X_TRACE(c) (std::cout << "\033[2m" << c << "\033[0m")
#else
#  define BF0X_TRACE(c)
#endif

namespace bf0x {

struct state {
    char data[65536];
    unsigned int pos;
    state()
        : data()
        , pos(0) {
    }
};

// A stack of interpreter templates structure
template <template <class, char...> class... blocks> struct bstack;

template <template <class, char...> class head, template <class, char...> class... tail> 
struct bstack<head, tail...> {
    enum { size = bstack<tail...>::size+1 };
    // Instantiate top of the stack
    template <char... chars> struct top {
        typedef head<bstack, chars...> type;
    };
    // Pop the top
    typedef bstack<tail...> pop;
    // Push on the stack
    template <template <class, char...> class pushed> struct push {
        typedef bstack<pushed, head, tail...> type;
    };
};

// Empty stack specialization
template <> struct bstack <> {
    enum { size = 0 };
    template <template <class, char...> class pushed> struct push {
        typedef bstack<pushed> type;
    };
};

// Syntactic sugar
template <class bstack, char... chars>
struct top {
    typedef typename bstack::template top<chars...>::type type;
};

template <class, char...> struct interpreter;
template <class, char...> struct loop_interpreter;
template <class, char...> struct tail_interpreter;

#if EXTENSIBLE_LITERALS_SUPPORTED
template<char... chars> 
interpreter<chars...> operator "" BF()  {
    return interpreter<bstack<interpreter>, chars...>();
}
#endif

template <class stack> struct interpreter<stack> {
    static void exec(state&) {}
};

template <class stack> struct tail_interpreter<stack> {
    static void exec(state&) {}
};

template <class stack> struct loop_interpreter<stack> {
    static void exec(state&) {}
};

// The basing parsing functions :
// - pop a char from the tail
// - execute corresponding code
// - parse the rest with the top-of-stack interpreter

template <class bstack, char c, char... tail> struct interpreter<bstack, c, tail...> {
    static void exec(state& st) {
        // Ignore unknown chars
        top<bstack, tail...>::type::exec(st);
    }
};

template <class bstack, char... tail> struct interpreter<bstack, '>', tail...> {
    static void exec(state& st) {
        BF0X_TRACE('>');
        st.pos++;
        top<bstack, tail...>::type::exec(st);
    }
};

template <class bstack, char... tail> struct interpreter<bstack, '<', tail...> {
    static void exec(state& st) {
        BF0X_TRACE('<');
        st.pos--;
        top<bstack, tail...>::type::exec(st);
    }
};

template <class bstack, char... tail> struct interpreter<bstack, '+', tail...> {
    static void exec(state& st) {
        BF0X_TRACE('+');
        st.data[st.pos]++;
        top<bstack, tail...>::type::exec(st);
    }
};

template <class bstack, char... tail> struct interpreter<bstack, '-', tail...> {
    static void exec(state& st) {
        BF0X_TRACE('-');
        st.data[st.pos]--;
        top<bstack, tail...>::type::exec(st);
    }
};

template <class bstack, char... tail> struct interpreter<bstack, '.', tail...> {
    static void exec(state& st) {
        BF0X_TRACE('.');
        std::cout.put(st.data[st.pos]);
        top<bstack, tail...>::type::exec(st);
    }
};

template <class bstack, char... tail> struct interpreter<bstack, ',', tail...> {
    static void exec(state& st) {
        BF0X_TRACE(',');
        std::cin.get(st.data[st.pos]);
        top<bstack, tail...>::type::exec(st);
    }
};

// A slightly more complex case, we want to do compile-time detection of 
// loop pattern (aka. balanced brackets)
// We parse inside of the loop with one parser and outside with another
template <class bstack, char... tail> struct interpreter<bstack, '[', tail...> {
    static void exec(state& st) {
        BF0X_TRACE("[\n");
        while (st.data[st.pos]) {
            for (int i=0; i<bstack::size; ++i) BF0X_TRACE("  ");

            top<typename bstack::template push<loop_interpreter>::type
                , tail...>::type::exec(st);
        }
        for (int i=0; i<bstack::size-1; ++i) BF0X_TRACE("  ");
        BF0X_TRACE("]");

        top<typename bstack::template push<tail_interpreter>::type
            , tail...>::type::exec(st);
    }
};

// We can use the default interpreter implementation for evaluating since
// interpreter code take care of using the top-of-stack for parsing next
// char, which will correctly call loop_interpreter for next-char
template <class bstack, char c, char... tail> struct loop_interpreter<bstack, c, tail...> {
    // let the base interpreter interpret this char
    static void exec(state& st) {
        interpreter<bstack, c, tail...>::exec(st);    
    }
};

template <class bstack, char... tail> struct loop_interpreter<bstack, ']', tail...> {
    // stop interpretation at ']'
    static void exec(state&) {
        BF0X_TRACE('\n');
    }
};

template <class bstack, char c, char... tail> struct tail_interpreter<bstack, c, tail...> {
    static void exec(state& st) {
        // ignore char and continue parsing using itself
        top<bstack, tail...>::type::exec(st);
    }
};

template <class bstack, char... tail> struct tail_interpreter<bstack, '[', tail...> {
    // push down one more interpreter to count balanced brackets
    static void exec(state& st) {
        top<typename bstack::template push<bf0x::tail_interpreter>::type
            , tail...>::type::exec(st);
    }
};

template <class bstack, char... tail> struct tail_interpreter<bstack, ']', tail...> {
    // pop itself from the interpreter stack and let the parent parse the rest
    static void exec(state& st) {
        top<typename bstack::pop, tail...>::type::exec(st);
    }
};

}

#endif
