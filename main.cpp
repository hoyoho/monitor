#include <iostream>
#include <vector>
#include <csignal>
#include <pthread.h>

#include "commandparser.h"
#include "monitor.h"

using namespace std;

void signalHandler( int sigNum );
void*  singleStageProcess(void* parameter);

vector<pthread_t> pidVector; 
int main( int argc, char* argv[] )
{
//绑定信号处理函数
   	signal( SIGINT, signalHandler );  
//解析参数
	CommandParser parser(argc, argv);
    if(parser.getStageCount() == 0)
        exit(0);
//建立线程    
    for(unsigned i = parser.getStageCount(); i > 0; i--)
    {
        pthread_t* pPid = new pthread_t;
        Parameter* pTempPara = new Parameter(parser.getParameter(i));
        int status = pthread_create( pPid, NULL, singleStageProcess, pTempPara );
        if ( status != 0 )
        {
            cerr << "thread create fail." << endl;
            exit(1);
        }
        pidVector.push_back( *pPid );
    }
//等待各个线程终止返回
    for(auto it = pidVector.begin(); it != pidVector.begin(); ++it)
    {
        pthread_join( *it, NULL );
    }
	return 0;
}



void signalHandler( int sigNum )
{   
    switch(sigNum)
    {
        default:
            for(auto it = pidVector.begin();it != pidVector.begin(); ++it)
            {
                pthread_cancel( *it );
                //pcap_close( pt_a );
                //libnet_destroy( net_a );
            }
//            exit(sigNum);
        break;
    }
}


/*-----------------------------------------
设置取消点以便函数退出
-----------------------------------------*/
void* singleStageProcess(void* parameter)
{
    
    Parameter* pTempPara = static_cast < Parameter* > ( parameter );
	//Monitor monitor(paraser.parameter());
	cout << "i amd stage " << endl;
    return NULL;
}

