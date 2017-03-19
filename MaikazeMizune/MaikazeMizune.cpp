// MaikazeMizune Logger System Demo
//
#include "LogMkz.hpp"
#include <memory>

int main(int argc, char* argv[])
{
    constexpr auto k = 0x10;
	const auto LOG = std::make_unique<Log>();
    LOG->log(SUCCESS, LOG_REC, "First Try!");
    LOG->Init(true, "d:/maikaze.log");
    LOG->log(SUCCESS, LOG_REC, "k = " + ToString(k));
	LOG->LogWithContext(SUCCESS, LOG_REC, "Try log.");
	LOG->LogWithContext(RECORD, LOG_REC, "Try log2");
    LOG->Recv(vname(k), ToString(k));
    LOG->SaveRefresh();
}

