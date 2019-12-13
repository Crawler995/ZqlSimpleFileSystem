#include "SystemMemoryCenter.h"

#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <cstdio>
#include <io.h>
#include <direct.h>

char* SystemMemoryCenter::systemMemoryStartAddr = NULL;
char* SystemMemoryCenter::systemInfoBlockStartAddr = NULL;
int SystemMemoryCenter::systemMemorySize = 0;

int SystemMemoryCenter::emptyFCBNum = 0;
int SystemMemoryCenter::fullFCBNum = 0;
int SystemMemoryCenter::emptyFIBNum = 0;
int SystemMemoryCenter::fullFIBNum = 0;

std::string SystemMemoryCenter::path = "";
char* SystemMemoryCenter::curFCBPos = NULL;

char* SystemMemoryCenter::openFileFCB = NULL;


bool SystemMemoryCenter::initSystemMemory(int systemMemorySize)
{
	if (systemMemoryStartAddr != NULL) {
		std::cout << "Error: you have created a system and the system is still running." << std::endl;
		return false;
	}

	systemMemoryStartAddr = (char*)malloc(sizeof(char) * systemMemorySize * 1024);
	SystemMemoryCenter::systemMemorySize = systemMemorySize * 1024;
	path = "/";

	std::cout << "create new file system successfully!" << std::endl;
	return true;
}

void SystemMemoryCenter::destroySystemMemory()
{
	if (systemMemoryStartAddr == NULL) {
		std::cout << "Error: there's no running system!" << std::endl;
		return;
	}

	saveSystemToFile();

	free(systemMemoryStartAddr);
	systemMemoryStartAddr = NULL;
	systemInfoBlockStartAddr = NULL;
	curFCBPos = NULL;
	path = "";

	std::cout << "There's a running system...exit it safely!" << std::endl;
}

void SystemMemoryCenter::outputBlocksNum()
{
	std::cout
		<< "empty File Control Block: " << emptyFCBNum << ", " << std::endl
		<< "full File Control Block: " << fullFCBNum << ", " << std::endl
		<< "empty File Info Block: " << emptyFIBNum << ", " << std::endl
		<< "full File Info Block: " << fullFIBNum << std::endl;
}

void SystemMemoryCenter::makeDir(std::string dirName, char* parentDir)
{
	if (systemMemoryStartAddr == NULL) {
		std::cerr << "Error: there's no file system running." << std::endl;
		return;
	}
	SystemInfoBlock* sib = (SystemInfoBlock*)systemInfoBlockStartAddr;
	FileControlBlock* fcb = (FileControlBlock*)sib->freeFCBAddr;
	// no free FCB
	// failure
	if (fcb == NULL) {
		std::cerr << "Error: there's no free FCB." << std::endl;
		return;
	}
	if (findFCBByName(dirName, true) != NULL) {
		std::cerr << "Error: there's already same name directory." << std::endl;
		return;
	}

	// choose first FCB in the free list
	// take it out
	FileControlBlock* curUseFCB = fcb;
	sib->freeFCBAddr = curUseFCB->nextFCB;
	curUseFCB->nextFCB = NULL;
	curUseFCB->parentDirFCB = parentDir;

	// add it to the head of the parent dir list
	FileControlBlock* pDir = (FileControlBlock*)parentDir;
	// init root dir
	if (parentDir == NULL) {
		curUseFCB->nextFCB = sib->fullFCBAddr;
		sib->fullFCBAddr = (char*)curUseFCB;
	}
	else {
		curUseFCB->nextFCB = pDir->childrenListHead;
		pDir->childrenListHead = (char*)curUseFCB;
		curUseFCB->parentDirFCB = (char*)pDir;
	}

	// set the info
	strcpy(curUseFCB->name, dirName.c_str());
	curUseFCB->folderFlag = 1;
	strcpy(curUseFCB->createTime, getCurTime().c_str());
	strcpy(curUseFCB->lastModifiedTime, getCurTime().c_str());
	curUseFCB->size = 0;
	curUseFCB->FIB = NULL;
	curUseFCB->deleteFlag = 0;
}

