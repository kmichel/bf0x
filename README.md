# bf0x
A static brainfuck compiler in c++0x.

Code is fed to the compiler using a big ugly variadic template or much better
(but not yet supported by gcc) a string with the custom litteral suffix `BF`.

    auto multiply = ",>,>++++++++[<------<------>>-]"
                    "<<[>[>+>+<<-]>>[<<+>>-]<<<-]"
                    ">>>++++++[<++++++++>-],<.>."BF;
    bf0x::state st;
    multiply.exec(st);

A bunch of variadic templates consume the string char by char, using pattern
matching (partial specialization on the first char) and build a big recursive
function `exec(state& st)` which is then inlined and optimized by the c++0x
compiler.

Thanks to the c++0x compiler, mix of `+`, `-`, `<` and `>` are simplified.
For example, the code "`>++++++++[<------<------>>-]`" is assembled into
something equivalent to :

    data[0] = 8;
    while (data[0] != 0) {
        data[1] -= 6;
        data[2] -= 6;
    }

