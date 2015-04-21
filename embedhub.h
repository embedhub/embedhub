#ifndef __EMBSERVER_H__
#define __EMBSERVER_H__

#define MAXBUFSIZE	1024

#define EMBSERPORT	2886


typedef struct EMBSERVER{
	unsigned char buf[MAXBUFSIZE];
	int fd;
	struct sockaddr_in cli_addr;
	pthread_t pthid;
}embser;

embser *user;

#endif