void SystemMemoryCenter::removeDir(std::string destDirName)
{
	FileControlBlock* fcb = (FileControlBlock*)findFCBByName(destDirName, true);
	if (fcb == NULL) {
		std::cerr << "Error: there's no matched directory can be deleted." << std::endl;
		return;
	}

	removeDirByPointer(fcb, (FileControlBlock*)curFCBPos);
}

void SystemMemoryCenter::listCurDir()
{
	// no root dir
	if (systemMemoryStartAddr == NULL) {
		std::cerr << "Error: there's no file system running." << std::endl;
		return;
	}
	
	// set list start in curFCBPos
	FileControlBlock* fcb = (FileControlBlock*)curFCBPos;
	if (fcb->childrenListHead == NULL) {
		// no dir in this dir
		return;
	}
	fcb = (FileControlBlock*)fcb->childrenListHead;

	while (fcb->nextFCB != NULL) {
		if (fcb->folderFlag) {
			std::cout << fcb->name << "/    " << std::endl;
		}
		else {
			std::cout << fcb->name << "    " << fcb->size << "B    " << fcb->lastModifiedTime << std::endl;
		}
		
		fcb = (FileControlBlock*)fcb->nextFCB;
	}
	if (fcb->folderFlag) {
		std::cout << fcb->name << "/    " << std::endl;
	}
	else {
		std::cout << fcb->name << "    " << fcb->size << "B    " << fcb->lastModifiedTime << std::endl;
	}
}

void SystemMemoryCenter::changeDir(std::string destDirName)
{
	if (systemMemoryStartAddr == NULL) {
		std::cerr << "Error: there's no file system running." << std::endl;
		return;
	}

	FileControlBlock* fcb = (FileControlBlock*)curFCBPos;

	if (destDirName == ".") {
		return;
	}
	else if (destDirName == "..") {
		if (fcb->parentDirFCB == NULL) {
			std::cerr << "Error: there's no parent directory of current directory." << std::endl;
			return;
		}

		curFCBPos = fcb->parentDirFCB;
		int i;
		for (i = path.size() - 1; path[i] != '/'; i--);
		if (i == 0) {
			path = "/";
		}
		else {
			path = path.substr(0, i);
		}

		return;
	}
	else {
		if (fcb->childrenListHead == NULL) {
			std::cerr << "Error: there's no matched directory." << std::endl;
			return;
		}
		fcb = (FileControlBlock*)fcb->childrenListHead;

		while (fcb->nextFCB != NULL) {
			// success find dest dir
			if (fcb->folderFlag == 1 && fcb->name == destDirName) {
				curFCBPos = (char*)fcb;
				if(path != "/")
					path += "/";
				path += fcb->name;

				return;
			}

			fcb = (FileControlBlock*)fcb->nextFCB;
		}
		// success find dest dir
		if (fcb->folderFlag == 1 && fcb->name == destDirName) {
			curFCBPos = (char*)fcb;
			if(path != "/")
				path += "/";
			path += fcb->name;

			return;
		}

		std::cerr << "Error: there's no matched directory." << std::endl;
		return;
	}
}

