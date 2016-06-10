#include "LiteSetup.h"
#include "shlwapi.h"

#pragma comment(lib, "shlwapi.lib")

static char log_buffer[5000000];// 5mb for buffer size

static mutex _mutex_core_compressor;

static mutex _mutex_core_uncompressor;

FileBlock * ReadFile(const char *file_name)
{

	unsigned long long   _size = 0;

	FILE *file = fopen(file_name, "rb");
	FileBlock* ret = new FileBlock;


	_fseeki64(file, 0, SEEK_END);

	_size = ftell(file);

	_fseeki64(file, 0, SEEK_SET);

	ret->_size = _size;
	ret->file_name = file_name;


	char *buf = new char[_size];

	fread(buf, 1, _size, file);

	ret->buffer = buf;
	fclose(file);

	return ret;

}




void WriteFile(const char *name, char*buf, unsigned int len)
{
	FILE *file = fopen(name, "wb");
	fwrite(buf, 1, len, file);
	fclose(file);
}






UnCompressBlock*	Core::UnCompressBuffer(char*buffer, unsigned int len, unsigned int len_orign)
{
	UnCompressBlock *ret = new UnCompressBlock;

	char *dest = new char[len_orign + 10];


	if (0 == uncompress((Bytef*)dest, (uLongf*)&len_orign, (Bytef*)buffer, len))
	{
		ret->buffer = dest;
		ret->_size = len_orign;

		return ret;
	}

	delete[] dest;

	return nullptr;
}


CompressBlock* Core::CompressBuffer(char * buffer, unsigned int _size)
{
	CompressBlock *ret = new CompressBlock;

	unsigned int len = compressBound((uLong)_size);

	char * dest = new char[len];

	if (compress((Bytef*)dest, (uLongf*)&len, (Bytef*)buffer, _size) == 0)
	{
		ret->buffer = dest;
		ret->_size = len;
		ret->_size_orign = _size;

		//	cout << __FUNCTION__ << "  " << len << "  " << _size << endl;;

		return ret;
	}

	delete[]dest;
	return nullptr;
}



LPWSTR Utils::toUnicode(const char* _str)
{
	LPWSTR _ret;
	int _len = strlen(_str) * 2;

	_ret = new WCHAR[_len];

	MultiByteToWideChar(CP_ACP, 0, _str, -1, _ret, _len);

	return _ret;
}



void  Utils::log(const char *str, ...)
{

	va_list l;
	va_start(l, str);

	vsprintf(log_buffer, str, l);

	va_end(l);
	strcat(log_buffer, "\n\0");

	//	OutputDebugString("Lite2D:");
	OutputDebugStringA(log_buffer);

	//	printf(buffer);


}


unsigned long long  Utils::GetFileSize(string file_name)
{

	FILE *file = fopen(file_name.c_str(), "rb");

	_fseeki64(file, 0, SEEK_END);

	unsigned long long ret = _ftelli64(file);

	fclose(file);

	return ret;
}


void   Utils::system_hide_cmd(const string &cmd)
{
	WCHAR path[MAX_PATH];
	GetSystemDirectoryW(path, MAX_PATH);
	string x = Utils::toAscii(path);
	x += "\\cmd.exe";

	SHELLEXECUTEINFOW ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = Utils::toUnicode(x.c_str());
	ShExecInfo.lpParameters = Utils::toUnicode(cmd.c_str());
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteExW(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);////wait for exit

}


char *  Utils::toAscii(LPWSTR str)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP,
		0,
		str,
		-1,
		NULL,
		0,
		NULL,
		NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char)* (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,
		0,
		str,
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL);
	string strText;
	strText = pElementText;
	//delete[] pElementText;
	return pElementText;
}


bool Utils::isFile(string name)
{
	FILE *file = fopen(name.c_str(), "rb");

	if (file)
	{

		fclose(file);
		return true;
	}
	return false;

}

bool Utils::isFileExist(string name)
{
	return Utils::isFile(name);
}


void Utils::DeleteFileForName(string name)
{  // DeleteFile 

	DeleteFileW(Utils::toUnicode(name.c_str()));



}



