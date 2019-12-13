#pragma once

#include "SystemMemoryCenter.h"

class SystemCenterFacade
{
public:
	static void newSystem(int systemMemorySize);
	static void quitSystem();
	static void exitSystem();
	static void mkdir(std::string dirName);
	static void rmdir(std::string dirName);
	static void ls();
	static void cd(std::string destDirName);
	static void create(std::string fileName);
	static void open(std::string fileName);
	static void close();
	static void write();
	static void read();
	static void ddelete(std::string fileName);
	static void sfs(std::string fileName);

	static std::string getCurPath();
};

