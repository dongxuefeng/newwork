
#ifndef _check_file_path_h__
#define _check_file_path_h__




#include <stdio.h>
#include <string>
using namespace std;

//��bAddSpec Ϊ�棬��ȷ��szDst·��ĩβ����"\", ��bAddSpecΪ�٣���ȥ��lpszPathĩβ��"\"��"/"  
void ModifyPathSpec( char  * szDst, bool  bAddSpec )  ;
string& replace_all_distinct(string& str,const string& old_value,const string& new_value);

bool get_parent_dir(string &strdir );
bool check_file_path(const char * lpszDir);


#endif