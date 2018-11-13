#ifndef PARAMETER_H
#define PARAMETER_H
#include <vector>
#include <string>
/*----------------------------------------------

----------------------------------------------- */
const unsigned POSITIVE     =   0x00000001;
const unsigned NEGITIVE     =   0x00000002;
const unsigned PKT_COUNT    =   0x00000004;
const unsigned GAP_VALUE    =   0x00000008;
const unsigned SERIAL_NUM   =   0x00000010;
const unsigned BITMAP       =   0x00000020;
const unsigned FUNCTION     =   0x00000040;
const unsigned PKT_DROP     =   0x00000080;
const unsigned PKT_REPEAT   =   0x00000100;
const unsigned PKT_DELAY    =   0x00000200;
const unsigned PROTOCOL     =   0x00000400;



class Parameter
{
    public:
        unsigned int    flag;
        std::string     src;
        std::string     dst;
        unsigned int    gap;
        unsigned int    stageId;
        std::vector<unsigned int> choSequence;
};

#endif

