/*
this is core header   for litesetup

by dreamyouxi.com
*/




#ifndef  __LITESETUP_H__
#define  __LITESETUP_H__

#include<iostream>
#include<fstream>
#include"windows.h" 
#include "math.h" 
#include "zlib.h"
#include "zconf.h"
#include <vector>
#include <functional>
#include <string>
#include <string.h>
#include <thread>
#include <mutex>
#include <assert.h>


using namespace std;




/*
disable of warning

*/
#pragma warning (disable : 4996)
#pragma warning (disable : 4244)
#pragma warning (disable : 4800)



//定义一些编译到exe的常量
namespace const_for_exe
{
	//安装程序根目录(用户选完安装路径后  带上的文件夹)
	static  string dir_install = "\\myApp";

	//程序名字
	static string app_name = "测试程序app";

	static unsigned long long _total_file_size = 0;//1 GB
	static  string _total_file_size_string;//1 GB

	static string _shortcut_exe_name="example.exe";
	static string _shortcut_name = "测试程序app";


}



// file name for uncompress log file
#define UNCOMPRESS_LOG_FILE "data.dat"

//10 MB for compress each fragment file size
#define  FRAGMENT_SIZE 1024*1024*10

//tmp  dir's file 's file for  get files for compressor
#define  TEM_WORK_FILE_NAME "dir.txt"


// data file name
#define  DATA_FILE_NAME "data"





/**
* @brief the class of  file's RAW  data info
*/
class FileBlock
{
public:
	unsigned int _size;
	string file_name;
	char *buffer;

	~FileBlock()
	{
		delete[]buffer;
	}
};

/**
* @brief a class of compressed block 's info
*/
class CompressBlock
{
public:
	unsigned  int _size;
	unsigned int _size_orign;
	string name;

	char *buffer;
	~CompressBlock()
	{
		delete[]buffer;
	}
};




/**
* @brief  a class for unpressed's  block  info
*/
class UnCompressBlock
{
public:
	unsigned int _size;
	string name;
	char *buffer;
	~UnCompressBlock()
	{
		delete[] buffer;
	}

};






//namespace core
//{


/*
文件名字  原大小  压缩后大小
1.pdf  12454  25425

*/

/**
* @brief  read a file to FileBlock*
*/
static FileBlock * ReadFile(const char *file_name);

/**
* @brief  write a file
*/
static void WriteFile(const char *name, char*buf, unsigned int len);





/**
* @brief  compress uncompress core class
*/
class Core
{
	/******************************  compress uncompress   ************************************/
public:
	/**
	* @brief uncompress a memory buffer
	* @param len  buffer size
	* @param len_orign  the size of uncompressed
	* @return UnCompressBlock*
	*/
	static UnCompressBlock*	UnCompressBuffer(char*buffer, unsigned int len, unsigned int len_orign);


	/**
	* @brief compress a memory buffer
	* @param _size  buffer size
	* @return CompressBlock*
	*/
	static CompressBlock* CompressBuffer(char * buffer, unsigned int _size);
	/******************************  end of   compress uncompress   ********************************/



public:
};



/**
 * @brief  a data structure for class FileReader
 */
class FileReaderBlock
{
public:
	unsigned int _size;
	string file_name;
	char *buffer;

	~FileReaderBlock()
	{
		delete[]buffer;
	}
};



/**
 * @brief   file reader  for a file which could read by much clip
 * @brief this will read a fragment size(FRAGMENT_SIZE ) rather than  whole file to memeroy
 *
 * @note provide a way to read a file by much clip rather than read the whole file
 * etc. a file which size is 1024mb. this can read this file by 10MB (FRAGMENT_SIZE )
 *   each time(by FileReader::ReadFragmentNext())
 */
class FileReader
{
public:
	string file_name;
	FILE *file = nullptr;
	unsigned long long  file_size;


	FileReader(string file_name);


	~FileReader();
	/**
	 * @brief read a fragment for FileReaderBlock*
	 * @note you can call this func in a while(...). to make sure read file done
	 * @return the fragment's structure(FileReaderBlock*)
	 */
	FileReaderBlock *ReadFragmentNext();


	/**
	 * @brief
	 */
	const unsigned long long GetCurrentOffset()const;

private:
	unsigned long long  _current_offset;

	FileReader(){}

	unsigned long long GetFragmentSize();

	/**
	 * @brief  read a _size file
	 * @return the buffer which include bin. data of  file _size 's offset
	 */
	char *ReadFileFragment(unsigned long long _size);


	/**
	* @brief  read a  fragment block from  file size
	*/
	FileReaderBlock *ReadFragment(unsigned long long _fragment_size);


	bool isEnd();

	void Init();
};






/**
 * @brief provide  a way to write multi block file and log in log.txt for UnCompress data
 * @warning this will append to the file so while writting the data file
 *   please fo not do other process of I/O of the data file
 * @note this will create a file which name is log.txt
 */
class FileWriter
{
public:
	FILE*file;
	FILE*filelog;


	string file_name;
	unsigned long long  _current_file_size;

	/**
	 * @brief  write a block to  data file
	 */
	void WriteCompressFile(CompressBlock *block);

	FileWriter(string file_name);

	~FileWriter();

	unsigned long long  GetCurrentFileSize()const;

	//total num of (out put list  this can for uncompress progress)
	unsigned int _num_of_file_fragment = 0;

private:
	/**
	 * @brief  append a log message to log.txt
	 */
	void WriteLog(string name, unsigned int len, unsigned int len_orign, FILE *filelog);

	unsigned long long _current_file_fragment_size;

	/**
	 * @brief  init and create the file log.txt
	 */
	void Init();
};