bool Utils::isDirExist(string dir)
{
	return	(bool)PathIsDirectoryW(Utils::toUnicode(dir.c_str()));


}




std::string  Utils::GetStorageSizeBrief(const unsigned long long _size,
	const char*output_format)
{
	int len = 0;
	unsigned long long _file_size_tmp = _size;
	while (_file_size_tmp /= 1024)
	{
		len++;
	}

	static  char * _size_array[5] = { " Bytes", " KB", " MB", " GB", " TB" };

	_file_size_tmp = _size;


	double _file_size_double = 0;

	char buf[200] = { 0 };

	if (len == 0)
	{
		// b
		sprintf(buf, "%d", _file_size_tmp);
	}
	else if (len == 1)
	{
		//kb
		_file_size_double = _file_size_tmp / 1024.0;
		sprintf(buf, output_format, _file_size_double);

	}
	else if (len == 2)
	{
		//mb
		_file_size_double = _file_size_tmp / 1024.0 / 1024.0;
		sprintf(buf, output_format, _file_size_double);
	}
	else if (len == 3)
	{
		//gb
		_file_size_tmp /= 1024;
		_file_size_double = _file_size_tmp / 1024.0 / 1024.0;
		sprintf(buf, output_format, _file_size_double);

	}
	else if (len == 4)
	{
		//tb
		_file_size_tmp /= 1024;
		_file_size_tmp /= 1024;
		_file_size_double = _file_size_tmp / 1024.0 / 1024.0;
		sprintf(buf, output_format, _file_size_double);
	}

	string ret = buf;
	ret += _size_array[len];


	return (ret);
}


std::string   Utils::GetStorageSizeBrief(const string _size, const char*output_format)
{
	unsigned long long _file_size = 0;
	{
		unsigned long long des = 1;
		for (int i = _size.size() - 1;
			i >= 0; i--, des *= 10)
		{
			_file_size += des*(_size[i] - '0');
		}
	}
	return Utils::GetStorageSizeBrief(_file_size, output_format);
}

string  Utils::ull_to_string(unsigned long long _num)
{
	const unsigned long long  _div = 10000000;

	unsigned long long    upper_part = _num / _div;
	unsigned long long  lower_part = _num - upper_part * _div;

	char buf[100];

	memset(buf, 0, sizeof (char)* 100);

	itoa((int)upper_part, buf, 10);
	string ret = buf;

	memset(buf, 0, sizeof (char)* 100);
	itoa((int)lower_part, buf, 10);
	ret.append(buf);


	while (ret[0] == '0')
	{
		ret = ret.substr(1, -1);

	}

	return ret;

}


unsigned long long  Utils::string_to_ull(const string num)
{
	unsigned long long _file_size = 0;
	unsigned long long des = 1;
	for (int i = num.size() - 1;
		i >= 0; i--, des *= 10)
	{
		_file_size += des*(num[i] - '0');
	}
	return _file_size;
}



void   Utils::CreateDir(string dir)
{
	if (Utils::isDirExist(dir) == true)return;



	if (CreateDirectoryW(Utils::toUnicode(dir.c_str()), 0) == 0)
	{
		//error
		// not multi level dir
		if (string::npos == dir.find('\\'))return;

		int i = dir.size();
		for (; i >= 0; i--)
		{
			if (dir[i] == '\\')break;;

		}
		CreateDir(dir.substr(0, i));

		CreateDirectoryW(Utils::toUnicode(dir.c_str()), 0);

	}

}




FileReader::FileReader(string file_name)
{
	this->file_name = file_name;
	this->Init();
}


FileReader:: ~FileReader()
{
	fclose(file);
}


FileReaderBlock *FileReader::ReadFragment(unsigned long long _fragment_size)
{

	FileReaderBlock* ret = new FileReaderBlock;
	ret->file_name = file_name;
	ret->_size = _fragment_size;

	ret->buffer = ReadFileFragment(_fragment_size);

	//this->SetOffset(_fragment_size);


	return ret;

	return nullptr;
}


