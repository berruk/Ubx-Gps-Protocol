#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

#include "gps_l2_cnav_decode.h"
#include "bit.h" 
#include "binaryfile.h"
#include "crc24q.h"
#include "gpstime.h"

#define uLOG(x) std::cout << (unsigned)x << std::endl;
#define sLOG(x) std::cout <<           x << std::endl;

#define GPS_SATS 32

int main(int argv, char** argc)
{
    std::string input_file = "C:\\Users\\berru\\CLionProjects\\navdata\\COM3_210730_115228.ubx";
    std::string output_file = "C:\\Users\\berru\\CLionProjects\\navdata\\COM3_210730_115228.ubx";

    SatelliteFile m;

    m.gps_file(input_file);

    for(int i = 1 ; i < GPS_SATS; ++i)
        if(m.satellite[i]->flag)
            //m.satellite[i]->rinex_formatter(output_file);
            std::cout << *(m.satellite[i]) ;
            //m.msg_count(i);

    gtime_t x = gpst2time(m.satellite[1]->m10[0].w2.WN,
                              m.satellite[1]->m10[0].TOW);

    printf ( "Time %s", ctime (&x.time) );;



}