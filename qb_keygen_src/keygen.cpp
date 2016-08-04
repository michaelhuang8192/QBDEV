#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <Wincrypt.h>
#include "bigd.h"

// 4/24/2011

typedef struct _ChkCTX {
	CRITICAL_SECTION CriticalSection;
	int ht_idx;
	int done;
	int code0;
	int code1;
	int cmap_sz;
	unsigned char cmap[50];
} ChkCTX, *PChkCTX;

static ChkCTX _chkctx;
static HCRYPTPROV _hprov;
static unsigned char _hash_table[100000][20];

static unsigned __int64 get_key(int key_len, int key_sum_bit_len)
{
	char buf[32];
	unsigned __int64 key = 0;
	unsigned __int64 tmp;

	unsigned int key_val_bit_len = (unsigned int)(log((float)10) / log((float)2) * key_len) - key_sum_bit_len;

	//printf("%d, %d\n", key_val_bit_len, key_sum_bit_len);

	srand(GetTickCount());
	for(int i = 0; i < sizeof(key); i++){
		tmp = (unsigned int)(rand()) % (1 << 8);
		key |= tmp << (i * 8);
	}

	key &= ((((unsigned __int64)1) << key_val_bit_len) - 1) & ((((unsigned __int64)1) << 63) - 1);
	int len = sprintf(buf, "%I64d", key);
	key <<= key_sum_bit_len;
	tmp = 0;
	while(--len >= 0) {
		tmp *= 7;
		tmp += buf[len] - '0';
	}
	tmp %= (((unsigned __int64)1) << key_sum_bit_len);
	key |= tmp;

	return key;
}

#ifdef MS_SHA

static int sha1_sum(const char *s, int s_sz, char *d)
{
	int ret;
	int d_sz = 20;

	HCRYPTHASH hhash;
	CryptCreateHash(_hprov, CALG_SHA1, 0, 0, &hhash);
	ret = CryptHashData(hhash, (const unsigned char*)s, s_sz, 0);
	ret &= CryptGetHashParam(hhash, HP_HASHVAL, (BYTE*)d, (DWORD*)d_sz, 0);
	CryptDestroyHash(hhash);

	return ret;
}

#else

#include "sha1.h"
static inline int sha1_sum(const char *s, int s_sz, char *d)
{
	sha1::calc(s, s_sz, (unsigned char*)d);
	return 1;
}

#endif

static int compute_index(const char *s, int sz)
{
	char buf[32];

	sha1_sum(s, sz, buf);
	for(int i = 0; i < 0x2710 - 1; i++) {
		sha1_sum(buf, 20, buf);
	}

	BIGD m = bdNew();
	BIGD a = bdNew();

	bdConvFromOctets(a, (const unsigned char*)buf, 20);
	int ret = bdShortMod(m, a, 0x186a0);
	bdFree(&a);
	bdFree(&m);

	return ret;
}

static int get_range(int *start, int *end, int *sz)
{
	*start = *end = *sz = 0;
	int ret = 0;
	int csz = 0;
	int rn = rand();
	int idx = 1;
	int i = -1;

	EnterCriticalSection(&_chkctx.CriticalSection);

	if(_chkctx.cmap_sz) {
		idx = (rn % _chkctx.cmap_sz) + 1;

		for(i = 0; i < sizeof(_chkctx.cmap) * 8 && idx; i++) {
			if( _chkctx.cmap[i / 8] & (1 << (i % 8)) ) {
				if(--idx) {
				} else {
					_chkctx.cmap_sz--;
					_chkctx.cmap[i / 8] &= ~(1 << (i % 8));
					csz = _chkctx.cmap_sz;
					break;
				}
			}
		}
		
	}

	LeaveCriticalSection(&_chkctx.CriticalSection);

	if(!idx) {
		ret = 1;
		*start = i * 2500;
		*end = (i + 1) * 2500;
		*sz = csz;

	} else if(i >= 0) {
		printf(">>Error.. %d\n", idx);
		getchar();
	}

	return ret;
}