void SystemMemoryCenter::createFile(std::string fileName)
{
	if (systemMemoryStartAddr == NULL) {
		std::cerr << "Error: there's no file system running." << std::endl;
		return;
	}
	SystemInfoBlock* sib = (SystemInfoBlock*)systemInfoBlockStartAddr;
	FileControlBlock* fcb = (FileControlBlock*)sib->freeFCBAddr;
	// no free FCB
	// failure
	if (fcb == NULL) {
		std::cerr << "Error: there's no free FCB." << std::endl;
		return;
	}
	// find a free FIB
	// get the first FIB in freeIBList
	FileInfoBlock* fib = (FileInfoBlock*)sib->freeFIBAddr;
	if (fib == NULL) {
		std::cerr << "Error: there's no free IB." << std::endl;
		return;
	}
	if (findFCBByName(fileName, false) != NULL) {
		std::cerr << "Error: there's alreay same name file in this directory." << std::endl;
		return;
	}

	// choose first FCB in the free list
	// take it out
	FileControlBlock* curUseFCB = fcb;
	sib->freeFCBAddr = curUseFCB->nextFCB;
	curUseFCB->nextFCB = NULL;

	// add it to the head of cur dir FCB childrenList
	FileControlBlock* curDirFCB = (FileControlBlock*)curFCBPos;
	curUseFCB->nextFCB = curDirFCB->childrenListHead;
	curDirFCB->childrenListHead = (char*)curUseFCB;

	// set the info
	strcpy(curUseFCB->name, fileName.c_str());
	curUseFCB->folderFlag = 0;
	strcpy(curUseFCB->createTime, getCurTime().c_str());
	strcpy(curUseFCB->lastModifiedTime, getCurTime().c_str());
	curUseFCB->size = 0;
	curUseFCB->deleteFlag = 0;
	curUseFCB->parentDirFCB = NULL;
	
	// choose first free FIB in the free list
	// take it out
	FileInfoBlock* curUseFIB = fib;
	sib->freeFIBAddr = curUseFIB->nextFIB;
	curUseFIB->nextFIB = NULL;
	// link FCB and FIB
	curUseFCB->FIB = (char*)curUseFIB;
}

void SystemMemoryCenter::openFile(std::string fileName)
{
	if (systemMemoryStartAddr == NULL) {
		std::cerr << "Error: there's no file system running." << std::endl;
		return;
	}

	if (openFileFCB != NULL) {
		std::cerr << "Error: a file is already opening! You must close it before open another file." << std::endl;
		return;
	}

	openFileFCB = findFCBByName(fileName, false);
	if (openFileFCB == NULL) {
		std::cerr << "Error: there's no file named \"" << fileName << "\"!" << std::endl;
		return;
	}
}

void SystemMemoryCenter::closeFile()
{
	if (systemMemoryStartAddr == NULL) {
		std::cerr << "Error: there's no file system running." << std::endl;
		return;
	}

	if (openFileFCB == NULL) {
		std::cerr << "Error: there's no opening file." << std::endl;
		return;
	}

	openFileFCB = NULL;
}

void SystemMemoryCenter::writeFile()
{
	if (systemMemoryStartAddr == NULL) {
		std::cerr << "Error: there's no file system running." << std::endl;
		return;
	}
	if (openFileFCB == NULL) {
		std::cerr << "Error: there' no opening file can be written!" << std::endl;
		return;
	}

	FileControlBlock* fcb = (FileControlBlock*)openFileFCB;
	FileInfoBlock* fib = (FileInfoBlock*)fcb->FIB;
	SystemInfoBlock* sib = (SystemInfoBlock*)systemInfoBlockStartAddr;

	int freeFIBNum = getFreeFIBNum();
	std::cout
		<< "Tips: there are " << freeFIBNum << " free FIB can be used." << std::endl
		<< "It means that you can write " << freeFIBNum * 508 - 1 << "B data into \"" << fcb->name << "\" now." << std::endl
		<< "Now you can input freely. use Ctrl + Z(Windows) or Ctrl + D(Linux) to end this write (input EOF). "
		<< "After it, the data you typed will be saved automatically." << std::endl << std::endl;

	std::string userInput;
	char c;

	while ((c = getchar()) != EOF) {
		userInput.push_back(c);
	}

	// if user input is too much
	// no free FIB can be used
	// '\0' so + 1
	if (userInput.size() + 1 > freeFIBNum * 508) {
		std::cerr
			<< "Error: you write too much data (" << userInput.size() + 1 << "B) to this file!" << std::endl
			<< "This write will be canceled, the file will not be changed." << std::endl;
		return;
	}

	int last = 0;
	for (int i = 0; i < userInput.size() / 508 + 1; i++) {
		for (int j = 0; j < 508; j++) {
			int index = i * 508 + j;
			if (index > userInput.size() - 1) {
				last = j;
				goto end;
			}
			
			fib->info[j] = userInput[index];
		}

		// find next free FIB
		// choose first free FIB in the free list
		// take it out
		FileInfoBlock* nFIB = (FileInfoBlock*)sib->freeFIBAddr;
		sib->freeFIBAddr = nFIB->nextFIB;
		nFIB->nextFIB = NULL;
		// link FIB and nFIB
		fib->nextFIB = (char*)nFIB;
		fib = nFIB;
	}

	end:
	fib->info[last] = '\0';


	strcpy(fcb->lastModifiedTime, getCurTime().c_str());
	fcb->size = userInput.size() + 1;
}

