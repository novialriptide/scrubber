#ifdef _WIN32
// clang-format off
#include <windows.h>
#include <gdiplus.h>
#include <wchar.h>
// clang-format on
#endif
#include <stdio.h>
#include <stdexcept>

#ifdef _WIN32
/**
 * @brief Get the Encoder Clsid object
 * 
 * Code taken straight from https://learn.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-retrieving-the-class-identifier-for-an-encoder-use?redirectedfrom=MSDN
 * I have no idea what this does. - Andrew
 * 
 * @param format 
 * @param pClsid 
 * @return int 
 */
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
#endif

/**
 * @brief Get the Primary Monitor Stream Data object in JPEG format.
 * 
 * @return IStream* 
 */
#ifdef _WIN32
IStream* GetPrimaryMonitorStreamData() {
  IStream* istream = (IStream*)malloc(sizeof(IStream));

  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;

  Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
  {
    HDC src = GetDC(NULL);
    HDC dest = CreateCompatibleDC(src);

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    HBITMAP result = CreateCompatibleBitmap(src, width, height);
    SelectObject(dest, result);

    // Copy from the desktop device context to the bitmap device context.
    BitBlt(dest, 0, 0, width, height, src, 0, 0, SRCCOPY);

    Gdiplus::Bitmap bitmap(result, NULL);
    CLSID clsid;
    int encoder_clsid = GetEncoderClsid(L"image/jpeg", &clsid);
    if (encoder_clsid < 0) {
      throw std::runtime_error("JPEG encoder not included with system.");
    }

    bitmap.Save(L"screen.jpeg", &clsid, NULL);
    // bitmap.Save(istream, &clsid, NULL);

    DeleteObject(result);
    DeleteObject(dest);
    ReleaseDC(NULL, src);
  }

  Gdiplus::GdiplusShutdown(gdiplusToken);

  return istream;
}
#endif