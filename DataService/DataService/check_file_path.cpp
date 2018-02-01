#include "check_file_path.h"
#include <shlwapi.h>
#include <direct.h>


void ModifyPathSpec( char  * szDst, bool  bAddSpec )  
{  
    int nLen = lstrlen(szDst);  

    char  ch  = szDst[ nLen - 1 ];  
    if( ( ch == ('\\') ) || ( ch == ('/') ) )  
    {  
        if( !bAddSpec )  
        {  
            *(szDst + nLen - 1 ) = ('\0');  
        }  
    }  
    else  
    {  
        if( bAddSpec )  
        {  
            *(szDst + nLen ) = ('\\');  
            *(szDst + nLen + 1 ) = ('\0');  
        }  
    }  
};  

string& replace_all_distinct(string& str,const string& old_value,const string& new_value) 
{ 
	for (string::size_type pos(0); pos!=string::npos; pos+=new_value.length()) 
	{ 
		if( (pos=str.find(old_value,pos))!=string::npos ) 
			str.replace(pos,old_value.length(),new_value); 
		else break; 
	} 
	return str; 
} ;


bool get_parent_dir(string &strdir )
 {
	 //strdir.replace(strdir.begin(),strdir.end(),'\\','/');
	  replace_all_distinct(strdir,"\\","/") ;
	 int itmp = strdir.rfind('/');
	
	 strdir.at(itmp) = '\0';
	 return true;

 }

bool check_file_path(const char * lpszDir)
{
	//��֤�ļ�Ŀ¼�Ƿ����
    if(::PathIsDirectory(lpszDir )) return TRUE;  
  
    TCHAR   szPreDir[ MAX_PATH ];  
    sprintf( szPreDir, lpszDir );  
    //ȷ��·��ĩβû�з�б��  
    ModifyPathSpec( szPreDir, false );  
  
    //��ȡ�ϼ�Ŀ¼  
   // BOOL  bGetPreDir  = ::PathRemoveFileSpec( szPreDir );  

	string strtmp = szPreDir;
	BOOL  bGetPreDir = get_parent_dir(strtmp);
    if( !bGetPreDir ) return FALSE;  
  
    //����ϼ�Ŀ¼������,��ݹ鴴���ϼ�Ŀ¼  
    if( !::PathIsDirectory( strtmp.c_str() ) )  
    {  
        check_file_path( (char *)strtmp.c_str() );  
    }  
   // return ::CreateDirectory( lpszDir, NULL );  
	return mkdir(lpszDir)==0;
};
