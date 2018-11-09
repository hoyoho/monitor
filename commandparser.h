#include <string>
#include <vector>
#include "parameter.h"

class CommandParser
{
    private:
        enum {ASK_HELP = 0};
        unsigned int stageCount;
        std::vector<Parameter> vParameter; 
        void printInfo(int info);
        int createStagePara(string cmd);
    public:
        
        CommandParser(int argc, char* argv[]);
        Parameter& getParameter(int i );
        unsigned int getStageCount();
        std::vector<Parameter> vStageParameter;
};
