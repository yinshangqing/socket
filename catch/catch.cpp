/*************************************************************************
	> File Name: catch.cpp
	> Author: yinshangqing
	> Mail: 841668821@qq.com 
	> Created Time: 2018年02月02日 星期五 15时33分38秒
 ************************************************************************/

#include <iostream> 
#include <string>
#include <exception>  

using namespace std;  

int main ()
{  
    try  
	{  
		//throw 1;  
		throw "error";  
	}  
	catch(double)  
	{  
		cout << ".........." << endl;
		cout << "this is a test !!!" << endl;  
	}  
	catch(string i)  
	{  
		cout << i << endl;  
	} 
	catch(char *str)
	{
		cout << str << endl;
	}
	catch(char const *str)
	{
		cout << str << endl;
	}
}
