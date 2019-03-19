#include "test_OptHandler.h"
#include "common/OptHandler.h"

#include <iostream>

void test_OptHandler(int argc, char **argv)
{
    // try: ./main -s -f heihei

    std::vector<Opt> opts = {
        {'s', false},
        {'f', true}
    };

    OptHandler oh;
    if (!oh.Parse(argc, argv, opts)) {
        std::cout << oh.ErrorDesc() << std::endl;
        return;
    }

    std::cout << std::boolalpha << "Has opt s? " << oh.HasOpt('s') << std::endl;
    
    if (oh.HasOpt('f')) {
        std::cout << "Has opt f with param " << oh.GetOptParam('f') << std::endl;
    }
    else {
        std::cout << "Has opt f? false\n";
    }
}
