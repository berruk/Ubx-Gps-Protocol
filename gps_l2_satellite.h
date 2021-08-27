/****************************************
 *
 *   Gps constellation satellites 1- 32
 *   Ephemeris data from
 *   Message types 10, 11, 30
 *
 *
 *   @author :  Berru Karakas
 *   @info   :  karakasb19@itu.edu.tr
 *
 ****************************************/


#ifndef MSG_STR_H
#define MSG_STR_H

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

#include "gps_l2_message_types.hpp"
#include "crc24q.h"

/*
* UBX data types
*/
#define U1 uint8_t
#define U2 uint16_t
#define U4 uint32_t

/*
* checksum struct
*/
struct ck
{
    U1 ck_a = 0;
    U1 ck_b = 0;
};


/* 
___________________________________________________
   UbxFrame Struct:
        Satellite frame 
        Common for each message 
        U1: Ubx data type, 8bits unsigned
        U2: Ubx data type, 16bits unsigned    
___________________________________________________

*/
typedef struct  
{
    public:
    U1 preamble1;     /* Sync character   0xb5   */
    U1 preamble2;     /* Sync character   0x62   */
    U1 msgClass;      /* Satellite class field     */
    U1 msgID;         /* Satellite ID              */
    U2 length;        /* Payload length          */
    U1 gnssId;        /* GNSS Identifier         */
    U1 svId;          /* Satellite Identifier    */
    U1 reserved0;     /* Reserved                */
    U1 freqId;        /* Only used for GLONASS:  */
    U1 numWords;      /* Data words in message   */
    U1 chn;           /* Tracking channel number */
    U1 version;       /* Satellite version         */
    U1 reserved1;     /* Reserved                */
    U1 CK_A;          /* 8 bit checksum          */
    U1 CK_B;          /* 8 bit checksum          */

} UbxFrame;


/*
 * Ephemeris Message
 */

typedef struct{

    double clock_bias;
    double clock_drift;
    double clock_rate;
    double IODE;
    double Crs;
    double delta_n;
    double M0;
    double Cuc;
    double eccentricity;
    double Cus;
    double sqrtA;
    double TOE ;
    double Cic;
    double OMEGA;
    double Cis;
    double I0;
    double Crc;
    double omega;
    double OMEGA_DOT;
    double IDOT;
    double l2_codes;
    double week;
    double l2_p_flag;
    uint8_t sv_acc;
    uint8_t sv_health;
    double tgd;
    double IODC;
    uint32_t trans_time;
    uint8_t fit_interval;

} eph;


/*___________________________________________________
   Satellite Class:
        :binfile: stream of binary input file
        :data: ubx wrapper attributes
        :flag: if satellite sent a msg
        :eph_completed: if ephemeris msg is gathered
        :eph_mssg: ephemeris message of satellite
        :mX vectors: container for message types 
        :msgX pointers: msgX struct's ptr
        :member functions::::::::::::::::::::: 
            -sumchecks: end of msg checksums
            -dec_msgX: X's unique attributes
_____________________________________________________

*/
class Satellite{

    public: 
    
    UbxFrame* data;
    bool flag;
    bool eph_completed;
    eph eph_mssg;

    std::vector<Msg_Type_10> m10;
    std::vector<Msg_Type_11> m11; 
    std::vector<Msg_Type_12> m12;
    std::vector<Msg_Type_13> m13;
    std::vector<Msg_Type_14> m14;
    std::vector<Msg_Type_15> m15;
    std::vector<Msg_Type_30> m30;
    std::vector<Msg_Type_31> m31;
    std::vector<Msg_Type_32> m32;
    std::vector<Msg_Type_33> m33;
    std::vector<Msg_Type_34> m34;
    std::vector<Msg_Type_35> m35;
    std::vector<Msg_Type_36> m36;
    std::vector<Msg_Type_37> m37;


    void decode_gps_l2c(std::ifstream&);
    void read_words(uint32_t*, std::ifstream& binfile);
    bool check_sum(uint32_t*);

    void dec_msg10(uint32_t*);
    void dec_msg11(uint32_t*);
    void dec_msg12(uint32_t*);
    void dec_msg13(uint32_t*);
    void dec_msg14(uint32_t*);
    void dec_msg30(uint32_t*);
    void dec_msg31(uint32_t*);
    void dec_msg32(uint32_t*);
    void dec_msg33(uint32_t*);
    void dec_msg34(uint32_t*);
    void dec_msg35(uint32_t*);
    void dec_msg36(uint32_t*);
    void dec_msg37(uint32_t*);
    
