#include <vector>
/*----------------------------------------------

----------------------------------------------- */

class Parameter
{
    private:
        const static int MAX_INTERFACE_LEN = 40;
    public:
        unsigned int    flag;
        char            src[MAX_INTERFACE_LEN];
        char            dst[MAX_INTERFACE_LEN];
        unsigned int    gap;
        unsigned int    stageId;
        std::vector<unsigned int> choSequence;
};

