#ifndef __DEBUG_LOG_H__
#define __DEBUG_LOG_H__ 

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>



#ifndef DEBUG_LOG

#define LOG(foramt, ...) do {} while(0);
#define LOG_X do {} while(0);

#else

#define LOG(foramt, ...) do{                            \
			fprintf(stderr, "[%s] [%04d] [%s] \n----> ", \
				 __FILE__, __LINE__, __FUNCTION__); \
			fprintf(stderr, foramt, ##__VA_ARGS__);                     \
			fprintf(stderr, "\n");                            \
		}while(0);

#define LOG_X do {	\
			fprintf(stderr, "[%s] [%04d] [%s] \n\n", \
				 __FILE__, __LINE__, __FUNCTION__); \
		} while (0);

#endif



#endif