void SystemMemoryCenter::readFile()
{
	if (systemMemoryStartAddr == NULL) {
		std::cerr << "Error: there's no file system running." << std::endl;
		return;
	}
	if (openFileFCB == NULL) {
		std::cerr << "Error: there' no opening file can be read!" << std::endl;
		return;
	}

	FileControlBlock* fcb = (FileControlBlock*)openFileFCB;
	FileInfoBlock* fib = (FileInfoBlock*)fcb->FIB;

	int i = 0;
	while (true) {
		// read end
		if (fib->info[i] == '\0') {
			break;
		}
		std::cout << fib->info[i];
		i++;

		// find next FIB
		if (i >= 508) {
			fib = (FileInfoBlock*)fib->nextFIB;
			i = 0;
		}
	}
}

void SystemMemoryCenter::deleteFile(std::string fileName)
{
	if (systemMemoryStartAddr == NULL) {
		std::cerr << "Error: there's no file system running." << std::endl;
		return;
	}

	FileControlBlock *fcb = (FileControlBlock*)findFCBByName(fileName, false);
	if (fcb == NULL) {
		std::cerr << "Error: there's no matched file can be deleted." << std::endl;
		return;
	}

	deleteFileByPointer(fcb, (FileControlBlock*)curFCBPos);
}

void SystemMemoryCenter::saveSystemToFile()
{
	std::string curTime = getCurTime();
	replace(curTime.begin(), curTime.end(), '/', '_');
	replace(curTime.begin(), curTime.end(), ':', '_');
	replace(curTime.begin(), curTime.end(), ' ', '_');

	if (_access("./sfs_cache/", 0) == -1) {
		_mkdir("sfs_cache");
	}

	FILE* sysFile;
	sysFile = fopen(std::string("./sfs_cache/" + curTime + ".zsfs").c_str(), "wb");

	uint32_t identifyFlag = ZSFS_FLAG;
	fwrite(&identifyFlag, 4, 1, sysFile);
	fwrite(systemMemoryStartAddr, systemMemorySize, 1, sysFile);
	fwrite(&systemMemoryStartAddr, sizeof(char*), 1, sysFile);

	fclose(sysFile);
}

