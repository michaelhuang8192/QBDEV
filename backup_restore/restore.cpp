#include <stdio.h>
#include <Windows.h>

int main(int argc, char *argv[])
{
	char *src, *dst;
	char be_ctx[512];
	char bh_ctx[512];
	char status[64];
	char buf[MAX_PATH];
	char *tmp;

	void (__thiscall *RestoreEngine)(void *ctx);
	//void (__thiscall *CBackupUIHelper)(void *ctx);
	void (__thiscall *InitializeRestoreEngine)(void *ctx, char **dst, char **src, void *bh_ctx);
	void (__thiscall *DoRestore)(void *ctx, void *status);


	//if(argc != 3) return 0;
	src = argv[1];
	dst = argv[2];


	GetModuleFileName(0, buf, sizeof(buf));
	if(tmp=strrchr(buf, '\\')) {
		*tmp = 0x00;
		SetCurrentDirectory(buf);
		printf("change cwd to %s\n", buf);
	}

	HMODULE lib = LoadLibrary("BackupLib.dll");
	*(unsigned long*)&RestoreEngine = (unsigned long)GetProcAddress(lib, "??0RestoreEngine@@QAE@XZ");
	//*(unsigned long*)&CBackupUIHelper = (unsigned long)GetProcAddress(lib, "??0CBackupUIHelper@@QAE@XZ");
	*(unsigned long*)&InitializeRestoreEngine = (unsigned long)GetProcAddress(lib, "?InitializeRestoreEngine@RestoreEngine@@QAEXABV?$TString@DV?$TStringAllocator@D@Util@@@Util@@0PAVCBackupUIHelper@@@Z");
	*(unsigned long*)&DoRestore = (unsigned long)GetProcAddress(lib, "?DoRestore@RestoreEngine@@UAE?AVStatus@Util@@XZ");

	memset(be_ctx, 0, sizeof(be_ctx));
	memset(bh_ctx, 0, sizeof(bh_ctx));
	memset(status, 0, sizeof(status));

	//printf("%x, %x, %x, %x\n", RestoreEngine, CBackupUIHelper, InitializeRestoreEngine, DoRestore);

	printf("RestoreEngine\n");
	RestoreEngine(be_ctx);

	printf("InitializeRestoreEngine\n");
	InitializeRestoreEngine(be_ctx, &dst, &src, 0);

	printf("DoRestore\n");
	DoRestore(be_ctx, &status);

	printf("RestoreEngine Done\n");

	return 0;
}