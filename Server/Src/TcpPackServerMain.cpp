// Tcp_Pack_Server.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ServerService.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv)
{
	START_EASYLOGGINGPP(argc, argv);
	
    std::string configPath= "..\\..\\Config\\my_log.conf";

    el::Configurations conf(configPath);
	el::Loggers::reconfigureAllLoggers(conf);
	el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
	el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
	el::Loggers::addFlag(el::LoggingFlag::NewLineForContainer); 
     
	std::shared_ptr<ServerService> serverService = std::make_shared<ServerService>();   
    serverService->Start();

	while (1)
	{
		Sleep(1000);
	}

    return 0;
}

