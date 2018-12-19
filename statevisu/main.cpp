 
#include <smsys.h>

SMSys *smsys;

int main(int argc, char *argv[]) {

    BAHA_TYPE baha;
    SMSys sys(&baha);

    make_treeuml_allSMs<SMSys>(&sys, "uml/");

}