FileReaderBlock *FileReader::ReadFragmentNext()
{
	unsigned long long fragmentsize = GetFragmentSize();

	if (fragmentsize == 0)return nullptr;

	FileReaderBlock*block = ReadFragment(fragmentsize);

	return block;


}

const unsigned long long FileReader::GetCurrentOffset()const
{
	return _current_offset;
}






unsigned long long FileReader::GetFragmentSize()
{
	if (_current_offset + FRAGMENT_SIZE <= file_size)
	{
		return FRAGMENT_SIZE;
	}
	else
	{
		return file_size - _current_offset;

	}
}



char * FileReader::ReadFileFragment(unsigned long long  _size)
{
	char *buffer = new char[_size];
	fread(buffer, 1, _size, this->file);
	_current_offset += _size;

	return buffer;
}

bool FileReader::isEnd()
{
	return _current_offset == file_size;
}



void FileReader::Init()
{
	file = fopen(file_name.c_str(), "rb");

	if (file)
	{

		FileBlock* ret = new FileBlock;

		_fseeki64(file, 0, SEEK_END);

		file_size = _ftelli64(file);

		_fseeki64(file, 0, SEEK_SET);
		_current_offset = 0;

	}
	else
	{

	}
}


void  FileWriter::WriteCompressFile(CompressBlock *block)
{

	fwrite(block->buffer, 1, block->_size, file);

	WriteLog(block->name.c_str(), block->_size, block->_size_orign, filelog);
	_current_file_size += block->_size;
	_num_of_file_fragment++;



	//cout << "  file  log  " << block->name << endl;

}

FileWriter::FileWriter(string file_name)
{
	this->file_name = file_name;
	this->Init();
}

FileWriter::~FileWriter()
{
	fclose(file);
	WriteLog("end", 0, _num_of_file_fragment, filelog);
	fclose(filelog);
}

unsigned long long   FileWriter::GetCurrentFileSize()const
{
	return _current_file_size;
}



void  FileWriter::WriteLog(string name, unsigned int len, unsigned int len_orign, FILE *filelog)
{
	char str[500];
	sprintf(str, "%s %d %d\n", name.c_str(), len_orign, len);


	fwrite(str, 1, strlen(str), filelog);
}



void   FileWriter::Init()
{
	file = fopen(file_name.c_str(), "wb");

	filelog = fopen(UNCOMPRESS_LOG_FILE, "a");


	const char* str = ".CMD_FILE.\n";

	fwrite(str, 1, strlen(str), filelog);



	_current_file_size = 0;
	_current_file_fragment_size = 0;

}




void MultiFileWriter::WriteUnCompressFile(UnCompressBlock *block)
{
	if (block->name != _last_name)
	{//difference file;

		file = fopen(block->name.c_str(), "wb");
		fwrite(block->buffer, 1, block->_size, file);
		fclose(file);
		_last_name = block->name;
	}
	else
	{
		//same file
		file = fopen(block->name.c_str(), "ab");
		fwrite(block->buffer, 1, block->_size, file);
		fclose(file);
		_last_name = block->name;
	}
	//	cout << __FUNCTION__ << endl;

}





void  CompressHelper::AddFile(string file_name, string file_name_exp_path)
{
	this->_file_list.push_back(file_name);
	this->file_name_exp_path_list.push_back(file_name_exp_path);


}

void  CompressHelper::ProcessAllFile(FileWriter*writer)
{
	for (unsigned int i = 0; i < _file_list.size(); i++)
	{
		this->ProcessOneFile(writer, _file_list[i], file_name_exp_path_list[i]);

	}

}

