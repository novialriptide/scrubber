/*
 * Implementation of the Screen module
 * 
 * Some of the implementations were copied over
 * from https://gist.github.com/prashanthrajagopal/05f8ad157ece964d8c4d
 * 
 */
#ifdef _WIN32
// clang-format off
#include <windows.h>
#include <gdiplus.h>
#include <wchar.h>
// clang-format on
#endif
#include <stdio.h>

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
  UINT num = 0;
  UINT size = 0;

  Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

  Gdiplus::GetImageEncodersSize(&num, &size);
  if (size == 0)
    return -1;

  pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
  if (pImageCodecInfo == NULL)
    return -1;

  GetImageEncoders(num, size, pImageCodecInfo);
  for (UINT j = 0; j < num; ++j) {
    if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
      *pClsid = pImageCodecInfo[j].Clsid;
      free(pImageCodecInfo);
      return j;
    }
  }
  free(pImageCodecInfo);
  return 0;
}

class Screen {
 public:
  Screen() {
    IStream* istream;
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    HDC hdc = GetDC(NULL);
    HDC hDest = CreateCompatibleDC(hdc);

    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    HBITMAP hbDesktop = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hDest, hbDesktop);

    // Copy from the desktop device context to the bitmap device context.
    BitBlt(hDest, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    Gdiplus::Bitmap bitmap(hbDesktop, NULL);
    CLSID clsid;
    GetEncoderClsid(L"image/jpeg", &clsid);
    bitmap.Save(L"screen.jpeg", &clsid, NULL);  // To save the jpeg to a file
    bitmap.Save(istream, &clsid, NULL);

    ReleaseDC(NULL, hdc);
    DeleteObject(hbDesktop);
    DeleteDC(hDest);
    Gdiplus::GdiplusShutdown(gdiplusToken);
  }
  int Dump() { return 0; }
};