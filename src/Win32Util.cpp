#ifdef _WIN32
#include "../include/slib/Win32Util.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shobjidl.h>
#include <winrt/base.h>
namespace winrt::impl
{
    template <typename Async>
    auto wait_for(Async const& async, Windows::Foundation::TimeSpan const& timeout);
}
#include <winrt/windows.ui.viewmanagement.h>

#pragma comment(lib, "windowsapp")

namespace slib {
    void Win32Util::enumerateDriveLetters(void(*callback)(char, void*), void* data) {
        const char* driveLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        DWORD presentDrives = GetLogicalDrives();
        for (int i = 0; i < 26; i++) {
            bool isPresent = (presentDrives & (1 << i)) > 0;

            if (isPresent)
                callback(driveLetters[i], data);
        }
    }

    slib::String Win32Util::openFolder() {
        IFileDialog* dialog = nullptr;

        HRESULT result = CoCreateInstance(
            CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&dialog)
        );

        DWORD flags;
        result = dialog->GetOptions(&flags);

        dialog->SetOptions(flags | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
        dialog->Show(nullptr);

        IShellItem* shellResult;
        result = dialog->GetResult(&shellResult);

        if (FAILED(result)) {
            return "";
        }

        PWSTR pszFilePath = nullptr;
        shellResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

        int size = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, nullptr, 0, nullptr, nullptr);

        slib::String path;
        path.resize(size);

        WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, path.data(), path.byteLength(), nullptr, nullptr);

        CoTaskMemFree(pszFilePath);

        return path;
    }

    const COMDLG_FILTERSPEC fileTypes[] =
    {
        {L"All Files (*.*)", L"*.*"}
    };

    slib::String Win32Util::openFile(const slib::String& extension) {
        IFileDialog* dialog = nullptr;

        HRESULT result = CoCreateInstance(
            CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&dialog)
        );

        DWORD flags;
        result = dialog->GetOptions(&flags);

        dialog->SetOptions(flags | FOS_FORCEFILESYSTEM);
        dialog->Show(nullptr);

        IShellItem* shellResult;
        result = dialog->GetResult(&shellResult);

        if (FAILED(result)) {
            return "";
        }

        PWSTR pszFilePath = nullptr;
        shellResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

        int size = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, nullptr, 0, nullptr, nullptr);

        slib::String path;
        path.resize(size);

        WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, path.data(), path.byteLength(), nullptr, nullptr);

        CoTaskMemFree(pszFilePath);

        return path;
    }

    bool Win32Util::isFileHidden(const char* path) {
        DWORD attributes = GetFileAttributesA(path);

        return (attributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN;
    }

    void Win32Util::initialiseWinRT() {
        winrt::init_apartment();
    }

    void Win32Util::getAccentColor(uint8_t& r, uint8_t& g, uint8_t& b) {
        winrt::Windows::UI::ViewManagement::UISettings settings;
        winrt::Windows::UI::Color col = settings.GetColorValue(winrt::Windows::UI::ViewManagement::UIColorType::Accent);
        r = col.R;
        g = col.G;
        b = col.B;
    }
}
#endif