void SystemMemoryCenter::initSystemFromFile(std::string fileName)
{
	if (fileName.substr(fileName.find_last_of('.') + 1) != "zsfs") {
		std::cerr << "Error: the given file isn't the legal cache file! "
			"Ensure you select the cache file in directory \"sfs_cache\"." << std::endl;
		return;
	}

	FILE* sysFile;
	sysFile = fopen(fileName.c_str(), "rb");

	if (sysFile == NULL) {
		std::cerr << "Error: the given cache file can't be opened! "
			"Ensure you select the cache file in directory \"sfs_cache\"." << std::endl;
		return;
	}

	uint32_t identifyFlag;
	fread(&identifyFlag, 4, 1, sysFile);
	if (identifyFlag != ZSFS_FLAG) {
		std::cerr << "Error: the given file isn't the legal cache file! It can't be used to recover the system."
			"Ensure you select the cache file in directory \"sfs_cache\"." << std::endl;
		return;
	}

	fseek(sysFile, 0, SEEK_SET);
	fseek(sysFile, 0, SEEK_END);
	systemMemorySize = ftell(sysFile) - 8;
	fseek(sysFile, 4, SEEK_SET);
	std::cout << "system total size: " << systemMemorySize << "B" << std::endl;

	systemMemoryStartAddr = (char*)malloc(sizeof(char) * systemMemorySize);

	fread(systemMemoryStartAddr, systemMemorySize, 1, sysFile);

	int start;
	fread(&start, sizeof(char*), 1, sysFile);
	char* preSystemMemoryStartAddr = (char*)start;
	fclose(sysFile);

	// modify all not null pointers
	// nowAddr = preAddr + (systemMemoryStartAddr - presystemMemoryStartAddr)
	// offset = systemMemoryStartAddr - preSystemMemoryStartAddr
	int offset = systemMemoryStartAddr - preSystemMemoryStartAddr;

	// modify SIB
	systemInfoBlockStartAddr = systemMemoryStartAddr;
	SystemInfoBlock* sib = (SystemInfoBlock*)systemInfoBlockStartAddr;

	pointerAddOffset(offset, sib->freeFCBAddr);
	pointerAddOffset(offset, sib->freeFIBAddr);
	pointerAddOffset(offset, sib->fullFCBAddr);

	// modify FCB
	char* FCBStartAddr = getBlock(FCB_START_BLOCK);
	for (int i = 0; i < FCB_END_BLOCK - FCB_START_BLOCK + 1; i++) {
		FileControlBlock* fcb = (FileControlBlock*)(FCBStartAddr + i * FCB_SIZE);
		pointerAddOffset(offset, fcb->childrenListHead);
		pointerAddOffset(offset, fcb->FIB);
		pointerAddOffset(offset, fcb->nextFCB);
		pointerAddOffset(offset, fcb->parentDirFCB);
	}

	// modify FIB
	for (int i = FIB_START_BLOCK; i < systemMemorySize / BLOCK_SIZE - 1; i++) {
		FileInfoBlock* fib = (FileInfoBlock*)getBlock(i);
		pointerAddOffset(offset, fib->nextFIB);
	}

	// init system info
	path = "/";
	curFCBPos = sib->fullFCBAddr;

	std::cout << "system loaded from disk successfully!" << std::endl;
}

void SystemMemoryCenter::initSystemInfoBlock()
{
	systemInfoBlockStartAddr = getBlock(SIB_START_BLOCK);
	SystemInfoBlock* addr = (SystemInfoBlock*)systemInfoBlockStartAddr;

	strcpy(addr->systemInfo, "SFS Developed By Zhang Qinglong");
	addr->freeFCBAddr = getBlock(FCB_START_BLOCK);
	addr->freeFIBAddr = getBlock(FIB_START_BLOCK);
	addr->fullFCBAddr = NULL;
	strcpy(addr->otherInfo, "");
}

void SystemMemoryCenter::initFCBList()
{
	SystemInfoBlock* sib = (SystemInfoBlock*)systemInfoBlockStartAddr;
	sib->freeFCBAddr = initFCBBlock(0);
	FileControlBlock* lastFCBAddr = (FileControlBlock*)sib->freeFCBAddr;

	for (int i = 1; i < (FCB_END_BLOCK - FCB_START_BLOCK + 1) * BLOCK_SIZE / FCB_SIZE; i++) {
		lastFCBAddr->nextFCB = initFCBBlock(i);
		lastFCBAddr = (FileControlBlock*)lastFCBAddr->nextFCB;
	}

	emptyFCBNum = (FCB_END_BLOCK - FCB_START_BLOCK + 1) * BLOCK_SIZE / FCB_SIZE;
}

