/*********************************
 *
 *   GPS CNAV Message Structures
 *   Message types 10 - 15
 *                 30 - 37
 *
 *   @author: Berru Karakas
 *   @info:   karakasb19@itu.edu.tr
 *
 **********************************/

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>
#include "bit.h"

#ifndef GPS_STRC_H
#define GPS_STRC_H

#define log(z) { std::cout << z << std::endl; }

#include <bitset> //test purposes

#define bit8(x){ std::bitset<8> c(x); \
std::cout << static_cast<unsigned int>(x) << " " << c << std::endl;}

#define bit16(x){ std::bitset<16> c(x); \
std::cout << static_cast<int>(x) << " " << c << std::endl;}

#define bit32(y){ std::bitset<32> c(y);\
std::cout << c << std::endl;}

/* Semi circles to radian */
#define PI 3.1415926535898

/* Scaling factors */
#define P2_8  0.00390625           /* 2^(-8)  */
#define P2_9  0.001953125          /* 2^(-9)  */
#define P2_21 4.76837158203125E-7  /* 2^(-21) */
#define P2_30 9.5367431640625E-7   /* 2^(-30) */
#define P2_32 2.328306436538E-10   /* 2^(-32) */
#define P2_34 5.820766091346E-11   /* 2^(-34) */
#define P2_35 2.910383045673E-11   /* 2^(-35) */
#define P2_44 5.684341886080E-14   /* 2^(-44) */
#define P2_48 3.552713678800E-15   /* 2^(-48) */
#define P2_51  4.440892098500E-16  /* 2^(-51) */
#define P2_57 6.938893903907E-18   /* 2^(-57) */
#define P2_60 8.673617379884E-19   /* 2^(-60) */

#define AREF          26559710              /* Semi major axis reference */
#define OMEGADOTREF  -2.6E-9                /* Right ascension reference */

/* Common attributes */
typedef union
{
    struct
    {
        uint32_t TOW       : 12;
        uint32_t msgTypeId :  6;
        uint32_t PRN       :  6;
        uint32_t preamble  :  8;
        
    };
    uint32_t word;
} common;

/* CDC struct */
union cdc
        {
    struct
    {
        uint64_t UDRA        : 5;
        uint64_t daf1        : 8;
        uint64_t daf0        : 13;
        uint64_t prn         : 8;

    };
    uint64_t word;

        } ;


/* 
___________________________________________________
   Message 10 Struct:
        Ephemeris 1 
___________________________________________________

*/
typedef struct {
    
public:
    
    typedef union
    {
        struct
        {
            uint32_t TOW        : 12;
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;

        };
        uint32_t word;

    } Word1;

    Word1  w1;

    
    typedef union 
    {
        struct
        {
            uint32_t top       : 10; //propagation tow
            uint32_t L5_health :  1;
            uint32_t L2_health :  1;
            uint32_t L1_health :  1;
            uint32_t WN        : 13; //week number
            uint32_t alert     :  1;
            uint32_t TOW       :  5; //time of week
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t deltaA    : 15; //semi major ax difference
            uint32_t toe        : 11; //time of ephemeris
            uint32_t URAindex   :  5;
            uint32_t top       :  1;
        };
        uint32_t word;

    } Word3;
    typedef union
    {
        struct
        {
            uint32_t Adot      : 21;
            uint32_t deltaA    : 11;
        };
        uint32_t word;

    } Word4;
    typedef union
    {
        struct
        {
            uint32_t deln0dot  : 11;
            uint32_t deltan0   : 17;
            uint32_t Adot      : 4;
        };
        uint32_t word;

    } Word5;
    typedef union
    {
        struct
        {
            uint32_t M0n      : 20;
            uint32_t deln0dot  : 12;
        };
        uint32_t word;

    } Word6;
    typedef union
    {
        struct
        {
            uint32_t en        : 19;
            uint32_t M0n       : 13;
        };
        uint32_t word;

    } Word7;
    typedef union
    {
        struct
        {
            uint32_t omegan    : 18;
            uint32_t en        : 14;
        };
        uint32_t word;
    } Word8;
    typedef union
    {
        struct
        {
            uint32_t CRC        : 12;
            uint32_t  reserved   : 3;
            uint32_t  L2_phase   : 1;
            uint32_t  integ_flag : 1;
            uint32_t omegan     : 15;
        };
        uint32_t word;
    } Word9;
    typedef union
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;
    } Word10;

    Word2   w2;
    Word3   w3;
    Word4   w4;
    Word5   w5;
    Word6   w6;
    Word7   w7;
    Word8   w8;
    Word9   w9;
    Word10 w10;

public:

    /* Interface */
    uint16_t WN;         /* propagation week number */
    uint32_t top ;       /* time of propagation */
    uint8_t L5_health;
    uint8_t L2_health;
    uint8_t L1_health;
    uint8_t alert;
    double deltaA ;      /* semi major ax difference */
    uint32_t toe;        /* time of ephemeris */
    int8_t URAi;         /* ED accuracy index */
    double Adot;         /* semi major ax change rate */
    double delntan0;     /* mean motion difference */
    double deln0dot;     /* rate of mean motion difference */
    double en;           /* eccentiricity */
    double M0n;          /* mean anomaly at ref time */
    double omegan ;      /* argument of perigee */
    uint8_t integ_flag ;
    uint8_t L2C_phasing;
    uint32_t CRC;
    uint32_t TOW;

    void decode(uint32_t* wrd)
    {
        w1.word  = wrd[0];
        w2.word  = wrd[1];
        w3.word  = wrd[2];
        w4.word  = wrd[3];
        w5.word  = wrd[4];
        w6.word  = wrd[5];
        w7.word  = wrd[6];
        w8.word  = wrd[7];
        w9.word  = wrd[8];
        w10.word = wrd[9];


        CRC = concatbin(w9.CRC, w10.CRC, 12);
        TOW = concatbin(w1.TOW, w2.TOW, 5);
        WN = w2.WN;
        top = concatbin(w2.top,w3.top,1) * 300;
        L5_health = w2.L5_health;
        L2_health = w2.L2_health;
        L1_health = w2.L1_health;
        alert     = w2.alert;
        deltaA = concatbin_signed_64(w3.deltaA,w4.deltaA,15,11) * P2_9;
        toe = w3.toe * 300;
        URAi = concatbin_signed_32(w3.URAindex,0,5,0);
        Adot = concatbin_signed_64(w4.Adot,w5.Adot,21,4) * P2_21;
        bit32(w6.M0n);
        bit32(w6.word)
        bit32(w6.M0n);
        bit32(w6.word);
        delntan0 = concatbin_signed_32(w5.deltan0,0,17,0);
        std::cout << "deln " << delntan0;
        delntan0 *= P2_44;
        deln0dot = concatbin_signed_32(w5.deln0dot,w6.deln0dot,11,12) * P2_57;
        M0n = concatbin_signed_64(w6.M0n,w7.M0n,20,13) * P2_32;
        en = concatbin(w7.en,w8.en,14) * P2_34 ;
        omegan = concatbin_signed_64(w8.omegan,w9.omegan,18,15) * P2_32;
        integ_flag = w9.integ_flag;
        L2C_phasing = w9.L2_phase;
    }

} Msg_Type_10;

