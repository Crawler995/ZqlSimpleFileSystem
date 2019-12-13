# ZqlSimpleFileSystem

在内存中模拟的简单文件系统。

## 内部结构

根据老师给的参考数据结构，在内存中开辟一整块一定大小的空间，以512B为单位分块。0-7块为系统信息块，8-63块为文件控制块（FCB），剩余的作为文件信息块。

## 支持操作

1. new (system memory size (> 200KB)) 新建文件系统
2. sfs (zsfs cache file path) 使用cache文件恢复文件系统
3. exit 退出文件系统
4. quit 退出程序
5. mkdir (dir name) 新建文件夹
6. rmdir (dir name) 删除文件夹
7. ls 显示文件夹内的文件和文件夹
8. cd (dir name) 更改当前目录
9. create (file name) 新建文件
10. open (file name) 打开文件
11. close 关闭文件
12. read 读取文件
13. write 写文件
14. delete (file name) 删除文件

## Tips

1. zsfs文件为此系统的信息文件，在exit或quit的时候会自动创建在sfs_cache文件夹下。可使用sfs命令读取此文件来恢复上次使用的系统。
2. 开发环境为VS2019，请注意编译的时候一定要选择x86，否则整个系统会直接崩溃，因为所有的结构体设计、内存对齐、内存分配等都是以x86来设计的（如一个指针占4B等）。