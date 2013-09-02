#include "bf0x.hpp"

int main(int, char*[]) {
    auto multiply = bf0x::interpreter<bf0x::bstack<bf0x::interpreter>,
         ',','>',',','>','+','+','+','+','+','+','+','+','[','<','-','-',
         '-','-','-','-','<','-','-','-','-','-','-','>','>','-',']','<',
         '<','[','>','[','>','+','>','+','<','<','-',']','>','>','[',
         '<','<','+','>','>','-',']','<','<','<','-',']','>','>','>',
         '+','+','+','+','+','+','[','<','+','+','+','+',
         '+','+','+','+','>','-',']',',','<','.','>','.'>();

    bf0x::state st;
    multiply.exec(st);

    std::cout << std::endl;
    return 0;
}
