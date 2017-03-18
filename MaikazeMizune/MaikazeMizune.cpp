// MaikazeMizune Logger System Demo
//
#include "LogMkz.hpp"

Log* LOG = new Log();

int main(int argc, char* argv[])
{
    int k = 0x10;
    LOG->log(SUCCESS, LOG_REC, "First Try!");
    LOG->Init(true, "d:/maikaze.log");
    LOG->log(SUCCESS, LOG_REC, "k = " + str(k));
    LOG->log(SUCCESS, LOG_REC, "Try log.", _F, _L);
    LOG->Rec("Try log2", _F, _L);
    LOG->Recv(vname(k), str(k));
    LOG->SaveRefresh();
    delete LOG;
	return 0;
}

