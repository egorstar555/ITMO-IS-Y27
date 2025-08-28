#include <functional>
#include <lib/ArgParser.h>

#include <iostream>
#include <numeric>

struct Options {
    bool sum = false;
    bool mult = false;
};


int main(int argc, char** argv) {
//    Options opt;
//    std::vector<int> values;
//
//    ArgumentParser::ArgParser parser("Program");
//    parser.AddIntArgument("N").MultiValue(1).Positional().StoreValues(values);
//    parser.AddFlag("sum", "add args").StoreValue(opt.sum);
//    parser.AddFlag("mult", "multiply args").StoreValue(opt.mult);
//    parser.AddHelp('h', "help", "Program accumulate arguments");
//
//    if(!parser.Parse(argc, argv)) {
//        std::cout << "Wrong argument" << std::endl;
//        std::cout << parser.HelpDescription() << std::endl;
//        return 1;
//    }
//
//    if(parser.Help()) {
//        std::cout << parser.HelpDescription() << std::endl;
//        return 0;
//    }
//
//    if(opt.sum) {
//        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
//    } else if(opt.mult) {
//        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<int>()) << std::endl;
//    } else {
//        std::cout << "No one options had chosen" << std::endl;
//        std::cout << parser.HelpDescription();
//        return 1;
//    }



    ArgumentParser::ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file").MultiValue(1);
    parser.AddFlag('s', "flag1", "Use some logic").Default(true);
    parser.AddFlag('p', "flag2", "Use some logic");
    parser.AddIntArgument("numer", "Some Number");


    parser.Parse({"app","--help"});
    std::cout << parser.HelpDescription();
    // Проверка закоментирована намеренно. Ождиается, что результат вызова функции будет приблизительно такой же,
    // но не с точностью до символа

    // ASSERT_EQ(
    //     parser.HelpDescription(),
    //     "My Parser\n"
    //     "Some Description about program\n"
    //     "\n"
    //     "-i,  --input=<string>,  File path for input file [repeated, min args = 1]\n"
    //     "-s,  --flag1,  Use some logic [default = true]\n"
    //     "-p,  --flag2,  Use some logic\n"
    //     "     --number=<int>,  Some Number\n"
    //     "\n"
    //     "-h, --help Display this help and exit\n"
    // );




    return 0;

}
