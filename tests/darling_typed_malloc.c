#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <dlfcn.h>
static int total, failures;
#define CHECK(v) do { ++total; if (!(v)) { ++failures; printf("FAIL %s\n", #v); } } while (0)
int main(void) {
 void *(*tm)(size_t,unsigned long long)=dlsym(RTLD_DEFAULT,"malloc_type_malloc");
 void *(*tc)(size_t,size_t,unsigned long long)=dlsym(RTLD_DEFAULT,"malloc_type_calloc");
 void *(*tr)(void*,size_t,unsigned long long)=dlsym(RTLD_DEFAULT,"malloc_type_realloc");
 CHECK(tm && tc && tr); if (!tm || !tc || !tr) return 1;
 unsigned char *p=tm(64,0xfedcba9876543210ULL); CHECK(p!=NULL);
 if(!p)return 1; memset(p,0x53,64);
 unsigned char *q=tr(p,4096,1); CHECK(q!=NULL); if(!q){free(p);return 1;}
 int same=1; for(int i=0;i<64;i++)same &= q[i]==0x53; CHECK(same);
 errno=0; void *r=tr(q,SIZE_MAX,0); CHECK(r==NULL); CHECK(errno==ENOMEM);
 same=1; for(int i=0;i<64;i++)same &= q[i]==0x53; CHECK(same); free(q);
 p=tc(33,7,UINT64_MAX); CHECK(p!=NULL); if(!p)return 1;
 same=1; for(int i=0;i<231;i++)same &= p[i]==0; CHECK(same);free(p);
 volatile size_t huge=SIZE_MAX;errno=0;CHECK(tc(huge,2,0)==NULL);CHECK(errno==ENOMEM);
 p=tr(NULL,32,0);CHECK(p!=NULL);free(p);
 p=tm(0,0);free(p);p=tc(0,7,0);free(p);
 void *(*to)(void*,size_t,size_t,unsigned long long,unsigned long long)=dlsym(RTLD_DEFAULT,"malloc_type_zone_malloc_with_options");
 void *(*toi)(void*,size_t,size_t,unsigned long long,unsigned long long)=dlsym(RTLD_DEFAULT,"malloc_type_zone_malloc_with_options_internal");
 CHECK(to && toi); if (!to || !toi) return 1;
 unsigned char *z=to(NULL,0,32,1,1ULL<<0); CHECK(z!=NULL);
 if (z) { int zero=1; for(int i=0;i<32;i++)zero &= z[i]==0; CHECK(zero); free(z); }
 unsigned char *al=to(NULL,64,128,2,0); CHECK(al!=NULL && ((uintptr_t)al % 64)==0);
 if (al) { memset(al,0x5a,128); free(al); }
 unsigned char *alz=to(NULL,64,128,3,1ULL<<0); CHECK(alz!=NULL && ((uintptr_t)alz % 64)==0);
 if (alz) { int zero=1; for(int i=0;i<128;i++)zero &= alz[i]==0; CHECK(zero); free(alz); }
 p=to(NULL,0,8,0,1ULL<<1); CHECK(p!=NULL); free(p);
 errno=0; CHECK(to(NULL,3,8,0,0)==NULL); CHECK(errno==EINVAL);
 errno=0; CHECK(to(NULL,64,72,0,0)==NULL); CHECK(errno==EINVAL);
 errno=0; CHECK(to(NULL,0,8,0,1ULL<<2)==NULL); CHECK(errno==EINVAL);
 p=toi(NULL,0,16,0,0); CHECK(p!=NULL); free(p);
 printf("typed allocations: %d checks, %d failures\n",total,failures);return failures!=0;
}
