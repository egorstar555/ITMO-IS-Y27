#include "lib/SimpleSearchEngine.h"
//#include <Windows.h>
// Параметры запуска:
//            -s C:\Users\user\CLionProjects\labwork11-egorstar555\Index\ind.fti
//            -c C:\Users\user\CLionProjects\labwork11-egorstar555\data C:\Users\user\CLionProjects\labwork11-egorstar555\Index\ind.fti
#include <ctime>

int main(int argc, char* argv[]) {
    InvertedIndex index;

//    SetConsoleCP(1251);
//    SetConsoleOutputCP(1251);


    unsigned int start_time = clock(); // начальное время

    ParseCommandLineArguments(argc, argv, index);

    unsigned int end_time = clock(); // конечное время

    unsigned int search_time = end_time - start_time; // искомое время
    std::cout << search_time;

    return 0;
}
// -c C:\Users\user\CLionProjects\SimpleSearchEngine D:\indexes\clion_p_11.fti
// -c D:\wilipedia D:\indexes\96GB_ind.fti
// -c D:\100263103 D:\indexes\word_and_peace.fti
// -s D:\indexes\word_and_peace.fti
// -s D:\indexes\word_and_peace.fti
// -s D:\indexes\war_and_peace_engl.fti
// D:\100263103
// -c D:\100263103 D:\indexes\war_and_peace_engl.fti