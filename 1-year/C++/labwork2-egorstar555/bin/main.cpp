#include <lib/number.h>
#include <iostream>
#include <cinttypes>

int main() {
    int2023_t value1, value2;
//    "405272312330606683982498447530407677486444946329741974138101544027695953739965",
//            "3626777458843887524118528",
//
//            "111744466521471062588629470729710044638866394325887658"
    value1 = from_string("405272312330606683982498447530407677486444946329741974138101544027695953739965");
    value2 = from_string("3626777458843887524118528");
//    value2 = from_int(-1000000000);

//    std::cout << value1 << std::endl;
//    std::cout << value2 << std::endl;
//    std::cout << value1/value2 << std::endl;
//    std::cout << value1*1000000000 << std::endl;
//    std::cout << value2 << std::endl;
//    std::cout << sizeof(int8_t);
//    "405272312330606683982498447530407677486444946329741970511324085183808429621438"
    std::cout << from_string("1234555555555555555555555555555555555555555") * -1;

    return 0;
}