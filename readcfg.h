#ifndef __READCFG_H__
#define __READCFG_H__

#define CONF_FILE_NAME	"embedhub.cfg"
#define CONF_DEFAULT_FUN	"EmbedHub"
#define KEYVALLEN	100

extern int readcfgstr(char *fun, char *key, char *val);
extern int readcfgint(char *fun, char *key, int *val);
extern int readcfgstrdef(char *key, char *val);
extern int readcfgintdef(char *key, int *val);

#endif