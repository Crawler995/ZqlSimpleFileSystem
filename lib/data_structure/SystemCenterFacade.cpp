#include "SystemCenterFacade.h"

#include <iostream>

void SystemCenterFacade::newSystem(int systemMemorySize)
{
	if (!SystemMemoryCenter::initSystemMemory(systemMemorySize)) {
		return;
	}

	SystemMemoryCenter::initSystemInfoBlock();
	SystemMemoryCenter::initFCBList();
	SystemMemoryCenter::initFIBList();
	SystemMemoryCenter::initRootDir();

	SystemMemoryCenter::outputBlocksNum();
}

void SystemCenterFacade::exitSystem()
{
	SystemMemoryCenter::destroySystemMemory();
}

void SystemCenterFacade::quitSystem()
{
	if (SystemMemoryCenter::getCurFCBPos() != NULL) {
		SystemMemoryCenter::destroySystemMemory();
	}
	std::cout << "quit the system successfully!" << std::endl;
}

void SystemCenterFacade::mkdir(std::string dirName)
{
	SystemMemoryCenter::makeDir(dirName, SystemMemoryCenter::getCurFCBPos());
}

void SystemCenterFacade::rmdir(std::string dirName)
{
	SystemMemoryCenter::removeDir(dirName);
}

void SystemCenterFacade::ls()
{
	SystemMemoryCenter::listCurDir();
}

void SystemCenterFacade::cd(std::string destDirName)
{
	SystemMemoryCenter::changeDir(destDirName);
}

void SystemCenterFacade::create(std::string fileName)
{
	SystemMemoryCenter::createFile(fileName);
}

void SystemCenterFacade::open(std::string fileName)
{
	SystemMemoryCenter::openFile(fileName);
}

void SystemCenterFacade::close()
{
	SystemMemoryCenter::closeFile();
}

void SystemCenterFacade::write()
{
	SystemMemoryCenter::writeFile();
}

void SystemCenterFacade::read()
{
	SystemMemoryCenter::readFile();
}

void SystemCenterFacade::ddelete(std::string fileName)
{
	SystemMemoryCenter::deleteFile(fileName);
}

void SystemCenterFacade::sfs(std::string fileName)
{
	SystemMemoryCenter::initSystemFromFile(fileName);
}

std::string SystemCenterFacade::getCurPath()
{
	return SystemMemoryCenter::getPath();
}
