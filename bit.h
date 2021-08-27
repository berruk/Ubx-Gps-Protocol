/*
* 
* Bit Manipulation Operations
*
* @author Berru
*
*/


#ifndef BIT_H
#define BIT_H

#include <iostream>
#include <bitset> //test purposes

#define bit64(y){ std::bitset<64> c(y);\
std::cout << c << std::endl;}


/*
* get bit at index
*/
bool getbit(const uint32_t& num, int index)
{
    return 1 == ( (num >> index) & 1);
}

/*
* extract bits big endian in interval
*/
uint32_t extractbit(uint32_t M, unsigned start_bit, unsigned end_bit)
{
    unsigned r = 0;
    end_bit++;
    for (unsigned i=32-end_bit; i<32-start_bit; i++)
        r |= 1 << i;

    return (r & M) >> (32 - end_bit);


}

/*
* extract bits little endian in interval
*/
uint32_t extractbit_LE(uint32_t M, unsigned start_bit, unsigned end_bit)
{
    unsigned r = 0;
    for (unsigned i=start_bit; i<end_bit; i++)
        r |= 1 << i;

    return (r & M);

}

/*
* concatenate two binary numbers
* @param length of first number
*/
uint64_t concatbin(uint32_t left, uint32_t right,
                   unsigned int right_length)
                   {
    uint64_t r = 0;
    r = left;
    r = (r << right_length) | right;

    return r;
 }

                   /*
                   * concatenate two binary numbers, 32 bits, signed
                   * /param left number
                   * /param right number
                   * /param length of leftside number
                   * /param length of rightside number
                   */
                   int32_t concatbin_signed_32(uint32_t left, uint32_t right,
                                               unsigned int left_length,
                                               unsigned int right_length)
                                               {
                       int32_t r = 0;
                       uint32_t mask;

                       mask = left;
                       mask = mask >> left_length-1;

                       r = left;
                       r = (r << right_length) | right;

                       if(mask==1){
                           mask = ~0;
                           mask = mask << left_length + right_length;
                           r = mask ^ r;

                       }

                       return r;
                                               }

                                               /*
                                               * concatenate two binary numbers, 64 bits, signed
                                               * /param left number
                                               * /param right number
                                               * /param length of leftside number
                                               * /param length of rightside number
                                               */
                                               int64_t concatbin_signed_64(uint32_t left, uint32_t right,
                                                                           unsigned int left_length,
                                                                           unsigned int right_length)
                                                                           {
                                                   int64_t r = 0;
                                                   uint64_t mask;

                                                   mask = left;
                                                   mask = mask >> left_length-1;

                                                   r = left;
                                                   r = (r << right_length) | right;

                                                   if(mask==1){
                                                       mask = ~0;
                                                       mask = mask << left_length + right_length;
                                                       r = mask ^ r;

                                                   }

                                                   return r;

                                                                           }
#endif