static void check_code(int worker_id)
{
	PChkCTX ctx = &_chkctx;
	const unsigned char* hsv = _hash_table[ctx->ht_idx];
	char cs[32];
	char bs[32];
	char su[32];
	int su_sz;
	int start, end, map_sz;
	int t_sz;

	sprintf(cs + 10, "%05d", ctx->ht_idx);
	while(!ctx->done) {
		if( !get_range(&start, &end, &map_sz) ) break;

		//printf("Worker[%d]: Range[%06d - %06d]\n", worker_id, start, end);
		//continue;
		DWORD ts = GetTickCount();

		*(unsigned int*)(cs) = 0x30303030;
		*(unsigned short*)(cs + 4) = 0x3030;
		for(int c0 = start; c0 < end && !ctx->done; c0++) {
			//sprintf(cs, "%06d", c0);
			_itoa(c0, bs, 10);
			t_sz = strlen(bs);
			memcpy(cs + 6 - t_sz, bs, t_sz);


			*(unsigned short*)(cs + 6) = 0x3030;
			*(unsigned short*)(cs + 6 + 2) = 0x3030;
			for(int c1 = 0; c1 < 0x2710; c1++) {
				//sprintf(cs + 6, "%04d", c1);
				//memcpy(cs + 6, cs + 16, 4);
				_itoa(c1, bs, 10);
				t_sz = strlen(bs);
				memcpy(cs + 6 + 4 - t_sz, bs, t_sz);

				//printf("%s\n", cs);
				su_sz = sizeof(su);
				sha1_sum(cs, 15, su);

				if(memcmp(hsv, su, 20)) {
					
				} else {
					ctx->done = 1;
					ctx->code0 = c0;
					ctx->code1 = c1;
					break;
				}
			}
		}

		printf("Worker[%d]: Range[%06d - %06d], MapSz[%d], MSec[%d]\n", worker_id, start, end, map_sz, GetTickCount() - ts);

	}

	printf("Worker[%0d]: Done, Found: %d, code0: %d, code2: %d\n", worker_id, ctx->done, ctx->code0, ctx->code1);
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	srand(GetTickCount());
	check_code((int)lpParameter);
	return 0;
}

int get_activation_code()
{
	char buf0[128];
	char buf1[128];
	int buf0_len;
	SYSTEM_INFO si;

	printf("ModelId: 19 For Quickbooks. 37 For POS\n");
	printf("Enter Digit Only - Format> LicenseNumProductNum[ModelIdUserLicenses]\n");
	buf0_len = strlen(fgets(buf0, sizeof(buf0), stdin));
	if(buf0_len && buf0[buf0_len - 1] == '\n') buf0[--buf0_len] = 0x00;
	_chkctx.ht_idx = compute_index(buf0, buf0_len);
	printf("Hash Index: %d, Input-Digits Len: %d\n\n", _chkctx.ht_idx, buf0_len);

	GetSystemInfo(&si);
	int t_count = si.dwNumberOfProcessors;
	printf("Total CPUs: %d, Worker Threads[default %d]: ", t_count, t_count);
	t_count = atoi(fgets(buf1, sizeof(buf1), stdin));
	if(t_count < 1 || t_count > (int)si.dwNumberOfProcessors) t_count = si.dwNumberOfProcessors;
	
	printf("%d Worker Threads Start..\n\n", t_count);
	HANDLE* th = (HANDLE*)malloc(sizeof(HANDLE) * t_count);
	for(int i = 0; i < t_count; i++) {
		th[i] = CreateThread(NULL, 0, ThreadProc, (LPVOID)i, 0, NULL);
	}

	for(int t = 0; t < t_count; t++) {
		WaitForSingleObject(th[t], INFINITE);
		CloseHandle(th[t]);
	}
	free(th);

	printf("\n>>>The Code Is: %06d[UserLicenses]\nDone[%d], map_sz[%d]\n", _chkctx.code0, _chkctx.done, _chkctx.cmap_sz);

	return 1;
}

int main()
{
	char buf[128];
	int uc = 0;

	CryptAcquireContext(&_hprov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);

	memset(&_chkctx, 0x00, sizeof(_chkctx));
	InitializeCriticalSection(&_chkctx.CriticalSection);

	memset(_chkctx.cmap, 0xFF, sizeof(_chkctx.cmap));
	_chkctx.cmap_sz = sizeof(_chkctx.cmap) * 8;

	FILE* fp = fopen("hash_table.dat", "rb");
	if(!fp) {
		printf("can not open file hash_table.dat\n");
		system("pause");
		return 1;
	}
	fread((void*)_hash_table, 1, sizeof(_hash_table), fp);
	fclose(fp);

	printf("(0)License Number (1)Validation Code\nEnter [default 0]: ");
	uc = atoi(fgets(buf, sizeof(buf), stdin));
	if(uc == 1) {
		get_activation_code();

	} else {
		printf("License Number: %015I64u\n", get_key(15, 10) );
		//printf("%06I64d\n", get_key(6, 4) );

	}

	printf("\n");
	system("pause");

	DeleteCriticalSection(&_chkctx.CriticalSection);
	CryptReleaseContext(_hprov, 0);

	return 0;
}