    /*
        Time of week to time in d h m s
        @param tow
    */
    inline void tell_time(int index = 0) const
    {
        uint32_t sec =  m11[index].TOW*1.5*4;
        uint16_t end_sec = sec%60; 
        uint32_t min = sec/60;
        uint16_t end_min = min%60;
        uint32_t hrs = min/60;
        uint8_t end_hrs = hrs%24;
        uint8_t days = hrs/24;

        std::cout 
        << "Day: "   << (unsigned)days
        << " Hr: "   << (unsigned)end_hrs
        << " Min: "  << (unsigned)end_min
        << " Secs: " << (unsigned)end_sec 
        << std::endl;
    }             

    inline double sv_accuracy(int URAed) const
    {
        double x;
        uint32_t mask = 0b00000000000000000000000000011111;
        URAed = (URAed ^ mask) + 1;

        if (URAed == 1) x = 2.8;
        else if (URAed == 3) x = 5.7;
        else if (URAed == 5) x = 11.3;
        else if (URAed <= 6 && URAed > -16) x = pow(2, 1 + URAed/2);
        else if (URAed > 6 && URAed < 15) x = pow(2, URAed -2);
        else if( URAed == -16 || URAed == -15) x = -1;

        return x;


    }                      

    /* crc24q check 276 - 300 bits */
    uint32_t crc_check(uint32_t* wrd)
    {
        uint8_t* bytes = new uint8_t[40];
        for(int i=0;i<10;i++)
         for(int j=0;j<4;j++)
             bytes[i*4 + j] = extractbit(wrd[i], j*8, (j+1)*8 - 1);

        uint8_t extracted = extractbit(bytes[34], 0, 3);
        uint32_t crc = crc24q_bits(bytes,276,false);
        free(bytes);
        return crc;
    }

    /* print parameters of rinex format */
    inline friend std::ostream &operator<< (std::ostream &output, 
                                            const Satellite &msg){
        
        int index_11 = msg.m11.size()-1,
            index_10 = msg.m10.size()-1,
            index_30 = msg.m30.size()-1,
            index_32 = msg.m32.size()-1,
            index_33 = msg.m33.size()-1,
            index_34 = msg.m34.size()-1;

        if(index_10 != -1
        && index_11 != -1
        && index_30 != -1
        && index_32 != -1 
        && index_33 != -1 ){

        msg.tell_time(index_11);
            
        std::cout.precision(12);

        output << std::scientific << 
               
               /* SV / EPOCH / SV CLK */
                "Satellite: G" << (unsigned)msg.data->svId  << std::endl

               << "SV clock bias: " << msg.m30[index_30].af0        << std::endl
               << "SV clock drift: " << msg.m30[index_30].af1       << std::endl
               << "SV clock drift rate: " << msg.m30[index_30].af2  << std::endl

                /* Broadcast orbit - 1 */
               // << "IODE, Ephemeris" << <<  std::endl     //only l1?
                << "Crs " << msg.m11[index_11].crsn << std::endl
                << "/Delta n " << msg.m10[index_10].delntan0 * PI << std::endl 
                << "M0 " << msg.m10[index_10].M0n * PI << std::endl

                /* Broadcast orbit - 2 */
               << "Cuc " << msg.m11[index_11].cucn << std::endl
                << "/e " <<  msg.m10[index_10].en << std::endl 
                << "Cus " << msg.m11[index_11].cusn << std::endl
                << "/sqrtA " << sqrt(msg.m10[index_10].deltaA + AREF) << std::endl 

                /* Broadcast orbit - 3 */
                << "/Toe " << msg.m10[index_10].toe << std::endl 
                << "/TOW " << msg.m10[index_10].TOW*1.5*4 - 12 << std::endl 
                << "Cic " << msg.m11[index_11].cicn << std::endl
                << "/omega " << msg.m10[index_10].omegan * PI << std::endl 
                << "Cis " << msg.m11[index_11].cisn << std::endl

                /* Broadcast orbit - 4 */               
                << "/i0 " << msg.m11[index_11].i0n * PI << std::endl
                << "Crc " << msg.m11[index_11].crcn << std::endl
                << "/OMEGA " << msg.m11[index_11].omega0n * PI << std::endl
                << "/Omega Dot " << (msg.m11[index_11].delomegadot + OMEGADOTREF) * PI << std::endl

                /* Broadcast orbit - 5 */
                << "I Dot " << msg.m11[index_11].i0nDOT * PI << std::endl
                //<< "Codes on L2 Channel" <<  << std::endl
                << "/Gps Week " << msg.m10[index_10].WN << std::endl
                << "/L2 P Data flag " << (unsigned)msg.m10[index_10].L2C_phasing << std::endl

                /* Broadcast orbit - 6 */
                //<< "Sv accuracy " << msg.sv_accuracy(msg.m32[index_32].URANED0) << std::endl //only l1?
                //<< "Sv health" <<  << std::endl //only l1?
                << "/TGD " << msg.m30[index_30].TGD << std::endl
                << "toc " << msg.m30[index_30].toc<< std::endl
                //<< "IODC Issue of Data, Clock" <<  << std::endl //only l1?

                /* Broadcast orbit - 7 */
                //<< "Transmission time of message" <<  << std::endl
                //<< "Fit interval in hours, Clock" <<  << std::endl
                //<< "Spare x2" <<

                << std::endl;
        }
        else if(msg.m10.size() == 0)
                output << "Ephemeris 1 not received!" << std::endl;
        else if(msg.m11.size() == 0)
                output << "Ephemeris 2 not received!" << std::endl;        
        else if(msg.m30.size() == 0)
                output << "30 not received!" << std::endl;
        else if(msg.m32.size() == 0)
                output << "32 not received!" << std::endl; 
        else if(msg.m33.size() == 0)
                output << "33 not received!" << std::endl;    

        return output;


    }