void SystemMemoryCenter::initFIBList()
{
	SystemInfoBlock* sib = (SystemInfoBlock*)systemInfoBlockStartAddr;
	sib->freeFIBAddr = initFIBBlock(0);
	FileInfoBlock* lastFIBAddr = (FileInfoBlock*)sib->freeFIBAddr;

	for (int i = 1; i < systemMemorySize / BLOCK_SIZE - FIB_START_BLOCK; i++) {
		lastFIBAddr->nextFIB = initFIBBlock(i);
		lastFIBAddr = (FileInfoBlock*)lastFIBAddr->nextFIB;
	}

	emptyFIBNum = systemMemorySize / BLOCK_SIZE - FIB_START_BLOCK;
}

void SystemMemoryCenter::initRootDir()
{
	makeDir("", NULL);
	SystemInfoBlock* sib = (SystemInfoBlock*)systemInfoBlockStartAddr;
	curFCBPos = sib->fullFCBAddr;
}

char* SystemMemoryCenter::initFCBBlock(int index)
{
	FileControlBlock* fcb = (FileControlBlock*)(getBlock(FCB_START_BLOCK) + FCB_SIZE * index);
	fcb->nextFCB = NULL;
	fcb->FIB = NULL;
	fcb->childrenListHead = NULL;
	fcb->parentDirFCB = NULL;
	fcb->size = 0;

	return (char*)fcb;
}

char* SystemMemoryCenter::initFIBBlock(int index)
{
	FileInfoBlock* fib = (FileInfoBlock*)(getBlock(FIB_START_BLOCK + index));
	fib->nextFIB = NULL;
	fib->info[0] = '\0';

	return (char*)fib;
}

void SystemMemoryCenter::removeDirByPointer(FileControlBlock* fcb, FileControlBlock* fcbParentFCB)
{
	FileControlBlock* child = (FileControlBlock*)fcb->childrenListHead;
	SystemInfoBlock* sib = (SystemInfoBlock*)systemInfoBlockStartAddr;

	if (child != NULL) {
	// delete children
		while (child->nextFCB != NULL) {
			if (child->folderFlag == 0) {
				// delete file by FCB pointer
				deleteFileByPointer(child, fcb);
			}
			else {
				removeDirByPointer(child, fcb);
			}

			child = (FileControlBlock*)fcb->childrenListHead;
		}
		if (child->folderFlag == 0) {
			// delete file by FCB pointer
			deleteFileByPointer(child, fcb);
		}
		else {
			removeDirByPointer(child, fcb);
		}
	}

	// take it out from full FCB list
	// add the used FCB into free FCB list

	// find the previous FCB of fcb
	FileControlBlock* f = (FileControlBlock*)fcbParentFCB->childrenListHead;
	if (f != fcb) {
		while ((FileControlBlock*)f->nextFCB != fcb) {
			f = (FileControlBlock*)f->nextFCB;
		}
		f->nextFCB = fcb->nextFCB;
		fcb->nextFCB = NULL;
	}
	else {
		((FileControlBlock*)fcbParentFCB)->childrenListHead = NULL;
		fcb->nextFCB = NULL;
	}

	fcb->nextFCB = ((FileControlBlock*)sib->freeFCBAddr)->nextFCB;
	sib->freeFCBAddr = (char*)fcb;

	fcb->parentDirFCB = NULL;
	fcb->folderFlag = 0;
	fcb->childrenListHead = NULL;
	fcb->size = 0;
}

