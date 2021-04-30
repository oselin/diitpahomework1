
#include "../include/catch2/catch2.hpp"
#include "../include/car_trailer.h"
#include "../include/machine_car_trailer.h"
#include <string>
#include <iostream>

using namespace std;
TEST_CASE("init should return NULL value if parameters are  wrong ", "[oselin_machine_init]"){

    OselinDevice dev;
    int ntrailers = -2;
    Parameters p;
    p.svgwidth  = 0;
    p.svgheight = 0;

    REQUIRE(oselin_machine_init(p, ntrailers)==NULL);

    REQUIRE(oselin_machine_init(p, 0)==NULL);
}

TEST_CASE("tostring function should return empty string if parameters are wrong","oselin_machine_to_string"){
    OselinMachine *mach = NULL;
    cout << (mach == NULL) << endl;
    cout << "ts" << endl;
    REQUIRE(oselin_machine_to_string(mach)=="");

    //REQUIRE(oselin_machine_to_string(&mach)=="");
}

TEST_CASE("tosave should not save if parameters are wrong","oselin_machine_save"){
    OselinMachine mach;
    
    REQUIRE(oselin_machine_save(&mach)=="The machine looks empty...");

    mach.parameters.length = 1;
    REQUIRE(oselin_machine_save(&mach)=="Something went wrong.");
}

TEST_CASE("parsing should ...","oselin_machine_parsing"){

}
