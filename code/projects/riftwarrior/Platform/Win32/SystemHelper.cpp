///
/// Talk is cheap. Show me the code.    - Linus Torvalds
///
/// Author: WiNGC (wingc@wingc.net)
/// All rights reserved
///

#include "SystemHelper.h"

#include <codecvt>
#include <string>
#include <mutex>

#include <XmlLite.h>
#include <Wrl\Client.h>

#include <AutoResource.h>
#include <ErrorThrows.h>

#pragma comment(lib, "XmlLite")

using namespace std;
using namespace Microsoft::WRL;

static std::once_flag       s_manifestFileReaderLoaded;
static unique_Heap          s_manifestFileBuffer;
static ComPtr<IXmlReader>   s_manifestFileReader;

void _GetManifestFileReader(ComPtr<IXmlReader>& outFileReader)
{
    std::call_once(
        s_manifestFileReaderLoaded,
        []()
        {
            ComPtr<IStream> fileReaderStream;
            ComPtr<IXmlReaderInput> fileReaderInput;
            unqiue_FileHandle manifestFile;
            FILE_STANDARD_INFO fileInfo = {};

            manifestFile = ::CreateFile2(
                L"WMAppManifest.xml",
                GENERIC_READ,
                FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                OPEN_EXISTING,
                nullptr);
            CHECK_WIN32ERR(manifestFile.is_valid());
            CHECK(::GetFileInformationByHandleEx(
                manifestFile.get(), 
                FILE_INFO_BY_HANDLE_CLASS::FileStandardInfo, 
                &fileInfo, 
                sizeof(fileInfo)));
            ASSERT(fileInfo.EndOfFile.LowPart > 0 && fileInfo.EndOfFile.HighPart == 0);

            s_manifestFileBuffer = ::HeapAlloc(::GetProcessHeap(), 0, fileInfo.EndOfFile.LowPart);
            CHECK_WIN32ERR(s_manifestFileBuffer.is_valid());

            CHECK(::ReadFile(manifestFile.get(), s_manifestFileBuffer.get(), fileInfo.EndOfFile.LowPart, nullptr, nullptr));
            CHECK(::CreateStreamOnHGlobal(reinterpret_cast<HGLOBAL>(s_manifestFileBuffer.get()), FALSE, fileReaderStream.GetAddressOf()));
            CHECK(::CreateXmlReader(
                __uuidof(IXmlReader), 
                reinterpret_cast<PVOID*>(s_manifestFileReader.GetAddressOf()), 
                nullptr));
            CHECK(::CreateXmlReaderInputWithEncodingName(
                fileReaderStream.Get(), 
                nullptr, 
                L"UTF-8", 
                TRUE, 
                nullptr, 
                fileReaderInput.GetAddressOf()));
            CHECK(s_manifestFileReader->SetInput(fileReaderInput.Get()));
    });

    CHECK(s_manifestFileReader.CopyTo(outFileReader.ReleaseAndGetAddressOf()));
}

string SystemHelper::getVersionInfo()
{
    ComPtr<IXmlReader> xmlReader;
    XmlNodeType nodeType = XmlNodeType_None;

    wstring version;
    string prefix("Version:");

    _GetManifestFileReader(xmlReader);

    bool foundVersion = false;
    while (!foundVersion)
    {
        CHECK(xmlReader->Read(&nodeType));

        switch (nodeType)
        {
        case XmlNodeType_Element:
        {
            PCWSTR pElementName = nullptr;
            PCWSTR pValue = nullptr;
            CHECK(xmlReader->GetLocalName(&pElementName, nullptr));
            if (::wcscmp(pElementName, L"App") == 0)
            {
                CHECK(xmlReader->MoveToAttributeByName(L"Version", nullptr));
                CHECK(xmlReader->GetValue(&pValue, nullptr));
                version.assign(pValue);
                foundVersion = true;
            }
            break;
        }
        default:
            break;
        }
    }

    return prefix + SystemHelper::WideStringToNarrower.to_bytes(version);
}

wstring_convert<codecvt_utf8<wchar_t>, wchar_t> SystemHelper::WideStringToNarrower;