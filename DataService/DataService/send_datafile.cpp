#include "public.h"
#include "file_ctrl.h"
#include <vector>
using namespace std;

int findstr(const char *str,const char * substr)
{
	int pos = 0;
	char * pstr = (char *)str;
	int lenstr = strlen(str);
	int lensub = strlen(substr);
	bool bresult = false;

	for(int i=0;i<lenstr;i++)
	{
		pstr = (char *)str+i;
		if(*pstr == *substr)
		{
			bresult = true;
			for(int i=0;i<lensub;i++)
			{

				if(*(pstr+i) != *(substr+i))
				{
					bresult = false;
					break;
				}
			}

			if(bresult)
			{
				pos = i+1;
				break;
			}

		}

	}

		return pos;
}

int find_file(vector<string> * filelist,string mspath,string sub_path="")
{
	int  findHandle;                                 // 查找文件句柄
	char fileSpec[256];                              // 查找文件条件
	struct _finddata_t  fileInfo;                    // 文件信息
	sprintf(fileSpec, "%s/%s%/*.*", mspath.c_str(), sub_path.c_str()); 

	/*   _sFileName.c_str()表示的是匹配的文件名：比如：*.txt
	*   sPath.c_str() 是文件的路径
	*/

	findHandle = _findfirst(fileSpec, &fileInfo);   // 开始查找
	if (findHandle != -1)                            // 找到文件
	{
		do
		{
			if (fileInfo.attrib & _A_SUBDIR)
			{
				//cout << (strcmp(fileInfo.name,"."))<< endl;
				if((strcmp(fileInfo.name,"."))&&strcmp(fileInfo.name,".."))
				{
					string tmpsub_path = sub_path;
					int len = tmpsub_path.length();
					if(len != 0)
					{
						len -= 1;
					}
					if(tmpsub_path[len] != '/')
					{
						tmpsub_path += "/";
					}
					tmpsub_path += fileInfo.name;
					find_file(filelist,mspath,tmpsub_path);
				};
			};
			if (fileInfo.attrib & _A_ARCH)  // 如果是文档，输出文件名            
			{ 

				//int ipos = findstr(fileInfo.name,"*.jpg");
				int ipos = 1;
				if(ipos)
				{
					
					cout<<"文件名:"     << fileInfo.name << endl;				
					string strfilename = sub_path;
					strfilename += "/";
					strfilename += fileInfo.name;
					filelist->push_back(strfilename);
					if(filelist->size()>= max_find_count)
					{
						break;
					}
				}
			}
		}while(_findnext(findHandle, &fileInfo) != -1);    // 查找下一个文件
	};

	_findclose(findHandle);                          // 关闭查找
	return (int)filelist->size();	
};

//bool send_datafile
#include "xxlib\plugin\user_plug_prcdata.h"
bool send_datafile(const char * m_path)
{

	//find datafile 每组50个
	vector<string> file_list;
	int ifilecount = find_file(&file_list,m_path);
	//for
	for(vector<string>::iterator iter = file_list.begin();
		iter != file_list.end();
		iter ++)
	{
		char strdata[256];
		char strref[1024];

		string strtmp = m_path;
		strtmp += "/";
		strtmp += iter->c_str();

		string ftpfile = g_local_dir.m_backdir;
		ftpfile += "/";
		ftpfile += iter->c_str();

		string ftpfail = g_local_dir.m_faildir;
		ftpfail += "/";
		ftpfail += iter->c_str();


		sprintf(strdata, "%s", iter->c_str());
		int iresult_code = 0;
		int ipos = findstr(strdata, "_hik.jpg");
		if (ipos)
		{
			plug_proc_data(strdata,&iresult_code, strref);
		}

		if (iresult_code == 0)
		{
			string strdir = ftpfile.c_str();
			get_parent_dir(strdir);
			check_file_path(strdir.c_str());

			for (int i = 0; i < 100; i++)
			{
				BOOL bmove =
					MoveFile(strtmp.c_str(), ftpfile.c_str());
				if (bmove)
				{
					break;
				}
				else
				{

					DWORD errno1 = GetLastError();
						printf("move file is fail!,error(%d)", errno1);
						if (errno1 == 183)
						{
							DeleteFile(strtmp.c_str());
							break;
						}
						

					Sleep(1000);
				};

			}

		}	   
		else
		{
			string strdir = ftpfail.c_str();
			get_parent_dir(strdir);
			check_file_path(strdir.c_str());

			check_file_path(strtmp.c_str());

			for (int i = 0; i < 100; i++)
			{
				BOOL bmove =
					MoveFile(strtmp.c_str(), ftpfail.c_str());
				if (bmove)
				{

					break;
				}
				else
				{
					DWORD errno1 = GetLastError();
					printf("move file is fail!,error(%d)", errno1);
					if (errno1 == 183)
					{
						DeleteFile(strtmp.c_str());
						break;
					}
					Sleep(1000);
				};

			}


		}
		
		/*
		//send datafile	
		string strtmp = m_path;
		strtmp += "/";
		strtmp += iter->c_str();

		//ftp datafile
		string ftpfile = g_ftp_dir.m_datafile;
		ftpfile += "/";
		ftpfile += iter->c_str();
		//

		cout << "send file ..." << strtmp;
		if(g_ftp_info.send_ftp_file(strtmp.c_str(),ftpfile.c_str()))
		{
			cout << "succer" << endl;
			remove(strtmp.c_str());
		}
		else
		{
			cout << "fail" << endl;
		}
		*/


	};
	return true;
};

//通过线程调用发送
//datafile
unsigned int __stdcall thread_send_datafile(void * param)
{
	_run_station * p_run_station = (_run_station *)param;
	while (p_run_station->get_work())
		{
			send_datafile(g_local_dir.m_datafile.c_str());
			Sleep(1000);
		}
	return 1;
}