    /* collect ephemeris data from
     * message 10, 11, 30          */
    void get_ephemeris(){

        int index_11 =m11.size()-1,
        index_10 = m10.size()-1,
        index_30 = m30.size()-1;

        std::cout << "Satellite: G" << (unsigned) data->svId  << std::endl;

        /* is ephemeris completed? */
        if(m10.size() == 0)
            std::cout << "Ephemeris 1 not received!" << std::endl;
        else if(m11.size() == 0)
            std::cout << "Ephemeris 2 not received!" << std::endl;
        else if(m30.size() == 0)
            std::cout << "Clock parameters not received!" << std::endl;
        else{
                std::cout << "Ephemeris data collected" << std::endl;
                eph_completed = true;
                eph_mssg.clock_bias = m30[index_30].af0;
                eph_mssg.clock_drift = m30[index_30].af1;
                eph_mssg.clock_rate = m30[index_30].af2;
                eph_mssg.IODE = 0;
                eph_mssg.Crs = m11[index_11].crsn;
                eph_mssg.delta_n = m10[index_10].delntan0 * PI;
                eph_mssg.M0 = m10[index_10].M0n * PI;
                eph_mssg.Cuc = m11[index_11].cucn;
                eph_mssg.eccentricity = m10[index_10].en;
                eph_mssg.Cus = m11[index_11].cusn;
                eph_mssg.sqrtA = sqrt(m10[index_10].deltaA + AREF);
                eph_mssg.TOE = m10[index_10].toe;
                eph_mssg.Cic = m11[index_11].cicn;
                eph_mssg.OMEGA = m11[index_11].omega0n * PI;
                eph_mssg.Cis = m11[index_11].cisn;
                eph_mssg.I0 = m11[index_11].i0n * PI;
                eph_mssg.Crc = m11[index_11].crcn;
                eph_mssg.omega = m10[index_10].omegan * PI;
                eph_mssg.OMEGA_DOT = (m11[index_11].delomegadot + OMEGADOTREF) * PI;
                eph_mssg.IDOT = m11[index_11].i0nDOT * PI;
                eph_mssg.week = m10[index_10].WN;
                eph_mssg.l2_p_flag = 0;
                eph_mssg.tgd = m30[index_30].TGD;
                eph_mssg.IODC = 0;
        }
    }

