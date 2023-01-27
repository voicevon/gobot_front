#include "all_applications.h"
#ifdef I_AM_GUANGDA_SHUIWU_ROUTER

#include "Mesh/Router/adhoc_router.h"

AdhocRouter router;

void setup(){

}

void loop(){
    router.SpinOnce();
}


#endif