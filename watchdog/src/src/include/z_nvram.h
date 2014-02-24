#ifndef __Z_NVRAM_H__
#define __Z_NVRAM_H__


char *z_nvram_get(const char *name);
int z_nvram_set(const char *name, const char *value);
int z_nvram_unset(const char *name);
int z_nvram_commit(void);


#endif
