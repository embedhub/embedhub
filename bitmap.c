#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "bitmap.h"

int bitmap_init(unsigned int num)
{
	bitmap = (unsigned char *)malloc(sizeof(unsigned char) * num);
	if(bitmap == NULL)
	{
		printf("bitmap init error:%s", strerror(errno));
		return -1;
	}
	memset(bitmap, 0, sizeof(unsigned char)*num);
	
	SIZEOFBITMAP = num;
	
	return 0;
}

void bitmap_destroy(void)
{
	free(bitmap);
}

int getbitmap(void)
{
	int i, j;
	
	for(i = 0; i < SIZEOFBITMAP/8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			//if( (bitmap[i] >> j) & 0x01 )
			if( getbit(bitmap[i], j) )
			{
				continue;
			}
			else
			{
				return (i*8+j);
			}
		}
	}
	
	return -1;
}

int setbitmap(int bit)
{
	int hang, lie;
	
	if(bit >= SIZEOFBITMAP)
		return -1;
		
	hang = bit / 8;
	lie = bit % 8;
	
	//bitmap[hang] |= (0x01<<lie);
	setbit(bitmap[hang], lie);
	
	return 0;
}

int resetbitmap(int bit)
{
	int hang, lie;
	
	if(bit >= SIZEOFBITMAP)
		return -1;
		
	hang = bit / 8;
	lie = bit % 8;
	
	//bitmap[hang] &= (~(0x01<<lie));
	resetbit(bitmap[hang], lie);
	
	return 0;
}