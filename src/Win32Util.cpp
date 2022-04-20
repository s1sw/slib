#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../include/slib/Win32Util.hpp"
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
