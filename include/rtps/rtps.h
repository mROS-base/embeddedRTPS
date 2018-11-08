/*
 *
 * Author: Andreas Wüstenberg (andreas.wuestenberg@rwth-aachen.de)
 */

#ifndef RTPS_RTPS_H
#define RTPS_RTPS_H

#include "types.h"

namespace rtps{
    void init();
    Time_t getCurrentTimeStamp();
}

#endif //RTPS_RTPS_H