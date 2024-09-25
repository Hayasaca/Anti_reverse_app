#include <iostream>
#include <Windows.h>
#include <winternl.h>

BOOL isFromSystem32(std::wstring modulePath)
{
    int endPos = 0, startPos = 0, inputLength = modulePath.length();
    while (--inputLength)
    {
        if (modulePath[inputLength] == L'\\' | modulePath[inputLength] == L'/')
            if(!endPos)
                endPos = inputLength;
            else {
                startPos = inputLength + 1;
                break;
            }
    }
    std::string tmp(modulePath.c_str() + startPos, modulePath.c_str() + endPos);
    return !_stricmp(tmp.c_str(), "system32");
}

PVOID loadDLLFromDisk(std::wstring dllPath)
{
    HANDLE dllHandle = CreateFileW(dllPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (dllHandle == INVALID_HANDLE_VALUE) {
        std::wcout << dllPath << ", error code: " << GetLastError() << std::endl;
        return NULL;
    }
    //std::cout << "Load from disk successfully\n";
    HANDLE moduleMap = CreateFileMappingW(dllHandle, NULL, PAGE_READONLY | SEC_IMAGE, 0, 0, NULL);
    return MapViewOfFile(moduleMap, FILE_MAP_READ, 0, 0, 0);
}

BOOL compareFunc(PVOID dllBaseLocal, PVOID dllBaseFromDisk, DWORD funcBegin, DWORD funcEnd)
{
    for (ULONG i = funcBegin; i < funcEnd; i++)
    {
        if ((BYTE)((ULONG_PTR)dllBaseLocal + i) != (BYTE)((ULONG_PTR)dllBaseFromDisk + i))
            return FALSE;
    }
    return TRUE;
}

BOOL accessSection(PVOID dllBaseLocal, PVOID dllBaseFromDisk)
{
    PIMAGE_DOS_HEADER imgDosHdr = (PIMAGE_DOS_HEADER)dllBaseLocal;
    /*if (imgDosHdr->e_magic == IMAGE_DOS_SIGNATURE)
        std::cout << "Yes\n";*/
    PIMAGE_NT_HEADERS imgNtHdr = (PIMAGE_NT_HEADERS)((ULONG_PTR)dllBaseLocal + imgDosHdr->e_lfanew);
    /*if (imgNtHdr->Signature == IMAGE_NT_SIGNATURE)
        std::cout << "Yes\n";*/
    int numberOfSection = imgNtHdr->FileHeader.NumberOfSections;
    PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)((ULONG_PTR)dllBaseLocal + imgDosHdr->e_lfanew + sizeof(IMAGE_NT_HEADERS));
    DWORD pdataRVA, pdataSize, textRVA, textSize;
    while (--numberOfSection)
    {
        if (!_strcmpi((const char*)pSectionHeader->Name, ".text"))
        {
            //std::cout << "Found .text section\n";
            textRVA = pSectionHeader->VirtualAddress;
            textSize = pSectionHeader->Misc.VirtualSize;
            //std::cout << ".text RVA: 0x" << std::hex << textRVA << std::endl;
            //std::cout << ".text Virtual size: 0x" << std::hex << textSize << std::endl;
        }
        if (!_strcmpi((const char*)pSectionHeader->Name, ".pdata"))
        {
            //std::cout << "Found .pdata section\n";

            pdataRVA = pSectionHeader->VirtualAddress;
            pdataSize = pSectionHeader->Misc.VirtualSize;
            //std::cout << ".pdata RVA: 0x" << std::hex << pdataRVA << std::endl;
            //std::cout << ".pdata Virtual size: 0x" << std::hex << pdataSize << std::endl;
        }
        pSectionHeader++;
    }
    int numberOfFunction = pdataSize / sizeof(RUNTIME_FUNCTION);
    PRUNTIME_FUNCTION pdataSectionFunc = (PRUNTIME_FUNCTION)((ULONG_PTR)dllBaseLocal + pdataRVA);
    DWORD funcBegin = 0, funcEnd = 0;
    //std::cout << std::dec << numberOfFunction << std::endl;
    int countFail = 0;
    while (--numberOfFunction)
    {
        funcBegin = pdataSectionFunc->BeginAddress;
        funcEnd = pdataSectionFunc->EndAddress;
        pdataSectionFunc++;
        if (funcBegin > textRVA && (funcEnd < (textRVA + textSize))) {
            if (compareFunc(dllBaseLocal, dllBaseFromDisk, funcBegin, funcEnd))
                //std::cout << "The same yay\n";
                int A = 1;
            else
                std::cout << "Hooked?\n";
            continue;
        }
        /*std::cout << "Begin address: 0x" << std::hex << funcBegin << std::endl;
        std::cout << "End address: 0x" << std::hex << funcEnd << std::endl;*/
        countFail++;
    }
    //std::cout << std::dec << "Failed: " << countFail << std::endl;
    return TRUE;
}

BOOL iterateLoadedModule() {
    PTEB info = NULL;
#if _WIN64
    info = (PTEB)__readgsqword(0x30);
#else
    info = (PTEB)__readfsdword(0x16);
#endif
    
    PPEB processInfo = info->ProcessEnvironmentBlock;
    LIST_ENTRY moduleInfo = processInfo->Ldr->InMemoryOrderModuleList;
    while (moduleInfo.Flink) {
        PLDR_DATA_TABLE_ENTRY pCurrentModuleInfo = (PLDR_DATA_TABLE_ENTRY)((ULONG_PTR)moduleInfo.Flink - sizeof(LIST_ENTRY));
        if (!pCurrentModuleInfo->DllBase)
            break;
        PUNICODE_STRING pDLLName = (PUNICODE_STRING)((PVOID)pCurrentModuleInfo->Reserved4);
        std::wcout << (std::wstring)pDLLName->Buffer << std::endl;
        if (isFromSystem32((std::wstring)pCurrentModuleInfo->FullDllName.Buffer))
        {
            std::cout << "This module is from system32\n";
            PVOID dllBaseLocal = pCurrentModuleInfo->DllBase;
            PVOID dllBaseFromDisk = loadDLLFromDisk((std::wstring)pCurrentModuleInfo->FullDllName.Buffer);
            //std::cout << "Module base address in local process: 0x" << std::hex << dllBaseLocal << std::endl;
            
            accessSection(dllBaseLocal, dllBaseFromDisk);
        }
        else
            std::cout << "This module is not from system32\n";
        moduleInfo = pCurrentModuleInfo->InMemoryOrderLinks;
    } 

    return TRUE;
}

int main()
{
    iterateLoadedModule();
    return 0;
}
