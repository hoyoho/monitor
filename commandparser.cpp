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
monitor -I ens2f0,ens2f1 -L 1 -P   -C 200 -G 100     -B "001101"      -M  -O tcp    -u                       -N  -L 2
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
    while(true)
    {
       ++itCurrent;
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
    }
}

vector<string>  CommandParser::split(string s, char delim)
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
    vPositiveParameter.push_back(*pPara);
    vNegitiveParameter.push_back(*pPara);
    return true;

}

bool CommandParser::fillOption(Parameter* pPara, const vector<string>& paraOption)
{
    auto console = Logger::getLogger();
    console -> debug("____________________");
    for(auto it = paraOption.begin(); it != paraOption.end(); ++it)
    {
        console -> debug(*it);
    }
    
    return true;
}


bool CommandParser::createStagePara(const vector<string>& paraCmd)
{
    auto console = Logger::getLogger();
    Parameter* pPara = new Parameter();
    //此处应该添加错误校验
    if((paraCmd.size() <= 1) || (atoi(paraCmd[1].c_str()) == 0))
    {
        console -> error("Please start with a valid layer id [1..]");
        return false;
    }
    pPara -> stageId = atoi(paraCmd[1].c_str());

    auto itLast = paraCmd.begin() + 2;
    decltype(itLast) itCurrent = itLast;
    
    while(true)
    {
       if(itCurrent == paraCmd.end())
       {
           if(fillOption(pPara, vector<string>(itLast,itCurrent)) != false)
           {
              if(*itLast == "-P")
                vPositiveParameter.push_back(*pPara);
              else
                vNegitiveParameter.push_back(*pPara);
              return true;
           }
       }
       if(*itCurrent == "-P" && itCurrent != itLast)
       {
            if(fillOption(pPara, vector<string>(itLast,itCurrent)) != false)
                vNegitiveParameter.push_back(*pPara);
            else
                return false;
            itLast = itCurrent;
            
       }
       if(*itCurrent == "-N" && itCurrent != itLast)
       {
           if(fillOption(pPara, vector<string>(itLast,itCurrent)) != false)
               vNegitiveParameter.push_back(*pPara);
           else
               return false;
            itLast = itCurrent;
       }
       ++itCurrent;
    }
    return false;
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
                 <<"monitor -I ens2f0,ens2f1 -L 1 -P   -C 200 -G 100     -B \"001101\"          \t-M      -O tcp      -u               -N ...   -L 2 ...\n"
                 <<"                                                                            \t|          |        -t Tser,Tech\n"
                 <<"                                                                            \t|          |        -i 0\n"
                 <<"                                                                            \t|          |        -m\n"
                 <<"                                                                            \t|          |        -o\n"
                 <<"                                                                            \t|          |_______ -e\n"
                 <<"                                                                            \t-R 1              -r remove\n"
                 <<"                                                     -F \"2x+3\"            \t-D\n"
                 <<"         interface       layer      count                                   \tDrop/Repeat/Modify/Timedelay\n"
                 <<"                              direction    gap       Function/Sequence/Bitmap       Object  options\n";

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
}
