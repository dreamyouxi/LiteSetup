#include "LiteSetup.h"








void testUncompressor()
{

	/*UnCompressor cl("C:\\Users\\hk\\Desktop\\LiteSetup\\out");

	cl.setCompressCallback([=](string name,unsigned int _curr, unsigned int _total,unsigned long long _c)
	{
		//cout <<   name << "   OK" << endl;

		cout << "[Log]:" << "UnCompress File:" << name <<"  "<< _curr*100/_total <<"  %" << endl;

	});

	cl.UnCompress();

	*/


}


void testCompressor()
{


	Compressor cl("C:\\Users\\hk\\Desktop\\cpp");


	cl.setCompressCallback([&](string name, int cur, int all)
	{

		cout << "[Log]:" << "Compress File:" << name << "   " << cur * 100 / all << "%" << endl;


	}, [&](CompressDoneCallBackData *_data)
	{

		if (_data->_is_done == true)return;


		cout << "[Log]:" << "Compress File:" << _data->file_name << "" << endl;


	});




	cl.setErrorCallback([&](ErrorCode code, string msg)
	{
	
	
		cout <<" ..... error  ..." <<msg << endl;


	});





	cl.Compress();

	


}




int main(int argc, char* argv[])
{



	testCompressor();

	//testUncompressor();

 


	system("pause");
	return 0;
}