    /* write to file in rinex 3.04 format */
    void rinex_formatter(std::string output_file)
    {
            get_ephemeris();
            if(!eph_completed) std::cout << "Ephemeris not completed!" << std::endl;
            else{
                double filler = 0;
                double fit_interval = 4; //?

                std::ofstream fileStream(output_file,
                                         std::ios_base::app);

                std::setprecision(12);
                if (fileStream.is_open()){
                    std::cout << "opened for writing" << std::endl;

                    fileStream
                    //<< "G"
                    << (unsigned) data->svId << " " <<
                    21 << " " << 13 << " " << 7 << " " << 12 << " " << 0 << " " << 0 << " "
                    << std::setprecision(12) << std::scientific
                    << eph_mssg.clock_bias
                    << " " << eph_mssg.clock_drift
                    << " " << eph_mssg.clock_rate << std::endl
                    << "\t"<< eph_mssg.IODE
                    << " " << eph_mssg.Crs
                    << " " << eph_mssg.delta_n
                    << " " << eph_mssg.M0 << std::endl
                    << "\t"<< eph_mssg.Cuc
                    << " " << eph_mssg.eccentricity
                    << " " << eph_mssg.Cus
                    << " " << eph_mssg.sqrtA << std::endl
                    << "\t"<< eph_mssg.TOE
                    << " " << eph_mssg.Cic
                    << " " << eph_mssg.OMEGA
                    << " " << eph_mssg.Cis << std::endl
                    << "\t"<< eph_mssg.I0
                    << " " << eph_mssg.Crc
                    << " " << eph_mssg.omega
                    << " " << eph_mssg.OMEGA_DOT << std::endl
                    << "\t"<< eph_mssg.IDOT
                    << " " << filler
                    << " " << eph_mssg.week
                    << " " << eph_mssg.l2_p_flag  << std::endl
                    << "\t"<< filler
                    << " " << filler
                    << " " << eph_mssg.tgd
                    << " " << eph_mssg.IODC << std::endl
                    << "\t"<< filler
                    << " " << fit_interval << std::endl;
                }
                fileStream.close();

            }
    }


    Satellite(){
        data = NULL;
        flag = false;
        eph_completed = false;
    }

    ~Satellite(){
        free(data);
    }

    
};

/*___________________________________________________
   SatelliteFile Class:
        :binfile: stream of binary input file
        :satellite: 1-32 gps satellites array
        :mX vectors: container for message types
        :member functions:::::::::::::::::::::
            -gps_file: extract from binary file
            -find_msg: find gps msgs in binary
            -msg_count: msg count of satellites
___________________________________________________

*/
class SatelliteFile{

    public:
    Satellite* satellite[33];
    std::ifstream binfile;
    void gps_file(std::string&);
    bool find_message();

    SatelliteFile()
    {
        for (int i = 1 ; i <= 32 ; i++)
            satellite[i] = new Satellite();
    }

    ~SatelliteFile(){
        for (int i = 1 ; i <= 32 ; i++)
        {
            free(satellite[i]);
            satellite[i] = NULL;
        }
    }

    void msg_count(int sat)
    {
        std::cout << "Satellite: " << (unsigned)satellite[sat]->data->svId << std::endl;
        std::cout << "Count 10 " << satellite[sat]->m10.size() << std::endl;
        std::cout << "Count 11 " << satellite[sat]->m11.size() << std::endl;
        std::cout << "Count 15 " << satellite[sat]->m15.size() << std::endl;
        std::cout << "Count 30 " << satellite[sat]->m30.size() << std::endl;
        std::cout << "Count 32 " << satellite[sat]->m32.size() << std::endl;
    }


};


/*
* Decode Satellite 10
*   decoding authentic msg10 properties
*/       
void Satellite::dec_msg10(uint32_t* wrd)
{
    Msg_Type_10 m;
    m.decode(wrd);    
    m10.push_back(m);

    uint32_t crc = crc_check(wrd);
    if(m.CRC != crc) std::cout << "Crc does not match" << std::endl;

}

/*
* Decode Satellite 11
*   decoding authentic msg11 properties
*/       
void Satellite::dec_msg11(uint32_t* wrd)
{
    Msg_Type_11 m;
    m.decode(wrd);
    m11.push_back(m);

    uint32_t crc = crc_check(wrd);
    if(m.CRC != crc) std::cout << "Crc does not match" << std::endl;

}


/*
* Decode Satellite 12
*   decoding authentic msg12 properties
*/       
void Satellite::dec_msg12(uint32_t* wrd)
{
    Msg_Type_12 m;
    m.decode(wrd);    
    m12.push_back(m);

    uint32_t crc = crc_check(wrd);
    if(m.CRC != crc) std::cout << "Crc does not match" << std::endl;
}

/*
* Decode Satellite 13
*   decoding authentic msg13 properties
*/       
void Satellite::dec_msg13(uint32_t* wrd)
{
    Msg_Type_13 m;
    m.decode(wrd);
    m13.push_back(m);

    uint32_t crc = crc_check(wrd);
    if(m.CRC != crc) std::cout << "Crc does not match" << std::endl;
}