CompressDoneCallBackData *  CompressHelper::ProcessOneFile(FileWriter*writer, string file_name,
	string file_exp_name)
{

	CompressDoneCallBackData * _ret = new CompressDoneCallBackData;


	{
		FileReader *reader = new FileReader(file_name);


		if (reader->file == nullptr)
		{

			delete _ret;

			return nullptr;
		}



		FileReaderBlock*block = reader->ReadFragmentNext();


		if (block == nullptr)
		{
			delete _ret;
			return nullptr;
		}


		_ret->file_name = file_name;

		while (block)
		{
			//cout << "     ..   " << (int)block->buffer << "   " << block->_size << endl;

			CompressBlock* cblock = Core::CompressBuffer(block->buffer, block->_size);

			if (cblock == nullptr)
			{
				delete _ret;

				return nullptr;
			}

			cblock->name = file_exp_name;

			cout << " ...........  " << file_exp_name.c_str() << endl;
			//	cout << " Compress File " << file_exp_name.c_str() << endl;;

			writer->WriteCompressFile(cblock);

			_ret->_cur_file_size += (unsigned long long) block->_size;
			_ret->_cur_data_file_size += (unsigned long long) cblock->_size;

			delete cblock;
			delete block;

			block = reader->ReadFragmentNext();
			//cout << endl;
		}

		delete reader;
	}

	return  _ret;
}




void DirectoryMgr::LoadDirectory(string path)
{
	this->target_dir = path;

	string cmd_txt = "/c  DIR  " + path + "  /S /B   >   ";
	cmd_txt += TEM_WORK_FILE_NAME;


	Utils::system_hide_cmd(cmd_txt.c_str());

	fstream f;

	f.open(TEM_WORK_FILE_NAME, ios::in);

	vector<string> _queue;

	string buf;


	while (getline(f, buf))
	{
		//	cout << buf << "       ... " << endl;
		//	cout<<buffer;
		int len = path.size() - 2;

		string file_ = buf.substr(len + 1, buf.size() - len);

		//	cout << file_.find('.') << endl;

		bool isDir = !Utils::isFile(buf);

		for (int i = buf.size() - 1; i >= 0; i--)
		{

			if (buf[i] == '\\')
			{
				file_ = buf.substr(path.size() - 1, -1);
				//	cout<<"                  ...  " << file_ <<endl;
				break;;

			}
		}


		if (isDir)
		{
			//directory
			_queue_dir.push_back(file_);
		}
		else
		{
			if (Utils::GetFileSize(buf) == 0)continue;

			//file		
			cout << " ... 000  ..  " << file_ << "     " << path << endl;
			_queue_file.push_back(buf);
			const_for_exe::_total_file_size += Utils::GetFileSize(buf);

			_queue_file_name.push_back(file_);
		}

	}
	//	this->WriteToFile();
	f.close();

	Utils::DeleteFileForName(TEM_WORK_FILE_NAME);



}

DirectoryMgr::DirectoryMgr(string name)
{
	this->target_dir = name;
}





void DirectoryMgr::MakeAllDir()
{
	//make root dir


	/*	string cmd = "mkdir  \"" + target_dir;
		cmd += "\"";

		system(cmd.c_str());
		*/

	string cmd = "\"" + target_dir;
	cmd += "\"";;
	Utils::CreateDir(cmd);


	//make sub dir
	for (unsigned int i = 0; i < _queue_dir.size(); i++)
	{
		/*cmd = "mkdir  \"";
		cmd += target_dir;
		cmd += "\\";

		cmd += _queue_dir[i];
		cmd += "\"";

		system(cmd.c_str());
		*/

		//	cmd = "\"";
		cmd = "";
		cmd += target_dir;
		cmd += "\\";

		cmd += _queue_dir[i];
		//	cmd += "\"";



		Utils::CreateDir(cmd);


	}
}




void DirectoryMgr::WriteToFile()
{
	fstream f;
	f.open(UNCOMPRESS_LOG_FILE, ios::out);

	{
		//write 
		f << ".CMD_INFO." << endl;

		f << const_for_exe::app_name << endl;
		f << "\\" << const_for_exe::dir_install << endl;
		f << const_for_exe::_total_file_size << endl;
		f << const_for_exe::_shortcut_exe_name << endl;
		f << const_for_exe::_shortcut_name << endl;

	}

	{//write dirs
		f << ".CMD_DIRS." << endl;


		for (unsigned int i = 0; i < _queue_dir.size(); i++)
		{
			f << _queue_dir[i] << endl;

		}


	}
	f.close();
}



