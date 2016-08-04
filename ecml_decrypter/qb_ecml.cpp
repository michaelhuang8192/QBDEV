#include <Windows.h>
#include <stdio.h>


/*
//void(__stdcall *CheckOfferingCodeA)(char* ecml, char* code, int* isValid, char* errorMessage, unsigned long installerContext);

.text:10007AD7                 mov     eax, [esi-0Ch]
.text:10007ADA                 push    eax
.text:10007ADB                 lea     eax, [ebp-14h]
.text:10007ADE                 push    esi
.text:10007ADF                 push    eax
.text:10007AE0                 call    sub_1000AAA0 ; #->>>>>> Decryptor
.text:10007AE5                 mov     eax, [esi-8]
.text:10007AE8                 push    eax
.text:10007AE9                 push    esi
.text:10007AEA                 mov     byte ptr [ebp-4], 1
.text:10007AEE                 call    sub_1002E0B1
.text:10007AF3                 add     esp, 14h
.text:10007AF6                 test    eax, eax
.text:10007AF8                 js      loc_10007BE3
.text:10007AFE                 cmp     eax, [esi-8]
.text:10007B01                 jg      loc_10007BE3
.text:10007B07                 mov     [esi-0Ch], eax
.text:10007B0A                 mov     byte ptr [eax+esi], 0
.text:10007B0E                 mov     edx, off_1005ADD8
.text:10007B14                 mov     eax, [edx+0Ch]
.text:10007B17                 mov     ecx, offset off_1005ADD8
.text:10007B1C                 call    eax
.text:10007B1E                 add     eax, 10h
.text:10007B21                 mov     [ebp-18h], eax
.text:10007B24                 mov     ecx, [ebp+0Ch]
.text:10007B27                 mov     edx, [ecx]
.text:10007B29                 push    edx
.text:10007B2A                 lea     eax, [ebp-18h]
.text:10007B2D                 push    offset aOfferingcodeSO ; "<OfferingCode>%s</OfferingCode>"

*/



struct FileBuffer {
	int size_on_disk;
	int max_size_on_disk;
	int unk_08;
	int unk_0C;
	int unk_10;
	int size;
	int max_size;
	int unk_1C;

	const char buf[1];
};

typedef void(__stdcall *decryptor)(void *buf_out, const char * fdata, int size);

int main()
{

	HMODULE lib = LoadLibrary("Intuit.Spc.Map.EntitlementClient.Install.dll");
	if (!lib) {
		MessageBox(0, "Can't Load Intuit.Spc.Map.EntitlementClient.Install.dll", "Error", 0);
		return -1;
	}

	printf("DLL Base: %08X\n", lib);

	FILE* fp = fopen("manifest.ecml", "rb");
	if (!fp) {
		MessageBox(0, "Can't Open manifest.ecml", "Error", 0);
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);

	fseek(fp, 0, SEEK_SET);
	char* fdata = (char*)malloc(fsize);
	fread(fdata, fsize, 1, fp);
	fclose(fp);


	char *buf = 0;
	((decryptor)((0x1000AAA0 - 0x10000000) + (unsigned long)lib))(&buf, fdata, fsize);
	printf("Result: %08X\n", buf);

	fp = fopen("manifest.xml", "wb");
	if (!fp) {
		MessageBox(0, "Can't write to manifest.xml", "Error", 0);
		return -1;
	}

	FileBuffer* fb = (FileBuffer*)((unsigned long)buf - (unsigned long)((FileBuffer*)0)->buf);
	printf("File Size: %d\n", fb->size);
	fwrite(buf, fb->size, 1, fp);
	fclose(fp);

}