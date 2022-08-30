//
// Created by wolfgang on 07/08/2022.
//

/*<===================== Includes =====================>*/
/*<---------------- System-Includes ------------------->*/
#include <cstdint>

/*<-------------- Application-Includes ---------------->*/
#include "application.hpp"

/*<----------------- Library-Includes ----------------->*/

int main(int argc, char** argv)
{
    Application app;
    app.init();
    int32_t code = app.loop();
    app.deinit();
    return code;
}