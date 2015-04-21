#ifndef __BITMAP_H__
#define __BITMAP_H__

#define getbit(hex,bit)		((hex) & (1<<(bit)))	//��ȡhex�ĵ�bitλ��0����1
#define _getbit(hex,bit)	( ((hex) & (1<<(bit))) >> bit )	//��ȡhex�ĵ�bitλ��0����1������λ
#define setbit(hex,bit)		((hex) |= (1<<(bit)))	//����hex�ĵ�bitλΪ1
#define resetbit(hex,bit)	((hex) &= ( ~(1<<(bit))))//����hex�ĵ�bitλΪ0
#define inverbit(hex,bit)	(getbit(hex,bit)?resetbit(hex,bit):setbit(hex,bit))//��תhex�ĵ�bitλ

unsigned int SIZEOFBITMAP;

unsigned char *bitmap;

extern int bitmap_init(unsigned int num);
void bitmap_destroy(void);
extern int getbitmap(void);
extern int setbitmap(int bit);
extern int resetbitmap(int bit);

#endif