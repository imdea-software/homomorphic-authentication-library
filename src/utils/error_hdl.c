#include "error_hdl.h"

void error_hdl(int error, char * msg)
{
	printf("Error: %s", msg);
	if (error != -1)
	{
		printf("\t\t\t(at round %d)", error);
	}
	printf("\n");
}