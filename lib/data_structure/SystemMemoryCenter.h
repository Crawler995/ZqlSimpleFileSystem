#pragma once

#define BLOCK_SIZE 512
#define FCB_SIZE 128

#define SIB_START_BLOCK 0
#define SIB_END_BLOCK 7
#define FCB_START_BLOCK 8
#define FCB_END_BLOCK 63
#define FIB_START_BLOCK 64

// Surprise: my student ID last eight digits
// Use to identify the cache file (*.zsfs)
#define ZSFS_FLAG 0x20172135 

#include <cstdlib>
#include <string>

// 1024B
struct SystemInfoBlock {
	char systemInfo[512]; // 512
	char otherInfo[500]; // 500
	char* freeFCBAddr; // 4
	char* fullFCBAddr; // 4
	char* freeFIBAddr; // 4
};

// 128B
struct FileControlBlock {
	char name[56]; // 56
	int size; // 4
	char* nextFCB; // 4
	char* FIB; // 4
	char createTime[24]; // xxxx-xx-xx xx:xx:xx 24
	char lastModifiedTime[24]; // xxxx-xx-xx xx:xx:xx 24
	char deleteFlag; // 1
	char folderFlag; // 1
	char* childrenListHead; // 4
	char* parentDirFCB; // 4
};

// 512B
struct FileInfoBlock {
	char info[508]; // 508
	char* nextFIB; // 4
};

class SystemMemoryCenter
{
public:
	static bool initSystemMemory(int systemMemorySize);
	static void destroySystemMemory();
	static void initSystemInfoBlock();
	static void initFCBList();
	static void initFIBList();
	static void initRootDir();
	static void outputBlocksNum();

	static void makeDir(std::string dirName, char* parentDir);
	static void removeDir(std::string destDirName);
	static void listCurDir();
	static void changeDir(std::string destDirName);

	static void createFile(std::string fileName);
	static void openFile(std::string fileName);
	static void closeFile();
	static void writeFile();
	static void readFile();
	static void deleteFile(std::string fileName);

	static void saveSystemToFile();
	static void initSystemFromFile(std::string fileName);

	static char* getBlock(int startBlockID);
	static char* getCurFCBPos();

	static std::string getPath();

private:
	static int systemMemorySize;
	static char* systemMemoryStartAddr;
	static char* systemInfoBlockStartAddr;

	static int emptyFCBNum;
	static int fullFCBNum;
	static int emptyFIBNum;
	static int fullFIBNum;

	static std::string path;
	static char* curFCBPos;

	static char* openFileFCB;

	static char* initFCBBlock(int index);
	static char* initFIBBlock(int index);

	static void removeDirByPointer(FileControlBlock* fcb, FileControlBlock* fcbParentFCB);
	static void deleteFileByPointer(FileControlBlock* fcb, FileControlBlock* fcbParentFCB);

	static char* findFCBByName(std::string name, bool isDir);
	static int getFreeFIBNum();

	static void pointerAddOffset(int offset, char*& c);

	static std::string getCurTime();
};