void DirectoryMgr::ParseConst_for_exe()
{

	fstream f;
	f.open(UNCOMPRESS_LOG_FILE, ios::in);
	string buf;

	::getline(f, buf);




	::getline(f, buf);
	if (buf != ".CMD_DIRS.")
	{
		const_for_exe::app_name = buf;
	}

	::getline(f, buf);
	if (buf != ".CMD_DIRS.")
	{
		const_for_exe::dir_install = buf;
	}


	::getline(f, buf);
	if (buf != ".CMD_DIRS.")
	{
		const_for_exe::_total_file_size_string = buf;
		const_for_exe::_total_file_size = Utils::string_to_ull(buf);
	}

	::getline(f, buf);
	if (buf != ".CMD_DIRS.")
	{
		const_for_exe::_shortcut_exe_name = buf;
	}
	::getline(f, buf);
	if (buf != ".CMD_DIRS.")
	{
		const_for_exe::_shortcut_name = buf;
	}


	f.close();


}



void DirectoryMgr::LoadWithFile()
{
	_queue_dir.clear();

	fstream f;
	f.open(UNCOMPRESS_LOG_FILE, ios::in);
	string buf;


	while (::getline(f, buf))
	{
		if (buf == ".CMD_DIRS.")break;
	}


	while (::getline(f, buf))
	{
		if (buf == ".CMD_FILE.")break;

		_queue_dir.push_back(buf);
	}
	f.close();


}







Compressor::Compressor(string path)
{
	this->dir = path;
	this->dir = "\"" + this->dir;
	this->dir += "\"";

}


void   Compressor::setCompressCallback(const CompressingCallBack & callback, const CompressDoneCallBack &_call_done)
{
	this->_calback = callback;
	this->_call_done = _call_done;
}

void  Compressor::setDirectory(string dir)
{
	this->dir = dir;
}



void   Compressor::setError(ErrorCode code, string msg)
{
	_mutex_core_compressor.lock();
	error_call(code, msg);
	_mutex_core_compressor.unlock();

}



void  Compressor::Compress()
{
	std::thread t([&]{


		//	Utils::CreateDir("Output");


		// scanf files
		DirectoryMgr mgr;

		FileWriter*writer = new FileWriter(DATA_FILE_NAME);

		CompressHelper helper;

		mgr.LoadDirectory(this->dir);

		mgr.WriteToFile();


		unsigned long long _cur_data_size = 0;

		for (unsigned int i = 0; i < mgr._queue_file.size(); i++)
		{

			helper.AddFile(mgr._queue_file[i], mgr._queue_file_name[i]);

			cout << mgr._queue_file_name[i] << "    queue  " << endl;

			if (_calback)
			{
				_mutex_core_compressor.lock();
				_calback(mgr._queue_file[i], 1 + i, mgr._queue_file.size());
				_mutex_core_compressor.unlock();
			}

			auto param = helper.ProcessOneFile(writer, mgr._queue_file[i], mgr._queue_file_name[i]);

			if (param == nullptr)
			{
				this->setError(ErrorCode::ERROR_COMPRESS, "compress file:" +
					mgr._queue_file[i] + " error");

				return;

			}


			_total.file_name = param->file_name;
			_total._cur_data_file_size += param->_cur_data_file_size;
			_total._cur_file_size += param->_cur_file_size;
			_total._is_done = param->_is_done;

			delete param;

			if (_call_done)
			{
				_mutex_core_compressor.lock();
				_call_done(&_total);
				_mutex_core_compressor.unlock();
			}

		}
		//	helper.ProcessAllFile(writer);
		delete writer;

		_total._is_done = true;

		if (_call_done)
		{
			_mutex_core_compressor.lock();
			_call_done(&_total);
			_mutex_core_compressor.unlock();
		}

	});	t.detach();




}




UnCompressor::UnCompressor(string path)
{
	this->dir = path;
}



void  UnCompressor::setCompressCallback(const UnCompressingCallBack & callback)
{
	this->_calback = callback;


}




