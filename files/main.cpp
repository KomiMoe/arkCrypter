#include "arkCrypter.h"
#include <iostream>

int main()
{
	printf(LifetimeText(skCrypt("TestString")));
	getchar();
	return 0;
}
