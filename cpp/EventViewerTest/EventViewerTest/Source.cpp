#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <sddl.h>
#include <stdio.h>
#include <winevt.h>

#pragma comment(lib, "wevtapi.lib")

#define ARRAY_SIZE 10
#define TIMEOUT 1000  // 1 second; Set and use in place of INFINITE in EvtNext call

DWORD PrintResults(EVT_HANDLE hResults);
DWORD PrintEvent(EVT_HANDLE hEvent); // Shown in the Rendering Events topic
LPWSTR GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId);

#define QUERY \
    L"<QueryList>" \
    L"   <Query Id = '0' Path = 'Microsoft-Windows-Windows Defender/Operational'>" \
    L"      <Select Path = 'Microsoft-Windows-Windows Defender/Operational'>*</Select>" \
    L"   </Query>" \
    L"</QueryList>"

#define PROVNAME \
   L"Microsoft-Windows-Windows Defender"

void main(void)
{
   DWORD status = ERROR_SUCCESS;
   EVT_HANDLE hResults = NULL;
   LPCWSTR pwsPath = L"Microsoft-Windows-Windows Defender/Operational";
   //LPCWSTR pwsQuery = L"Event/System[EventID=4]";
   LPCWSTR pwsQuery = L"*";
   

   hResults = EvtQuery(NULL, pwsPath, pwsQuery, EvtQueryChannelPath | EvtQueryReverseDirection);
   if (NULL == hResults)
   {
      status = GetLastError();

      if (ERROR_EVT_CHANNEL_NOT_FOUND == status)
         wprintf(L"The channel was not found.\n");
      else if (ERROR_EVT_INVALID_QUERY == status)
         // You can call the EvtGetExtendedStatus function to try to get 
         // additional information as to what is wrong with the query.
         wprintf(L"The query is not valid.\n");
      else
         wprintf(L"EvtQuery failed with %lu.\n", status);

      goto cleanup;
   }

   PrintResults(hResults);

cleanup:

   if (hResults)
      EvtClose(hResults);

}


// Enumerate all the events in the result set. 
DWORD PrintResults(EVT_HANDLE hResults)
{
   DWORD status = ERROR_SUCCESS;
   EVT_HANDLE hEvents[ARRAY_SIZE];
   DWORD dwReturned = 0;

   while (true)
   {
      // Get a block of events from the result set.
      if (!EvtNext(hResults, ARRAY_SIZE, hEvents, INFINITE, 0, &dwReturned))
      {
         if (ERROR_NO_MORE_ITEMS != (status = GetLastError()))
         {
            wprintf(L"EvtNext failed with %lu\n", status);
         }

         goto cleanup;
      }

      // For each event, call the PrintEvent function which renders the
      // event for display. PrintEvent is shown in RenderingEvents.
      for (DWORD i = 0; i < dwReturned; i++)
      {
         if (ERROR_SUCCESS == (status = PrintEvent(hEvents[i])))
         {
            EvtClose(hEvents[i]);
            hEvents[i] = NULL;
         }
         else
         {
            goto cleanup;
         }
      }
   }

cleanup:

   for (DWORD i = 0; i < dwReturned; i++)
   {
      if (NULL != hEvents[i])
         EvtClose(hEvents[i]);
   }

   return status;
}


DWORD PrintEvent(EVT_HANDLE hEvent)
{
   DWORD status = ERROR_SUCCESS;
   DWORD dwBufferSize = 0;
   DWORD dwBufferUsed = 0;
   DWORD dwPropertyCount = 0;
   LPWSTR pRenderedContent = NULL;
   LPWSTR pwsMessage = NULL;
   EVT_HANDLE hProviderMetadata = NULL;

   LPWSTR pwszPublisherName = L"Microsoft-Windows-Windows Defender";

   hProviderMetadata = EvtOpenPublisherMetadata(NULL, pwszPublisherName, NULL, 0, 0);
   if (NULL == hProviderMetadata)
   {
      wprintf(L"EvtOpenPublisherMetadata failed with %d\n", GetLastError());
         pRenderedContent = (LPWSTR)malloc(dwBufferSize);
         if (pRenderedContent)
      goto cleanup;
   }

   // The EvtRenderEventXml flag tells EvtRender to render the event as an XML string.
   if (!EvtRender(NULL, hEvent, EvtRenderEventXml, dwBufferSize, pRenderedContent, &dwBufferUsed, &dwPropertyCount))
   {
      if (ERROR_INSUFFICIENT_BUFFER == (status = GetLastError()))
      {
         dwBufferSize = dwBufferUsed;
         pRenderedContent = (LPWSTR)malloc(dwBufferSize);
         if (pRenderedContent)
         {
            //EvtRender(NULL, hEvent, EvtRenderEventXml, dwBufferSize, pRenderedContent, &dwBufferUsed, &dwPropertyCount);
            pwsMessage = GetMessageString(hProviderMetadata, hEvent, EvtFormatMessageEvent);
            if (pwsMessage)
            {
               wprintf(L"Event message string: %s\n\n", pwsMessage);
               free(pwsMessage);
               pwsMessage = NULL;
            }
         }
         else
         {
            wprintf(L"malloc failed\n");
            status = ERROR_OUTOFMEMORY;
            goto cleanup;
         }
      }

      if (ERROR_SUCCESS != (status = GetLastError()))
      {
         wprintf(L"EvtRender failed with %d\n", GetLastError());
         goto cleanup;
      }
   }

   wprintf(L"\n\n%s", pRenderedContent);

cleanup:

   if (pRenderedContent)
      free(pRenderedContent);

   return status;
}


// Gets the specified message string from the event. If the event does not
// contain the specified message, the function returns NULL.
LPWSTR GetMessageString(EVT_HANDLE hMetadata, EVT_HANDLE hEvent, EVT_FORMAT_MESSAGE_FLAGS FormatId)
{
   LPWSTR pBuffer = NULL;
   DWORD dwBufferSize = 0;
   DWORD dwBufferUsed = 0;
   DWORD status = 0;

   if (!EvtFormatMessage(hMetadata, hEvent, 0, 0, NULL, FormatId, dwBufferSize, pBuffer, &dwBufferUsed))
   {
      status = GetLastError();
      if (ERROR_INSUFFICIENT_BUFFER == status)
      {
         // An event can contain one or more keywords. The function returns keywords
         // as a list of keyword strings. To process the list, you need to know the
         // size of the buffer, so you know when you have read the last string, or you
         // can terminate the list of strings with a second null terminator character 
         // as this example does.
         if ((EvtFormatMessageKeyword == FormatId))
            pBuffer[dwBufferSize - 1] = L'\0';
         else
            dwBufferSize = dwBufferUsed;

         pBuffer = (LPWSTR)malloc(dwBufferSize * sizeof(WCHAR));

         if (pBuffer)
         {
            EvtFormatMessage(hMetadata, hEvent, 0, 0, NULL, FormatId, dwBufferSize, pBuffer, &dwBufferUsed);

            // Add the second null terminator character.
            if ((EvtFormatMessageKeyword == FormatId))
               pBuffer[dwBufferUsed - 1] = L'\0';
         }
         else
         {
            wprintf(L"malloc failed\n");
         }
      }
      else if (ERROR_EVT_MESSAGE_NOT_FOUND == status || ERROR_EVT_MESSAGE_ID_NOT_FOUND == status)
         ;
      else
      {
         wprintf(L"EvtFormatMessage failed with %u\n", status);
      }
   }

   return pBuffer;
}