/**
* @brief this class is for uncompress
* @brief  write a standlone file which include much UnCompressBlock*
* @warning this will append to the exist file,so make sure there is not file in target_path
* @note the reason why to write multi  standlone file is ( difference by UnCompressBlock's file name
* if  current's file_name  and  last_time 's file name is same then append  or create a new file)
*/
class MultiFileWriter
{
public:

	/**
	 * @brief write a block to file
	 */
	void WriteUnCompressFile(UnCompressBlock *block);
private:
	string _last_name;
	FILE*file;
	FILE*filelog;
};

class  CompressDoneCallBackData;


/**
 * @brief  compress helper
 */
class CompressHelper
{
public:
	/**
	 * @brief add file name for process
	 */
	void AddFile(string file_name, string file_name_exp_path);

	/**
	 * @brief  process all file which is added to queue(by AddFile(...))
	 */
	void ProcessAllFile(FileWriter*writer);

	/**
	 * @brief  process a file by given function's param
	 */
	CompressDoneCallBackData* ProcessOneFile(FileWriter*writer, string file_name, string file_exp_name);


private:
	vector<string> _file_list;
	vector<string> file_name_exp_path_list;

	unsigned int _current_file_num = 0;
};






/**
 * @brief  directory mgr for create and parser directoy
 */
class DirectoryMgr
{
public:
	string target_dir;

	DirectoryMgr(){};

	vector<string> _queue_dir;// file 's directory

	vector<string> _queue_file;// file name  by absolute path

	vector<string> _queue_file_name;// file name  by  relative path

	/**
	 * @brief  parse the directoy by param's path
	 */
	void LoadDirectory(string path);

	DirectoryMgr(string name);

	/**
	 * @brief  create all directory
	 */
	void MakeAllDir();

	/**
	 * @brief  write all dir to file "dirs.txt" for uncompress
	 */
	void WriteToFile();

	/**
	* @brief  load dirs data from "dirs.txt" for compress
	*/
	void LoadWithFile();

	static void ParseConst_for_exe();

};








class CompressDoneCallBackData
{
public:
	string file_name;
	unsigned long long _cur_data_file_size = 0;
	unsigned long long _cur_file_size = 0;
	bool _is_done = false;

};




enum  class  ErrorCode
{
	ERROR_OPEN_FILE,
	ERROR_COMPRESS,
	ERROR_UNCOMPRESS,
	ERROR_UNKNOWN
};

//error call back for compress
typedef std::function<void(ErrorCode code, string msg)> ErrorCallBack;



class Compressor
{
public:
	Compressor(string path);


	typedef std::function<void(string   file_name,
		unsigned int _current_file_num, unsigned int _file_num) >  CompressingCallBack;


	// a compress done callback
	typedef std::function<void(CompressDoneCallBackData * _data)>  CompressDoneCallBack;



	void setErrorCallback(const ErrorCallBack &error)
	{
		this->error_call = error;
	}


	void setCompressCallback(const CompressingCallBack & callback, const CompressDoneCallBack &_call_done);

	void setDirectory(string dir);

	/**
	* @brief  start to compress
	* @warning  this function will create a new thread to compress
	* @warning just callbacks is thread-safe
	*/
	void Compress();

	string dir;
	Compressor(){};

private:
	CompressDoneCallBackData  _total;
	ErrorCallBack error_call = nullptr;
	CompressDoneCallBack _call_done = nullptr;
	CompressingCallBack _calback = nullptr;

	void setError(ErrorCode code, string msg);
};





class UnCompressor
{
public:

	UnCompressor(string path);

	typedef std::function<void(string   file_name, unsigned int _curr, unsigned int _total,
		unsigned long long _cur_size)>  UnCompressingCallBack;

	UnCompressingCallBack _calback = nullptr;

	void setCompressCallback(const UnCompressingCallBack & callback);

	void setUnCompressDoneCallback(const std::function <void(void)> &_call)
	{

		this->_call_done = _call;
	}

	/**
	 * @brief  start to uncompress
	 * @warning  this function will create a new thread to uncompress
	 * @warning just callbacks is thread-safe
	 */
	void UnCompress();

	string dir;

private:

	UnCompressor(){};
	std::function <void(void)> _call_done = nullptr;
	unsigned int GetMaxFileNumber()const;

	ErrorCallBack error_call = nullptr;
	void setError(ErrorCode code, string msg);
};


namespace Utils
{
	/**
	 * @brief a function to replace ::system  of hide black rect
	 */
	void system_hide_cmd(const string &cmd);

	/**
	 * @brief  ascii code to unicode
	 */
	static LPWSTR toUnicode(const char* str);

	/**
	* @brief  unicode to ascii
	*/
	static char *  toAscii(LPWSTR str);

	/**
	 * @brief  return is file or directory
	 */
	static bool isFile(string name);
	static bool isFileExist(string name);

	static void  CreateDir(string dir);

	static void DeleteDir(string dir)
	{  //RemoveDirectory 
	}
	static void DeleteFileForName(string name);

	static bool isDirExist(string dir);

	/**
	* @brief print the message in visual studio output windows
	*/
	static void  log(const char *str, ...);

	static unsigned long long GetFileSize(string file);

	/**
	 * @brief  return size brief such "500 KB"
	 */
	static std::string  GetStorageSizeBrief(const unsigned long long _size,
		const char*output_format = "%.1f");

	static std::string  GetStorageSizeBrief(const string _size, const char*output_format = "%.1f");

	/**
	 * @brief convert a string to unsigned long long
	 * @brief cause of atoi  is int not unsigned long long
	 */
	static	unsigned long long string_to_ull(const string num);
	static  string ull_to_string(unsigned long long _num);

};










#endif