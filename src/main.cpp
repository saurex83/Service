#include <iostream>
#include "ServiceConfig.hpp"
#include "SerialCom.hpp"
#include <iostream>

int main(int, char*[])
{
	ServiceConfig test_obj;
	test_obj.Load();
	
	std::cout << "aaa";
	SerialCom com_test ;
	std::cout << "fff";
    return 0;
}





