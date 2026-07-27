//
//  Common.cpp
//  PeteSlang
//
//  Created by Peter on 29/04/17.
//  Copyright © 2017 Peter. All rights reserved.
//

#include "Common.h"
#include <cstdlib>

/*
 * Quits process , shows error message
 */
void exit_with_message(const char* errMsg_i) {

    std::cout << "\n" << errMsg_i << "\n";
    exit(0);
}
