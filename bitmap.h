#ifndef __BITMAP_H__
#define __BITMAP_H__

#define getbit(hex,bit)		((hex) & (1<<(bit)))	//获取hex的第bit位是0还是1
#define _getbit(hex,bit)	( ((hex) & (1<<(bit))) >> bit )	//获取hex的第bit位是0还是1并且移位
#define setbit(hex,bit)		((hex) |= (1<<(bit)))	//设置hex的第bit位为1
#define resetbit(hex,bit)	((hex) &= ( ~(1<<(bit))))//设置hex的第bit位为0
#define inverbit(hex,bit)	(getbit(hex,bit)?resetbit(hex,bit):setbit(hex,bit))//翻转hex的第bit位

unsigned int SIZEOFBITMAP;

unsigned char *bitmap;

extern int bitmap_init(unsigned int num);
void bitmap_destroy(void);
extern int getbitmap(void);
extern int setbitmap(int bit);
extern int resetbitmap(int bit);

#endif