#ifndef FCNTL_H
#define FCNTL_H
#include "../../typos.h"
#define F_DUPFD		0	
#define F_GETFD		1	
#define F_SETFD		2	
#define F_GETFL		3	
#define F_SETFL		4	
#ifndef F_GETLK
#define F_GETLK		5
#define F_SETLK		6
#define F_SETLKW	7
#endif
#ifndef F_SETOWN
#define F_SETOWN	8	
#define F_GETOWN	9	
#endif
#ifndef F_SETSIG
#define F_SETSIG	10	
#define F_GETSIG	11	
#endif
#ifndef F_SETOWN_EX
#define F_SETOWN_EX	15
#define F_GETOWN_EX	16
#endif
#ifndef F_GETOWNER_UIDS
#define F_GETOWNER_UIDS	17
#endif
#define F_OFD_GETLK	36
#define F_OFD_SETLK	37
#define F_OFD_SETLKW	38
#define F_OWNER_TID	0
#define F_OWNER_PID	1
#define F_OWNER_PGRP	2
#define FD_CLOEXEC	1	
#ifndef F_RDLCK
#define F_RDLCK		0
#define F_WRLCK		1
#define F_UNLCK		2
#endif
#ifndef F_EXLCK
#define F_EXLCK		4	
#define F_SHLCK		8	
#endif
i32 fcntl(i32 fd, i32 cmd, i64 arg);
#endif
