#include "commandparser.h"
#include <iostream>
#include <string>

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
    //公共解析部分
    if (argc < 2)
    {
       printInfo(ASK_HELP);
       return;
    }
    
    string cmdString(argv[1]);
    if( (cmdString.find("-L") == string::npos) || 
        (cmdString.find("-I") == string::npos) || 
        (cmdString.find("-I") > cmdString.find("-L")))
    {
        printInfo(ASK_HELP);
        return;
    }
    //语句拆分,分stage  填装参数
    
    int lastPosi = 0;
    int currentPosi = 0;
    while((currentPosi = cmdString.find("-L"), lastPosi + 2) != string::npos)
    {   
        createStagePara(cmdString.substr(lastPosi, currentPosi - lastPosi));
        lastPosi = currentPosi;
    }
    createStagePara(cmdString.substr(lastPosi, cmdString.size() - lastPosi));
}

int CommandParser::createStagePara(string cmd)
{   
    Parameter* temp = new Parameter;
    int posiLeft = 0;
    int posiRight = 0;
    if(posiLeft = cmd.find("-I") != string::npos)
    {
        posiLeft = cmd.find_first_of(' ', posiLeft);
        posiLeft = cmd.find_first_not_of(' ', posiLeft);
        posiRight = cmd.find_first_of(' ', posiLeft);
        string interface(cmd.substr(posiLeft,posiRight - posiLeft));
        std::strcpy(temp -> src, src); 
        
        return 0;
    }
    
    if(posiLeft = cmd.find("-I") != string::npos)
    {
        posiLeft = cmd.find_first_of(' ', posiLeft);
        posiLeft = cmd.find_first_not_of(' ', posiLeft);
        posiRight = cmd.find_first_of(' ', posiLeft);
        string interface(cmd.substr(posiLeft,posiRight - posiLeft));
        std::strcpy(temp -> src, src); 
        
        return 0;
    }

    
    vParameter.push_back(*temp);
    return 0;
}

void CommandParser::printInfo(int info)
{
    switch(info)
    {
        case ASK_HELP:
            cout <<"                                                     -S \"1,2,3,4\"         -T 1\n"
                 <<"                                                                            |        |        -p num,num\n"
                 <<"                                                                            |        |        -s num\n"
                 <<"                                                                            |        |        -a ACK\n"
                 <<"                                                                            |        |        -f  0x0F,0x0F\n"
                 <<"                                                                            |        |        -w window\n"
                 <<"                                                                            |        |        -c\n"
                 <<"monitor -I ens2f0,ens2f1 -L 1 -P   -C 200 -G 100     -B \"001101\"          -M      -P tcp    -u        -N ...   -L 2 ...\n"
                 <<"                                                                            |        |        -t Tser,Tech\n"
                 <<"                                                                            |        |        -i 0\n"
                 <<"                                                                            |        |        -m\n"
                 <<"                                                                            |        |        -o\n"
                 <<"                                                                            |        |        -e\n"
                 <<"                                                                            -R 1     |        -r remove\n"
                 <<"                                                     -F \"2x+3\"            -D\n"; 
                 <<"         interface       layer      count                                   Drop/Repeat/Modify/Timedelay\n";
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

Parameter& CommandParser::getParameter(int i)
{ 
   return vParameter[i];
}


