
/*********************************** UBX-RXM-SFRBX
 *
 *   Broadcast messages found from file
 *   Gps l2 band are extracted
 *   Decoded according to Cnav (IS GPS 200L)
 *   Contained for each satellite in a class
 *   Ephemeris data is gathered in a struct
 *   Written to file with rinex 3.04 format
 *
 *   @author : Berru Karakas
 *   @info : karakasb19@itu.edu.tr
 *
 *
 ************************************************/



#ifndef GPS_DEC_H
#define GPS_DEC_H

#include <fstream>
#include <iostream>
#include <vector>

#include "gps_l2_satellite.h"
#include "bit.h" 
#include "binaryfile.h"
#include "crc24q.h"


#define H1        0xb5       /*Header 1*/
#define H2        0x62       /*Header 2*/
#define BRD_CLASS 0x02       /*Class*/
#define BRD_ID    0x13       /*ID*/
#define GNSS_ID   0          /* GPS */
#define PRE       0b10001011 /*Preamble for gps 139*/
#define signal    0          /* 4 for L2 */
#define LENGTH    48         /* 4*10 + 8 */


/* reads from binary file
 *
 * @param binary filestream
 * @param reference to variable
 */
template <class T>
void read_from_file(std::ifstream& fileStream, T& variable)
{
    fileStream.read((char*) &variable, sizeof(variable));
}

/* 
* Read 10 words
* @param wrd: 32 bit 10 words 
  @param ck: checksum struct to calculate
*/
void Satellite::read_words(uint32_t* wrd, 
                         std::ifstream& binfile)
{
    for(int i=0; i<10; i++)
        read_from_file(binfile, wrd[i]);

}

/*
* end of msg sumchecks
* @param checksum: calculated checksums
*/
void calculate(ck& checksum, uint8_t data)
{
    checksum.ck_a += data;
    checksum.ck_b += checksum.ck_a;
    checksum.ck_a & 0xFF;
    checksum.ck_b & 0xFF;

} 

/*
 * calculate checksum over message
 * @return true or false
 */
bool Satellite::check_sum(uint32_t* words)
{
    /* reading post payload */

    ck checksum;

        calculate(checksum,data->msgClass); 
        calculate(checksum,data->msgID);    
        calculate(checksum,data->length);  

        calculate(checksum,data->gnssId);   
        calculate(checksum,data->svId);     
        calculate(checksum,data->reserved0);
        calculate(checksum,data->freqId);   
        calculate(checksum,data->numWords); 
        calculate(checksum,data->chn);      
        calculate(checksum,data->version);  
        calculate(checksum,data->reserved1);

        for(int i=0; i<10; i++)
            for(int j=0; j<4; j++)
                calculate(checksum, extractbit(words[i], j*8, (j+1)*8 - 1 ));
  
  
    //std::cout << "calculated checks" << (unsigned int)checksum.ck_a 
    //<< " " << (unsigned int)checksum.ck_b << std::endl;

    //std::cout << "CK_A" << (unsigned int)this->data->CK_A <<
    //"CK_B" << (unsigned int)this->data->CK_B  << std::endl;
    
    /* check summations */
    return (this->data->CK_A == checksum.ck_a);
            //&& this->data->CK_B == checksum.ck_b);
}

/*
* Decode CNAV structure message
* @param file_name: binary file
*/
void Satellite::decode_gps_l2c(std::ifstream& binfile){

    /* read words */ 
    uint32_t dwrd[10]; //as words

    read_words(dwrd, binfile);

    common C;
    C.word = dwrd[0];

    std::cout <<  "Msg ID" << (unsigned)C.msgTypeId << std::endl;

    read_from_file(binfile, data->CK_A);
    read_from_file(binfile, data->CK_B);

    /* check sums */
    if(check_sum(dwrd))
    {
        int ID = C.msgTypeId;
        
        /*decode unique message type*/
        switch(ID)
        {
            case 10 : dec_msg10(dwrd); break;
            case 11 : dec_msg11(dwrd); break; 
            case 12 : dec_msg12(dwrd); break; 
            case 13 : dec_msg13(dwrd); break;
            case 14 : dec_msg14(dwrd); break;
            case 30 : dec_msg30(dwrd); break; 
            case 31 : dec_msg31(dwrd); break;
            case 32 : dec_msg32(dwrd); break;
            case 33 : dec_msg33(dwrd); break;
            case 34 : dec_msg34(dwrd); break;
            case 35 : dec_msg35(dwrd); break;
            case 36 : dec_msg36(dwrd); break;
            case 37 : dec_msg37(dwrd); break;
        };
    
    }
    else std::cout << "Checksums do not hold" << std::endl;


}

/*
* finds index of broadcast messages
* & decode all gps messages from binary file
* @param file_name: binary file
*/
void SatelliteFile::gps_file(std::string& file_name){

     /* file operations */
    open_binary(binfile, file_name);
 
    /* find and decode gps messages in file */   
    //while(binfile.good() && !binfile.eof()){
    for(int i=0;i<125;i++) {
        find_message();
        std::cout << "found at: " << binfile.tellg() << std::endl;
    }
}

/* 
    UBX-RXM-SFRBX 
    Gnss Identifier: 
    GPS : 0 
    SBAS: 1 
    GAL : 2 
    BDS : 3 
    IMES: 4 
    QZSS: 5 
    GLO : 6
    Finds gps l2 band messages in binary file
        reserved0 : 4 for l2 band
    Decodes message
*/
 bool SatelliteFile::find_message(){

    UbxFrame* py;
    py = new UbxFrame;

    do
    {
        read_from_file(binfile,py->preamble1);


        /* first header */
        if(py->preamble1 == H1){
                read_from_file(binfile,py->preamble2);
                read_from_file(binfile,py->msgClass);
                read_from_file(binfile,py->msgID);

        /* broadcast msg or not */
        if(
            py->preamble2 == H2 &&
            py->msgClass == BRD_CLASS &&
            py->msgID == BRD_ID)
        {           

                read_from_file(binfile,py->length);
                read_from_file(binfile,py->gnssId);
                read_from_file(binfile,py->svId);
                read_from_file(binfile,py->reserved0);
                read_from_file(binfile,py->freqId);
                read_from_file(binfile,py->numWords);
                read_from_file(binfile,py->chn);
                read_from_file(binfile,py->version);
                read_from_file(binfile,py->reserved1);


                /* gps and l2 freq */
                if((int)py->gnssId == GNSS_ID
                     && (int)py->reserved0== signal
                     && py->length == LENGTH) 
                { 
                    std::cout << "Satellite ID: " << (unsigned) py->svId << std::endl;

                    //If satellites first message ever
                    if (!satellite[py->svId]->flag)
                   {
                        satellite[py->svId]->data = py;
                        satellite[py->svId]->decode_gps_l2c(binfile);
                        satellite[py->svId]->flag = true;

                    }
                    else{
                    
                        free(satellite[py->svId]->data);
                        satellite[py->svId]->data = py;
                        satellite[py->svId]->decode_gps_l2c(binfile);
                    }
                    
                    return true;

                }
        }
        }

    } while( binfile.good() && !binfile.eof() );

    free(py);
    return false;
}


#endif