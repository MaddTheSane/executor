
#include "osx_file.h"
#include <dlfcn.h>
#include <stdbool.h>
#include <string.h>

struct OSXFSRef {
  unsigned char        hidden[80];
};
typedef struct OSXFSRef OSXFSRef;


static void *CoreServicesdlReference = NULL;


static int (*pFSPathMakeRef)(const unsigned char *path, OSXFSRef *ref, char *isDirectory) = NULL;

void osx_fileFixPercent(char* filename)
{

}

int osx_fileInit(void)
{
	CoreServicesdlReference = dlopen("/System/Library/Frameworks/CoreServices.framework/CoreServices", RTLD_LAZY);
	if (!CoreServicesdlReference)
		return 0;
	pFSPathMakeRef = dlsym(CoreServicesdlReference, "FSPathMakeRef");
}
