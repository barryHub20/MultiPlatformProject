#include "AssetManager.h"

/*********************************************************************************************************
Load file from PC
/*********************************************************************************************************/
FILE* AssetManager::LoadFile(const char* address)
{
	FILE *fp = NULL;

    //open file----------------------------------------------------------------//
    fp = fopen(address, "rb");    //_s functions cannot be ported to other platforms



	return fp;
}