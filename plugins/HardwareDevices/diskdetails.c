/*
 * Process Hacker Plugins -
 *   Hardware Devices Plugin
 *
 * Copyright (C) 2016 dmex
 *
 * This file is part of Process Hacker.
 *
 * Process Hacker is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Process Hacker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Process Hacker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "devices.h"

typedef struct _COMMON_PAGE_CONTEXT
{
    HWND ParentHandle;

    PPH_STRING DiskName;
    DV_DISK_ID DiskId;
    ULONG DiskIndex;

    HANDLE DeviceHandle;
} COMMON_PAGE_CONTEXT, *PCOMMON_PAGE_CONTEXT;

typedef struct _DV_DISK_PAGE_CONTEXT
{
    HWND WindowHandle;
    HWND ListViewHandle;

    PH_LAYOUT_MANAGER LayoutManager;

    PCOMMON_PAGE_CONTEXT PageContext;
} DV_DISK_PAGE_CONTEXT, *PDV_DISK_PAGE_CONTEXT;

typedef enum _DISKDRIVE_DETAILS_INDEX
{
    DISKDRIVE_DETAILS_INDEX_FS_CREATION_TIME,
    DISKDRIVE_DETAILS_INDEX_SERIAL_NUMBER,
    DISKDRIVE_DETAILS_INDEX_FILE_SYSTEM,
    DISKDRIVE_DETAILS_INDEX_FS_VERSION,
    DISKDRIVE_DETAILS_INDEX_LFS_VERSION,

    DISKDRIVE_DETAILS_INDEX_TOTAL_SIZE,
    DISKDRIVE_DETAILS_INDEX_TOTAL_FREE,
    DISKDRIVE_DETAILS_INDEX_TOTAL_SECTORS,
    DISKDRIVE_DETAILS_INDEX_TOTAL_CLUSTERS,
    DISKDRIVE_DETAILS_INDEX_FREE_CLUSTERS,
    DISKDRIVE_DETAILS_INDEX_TOTAL_RESERVED,
    DISKDRIVE_DETAILS_INDEX_TOTAL_BYTES_PER_SECTOR,
    DISKDRIVE_DETAILS_INDEX_TOTAL_BYTES_PER_CLUSTER,
    DISKDRIVE_DETAILS_INDEX_TOTAL_BYTES_PER_RECORD,
    DISKDRIVE_DETAILS_INDEX_TOTAL_CLUSTERS_PER_RECORD,

    DISKDRIVE_DETAILS_INDEX_MFT_RECORDS,
    DISKDRIVE_DETAILS_INDEX_MFT_SIZE,
    DISKDRIVE_DETAILS_INDEX_MFT_START,
    DISKDRIVE_DETAILS_INDEX_MFT_ZONE,
    DISKDRIVE_DETAILS_INDEX_MFT_ZONE_SIZE,
    DISKDRIVE_DETAILS_INDEX_MFT_MIRROR_START,

    DISKDRIVE_DETAILS_INDEX_FILE_READS,
    DISKDRIVE_DETAILS_INDEX_FILE_WRITES,
    DISKDRIVE_DETAILS_INDEX_DISK_READS,
    DISKDRIVE_DETAILS_INDEX_DISK_WRITES,
    DISKDRIVE_DETAILS_INDEX_FILE_READ_BYTES,
    DISKDRIVE_DETAILS_INDEX_FILE_WRITE_BYTES,

    DISKDRIVE_DETAILS_INDEX_METADATA_READS,
    DISKDRIVE_DETAILS_INDEX_METADATA_WRITES,
    DISKDRIVE_DETAILS_INDEX_METADATA_DISK_READS,
    DISKDRIVE_DETAILS_INDEX_METADATA_DISK_WRITES,
    DISKDRIVE_DETAILS_INDEX_METADATA_READ_BYTES,
    DISKDRIVE_DETAILS_INDEX_METADATA_WRITE_BYTES,

    DISKDRIVE_DETAILS_INDEX_MFT_READS,
    DISKDRIVE_DETAILS_INDEX_MFT_WRITES,
    DISKDRIVE_DETAILS_INDEX_MFT_READ_BYTES,
    DISKDRIVE_DETAILS_INDEX_MFT_WRITE_BYTES,

    DISKDRIVE_DETAILS_INDEX_ROOT_INDEX_READS,
    DISKDRIVE_DETAILS_INDEX_ROOT_INDEX_WRITES,
    DISKDRIVE_DETAILS_INDEX_ROOT_INDEX_READ_BYTES,
    DISKDRIVE_DETAILS_INDEX_ROOT_INDEX_WRITE_BYTES,

    DISKDRIVE_DETAILS_INDEX_BITMAP_READS,
    DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES,
    DISKDRIVE_DETAILS_INDEX_BITMAP_READ_BYTES,
    DISKDRIVE_DETAILS_INDEX_BITMAP_WRITE_BYTES,

    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_READS,
    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_WRITES,
    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_READ_BYTES,
    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_WRITE_BYTES,

    DISKDRIVE_DETAILS_INDEX_USER_INDEX_READS,
    DISKDRIVE_DETAILS_INDEX_USER_INDEX_WRITES,
    DISKDRIVE_DETAILS_INDEX_USER_INDEX_READ_BYTES,
    DISKDRIVE_DETAILS_INDEX_USER_INDEX_WRITE_BYTES,

    DISKDRIVE_DETAILS_INDEX_LOGFILE_READS,
    DISKDRIVE_DETAILS_INDEX_LOGFILE_WRITES,
    DISKDRIVE_DETAILS_INDEX_LOGFILE_READ_BYTES,
    DISKDRIVE_DETAILS_INDEX_LOGFILE_WRITE_BYTES,

    DISKDRIVE_DETAILS_INDEX_MFT_USER_LEVEL_WRITE,
    DISKDRIVE_DETAILS_INDEX_MFT_USER_LEVEL_CREATE,
    DISKDRIVE_DETAILS_INDEX_MFT_USER_LEVEL_SETINFO,
    DISKDRIVE_DETAILS_INDEX_MFT_USER_LEVEL_FLUSH,

    DISKDRIVE_DETAILS_INDEX_MFT_WRITES_FLUSH_LOGFILE,
    DISKDRIVE_DETAILS_INDEX_MFT_WRITES_LAZY_WRITER,
    DISKDRIVE_DETAILS_INDEX_MFT_WRITES_USER_REQUEST,

    DISKDRIVE_DETAILS_INDEX_MFT2_WRITES,
    DISKDRIVE_DETAILS_INDEX_MFT2_WRITE_BYTES,

    DISKDRIVE_DETAILS_INDEX_MFT2_USER_LEVEL_WRITE,
    DISKDRIVE_DETAILS_INDEX_MFT2_USER_LEVEL_CREATE,
    DISKDRIVE_DETAILS_INDEX_MFT2_USER_LEVEL_SETINFO,
    DISKDRIVE_DETAILS_INDEX_MFT2_USER_LEVEL_FLUSH,

    DISKDRIVE_DETAILS_INDEX_MFT2_WRITES_FLUSH_LOGFILE,
    DISKDRIVE_DETAILS_INDEX_MFT2_WRITES_LAZY_WRITER,
    DISKDRIVE_DETAILS_INDEX_MFT2_WRITES_USER_REQUEST,

    DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_FLUSH_LOGFILE,
    DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_LAZY_WRITER,
    DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_USER_REQUEST,

    DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_WRITE,
    DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_CREATE,
    DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_SETINFO,

    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_FLUSH_LOGFILE,
    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_LAZY_WRITER,
    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_USER_REQUEST,

    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_USER_LEVEL_WRITE,
    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_USER_LEVEL_CREATE,
    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_USER_LEVEL_SETINFO,
    DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_USER_LEVEL_FLUSH,

    DISKDRIVE_DETAILS_INDEX_ALLOCATE_CALLS,
    DISKDRIVE_DETAILS_INDEX_ALLOCATE_CLUSTERS,
    DISKDRIVE_DETAILS_INDEX_ALLOCATE_HINTS,
    DISKDRIVE_DETAILS_INDEX_ALLOCATE_RUNS_RETURNED,
    DISKDRIVE_DETAILS_INDEX_ALLOCATE_HITS_HONORED,
    DISKDRIVE_DETAILS_INDEX_ALLOCATE_HITS_CLUSTERS,
    DISKDRIVE_DETAILS_INDEX_ALLOCATE_CACHE,
    DISKDRIVE_DETAILS_INDEX_ALLOCATE_CACHE_CLUSTERS,
    DISKDRIVE_DETAILS_INDEX_ALLOCATE_CACHE_MISS,
    DISKDRIVE_DETAILS_INDEX_ALLOCATE_CACHE_MISS_CLUSTERS,

    DISKDRIVE_DETAILS_INDEX_LOGFILE_EXCEPTIONS,
    DISKDRIVE_DETAILS_INDEX_OTHER_EXCEPTIONS
} DISKDRIVE_DETAILS_INDEX;

VOID DiskDriveAddListViewItemGroups(
    _In_ HWND ListViewHandle,
    _In_ INT DiskGroupId
    )
{
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_FS_CREATION_TIME, L"Volume creation time", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_SERIAL_NUMBER, L"Volume serial number", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_FILE_SYSTEM, L"Volume file system", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_FS_VERSION, L"Volume version", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_LFS_VERSION, L"LFS version", NULL);

    //AddListViewItemGroupId(Context->ListViewHandle, diskGroupId, MAXINT, L"BytesPerPhysicalSector", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_TOTAL_SIZE, L"Total size", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_TOTAL_FREE, L"Total free", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_TOTAL_SECTORS, L"Total sectors", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_TOTAL_CLUSTERS, L"Total clusters", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_FREE_CLUSTERS, L"Free clusters", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_TOTAL_RESERVED, L"Reserved clusters", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_TOTAL_BYTES_PER_SECTOR, L"Bytes per sector", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_TOTAL_BYTES_PER_CLUSTER, L"Bytes per cluster", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_TOTAL_BYTES_PER_RECORD, L"Bytes per file record segment", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_TOTAL_CLUSTERS_PER_RECORD, L"Clusters per File record segment", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_RECORDS, L"MFT records", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_SIZE, L"MFT size", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_START, L"MFT start", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_ZONE, L"MFT Zone clusters", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_ZONE_SIZE, L"MFT zone size", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_MIRROR_START, L"MFT mirror start", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_FILE_READS, L"File reads", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_FILE_WRITES, L"File writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_DISK_READS, L"Disk reads", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_DISK_WRITES, L"Disk writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_FILE_READ_BYTES, L"File read bytes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_FILE_WRITE_BYTES, L"File write bytes", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_METADATA_READS, L"Metadata reads", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_METADATA_WRITES, L"Metadata writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_METADATA_DISK_READS, L"Metadata disk reads", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_METADATA_DISK_WRITES, L"Metadata disk writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_METADATA_READ_BYTES, L"Metadata read bytes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_METADATA_WRITE_BYTES, L"Metadata write bytes", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_READS, L"Mft reads", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_WRITES, L"Mft writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_READ_BYTES, L"Mft read bytes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_WRITE_BYTES, L"Mft write bytes", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ROOT_INDEX_READS, L"RootIndex reads", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ROOT_INDEX_WRITES, L"RootIndex writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ROOT_INDEX_READ_BYTES, L"RootIndex read bytes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ROOT_INDEX_WRITE_BYTES, L"RootIndex write bytes", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_BITMAP_READS, L"Bitmap reads", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES, L"Bitmap writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_BITMAP_READ_BYTES, L"Bitmap read bytes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_BITMAP_WRITE_BYTES, L"Bitmap write bytes", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_READS, L"Mft bitmap reads", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_WRITES, L"Mft bitmap writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_READ_BYTES, L"Mft bitmap read bytes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_WRITE_BYTES, L"Mft bitmap write bytes", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_USER_INDEX_READS, L"User Index reads", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_USER_INDEX_WRITES, L"User Index writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_USER_INDEX_READ_BYTES, L"User Index read bytes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_USER_INDEX_WRITE_BYTES, L"User Index write bytes", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_LOGFILE_READS, L"LogFile reads", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_LOGFILE_WRITES, L"LogFile writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_LOGFILE_READ_BYTES, L"LogFile read bytes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_LOGFILE_WRITE_BYTES, L"LogFile write bytes", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_USER_LEVEL_WRITE, L"MftWritesUserLevel-Write", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_USER_LEVEL_CREATE, L"MftWritesUserLevel-Create", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_USER_LEVEL_SETINFO, L"MftWritesUserLevel-SetInfo", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_USER_LEVEL_FLUSH, L"MftWritesUserLevel-Flush", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_WRITES_FLUSH_LOGFILE, L"MftWritesFlushForLogFileFull", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_WRITES_LAZY_WRITER, L"MftWritesLazyWriter", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_WRITES_USER_REQUEST, L"MftWritesUserRequest", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT2_WRITES, L"Mft2Writes", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT2_WRITE_BYTES, L"Mft2WriteBytes", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT2_USER_LEVEL_WRITE, L"Mft2WritesUserLevel-Write", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT2_USER_LEVEL_CREATE, L"Mft2WritesUserLevel-Create", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT2_USER_LEVEL_SETINFO, L"Mft2WritesUserLevel-SetInfo", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT2_USER_LEVEL_FLUSH, L"Mft2WritesUserLevel-Flush", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT2_WRITES_FLUSH_LOGFILE, L"Mft2WritesFlushForLogFileFull", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT2_WRITES_LAZY_WRITER, L"Mft2WritesLazyWriter", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT2_WRITES_USER_REQUEST, L"Mft2WritesUserRequest", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_FLUSH_LOGFILE, L"BitmapWritesFlushForLogFileFull", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_LAZY_WRITER, L"BitmapWritesLazyWriter", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_USER_REQUEST, L"BitmapWritesUserRequest", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_WRITE, L"BitmapWritesUserLevel-Write", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_CREATE, L"BitmapWritesUserLevel-Create", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_BITMAP_WRITES_SETINFO, L"BitmapWritesUserLevel-SetInfo", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_FLUSH_LOGFILE, L"MftBitmapWritesFlushForLogFileFull", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_LAZY_WRITER, L"MftBitmapWritesLazyWriter", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_USER_REQUEST, L"MftBitmapWritesUserRequest", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_USER_LEVEL_WRITE, L"MftBitmapWritesUserLevel-Write", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_USER_LEVEL_CREATE, L"MftBitmapWritesUserLevel-Create", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_USER_LEVEL_SETINFO, L"MftBitmapWritesUserLevel-SetInfo", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_MFT_BITMAP_USER_LEVEL_FLUSH, L"MftBitmapWritesUserLevel-Flush", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ALLOCATE_CALLS, L"Allocate-Calls", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ALLOCATE_CLUSTERS, L"Allocate-Clusters", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ALLOCATE_HINTS, L"Allocate-Hints", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ALLOCATE_RUNS_RETURNED, L"Allocate-RunsReturned", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ALLOCATE_HITS_HONORED, L"Allocate-HintsHonored", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ALLOCATE_HITS_CLUSTERS, L"Allocate-HintsClusters", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ALLOCATE_CACHE, L"Allocate-Cache", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ALLOCATE_CACHE_CLUSTERS, L"Allocate-CacheClusters", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ALLOCATE_CACHE_MISS, L"Allocate-CacheMiss", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_ALLOCATE_CACHE_MISS_CLUSTERS, L"Allocate-CacheMissClusters", NULL);

    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_LOGFILE_EXCEPTIONS, L"LogFileFullExceptions", NULL);
    AddListViewItemGroupId(ListViewHandle, DiskGroupId, DISKDRIVE_DETAILS_INDEX_OTHER_EXCEPTIONS, L"OtherExceptions", NULL);
}

VOID DiskDriveQuerySmart(
    _Inout_ PDV_DISK_PAGE_CONTEXT Context
    )
{
    PPH_LIST attributes;

    if (DiskDriveQueryImminentFailure(Context->PageContext->DeviceHandle, &attributes))
    {
        for (ULONG i = 0; i < attributes->Count; i++)
        {
            PSMART_ATTRIBUTES attribute = attributes->Items[i];

            INT lvItemIndex = PhAddListViewItem(
                Context->ListViewHandle,
                MAXINT,
                SmartAttributeGetText(attribute->AttributeId),
                IntToPtr(attribute->AttributeId)
                );

            PhSetListViewSubItem(
                Context->ListViewHandle,
                lvItemIndex,
                1,
                PhaFormatString(L"%lu", attribute->CurrentValue)->Buffer
                );
            PhSetListViewSubItem(
                Context->ListViewHandle,
                lvItemIndex,
                2,
                PhaFormatString(L"%lu", attribute->WorstValue)->Buffer
                );

            if (attribute->RawValue)
            {
                PhSetListViewSubItem(
                    Context->ListViewHandle,
                    lvItemIndex,
                    3,
                    PhaFormatString(L"%lu", attribute->RawValue)->Buffer
                    );
            }

            PhFree(attribute);
        }

        PhDereferenceObject(attributes);
    }
}

//COLORREF NTAPI PhpColorItemColorFunction(
//    _In_ INT Index,
//    _In_ PVOID Param,
//    _In_opt_ PVOID Context
//    )
//{
//    PSMART_ATTRIBUTES item = Param;
//
//    if (item->FailureImminent)
//        return RGB(255, 119, 0);// RGB(255, 60, 40);
//
//    return RGB(0xFF, 0xFF, 0xFF);
//}

INT_PTR CALLBACK DiskDriveSmartDetailsDlgProc(
    _In_ HWND hwndDlg,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
    )
{
    PDV_DISK_PAGE_CONTEXT context;

    if (uMsg == WM_INITDIALOG)
    {
        LPPROPSHEETPAGE propSheetPage = (LPPROPSHEETPAGE)lParam;

        context = PhAllocate(sizeof(DV_DISK_PAGE_CONTEXT));
        memset(context, 0, sizeof(DV_DISK_PAGE_CONTEXT));

        context->PageContext = (PCOMMON_PAGE_CONTEXT)propSheetPage->lParam;

        SetProp(hwndDlg, L"Context", (HANDLE)context);
    }
    else
    {
        context = (PDV_DISK_PAGE_CONTEXT)GetProp(hwndDlg, L"Context");

        if (uMsg == WM_DESTROY)
        {
            RemoveProp(hwndDlg, L"Context");
            PhFree(context);
        }
    }

    if (!context)
        return FALSE;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            context->WindowHandle = hwndDlg;
            context->ListViewHandle = GetDlgItem(hwndDlg, IDC_DETAILS_LIST);

            PhSetListViewStyle(context->ListViewHandle, FALSE, TRUE);
            PhSetControlTheme(context->ListViewHandle, L"explorer");
            PhAddListViewColumn(context->ListViewHandle, 0, 0, 0, LVCFMT_LEFT, 240, L"Property");
            PhAddListViewColumn(context->ListViewHandle, 1, 1, 1, LVCFMT_LEFT, 50, L"Value");
            PhAddListViewColumn(context->ListViewHandle, 2, 2, 2, LVCFMT_LEFT, 50, L"Best");
            PhAddListViewColumn(context->ListViewHandle, 3, 3, 3, LVCFMT_LEFT, 80, L"Raw");
            PhSetExtendedListView(context->ListViewHandle);
            //ExtendedListView_SetItemColorFunction(context->ListViewHandle, PhpColorItemColorFunction);

            PhInitializeLayoutManager(&context->LayoutManager, hwndDlg);
            PhAddLayoutItem(&context->LayoutManager, context->ListViewHandle, NULL, PH_ANCHOR_ALL);
            PhAddLayoutItem(&context->LayoutManager, GetDlgItem(hwndDlg, IDC_DESCRIPTION), NULL, PH_ANCHOR_LEFT | PH_ANCHOR_BOTTOM);
            PhAddLayoutItem(&context->LayoutManager, GetDlgItem(hwndDlg, IDC_EDIT1), NULL, PH_ANCHOR_LEFT | PH_ANCHOR_BOTTOM | PH_ANCHOR_RIGHT);

            DiskDriveQuerySmart(context);
        }
        break;
    case WM_DESTROY:
        {
            PhDeleteLayoutManager(&context->LayoutManager);
        }
        break;
    case WM_SIZE:
        {
            PhLayoutManagerLayout(&context->LayoutManager);
        }
        break;
    case WM_NOTIFY:
        {
            LPNMHDR header = (LPNMHDR)lParam;

            if (header->code == LVN_ITEMCHANGED)
            {
                PWSTR description;

                if (ListView_GetSelectedCount(context->ListViewHandle) == 1)
                    description = SmartAttributeGetDescription((SMART_ATTRIBUTE_ID)PhGetSelectedListViewItemParam(context->ListViewHandle));
                else
                    description = L"";

                SetDlgItemText(hwndDlg, IDC_EDIT1, description);
            }
        }
        break;
    }

    return FALSE;
}

INT_PTR CALLBACK DiskDriveFileSystemDetailsDlgProc(
    _In_ HWND hwndDlg,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
    )
{
    PDV_DISK_PAGE_CONTEXT context;

    if (uMsg == WM_INITDIALOG)
    {
        LPPROPSHEETPAGE propSheetPage = (LPPROPSHEETPAGE)lParam;

        context = PhAllocate(sizeof(DV_DISK_PAGE_CONTEXT));
        memset(context, 0, sizeof(DV_DISK_PAGE_CONTEXT));

        context->PageContext = (PCOMMON_PAGE_CONTEXT)propSheetPage->lParam;

        SetProp(hwndDlg, L"Context", (HANDLE)context);
    }
    else
    {
        context = (PDV_DISK_PAGE_CONTEXT)GetProp(hwndDlg, L"Context");

        if (uMsg == WM_DESTROY)
        {
            RemoveProp(hwndDlg, L"Context");
            PhFree(context);
        }
    }

    if (!context)
        return FALSE;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        {
            context->WindowHandle = hwndDlg;
            context->ListViewHandle = GetDlgItem(hwndDlg, IDC_DETAILS_LIST);

            PhCenterWindow(GetParent(hwndDlg), context->PageContext->ParentHandle);

            PhSetListViewStyle(context->ListViewHandle, FALSE, TRUE);
            PhSetControlTheme(context->ListViewHandle, L"explorer");
            PhAddListViewColumn(context->ListViewHandle, 0, 0, 0, LVCFMT_LEFT, 290, L"Property");
            PhAddListViewColumn(context->ListViewHandle, 1, 1, 1, LVCFMT_LEFT, 130, L"Value");
            PhSetExtendedListView(context->ListViewHandle);
            ListView_EnableGroupView(context->ListViewHandle, TRUE);

            PhInitializeLayoutManager(&context->LayoutManager, hwndDlg);
            PhAddLayoutItem(&context->LayoutManager, context->ListViewHandle, NULL, PH_ANCHOR_ALL);
        }
        break;
    case WM_DESTROY:
        {
            PhDeleteLayoutManager(&context->LayoutManager);
        }
        break;
    case WM_SIZE:
        {
            PhLayoutManagerLayout(&context->LayoutManager);
        }
        break;
    }

    return FALSE;
}

VOID FreeDiskDriveDetailsContext(
    _In_ PCOMMON_PAGE_CONTEXT Context
    )
{
    if (Context->DeviceHandle)
        NtClose(Context->DeviceHandle);

    DeleteDiskId(&Context->DiskId);
    PhClearReference(&Context->DiskName);
    PhFree(Context);
}

NTSTATUS ShowDiskDriveDetailsDialogThread(
    _In_ PVOID Parameter
    )
{
    PROPSHEETHEADER propSheetHeader = { sizeof(propSheetHeader) };
    PROPSHEETPAGE propSheetPage;
    HPROPSHEETPAGE pages[2];
    PCOMMON_PAGE_CONTEXT pageContext = Parameter;
    HANDLE deviceHandle;

    if (NT_SUCCESS(DiskDriveCreateHandle(&deviceHandle, pageContext->DiskId.DevicePath)))
    {
        pageContext->DeviceHandle = deviceHandle;
    }

    propSheetHeader.dwFlags =
        PSH_NOAPPLYNOW |
        PSH_NOCONTEXTHELP;
    propSheetHeader.pszCaption = L"Disk Drive";
    propSheetHeader.nPages = 0;
    propSheetHeader.nStartPage = 0;
    propSheetHeader.phpage = pages;

    // General
    //memset(&propSheetPage, 0, sizeof(PROPSHEETPAGE));
    //propSheetPage.dwSize = sizeof(PROPSHEETPAGE);
    //propSheetPage.hInstance = PluginInstance->DllBase;
    //propSheetPage.pszTemplate = MAKEINTRESOURCE(IDD_DISKDRIVE_DETAILS_GENERAL);
    //propSheetPage.pfnDlgProc = DiskDriveDetailsDlgProc;
    //pages[propSheetHeader.nPages++] = CreatePropertySheetPage(&propSheetPage);

    // FileSystem
    memset(&propSheetPage, 0, sizeof(PROPSHEETPAGE));
    propSheetPage.dwSize = sizeof(PROPSHEETPAGE);
    propSheetPage.hInstance = PluginInstance->DllBase;
    propSheetPage.pszTemplate = MAKEINTRESOURCE(IDD_DISKDRIVE_DETAILS_FILESYSTEM);
    propSheetPage.pfnDlgProc = DiskDriveFileSystemDetailsDlgProc;
    propSheetPage.lParam = (LPARAM)pageContext;
    pages[propSheetHeader.nPages++] = CreatePropertySheetPage(&propSheetPage);

    // SMART
    memset(&propSheetPage, 0, sizeof(PROPSHEETPAGE));
    propSheetPage.dwSize = sizeof(PROPSHEETPAGE);
    propSheetPage.hInstance = PluginInstance->DllBase;
    propSheetPage.pszTemplate = MAKEINTRESOURCE(IDD_DISKDRIVE_DETAILS_SMART);
    propSheetPage.pfnDlgProc = DiskDriveSmartDetailsDlgProc;
    propSheetPage.lParam = (LPARAM)pageContext;
    pages[propSheetHeader.nPages++] = CreatePropertySheetPage(&propSheetPage);

    PhModalPropertySheet(&propSheetHeader);

    FreeDiskDriveDetailsContext(pageContext);
    return STATUS_SUCCESS;
}

VOID ShowDiskDriveDetailsDialog(
    _In_ PDV_DISK_SYSINFO_CONTEXT Context
    )
{
    HANDLE dialogThread = NULL;
    PCOMMON_PAGE_CONTEXT pageContext;

    pageContext = PhAllocate(sizeof(COMMON_PAGE_CONTEXT));
    memset(pageContext, 0, sizeof(COMMON_PAGE_CONTEXT));

    pageContext->ParentHandle = GetParent(GetParent(Context->WindowHandle));
    pageContext->DiskIndex = Context->DiskEntry->DiskIndex;
    //pageContext->Length = Context->DiskEntry->DiskLength;

    PhSetReference(&pageContext->DiskName, Context->DiskEntry->DiskName);
    CopyDiskId(&pageContext->DiskId, &Context->DiskEntry->Id);

    if (dialogThread = PhCreateThread(0, ShowDiskDriveDetailsDialogThread, pageContext))
        NtClose(dialogThread);
    else
        FreeDiskDriveDetailsContext(pageContext);
}