/*
* Decode Satellite 14
*   decoding authentic msg14 properties
*/       
void Satellite::dec_msg14(uint32_t* wrd){

    Msg_Type_14 m;
    m.w1.word = wrd[0];
    m.w2.word = wrd[1];
    m.w3.word = wrd[2];
    m.w4.word = wrd[3];
    m.w5.word = wrd[4];
    m.w6.word = wrd[5];
    m.w7.word = wrd[6];
    m.w8.word = wrd[7];
    m.w9.word = wrd[8]; 
    m.w10.word = wrd[9];    
    m14.push_back(m); 

}

/*
* Decode Satellite 30
*   decoding authentic msg30 properties
*/       
void Satellite::dec_msg30(uint32_t* wrd)
{
    Msg_Type_30 m;
    m.decode(wrd);
    m30.push_back(m); 

    uint32_t crc = crc_check(wrd);
    if(m.CRC != crc) std::cout << "Crc does not match" << std::endl;

}

/*
* Decode Satellite 31
*   decoding authentic msg31 properties
*/       
void Satellite::dec_msg31(uint32_t* wrd)
{
    Msg_Type_31 m;
    m.w1.word = wrd[0];
    m.w2.word = wrd[1];
    m.w3.word = wrd[2];
    m.w4.word = wrd[3];
    m.w5.word = wrd[4];
    m.w6.word = wrd[5];
    m.w7.word = wrd[6];
    m.w8.word = wrd[7];
    m.w9.word = wrd[8]; 
    m.w10.word = wrd[9];
    m31.push_back(m); 

}

/*
* Decode Satellite 32
*   decoding authentic msg32 properties
*/       
void Satellite::dec_msg32(uint32_t* wrd){

    Msg_Type_32 m;
    m.decode(wrd);
    m32.push_back(m); 
    uint32_t crc = crc_check(wrd);
    if(m.CRC != crc) std::cout << "Crc does not match" << std::endl;
}

/*
* Decode Satellite 33
*   decoding authentic msg33 properties
*/       
void Satellite::dec_msg33(uint32_t* wrd)
{
    Msg_Type_33 m;
    m.decode(wrd);
    m33.push_back(m); 
    uint32_t crc = crc_check(wrd);
    if(m.CRC != crc) std::cout << "Crc does not match" << std::endl;

}

/*
* Decode Satellite 34
*   decoding authentic msg34 properties
*/       
void Satellite::dec_msg34(uint32_t* wrd)
{
    Msg_Type_34 m;
    m.decode(wrd);
    m34.push_back(m); 
    
    uint32_t crc = crc_check(wrd);
    if(m.CRC != crc) std::cout << "Crc does not match" << std::endl;

}

/*
* Decode Satellite 35
*   decoding authentic msg35 properties
*/       
void Satellite::dec_msg35(uint32_t* wrd)
{
    Msg_Type_35 m;
    m.w1.word = wrd[0];
    m.w2.word = wrd[1];
    m.w3.word = wrd[2];
    m.w4.word = wrd[3];
    m.w5.word = wrd[4];
    m.w6.word = wrd[5];
    m.w7.word = wrd[6];
    m.w8.word = wrd[7];
    m.w9.word = wrd[8]; 
    m.w10.word = wrd[9];
    m35.push_back(m); 

}

/*
* Decode Satellite 36
*   decoding authentic msg36 properties
*/       
void Satellite::dec_msg36(uint32_t* wrd)
{
    Msg_Type_36 m;
    m.w1.word = wrd[0];
    m.w2.word = wrd[1];
    m.w3.word = wrd[2];
    m.w4.word = wrd[3];
    m.w5.word = wrd[4];
    m.w6.word = wrd[5];
    m.w7.word = wrd[6];
    m.w8.word = wrd[7];
    m.w9.word = wrd[8]; 
    m.w10.word = wrd[9];  
    m36.push_back(m); 

}

/*
* Decode Satellite 37
*   decoding authentic msg37 properties
*/       
void Satellite::dec_msg37(uint32_t* wrd)
{
    Msg_Type_37 m;
    m.w1.word = wrd[0];
    m.w2.word = wrd[1];
    m.w3.word = wrd[2];
    m.w4.word = wrd[3];
    m.w5.word = wrd[4];
    m.w6.word = wrd[5];
    m.w7.word = wrd[6];
    m.w8.word = wrd[7];
    m.w9.word = wrd[8]; 
    m.w10.word = wrd[9];    
    m37.push_back(m); 

}

#endif

