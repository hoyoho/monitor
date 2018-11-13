#include "logger.h"
#include "commandparser.h"
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

using namespace std;
/*-------------------------------------------------------------------------------------------
                                                                            -S "1,2,3,4"      -T
                                                                            |                    |                -p num,num
                                                                            |                    |                -s num
                                                                            |                    |                -a ACK
                                                                            |                    |                -f  0x0F,0x0F
                                                                            |                    |                -w window
                                                                            |                    |                -c
monitor -I ens2f0,ens2f1 -L 1 -P   -C 200 -G 100     -B "001101"      -M  -P tcp    -u                       -N  -L 2
                                                                            |                    |                -t Tser,Tech
                                                                            |                    |                -i 0
                                                                            |                    |                -m
                                                                            |                    |                -o
                                                                            |                    |                -e
                                                                            |                    |                -r remove
                                                                            -F "2x+3"          -D
-------------------------------------------------------------------------------------------*/

//拆分字符串
//接口+  层
           //正反



CommandParser::CommandParser(int argc, char* argv[])
{
    // 初始化logger
    auto console = Logger::getLogger();
    //公共解析部分
    vector<string> command;
    for(vector<string>::size_type i = 0; i < argc; i++)
    {
        command.push_back(string(argv[i]));
    }

    //提取接口
    if(command.size() < 3)//monitor  -I interface,interface
    {
        //这里可以根据用户已经输入的参数多少
        //提供更加精准的帮助内容
        printInfo(ASK_HELP);
        return;
    }
    auto it = command.begin() + 1;
    if(*it != "-I")
    {
        //这里可以根据用户已经输入的参数多少
        //提供更加精准的帮助内容
        printInfo(ASK_HELP);
        return;
    }
    createStagePara(vector<string>(it, it + 2));


    //提取stage
    if(command.size() < 5)//monitor  -I interface,interface -L 1
    {
        //这里可以根据用户已经输入的参数多少
        //提供更加精准的帮助内容
        printInfo(ASK_HELP);
        return;
    }
    it = command.begin() + 3;
    if(*it != "-L")
    {
        //这里可以根据用户已经输入的参数多少
        //提供更加精准的帮助内容
        printInfo(ASK_HELP);
        return;
    }
    auto tailIt = it + 1;
    while(true)
    {
       if(tailIt == command.end())
       {
            createStagePara(vector<string>(it, tailIt));
            break;
       }
       if(*tailIt == "-L")
       {
            createStagePara(vector<string>(it, tailIt));
            it = tailIt;
       }
       ++tailIt;
    }
}


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
        bool validity;
        
};

int CommandParser::createStagePara(const vector<string>& paraCmd)
{

    auto console = Logger::getLogger();
    Parameter* pPara = new Parameter();
    if(paraCmd[0] == "-I")
    {
        strcpy(pPara->src, strtok(paraCmd.c_str(), ", "));
        strcpy(pPara->dst, strtok(NULL, paraCmd.c_str()));
        pPara->stageId = 0;
        vParameter.push_back(*pPara);
    }
    if(paraCmd[0] == "-L")
    {
        pPara->stageId = 0;
        vParameter.push_back(*pPara);
    }



    for(auto it = para.begin(); it != para.end(); ++it)
    {
        console->debug(*it);
    }
    return 0;
}

void CommandParser::printInfo(int info)
{
    switch(info)
    {
        case ASK_HELP:
            cout <<"                                                     -S \"1,2,3,4\"         \t-T 1\n"
                 <<"                                                                            \t|          ________ -p num,num\n"
                 <<"                                                                            \t|          |        -s num\n"
                 <<"                                                                            \t|          |        -a ACK\n"
                 <<"                                                                            \t|          |        -f  0x0F,0x0F\n"
                 <<"                                                                            \t|          |        -w window\n"
                 <<"                                                                            \t|          |        -c\n"
                 <<"monitor -I ens2f0,ens2f1 -L 1 -P   -C 200 -G 100     -B \"001101\"          \t-M      -P tcp      -u               -N ...   -L 2 ...\n"
                 <<"                                                                            \t|          |        -t Tser,Tech\n"
                 <<"                                                                            \t|          |        -i 0\n"
                 <<"                                                                            \t|          |        -m\n"
                 <<"                                                                            \t|          |        -o\n"
                 <<"                                                                            \t|          |_______ -e\n"
                 <<"                                                                            \t-R 1              -r remove\n"
                 <<"                                                     -F \"2x+3\"            \t-D\n"
                 <<"         interface       layer      count                                   \tDrop/Repeat/Modify/Timedelay\n"
                 <<"                              direction    gap       Function/Sequence/Bitmap       protocol  options\n";

        break;

        default:
        break;
    }
}

unsigned int CommandParser::getStageCount()
{
    return stageCount;
}

Parameter& CommandParser::getParameter(int stage)
{
   if(stage >= vParameter.size())
        exit(0);
   else
       return vParameter[stage];
}