void SystemMemoryCenter::deleteFileByPointer(FileControlBlock* fcb, FileControlBlock* fcbParentFCB)
{
	if ((char*)fcb == openFileFCB) {
		std::cout
			<< "Warning: file " << path << ((FileControlBlock*)fcbParentFCB)->name << "/" 
			<< ((FileControlBlock*)openFileFCB)->name
			<< " is opening. It will be closed automatically." << std::endl;

		openFileFCB = NULL;
	}

	FileInfoBlock* fib = (FileInfoBlock*)fcb->FIB, * next = NULL;
	SystemInfoBlock* sib = (SystemInfoBlock*)systemInfoBlockStartAddr;

	// add the used FIB into free FIB list
	if (fib != NULL) {
		while (fib->nextFIB) {
			// take out this FIB
			next = (FileInfoBlock*)fib->nextFIB;

			fcb->FIB = (char*)next;
			fib->nextFIB = NULL;
			fib->info[0] = '\0';

			// add this FIB into free FIB list
			fib->nextFIB = ((FileInfoBlock*)sib->freeFIBAddr)->nextFIB;
			sib->freeFIBAddr = (char*)fib;

			fib = next;
		}
		// last FIB
		fcb->FIB = NULL;
		fib->nextFIB = ((FileInfoBlock*)sib->freeFIBAddr)->nextFIB;
		fib->info[0] = '\0';
		sib->freeFIBAddr = (char*)fib;
	}

	// take it out from full FCB list
	// add the used FCB into free FCB list

	// find the previous FCB of fcb
	FileControlBlock* f = (FileControlBlock*)fcbParentFCB->childrenListHead;
	if (f != fcb) {
		while ((FileControlBlock*)f->nextFCB != fcb) {
			f = (FileControlBlock*)f->nextFCB;
		}
		f->nextFCB = fcb->nextFCB;
		fcb->nextFCB = NULL;
	}
	else {
		((FileControlBlock*)fcbParentFCB)->childrenListHead = fcb->nextFCB;
		fcb->nextFCB = NULL;
	}

	fcb->nextFCB = ((FileControlBlock*)sib->freeFCBAddr)->nextFCB;
	sib->freeFCBAddr = (char*)fcb;

	fcb->parentDirFCB = NULL;
	fcb->size = 0;
}

char* SystemMemoryCenter::findFCBByName(std::string name, bool isDir)
{
	if (curFCBPos == NULL) {
		return NULL;
	}
	FileControlBlock* fcb = (FileControlBlock*)((FileControlBlock*)curFCBPos)->childrenListHead;
	if (fcb == NULL) {
		return NULL;
	}
	while (fcb->nextFCB != NULL) {
		if (fcb->name == name) {
			if (isDir == fcb->folderFlag) {
				return (char*)fcb;
			}
		}
		fcb = (FileControlBlock*)fcb->nextFCB;
	}
	if (fcb->name == name) {
		if (isDir == fcb->folderFlag) {
			return (char*)fcb;
		}
	}
	return NULL;
}

int SystemMemoryCenter::getFreeFIBNum()
{
	SystemInfoBlock* sib = (SystemInfoBlock*)systemInfoBlockStartAddr;
	FileInfoBlock* fib = (FileInfoBlock*)sib->freeFIBAddr;

	int res = 0;
	for (; fib->nextFIB != NULL; res++, fib = (FileInfoBlock*)fib->nextFIB);

	return res + 1;
}

void SystemMemoryCenter::pointerAddOffset(int offset, char*& c)
{
	if (c != NULL) {
		c += offset;
	}
}

std::string SystemMemoryCenter::getCurTime()
{
	time_t curTime;
	time(&curTime);
	struct tm* timeInfo;

	char buf[24];

	timeInfo = localtime(&curTime);
	strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", timeInfo);

	return std::string(buf);
}

char* SystemMemoryCenter::getBlock(int blockID)
{
	return systemMemoryStartAddr + BLOCK_SIZE * blockID;
}

char* SystemMemoryCenter::getCurFCBPos()
{
	return curFCBPos;
}

std::string SystemMemoryCenter::getPath()
{
	return path;
}
