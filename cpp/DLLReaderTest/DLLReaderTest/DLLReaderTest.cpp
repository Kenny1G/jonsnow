// DLLReaderTest.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include <stdio.h>

int i = 0;

int ProcessBlock(MESSAGE_RESOURCE_DATA* data, MESSAGE_RESOURCE_BLOCK* block)
{
   MESSAGE_RESOURCE_ENTRY* entry = (MESSAGE_RESOURCE_ENTRY*)((unsigned char*)data + block->OffsetToEntries);
   for (DWORD id = block->LowId; id <= block->HighId; id++)
   {
      if (entry->Flags == 0x0001) {// wide char
         i++;
         printf("%d, %ls", id, entry->Text);
      }
      else if (entry->Flags == 0x0000) { // ANSI
         printf("%d, %s", id, entry->Text);
         i++;
      }
      entry = (MESSAGE_RESOURCE_ENTRY*)((unsigned char*)entry + entry->Length);
   }
   
   return 1;
}

int main(void)
{
   //HMODULE hMod = LoadLibrary(L"C:\\Users\\osele\\Desktop\\MpEvMsg.dll");
   HMODULE hMod = LoadLibraryEx(L"C:\\Program Files\\Windows Defender\\MpEvMsg.dll", NULL, LOAD_LIBRARY_AS_DATAFILE); 
   if (hMod == NULL) {
      DWORD status = GetLastError();
      return 1;
   }

   HRSRC hRsrc = FindResource(hMod, MAKEINTRESOURCE(1), RT_MESSAGETABLE);
   if (hRsrc == NULL) return 1;

   HGLOBAL hGlobal = LoadResource(hMod, hRsrc);
   if (hGlobal == NULL) return 1;

   MESSAGE_RESOURCE_DATA* data = (MESSAGE_RESOURCE_DATA*)LockResource(hGlobal);
   if (data == NULL) return 1;
   printf("%d \n", data->NumberOfBlocks);

   for (DWORD block = 0; block < data->NumberOfBlocks; block++)
      if (!ProcessBlock(data, &data->Blocks[block]))
         return 1;
   printf("%d \n", i);

   return 0;
}