/*
___________________________________________________
   Message 11 Struct:
        Ephemeris 2
        data scaled and formatted
___________________________________________________

*/

class  Msg_Type_11 {

public:


    typedef union
    {
        struct
        {
            uint32_t TOW        : 12;
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;

        };
        uint32_t word;

    } Word1;

    Word1  w1;

    typedef union
    {
        struct
        {
            uint32_t omega0n    : 15;
            uint32_t toe        : 11;
            uint32_t alert      :  1;
            uint32_t TOW       :   5;

        };
        uint32_t word;

    } Word2;

    typedef union
    {
        struct
        {
            uint32_t i0n        : 14;
            uint32_t omega0n    : 18;
        };
        uint32_t word;

    } Word3;

    typedef union
    {
        struct
        {
            uint32_t deltaomega : 13;
            uint32_t i0n        : 19;
        };
        uint32_t word;

    } Word4;

    typedef union
    {
        struct
        {
            uint32_t cisn       : 13;
            uint32_t i0nDOT     : 15;
            uint32_t deltaomega : 4;

        };
        uint32_t word;

    } Word5;

    typedef union
    {
        struct
        {
            uint32_t crsn       : 13;
            uint32_t cicn       : 16;
            uint32_t cisn       : 3;

        };
        uint32_t word;

    } Word6;

    typedef union
    {
        struct
        {
            uint32_t crcn       : 21;
            uint32_t crsn       : 11;

        };
        uint32_t word;

    } Word7;

    typedef union
    {
        struct
        {
            uint32_t cucn       : 8;
            uint32_t cusn       : 21;
            uint32_t crcn       : 3;
        };
        uint32_t word;
    } Word8;

    typedef union
    {
        struct
        {
            uint32_t CRC        : 12;
            uint32_t reserved    : 7;
            uint32_t cucn       : 13;
        };
        uint32_t word;
    } Word9;

    typedef union
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;
    } Word10;

    Word2   w2;
    Word3   w3;
    Word4   w4;
    Word5   w5;
    Word6   w6;
    Word7   w7;
    Word8   w8;
    Word9   w9;
    Word10 w10;

public:

    /* Interface */
    uint32_t TOW   ;
    double toe   ;         /* time of ephemeris */
    double omega0n;        /* longitude of ascending node */
    double i0n    ;        /* inclination angle at ref time */
    double delomegadot;    /* rate of right ascension difference */
    double i0nDOT     ;    /* rate of inclination angle */
    double cisn       ;    /* sine harmonic correction term to the angle of inclination */
    double cicn       ;    /* cosine harmonic correction term to the angle of inclination */
    double crsn       ;    /* sine correction term to the orbit radius */
    double crcn       ;    /* cosine correction term to the orbit radius */
    double cusn       ;    /* sine harmnoic correction term to the argument of latitude */
    double cucn       ;    /* cosine harmonic correction term to the argument of latitude */
    uint8_t reserved;
    uint32_t CRC   ;

    /* words read */
    void decode(uint32_t* wrd)
    {
        w1.word  = wrd[0];
        w2.word  = wrd[1];
        w3.word  = wrd[2];
        w4.word  = wrd[3];
        w5.word  = wrd[4];
        w6.word  = wrd[5];
        w7.word  = wrd[6];
        w8.word  = wrd[7];
        w9.word  = wrd[8];
        w10.word = wrd[9];


        TOW         = concatbin(w1.TOW, w2.TOW, 5);
        toe         = w2.toe * 300;
        omega0n     = concatbin_signed_64(w2.omega0n,w3.omega0n,15,18)* P2_32;
        i0n         = concatbin_signed_64(w3.i0n,w4.i0n,14,19)* P2_32;
        delomegadot = concatbin_signed_64(w4.deltaomega,w4.deltaomega,13,4)* P2_44;
        i0nDOT      = concatbin_signed_32(w5.i0nDOT,0,15,0)* P2_44;
        cisn        = concatbin_signed_32(w5.cisn,w6.cisn,13,3) * P2_30;
        cicn        = concatbin_signed_32(w6.cicn,0,16,0)* P2_30  ;
        crsn        = concatbin_signed_32(w6.crsn,w7.crsn,13,11)* P2_8 ;
        crcn        = concatbin_signed_32(w7.crcn,w8.crcn,21,3)  * P2_8 ;
        cusn        = concatbin_signed_32(w8.cusn,0,21,0)* P2_30 ;
        cucn        = concatbin_signed_32(w8.cucn,w9.cucn,8,13)* P2_30;
        reserved    = w9.reserved;
        CRC         = concatbin(w9.CRC,w10.CRC,12);

    }



};



