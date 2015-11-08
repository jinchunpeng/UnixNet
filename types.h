#include <stdio.h>

///log
#define   ELOG                printf
#define   ILOG(fmt, ...)      ELOG("ILOG||%s|%s|%d, "fmt"\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define   ENPLOG              ELOG("ENPLOG|%s|%s|%d, error!\n", __FILE__, __FUNCTION__, __LINE__);

#define   RET_ERROR(RET)\
				do\
				{\
					ENPLOG;\
					return RET;\	
				}while(0);
				
#define   CHK_EQ_RETURN(VAR, EQ_VAR, ERR_RET) \
				 	if (VAR == EQ_VAR) \
					{\
						RET_ERROR(ERR_RET);\
					}      
					
#define   CHK_UNEQ_RETURN(VAR, UNEQ_VAR, ERR_RET)\
  					if (VAR != UNEQ_VAR)\
					{\
						RET_ERROR(ERR_RET);\
					}	

//types
typedef   int            sint32;
typedef   unsigned int   uint32;
typedef   unsigned short uint16;

//functions








