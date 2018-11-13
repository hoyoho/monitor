#include "logger.h"
#include "commandparser.h"
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
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
    auto itLast = command.begin();
    decltype(itLast) itCurrent = itLast;
    while(*itLast != "-I") 
    {
        ++itLast;
        if(itLast == command.end())
        {
            //这里可以根据用户已经输入的参数多少
            //提供更加精准的帮助内容
            printInfo(ASK_HELP);
            return;
        }
    }
    while(*itCurrent != "-L") 
    {
        ++itCurrent;
        if(itCurrent == command.end())
        {
            //这里可以根据用户已经输入的参数多少
            //提供更加精准的帮助内容
            printInfo(ASK_HELP);
            return;
        }
    }
    if(createInterfacePara(vector<string>(itLast, itCurrent)) != true)
    {
        exit(0);
    }


    //提取stage
    itLast= itCurrent;
    itCurrent += 1;
    while(true)
    {
       if(itCurrent == command.end())
       {
            createStagePara(vector<string>(itLast, itCurrent));
            break;
       }
       if(*itCurrent == "-L")
       {
            createStagePara(vector<string>(itLast, itCurrent));
            itLast = itCurrent;
       }
       ++itCurrent;
    }
}
/*------------------------------------------------------
const unsigned NEGITIVE     =   0x0001;
const unsigned PKT_COUNT    =   0x0002;
const unsigned GAP_VALUE    =   0x0004;
const unsigned SERIAL_NUM   =   0x0008;
const unsigned BITMAP         =   0x0010;
const unsigned FUNCTION     =   0x0020;
const unsigned PKT_DROP     =   0x0040;
const unsigned PKT_REPEAT   =   0x0080;
const unsigned PKT_DELAY    =   0x0100;
const unsigned PROTOCOL     =   0x0200;



class Parameter
{
    private:
        const static int MAX_INTERFACE_LEN = 40;
    public:

        DirectionFlag flag;
        unsigned int    flag;
        char            src[MAX_INTERFACE_LEN];
        char            dst[MAX_INTERFACE_LEN];
        unsigned int    gap;
        unsigned int    stageId;
        std::vector<unsigned int> choSequence;
};

-------------------------------------------------------*/

std::vector<std::string>  CommandParser::split(std::string s, char delim)
{
    std::vector<std::string> elems;
    if(s.empty())
        return elems;
    std::stringstream ss(s);
    std::string item;
 
    while (std::getline(ss, item, delim)) 
    {
        elems.push_back(item);
    }
    return elems;
}

bool CommandParser::createInterfacePara(const vector<string>& paraCmd)
{
    auto console = Logger::getLogger();
    Parameter* pPara = new Parameter();
    //此处应该添加错误校验
    std::vector<std::string> temp; 
    string tempItrfc;
    for(auto it = paraCmd.begin() + 1; it != paraCmd.end(); ++it)
    {
        tempItrfc += *it;
    }
    temp = split(tempItrfc, ',');
    if(temp.size() < 2)
    {
        return false;
    }
    pPara->src = temp[0];
    pPara->dst = temp[1];
    pPara->stageId = 0;
    vParameter.push_back(*pPara);
    return true;

}

bool CommandParser::createStagePara(const vector<string>& paraCmd)
{
    auto console = Logger::getLogger();
    Parameter* pPara = new Parameter();
    //此处应该添加错误校验
    pPara -> stageId = atoi(optarg);
    auto it = paraCmd.begin();
    auto tailIt = it + 1;
    while(true)
    {
       if(tailIt == paraCmd.end())
       {
            //处理从开始到结尾的所有
            for(auto itDebug = it; itDebug != tailIt; it++)
                console->debug(*it);
            break;
       }
       if(*tailIt == "-N")
       {
            //处理两次
            for(decltype(it) itDebug = it; itDebug != tailIt; it++)
                console->debug(*it);
            it = tailIt;
            tailIt = paraCmd.end();
            console->debug("again");
            for(decltype(it) itDebug = it; itDebug != tailIt; it++)
                console->debug(*it);
       }
       ++tailIt;
    }
    pPara->stageId = 0;
    vParameter.push_back(*pPara);
    return true;
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