/*
___________________________________
   Message 12 Struct:
        Reduced Almanac
___________________________________

*/
class Msg_Type_12 {
    
public:
    typedef union
    {
        struct
        {
            uint32_t  TOW      : 12; 
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;
    
    Word1 w1;

    //private:
    typedef union 
    {
        struct
        {
            uint32_t redAlm1   :   5; 
            uint32_t  toa        : 8; 
            uint32_t WNa0_n      : 13;
            uint32_t alert      :  1;
            uint32_t TOW       :   5; 
        
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t redAlm2   : 6; 
            uint32_t redAlm1   : 26; 
           
        };
        uint32_t word;
    
    } Word3;
    typedef union 
    {
        struct
        {
            uint32_t redAlm3   : 7; 
            uint32_t redAlm2   : 25;

        };
        uint32_t word;

    } Word4;
    typedef union 
    {
        struct
        {    
        
            uint32_t redAlm4   : 8; 
            uint32_t redAlm3   : 24;
        };
        uint32_t word;

    } Word5;
    typedef union 
    {
        struct
        {
            uint32_t redAlm5   : 9;   
            uint32_t redAlm4   : 23;
            
        };
        uint32_t word;

    } Word6;
    typedef union 
    {
        struct
        {
            uint32_t redAlm6   : 10; 
            uint32_t redAlm5   : 22;
            
        };
        uint32_t word;

    } Word7;
    typedef union 
    {
        struct
        {
            uint32_t redAlm7   : 11; 
            uint32_t redAlm6   : 21;

        };
        uint32_t word;
    } Word8;
    typedef union 
    {
        struct
        {
            uint32_t CRC        : 12;
            uint32_t redAlm7   : 20;

        };
        uint32_t word;
    } Word9;
    typedef union 
    {
        struct
        {   
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;
    } Word10;

    Word2   w2;
    Word3   w3;
    Word4   w4;
    Word5   w5;
    Word6   w6;
    Word7   w7;
    Word8   w8;
    Word9   w9;
    Word10 w10;

    /* reduced almanac packets */
    typedef union{        
        struct
        {
            uint32_t L5 :1;
            uint32_t L2 :1;
            uint32_t L1 :1;
            uint32_t phi_0 : 7;
            uint32_t omega_0 : 7;
            uint32_t sigma_A : 8;
            uint32_t PRNa :6;
        };
        uint32_t packet;

    } raw_reduced_almanac;

    typedef struct
    {
        uint8_t L5;
        uint8_t L2;
        uint8_t L1;
        int8_t phi_0; /* argument of latitude at ref time */
        int8_t omega_0; /* longtitude of ascending node */
        int8_t sigma_A; /* semi major ax correction */
        uint8_t PRNa;
    } reduced_almanac;

public:

    uint16_t WNan;          /* number of week */
    uint8_t  toa;              /* time of almanac */
    uint32_t TOW;
    uint8_t alert = w2.alert;
    std::vector<reduced_almanac> redalm; /* vector of reduced almanacs */
    uint32_t CRC;

    void decode(uint32_t* wrd)
    {
        w1.word = wrd[0];
        w2.word = wrd[1];
        w3.word = wrd[2];
        w4.word = wrd[3];
        w5.word = wrd[4];
        w6.word = wrd[5];
        w7.word = wrd[6];
        w8.word = wrd[7];
        w9.word = wrd[8]; 
        w10.word = wrd[9];



        CRC = concatbin(w9.CRC,w10.CRC,12);

        /* bind reduced almanac packets */
        raw_reduced_almanac red_alm[7]; 
        red_alm[0].packet = concatbin(w2.redAlm1, w3.redAlm1, 26); //packet 1
        red_alm[1].packet = concatbin(w3.redAlm2, w4.redAlm2, 25); //packet 2
        red_alm[2].packet = concatbin(w4.redAlm3, w5.redAlm3, 24); //packet 3
        red_alm[3].packet = concatbin(w5.redAlm4, w6.redAlm4, 23); //packet 4
        red_alm[4].packet = concatbin(w6.redAlm5, w7.redAlm5, 22); //packet 5
        red_alm[5].packet = concatbin(w7.redAlm6, w8.redAlm6, 21); //packet 6
        red_alm[6].packet = concatbin(w8.redAlm7, w9.redAlm7, 20); //packet 7

        /* scale almanac packets */
        for(auto& it: red_alm)
            scale_alm(it);

        TOW = concatbin(w1.TOW, w2.TOW, 5);
        WNan = w2.WNa0_n;
        toa = w2.toa;
        alert = w2.alert;
    }


    void scale_alm(raw_reduced_almanac in)
    {
        reduced_almanac red;
        red.L1 = in.L1;
        red.L2 = in.L2;
        red.L5 = in.L5;
        red.omega_0 = in.omega_0 * pow(2,-6); 
        red.phi_0  = in.phi_0 * pow(2,-6);
        red.sigma_A = in.sigma_A* pow(2,9);
        
        redalm.push_back(red);
    }


};

/*

___________________________________
Message 13 Struct:
    Clock Diff Correction
___________________________________

*/
struct Msg_Type_13 {
    
    typedef union
    {
        struct
        {
            uint32_t  TOW      : 12;
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;

    typedef union
    {
        struct
        {
            uint32_t CDC_1m        : 3;
            uint32_t Dc_1_type     :  1;
            uint32_t tOD          : 11;
            uint32_t topD         : 11;
            uint32_t toe        : 11;
            uint32_t alert      :  1;
            uint32_t TOW       :   5;
        };
        uint32_t word;
    
    } Word2;

    typedef union
    {
        struct
        {
            uint32_t  Dc_2_type     :  1;
            uint32_t CDC_1l        : 31;
           
           
        };
        uint32_t word;
    
    } Word3;

    typedef union
    {
        struct
        {
            uint32_t CDC_2m        : 32;

        };
        uint32_t word;

    } Word4;

    typedef union
    {
        struct
        {
            uint32_t CDC_3m     : 29;
            uint32_t  Dc_3_type     :  1;
            uint32_t CDC_2l     : 2;
        
        };
        uint32_t word;

    } Word5;

    typedef union
    {
        struct
        {
            
            uint32_t CDC_4m     : 26;
            uint32_t  Dc_4_type :  1;
            uint32_t CDC_3l     : 5;
        };
        uint32_t word;

    } Word6;

    typedef union
    {
        struct
        {
            uint32_t CDC_5m     : 23;
            uint32_t  Dc_5_type :  1;
            uint32_t CDC_4l     : 8;

            
        };
        uint32_t word;

    } Word7;

    typedef union
    {
        struct
        {
            uint32_t CDC_6m     : 20;
            uint32_t Dc_6_type  :  1;
            uint32_t CDC_5l     : 11;
        
        
        };
        uint32_t word;
    } Word8;

    typedef union
    {
        struct
        {
            uint32_t CRC        : 12;
            uint32_t reserved    : 6;
            uint32_t CDC_6m     : 14;
        };
        uint32_t word;
    } Word9;

    typedef union
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;
    } Word10;

    typedef struct{

        int16_t daf0;
        int16_t daf1;
        int8_t UDRA;
        uint8_t prn;
        uint8_t type;

    } CDC_scaled;

    Word1   w1;
    Word2   w2;
    Word3   w3;
    Word4   w4;
    Word5   w5;
    Word6   w6;
    Word7   w7;
    Word8   w8;
    Word9   w9;
    Word10 w10;

    std::vector<CDC_scaled> ClockDifs;
    uint32_t TOW;
    uint32_t CRC;

    void decode(uint32_t* wrd)
    {
        w1.word = wrd[0];
        w2.word = wrd[1];
        w3.word = wrd[2];
        w4.word = wrd[3];
        w5.word = wrd[4];
        w6.word = wrd[5];
        w7.word = wrd[6];
        w8.word = wrd[7];
        w9.word = wrd[8];
        w10.word = wrd[9];

        /* bind clockdifs */
        cdc clockdif_correction[7];
        clockdif_correction[0].word = concatbin(w2.CDC_1m, w3.CDC_1l, 31); //packet 1
        clockdif_correction[1].word = concatbin(w4.CDC_2m, w5.CDC_2l, 2);  //packet 2
        clockdif_correction[2].word = concatbin(w5.CDC_3m, w6.CDC_3l, 5);  //packet 3
        clockdif_correction[3].word = concatbin(w6.CDC_4m, w7.CDC_4l, 8);  //packet 4
        clockdif_correction[4].word = concatbin(w7.CDC_5m, w8.CDC_5l, 11); //packet 5
        clockdif_correction[5].word = concatbin(w8.CDC_6m, w9.CDC_6m, 14); //packet 6

        /* scale clock corrections */
        for(auto& it: clockdif_correction)
        {
            CDC_scaled temp;
            temp.daf1 = it.daf1 * P2_51;
            temp.daf0 = it.daf0 * P2_35;
            temp.prn = it.prn;
            temp.UDRA = it.UDRA;

            ClockDifs.push_back(temp);

        }

        ClockDifs[0].type = w2.Dc_1_type;
        ClockDifs[1].type = w3.Dc_2_type;
        ClockDifs[2].type = w5.Dc_3_type;
        ClockDifs[3].type = w6.Dc_4_type;
        ClockDifs[4].type = w7.Dc_5_type;
        ClockDifs[5].type = w8.Dc_6_type;

        TOW = concatbin(w1.TOW, w2.TOW, 5);
        CRC = concatbin(w9.CRC,w10.CRC,12);


    }

};

/* 
___________________________________
   Message 14 Struct:
        Ephemeris Diff Correction
___________________________________

*/
struct Msg_Type_14 {
    
    typedef union
    {
        struct
        {
            
            uint32_t  TOW      : 12; 
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
        };
        uint32_t word;

    } Word1;

    typedef union 
    {
        struct
        {
            uint32_t EDC_msb         :3 ; 
            uint32_t Dc_1_type     :  1; 
            uint32_t tOD          : 11; 
            uint32_t topD         : 11;
            uint32_t alert      :  1;
            uint32_t TOW       :   5; 
        
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t EDC_msb0         :32;          
        };
        uint32_t word;
    
    } Word3;

    typedef union 
    {
        struct
        {
            uint32_t EDC_msb1         :32;          


        };
        uint32_t word;

    } Word4;

    typedef union 
    {
        struct
        {    
            uint32_t EDC_msb        : 6; 
            uint32_t Dc_2_type     :  1; 
            uint32_t EDC_lsb         :25;          

        };
        uint32_t word;

    } Word5;

    typedef union 
    {
        struct
        {
            uint32_t EDC_msb0         ;  
            
        };
        uint32_t word;

    } Word6;

    typedef union 
    {
        struct
        {
            uint32_t EDC_msb1         ;  
            
        };
        uint32_t word;

    } Word7;

    typedef union 
    {
        struct
        {
            uint32_t reserved  : 10;
            uint32_t EDC_lsb   :22 ;  

        };
        uint32_t word;

    } Word8;

    typedef union 
    {
        struct
        {
            uint32_t CRC        : 12;
            uint32_t reserved    : 20;
        };
        uint32_t word;

    } Word9;

    typedef union 
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;

    } Word10;

    struct EDC
            {
        uint8_t prn;
        int16_t delalph;
        int16_t delbeta;
        int16_t delgamm;
        int16_t deli;
        int16_t delomg;
        int16_t delA;
        int8_t UDRAdot;

            };

        Word1   w1;
        Word2   w2;
        Word3   w3;
        Word4   w4;
        Word5   w5;
        Word6   w6;
        Word7   w7;
        Word8   w8;
        Word9   w9;
        Word10 w10;

        uint32_t TOW;
        uint32_t CRC;

        std::vector<EDC> ephdif_corrections;

    void decode(uint32_t* wrd)
    {
        w1.word = wrd[0];
        w2.word = wrd[1];
        w3.word = wrd[2];
        w4.word = wrd[3];
        w5.word = wrd[4];
        w6.word = wrd[5];
        w7.word = wrd[6];
        w8.word = wrd[7];
        w9.word = wrd[8];
        w10.word = wrd[9];

        TOW = concatbin(w1.TOW, w2.TOW, 5);
        CRC = concatbin(w9.CRC,w10.CRC,12);

        EDC edc_1, edc_2;

        edc_1.prn = concatbin(w2.EDC_msb, extractbit(w3.EDC_msb0,0,4), 5);
        edc_1.delalph = extractbit(w3.EDC_msb0, 5, 18) * P2_34;
        edc_1.delbeta = concatbin(extractbit(w3.EDC_msb0, 19, 31), extractbit(w4.EDC_msb1, 0, 0), 1) * P2_34;
        edc_1.delgamm = extractbit(w4.EDC_msb1, 1, 15) * P2_32;
        edc_1.deli = extractbit(w4.EDC_msb1, 16, 27) * P2_32;
        edc_1.delomg = concatbin(extractbit(w4.EDC_msb1, 28, 31), extractbit(w5.EDC_lsb, 0, 7), 8) * P2_32;
        edc_1.delA = extractbit(w5.EDC_lsb, 8, 19) * P2_9;
        edc_1.UDRAdot = extractbit(w5.EDC_lsb, 20, 24);

        ephdif_corrections.push_back(edc_1);

        edc_1.prn = concatbin(w5.EDC_msb, extractbit(w6.EDC_msb0, 0, 1), 2);
        edc_1.delalph = extractbit(w6.EDC_msb0, 2, 15) * P2_34;
        edc_1.delbeta = extractbit(w6.EDC_msb0, 16, 29) * P2_34;
        edc_1.delgamm = concatbin(extractbit(w6.EDC_msb0, 30, 31), extractbit(w7.EDC_msb1, 0, 12), 13) * P2_32;
        edc_1.deli = extractbit(w7.EDC_msb1, 13, 24) * P2_32;
        edc_1.delomg = concatbin(extractbit(w7.EDC_msb1, 25, 31), extractbit(w8.EDC_lsb, 0, 4), 5) * P2_32;
        edc_1.delA = extractbit(w8.EDC_lsb, 5, 16) * P2_9;
        edc_1.UDRAdot = extractbit(w8.EDC_lsb, 17, 21);

        ephdif_corrections.push_back(edc_2);

    }
    
};

/* 
___________________________________
   Message 15 Struct:
       Text
___________________________________

*/
typedef struct {
    
    typedef union
    {
        struct
        {
            uint32_t  TOW      : 12; 
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;

    typedef union 
    {
        struct
        {
            uint32_t EDC_msb         :3 ; 
            uint32_t Dc_1_type     :  1; 
            uint32_t tOD          : 11; 
            uint32_t topD         : 11;
            uint32_t alert      :  1;
            uint32_t TOW       :   5; 
        
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t EDC_msb0         :32;          
        };
        uint32_t word;
    
    } Word3;
    typedef union 
    {
        struct
        {
            uint32_t EDC_msb1         :32;          
        };
        uint32_t word;

    } Word4;
    typedef union 
    {
        struct
        {    
            uint32_t EDC_msb        : 6; 
            uint32_t Dc_2_type     :  1; 
            uint32_t EDC_lsb         :25;          

        };
        uint32_t word;

    } Word5;
    typedef union 
    {
        struct
        {
            uint32_t EDC_msb0         ;  
            
        };
        uint32_t word;

    } Word6;
    typedef union 
    {
        struct
        {
            uint32_t EDC_msb1         ;  
            
        };
        uint32_t word;

    } Word7;
    typedef union 
    {
        struct
        {
            uint32_t reserved  : 10;
            uint32_t EDC_lsb   :22 ;  

        };
        uint32_t word;
    } Word8;
    typedef union 
    {
        struct
        {
            uint32_t CRC        : 12;
            uint32_t reserved    : 20;
        };
        uint32_t word;
    } Word9;
    typedef union 
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;
    } Word10;

    Word1   w1;
    Word2   w2;
    Word3   w3;
    Word4   w4;
    Word5   w5;
    Word6   w6;
    Word7   w7;
    Word8   w8;
    Word9   w9;
    Word10 w10;

    //parameter decoding not added

} Msg_Type_15;


/* 
__________________________________________________
   Message 30 Struct:
        Clock, IONO, Group Delay 
__________________________________________________

*/
typedef class {
    
    typedef union
    {
        struct
        {
            uint32_t TOW      : 12; 
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;

    typedef union 
    {
        struct
        {
            uint32_t toc       : 4;
            uint32_t URANED2   :  3;
            uint32_t URANED1   :  3;
            uint32_t URANED0   :  5;
            uint32_t top       : 11;
            uint32_t alert      :  1;
            uint32_t TOW       :  5; 
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t af0n      : 25;
            uint32_t toc       : 7;
        
        };
        uint32_t word;
    
    } Word3;

    typedef union 
    {
        struct
        {
            uint32_t TGD       : 1;                                  
            uint32_t af2n      : 10;
            uint32_t af1n      : 20;
            uint32_t af0n      : 1;
        };
        uint32_t word;

    } Word4;

    typedef union 
    {
        struct
        {   
            uint32_t ISCL2C    : 7;
            uint32_t ISCL1CA   : 13;
            uint32_t TGD       :  11;
            
            
        };
        uint32_t word;

    } Word5;

    typedef union 
    {
        struct
        {

            uint32_t ISCL5Q5   : 13;
            uint32_t ISCL5I5   : 13;
            uint32_t ISCL2C    : 6;
        };
        uint32_t word;

    } Word6;

    typedef union 
    {
        struct
        {
            uint32_t a3 :8;
            uint32_t a2 :8;
            uint32_t a1 :8;
            uint32_t a0 :8;
            
        };
        uint32_t word;

    } Word7;

    typedef union 
    {
        struct
        {
            uint32_t b3 :8;
            uint32_t b2 :8;
            uint32_t b1 :8;
            uint32_t b0 :8;
            
        };
        uint32_t word;

    } Word8;

    typedef union 
    {
        struct
        {
            uint32_t CRC        : 12;
            uint32_t reserved   : 12; 
            uint32_t WnOP       : 8 ;
        };
        uint32_t word;

    } Word9;

    typedef union 
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;

    } Word10;

        Word1   w1;
        Word2   w2;
        Word3   w3;
        Word4   w4;
        Word5   w5;
        Word6   w6;
        Word7   w7;
        Word8   w8;
        Word9   w9;
        Word10 w10;

    public:

        uint32_t CRC;
        long double TGD;
        long double af0;
        long double af1;
        long double af2;
        uint32_t toc;

        void decode(uint32_t* wrd)
        {
            w1.word = wrd[0];
            w2.word = wrd[1];
            w3.word = wrd[2];
            w4.word = wrd[3];
            w5.word = wrd[4];
            w6.word = wrd[5];
            w7.word = wrd[6];
            w8.word = wrd[7];
            w9.word = wrd[8];
            w10.word = wrd[9];

            CRC = concatbin(w9.CRC, w10.CRC, 12);
            TGD = concatbin(w4.TGD, w5.TGD, 12) * P2_35;
            af0 = concatbin_signed_32(w3.af0n,w4.af0n,25, 1) * P2_35;
            af1 = concatbin_signed_32(w4.af1n,0,20,0) * P2_48;
            af2 = concatbin_signed_32(w4.af2n,0,10,0) * P2_60;
            toc = concatbin(w2.toc, w3.toc, 7) * 300 ;

        }

} Msg_Type_30;

    


/* 
___________________________________
   Message 31 Struct:
        Clock, Reduced Almanac 
___________________________________

*/ 

typedef struct {
    
    typedef union
    {
        struct
        {
            uint32_t  TOW      : 12; 
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;

    typedef union 
    {
        struct
        {
            uint32_t toc       : 4;
            uint32_t URANED2   :  3;
            uint32_t URANED1   :  3;
            uint32_t URANED0   :  5;
            uint32_t top       : 11;
            uint32_t alert      :  1;
            uint32_t TOW       :  5; 
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
        
            uint32_t af0n      : 25;
            uint32_t toc       : 11;
        };
        uint32_t word;
    
    } Word3;

    typedef union 
    {
        struct
        {
            uint32_t WNan      : 1; 
            uint32_t af2n      : 10;
            uint32_t af1n      : 20;
            uint32_t af0n      : 1;
        };
        uint32_t word;

    } Word4;

    typedef union 
    {
        struct
        {   
            uint32_t redAlm1   : 12; 
            uint32_t toa       :  8; 
            uint32_t WNan      : 13; 
            
            
        };
        uint32_t word;

    } Word5;

    typedef union 
    {
        struct
        {
            uint32_t redAlm2   : 13; 
            uint32_t redAlm1   : 19; 
            
        };
        uint32_t word;

    } Word6;

    typedef union 
    {
        struct
        {
            uint32_t redAlm3   : 14;             
            uint32_t redAlm2   : 18;
            
        };
        uint32_t word;

    } Word7;

    typedef union 
    {
        struct
        {
            uint32_t redAlm4   : 15;       
            uint32_t redAlm3   : 17;
            
            
        };
        uint32_t word;

    } Word8;

    typedef union 
    {
        struct
        {
            uint32_t CRC       : 12;
            uint32_t reserved  : 4; 
            uint32_t redAlm4   : 16;       
        };
        uint32_t word;

    } Word9;

    typedef union 
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;

    } Word10;

    Word1   w1;
    Word2   w2;
    Word3   w3;
    Word4   w4;
    Word5   w5;
    Word6   w6;
    Word7   w7;
    Word8   w8;
    Word9   w9;
    Word10 w10;
    
} Msg_Type_31;
 

/* 
___________________________________
   Message 32 Struct:
        Clock, EOP 
___________________________________

*/

typedef struct {
    
    typedef union
    {
        struct
        {
            uint32_t  TOW      : 12; 
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;

    typedef union 
    {
        struct
        {
            uint32_t toc       : 4;
            uint32_t URANED2   :  3;
            uint32_t URANED1   :  3;
            uint32_t URANED0   :  5;
            uint32_t top       : 11;
            uint32_t alert      :  1;
            uint32_t TOW       :  5; 
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t af0n      : 25;
            uint32_t toc       : 11;
        
        };
        uint32_t word;

    } Word3;

    typedef union 
    {
        struct
        {
            uint32_t tEOP      : 1; 
            uint32_t af2n      : 10;
            uint32_t af1n      : 20;
            uint32_t af0n      : 1;
        };
        uint32_t word;

    } Word4;

    typedef union 
    {
        struct
        {   
            uint32_t PM_X         : 17;                 
            uint32_t tEOP         : 15; 
        };
        uint32_t word;

    } Word5;

    typedef union 
    {
        struct
        {
            uint32_t PM_Xdot      : 15; 
            uint32_t PM_X         : 4; 
        };
        uint32_t word;

    } Word6;

    typedef union
    {
        struct
        {
            uint32_t deltaUTGPS   : 9;
            uint32_t PM_Ydot      : 15;
            uint32_t PM_Y         : 8; 
        
        };
        uint32_t word;

    } Word7;

    typedef union 
    {
        struct
        {
            uint32_t deltaUTGPSdot: 10;      
            uint32_t deltaUTGPS   : 22;
            
            
        };
        uint32_t word;
    } Word8;

    typedef union 
    {
        struct
        {
            uint32_t CRC       : 12;
            uint32_t reserved  : 11; 
            uint32_t deltaUTGPSdot: 9;      
        };
        uint32_t word;
    } Word9;

    typedef union 
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;
    } Word10;

        Word1   w1;
        Word2   w2;
        Word3   w3;
        Word4   w4;
        Word5   w5;
        Word6   w6;
        Word7   w7;
        Word8   w8;
        Word9   w9;
        Word10 w10;

    public:

        uint32_t CRC;
        long double af0;
        long double af1;
        long double af2;
        long double URANED0;
        long double URANED1;
        long double URANED2;

        void decode(uint32_t* wrd)
        {
            w1.word  = wrd[0];
            w2.word  = wrd[1];
            w3.word  = wrd[2];
            w4.word  = wrd[3];
            w5.word  = wrd[4];
            w6.word  = wrd[5];
            w7.word  = wrd[6];
            w8.word  = wrd[7];
            w9.word  = wrd[8];
            w10.word = wrd[9];

            CRC = concatbin(w9.CRC, w10.CRC, 12);
            af0 = concatbin(w3.af0n, w4.af0n, 1);

            af1 = w4.af1n * P2_48;
            af2 = w4.af2n * P2_60;
            af0 *= P2_35;

            URANED0 = w2.URANED0;
            URANED1 = w2.URANED1;
            URANED2 = w2.URANED2;

        }

} Msg_Type_32;


/* 
___________________________________
   Message 33 Struct:
        Clock, UTC 
___________________________________

*/
typedef struct {
    
    typedef union
    {
        struct
        {
            uint32_t TOW       : 12; 
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;

    typedef union 
    {
        struct
        {
            uint32_t toc       :  4;
            uint32_t URANED2   :  3;
            uint32_t URANED1   :  3;
            uint32_t URANED0   :  5;
            uint32_t top       : 11;
            uint32_t alert     :  1;
            uint32_t TOW       :  5; 
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t af0n      : 25;
            uint32_t toc       : 11;
        
        };
        uint32_t word;
    
    } Word3;

    typedef union 
    {
        struct
        {
            uint32_t A0n       :  1; 
            uint32_t af2n      : 10;
            uint32_t af1n      : 20;
            uint32_t af0n      :  1;
        };
        uint32_t word;

    } Word4;

    typedef union 
    {
        struct
        {   
            uint32_t A2n          :  4; 
            uint32_t A1n          : 13;
            uint32_t A0n          : 15;
                          
        };
        uint32_t word;

    } Word5;

    typedef union 
    {
        struct
        {
            uint32_t WNot         :  5;        
            uint32_t tot          : 16;
            uint32_t  deltatLS    :  8;
            uint32_t A2n          :  3; 
        };
        uint32_t word;

    } Word6;

    typedef union 
    {
        struct
        {
            uint32_t deltatLSF    :  7;
            uint32_t  DN          :  4;
            uint32_t WNLSF        : 13;
            uint32_t WNot         :  8;
        };
        uint32_t word;

    } Word7;

    typedef union 
    {
        struct
        {
            uint32_t reserved     : 31;            
            uint32_t deltatLSF    :  1;
            
        };
        uint32_t word;

    } Word8;
    
    typedef union 
    {
        struct
        {
            uint32_t CRC       : 12;
            uint32_t reserved  : 20; 
        };
        uint32_t word;

    } Word9;

    typedef union 
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;

    } Word10;

        Word1   w1;
        Word2   w2;
        Word3   w3;
        Word4   w4;
        Word5   w5;
        Word6   w6;
        Word7   w7;
        Word8   w8;
        Word9   w9;
        Word10 w10;


    public:

        uint32_t CRC;
        long double af0;
        long double af1;
        long double af2;

        void decode(uint32_t* wrd)
        {
            w1.word  = wrd[0];
            w2.word  = wrd[1];
            w3.word  = wrd[2];
            w4.word  = wrd[3];
            w5.word  = wrd[4];
            w6.word  = wrd[5];
            w7.word  = wrd[6];
            w8.word  = wrd[7];
            w9.word  = wrd[8];
            w10.word = wrd[9];

            CRC = concatbin(w9.CRC, w10.CRC, 12);
            af0 = concatbin(w3.af0n,w4.af0n,1) * P2_35;
            af1 = w4.af1n * P2_48;
            af2 = w4.af2n * P2_60;
        }

} Msg_Type_33;

/* 
___________________________________
   Message 34 Struct:
        Clock & Diff Correction
___________________________________

*/
typedef struct {
    
    typedef union
    {
        struct
        {
            uint32_t TOW       : 12; 
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;

    typedef union 
    {
        struct
        {
            uint32_t toc       :  4;
            uint32_t URANED2   :  3;
            uint32_t URANED1   :  3;
            uint32_t URANED0   :  5;
            uint32_t top       : 11;
            uint32_t alert     :  1;
            uint32_t TOW       :  5; 
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t af0n      : 25;
            uint32_t toc       : 11;
        
        };
        uint32_t word;
    
    } Word3;
    typedef union 
    {
        struct
        {
            uint32_t topD      : 1;
            uint32_t af2n      : 10;
            uint32_t af1n      : 20;
            uint32_t af0n      : 1;
        };
        uint32_t word;

    } Word4;

    typedef union 
    {
        struct
        {   
            uint32_t CDC_1        : 10;                                      
            uint32_t Dc_1_type    :  1;
            uint32_t tOD          : 11;
            uint32_t topD         : 10;
        };
        uint32_t word;

    } Word5;

    typedef union 
    {
        struct
        {
            uint32_t EDCmsb0      :  8; 
            uint32_t CDC_1        : 24;
    
        };
        uint32_t word;

    } Word6;

    typedef union 
    {
        struct
        {
            uint32_t EDCmsb1;                             
        };
        uint32_t word;

    } Word7;

    typedef union 
    {
        struct
        {
            uint32_t EDCmsb2;                                       
            
        };
        uint32_t word;

    } Word8;

    typedef union 
    {
        struct
        {
            uint32_t CRC       : 12;
            uint32_t EDClsb    : 20;                                       
        };
        uint32_t word;

    } Word9;

    typedef union 
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;

    } Word10;


    Word1   w1;
    Word2   w2;
    Word3   w3;
    Word4   w4;
    Word5   w5;
    Word6   w6;
    Word7   w7;
    Word8   w8;
    Word9   w9;
    Word10 w10;
    
    cdc ClockDifCor;

    uint32_t CRC;
    int16_t daf0;
    int16_t daf1;
    int8_t UDRA;
    uint8_t prn;

    /* which satellite is subject to cdc */
    void decode(uint32_t* wrd)
    {
        w1.word  = wrd[0];
        w2.word  = wrd[1];
        w3.word  = wrd[2];
        w4.word  = wrd[3];
        w5.word  = wrd[4];
        w6.word  = wrd[5];
        w7.word  = wrd[6];
        w8.word  = wrd[7];
        w9.word  = wrd[8]; 
        w10.word = wrd[9];
        
        ClockDifCor.word = concatbin(w5.CDC_1, w5.CDC_1, 24);
        CRC = concatbin(w9.CRC, w10.CRC, 12);
        prn = ClockDifCor.prn;
        daf0 = ClockDifCor.daf0 * P2_35;
        daf1 = ClockDifCor.daf1 * P2_51;


    }


    
    
} Msg_Type_34;

/* 
___________________________________
   Message 35 Struct:
        Clock & GGTO
___________________________________

*/
typedef struct {
    
    typedef union
    {
        struct
        {
            uint32_t  TOW      : 12; 
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;

    typedef union 
    {
        struct
        {
            uint32_t toc       : 4;
            uint32_t URANED2   :  3;
            uint32_t URANED1   :  3;
            uint32_t URANED0   :  5;
            uint32_t top       : 11;
            uint32_t alert      :  1;
            uint32_t TOW       :  5; 
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t af0n      : 25;
            uint32_t toc       : 11;
        
        };
        uint32_t word;
    
    } Word3;
    typedef union 
    {
        struct
        {
            uint32_t tGGTO     : 1; 
            uint32_t af2n      : 10;
            uint32_t af1n      : 20;
            uint32_t af0n      : 1;
        };
        uint32_t word;

    } Word4;
    typedef union 
    {
        struct
        {   
            uint32_t A0GGTO         :1;               
            uint32_t GNSSID        : 3;
            uint32_t WNGGTO       : 13;
            uint32_t tGGTO        : 15;
        };
        uint32_t word;

    } Word5;
    typedef union 
    {
        struct
        {
            uint32_t A2GGTO        : 4;
            uint32_t A1GGTO       : 13;
            uint32_t A0GGTO        : 15;
             
    
        };
        uint32_t word;

    } Word6;
    typedef union 
    {
        struct
        {
            uint32_t reservedmsb0   : 29; 
            uint32_t A2GGTO        : 3;
 
        };
        uint32_t word;

    } Word7;
    typedef union 
    {
        struct
        {
            uint32_t reservedmsb1; 
            
        };
        uint32_t word;
    } Word8;
    typedef union 
    {
        struct
        {
            uint32_t CRC       : 12;
            uint32_t reservedlsb : 20; 
        };
        uint32_t word;
    } Word9;
    typedef union 
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;
    } Word10;

    Word1   w1;
    Word2   w2;
    Word3   w3;
    Word4   w4;
    Word5   w5;
    Word6   w6;
    Word7   w7;
    Word8   w8;
    Word9   w9;
    Word10 w10;
    
} Msg_Type_35;

/* 
___________________________________
   Message 36 Struct:
        Clock & Text
___________________________________

*/
typedef struct {
    
    typedef union
    {
        struct
        {
            uint32_t  TOW      : 12; 
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;

    typedef union 
    {
        struct
        {
            uint32_t toc       : 4;
            uint32_t URANED2   :  3;
            uint32_t URANED1   :  3;
            uint32_t URANED0   :  5;
            uint32_t top       : 11;
            uint32_t alert      :  1;
            uint32_t TOW       :  5; 
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t af0n      : 25;
            uint32_t toc       : 11;
        
        };
        uint32_t word;
    
    } Word3;
    typedef union 
    {
        struct
        {
            uint32_t txt     : 1; 
            uint32_t af2n      : 10;
            uint32_t af1n      : 20;
            uint32_t af0n      : 1;
        };
        uint32_t word;

    } Word4;
    typedef union 
    {
        struct
        {   
            char txt[4];
        };
        uint32_t word;

    } Word5;
    typedef union 
    {
        struct
        {
            char txt[4];

        };
        uint32_t word;

    } Word6;
    typedef union 
    {
        struct
        {          
            char txt[4];                        
 
        };
        uint32_t word;

    } Word7;
    typedef union 
    {
        struct
        {
            char txt[4];
            
        };
        uint32_t word;
    } Word8;
    typedef union 
    {
        struct
        {
            uint32_t CRC       : 12;
            uint32_t  pg       :  4;
            char txt[2]; 
        };
        uint32_t word;
    } Word9;
    typedef union 
    {
        struct
        {
            uint32_t pad        : 20;
            uint32_t CRC        : 12;
        };
        uint32_t word;
    } Word10;

    Word1   w1;
    Word2   w2;
    Word3   w3;
    Word4   w4;
    Word5   w5;
    Word6   w6;
    Word7   w7;
    Word8   w8;
    Word9   w9;
    Word10 w10;
    
} Msg_Type_36;

/* 
___________________________________
   Message 37 Struct:
        Clock & Midi Almanac
___________________________________

*/
typedef struct {
    
    typedef union
    {
        struct
        {
            uint32_t  TOW      : 12; //12: w1 5:w
            uint32_t msgTypeId :  6;
            uint32_t PRN       :  6;
            uint32_t preamble  :  8;
            
        };
        uint32_t word;

    } Word1;

    typedef union 
    {
        struct
        {
            uint32_t toc       : 4;
            uint32_t URANED2   :  3;
            uint32_t URANED1   :  3;
            uint32_t URANED0   :  5;
            uint32_t top       : 11;
            uint32_t alert      :  1;
            uint32_t TOW       :  5; //12: w1 5:w
        };
        uint32_t word;
    
    } Word2;

    typedef union 
    {
        struct
        {
            uint32_t af0n      : 25;
            uint32_t toc       : 11;
        
        };
        uint32_t word;
    
    } Word3;
    typedef union 
    {
        struct
        {
            uint32_t Wnan      : 1; 
            uint32_t af2n      : 10;
            uint32_t af1n      : 20;
            uint32_t af0n      : 1;
        };
        uint32_t word;

    } Word4;
    typedef union 
    {
        struct
        {   
            uint32_t e            : 3; 
            uint32_t L15         :1;
            uint32_t L2         :1;          
            uint32_t L1         :1;  
            uint32_t PRNa        : 16;
            uint32_t tob       : 8;
            uint32_t Wnan      : 12; 


        };
        uint32_t word;

    } Word5;
    typedef union 
    {
        struct
        {
            uint32_t Asqrt        : 2;
            uint32_t omegadot        : 11;
            uint32_t delta       : 11;
            uint32_t e            : 8;
    
        };
        uint32_t word;

    } Word6;

    typedef union 
    {
        struct
        {
            uint32_t W          : 1;
            uint32_t omegadot   : 16; 
            uint32_t Asqrt        : 15;
        };
        uint32_t word;

    } Word7;
    
    typedef union 
    {
        struct
        {
            uint32_t af0          : 1;
            uint32_t M0          : 16; 
            uint32_t W           : 15;
        };
        uint32_t word;
    } Word8;
    typedef union 
    {
        struct
        {
            uint32_t CRC       : 12;
            uint32_t af1          : 10;
            uint32_t af0          : 10;
        };
        uint32_t word;
    } Word9;
    typedef union 
    {
        struct
        {
            uint32_t CRC        : 12;
            uint32_t pad        : 20;
        };
        uint32_t word;
    } Word10;

    Word1   w1;
    Word2   w2;
    Word3   w3;
    Word4   w4;
    Word5   w5;
    Word6   w6;
    Word7   w7;
    Word8   w8;
    Word9   w9;
    Word10 w10;
    
} Msg_Type_37;


#endif         