unsigned int UnCompressor::GetMaxFileNumber()const
{
	unsigned int max_file_num = 0;

	{// just for get uncompress file num for progress
		fstream filelog;
		filelog.open(UNCOMPRESS_LOG_FILE, ios::in);
		{
			string buf;

			while (getline(filelog, buf))
			{
				if (buf == ".CMD_FILE.")break;
			}
		}

		int i = 0;
		while (true)
		{
			//filelog >> name >> len_orign >> len;		
			unsigned int len_orign = 0;
			unsigned int len = 0;
			string name;
			//parser  three data
			getline(filelog, name);
			{
				//	cout << name<<"        ...." << endl;
				int index = 0;
				for (int i = name.size() - 1; i >= 0; i--)
				{
					if (name[i] == ' ' && index == 0)
					{
						index = i;
						len = atoi(name.substr(i + 1, name.size() - i).c_str());
						continue;
					}
					if (name[i] == ' ' && index != 0)
					{
						len_orign = atoi(name.substr(i, index - i).c_str());
						name = name.substr(0, i);
						break;
					}
				}
			}
			// len_orign is for number of file list size
			if (len == 0 && strcmp("end", name.c_str()) == 0)
			{
				max_file_num = len_orign;
				break;
			}
		}
		filelog.close();
	}
	return max_file_num;
}


void   UnCompressor::setError(ErrorCode code, string msg)
{
	_mutex_core_uncompressor.lock();
	error_call(code, msg);
	_mutex_core_uncompressor.unlock();

}

void  UnCompressor::UnCompress()
{

	std::thread t([&]{

		//process dirs 
		{
			DirectoryMgr mgr(dir);
			mgr.LoadWithFile();
			mgr.MakeAllDir();
		}

		MultiFileWriter writer;

		unsigned int max_file_num = this->GetMaxFileNumber();

		fstream filelog;
		filelog.open(UNCOMPRESS_LOG_FILE, ios::in);
		{
			string buf;
			while (getline(filelog, buf))
			{
				if (buf == ".CMD_FILE.")break;

			}
		}


		int i = 0;

		FILE *file = fopen(DATA_FILE_NAME, "rb");
		unsigned long long _cur_toital_size = 0;
		while (true)
		{
			//filelog >> name >> len_orign >> len;		
			unsigned int len_orign = 0;
			unsigned int len = 0;
			string name;

			//parser  three data
			getline(filelog, name);
			{
				//	cout << name<<"        ...." << endl;
				int index = 0;
				for (int i = name.size() - 1; i >= 0; i--)
				{
					if (name[i] == ' ' && index == 0)
					{
						index = i;
						len = atoi(name.substr(i + 1, name.size() - i).c_str());
						continue;
					}
					if (name[i] == ' ' && index != 0)
					{
						len_orign = atoi(name.substr(i, index - i).c_str());
						name = name.substr(0, i);
						break;
					}
				}
			}


			// len_orign is for number of file list size
			if (len == 0 && strcmp("end", name.c_str()) == 0)break;

			//if the size bigger than compress'block  fix bug
			// if len_orign < len ,the will memory error 
			// 1MB will be plus to the buffer for uncompress
			char *buffer = new char[len_orign + 1024 * 1024];


			//	cout << i << "  " << name << " " << len_orign << "  " << len << endl;
			//Un comppress

			fread(buffer, 1, len, file);


			UnCompressBlock *block = Core::UnCompressBuffer(buffer, len, len_orign);
			if (nullptr == block)
			{
				this->setError(ErrorCode::ERROR_UNCOMPRESS, "error uncompress");

			}



			_cur_toital_size += block->_size;


			if (_calback)
			{
				_mutex_core_uncompressor.lock();
				_calback(name, i + 1, max_file_num, _cur_toital_size);
				_mutex_core_uncompressor.unlock();
			}


			string x = this->dir; x += "\\"; x += name;	block->name = x;

			writer.WriteUnCompressFile(block);

			delete[] buffer;

			delete block;
			++i;
		}


		if (_call_done)
		{

			_mutex_core_uncompressor.lock();
			_call_done();
			_mutex_core_uncompressor.unlock();
		}
		fclose(file);


	});



	t.detach();

}

