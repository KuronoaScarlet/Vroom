#ifndef UUIDRANDOM
#define UUIDRANDOM

unsigned int generateUUID()
{
	return rand() % 4294967295;
}

#endif