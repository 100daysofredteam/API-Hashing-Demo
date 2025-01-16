#include <windows.h>

int main(void)
{
    MessageBox(
              0,       /*   [in, optional] HWND    hWnd,      */
              "100 Days of red team", /*   [in, optional] LPCTSTR lpText,    */
              "100 Days of red team", /*   [in, optional] LPCTSTR lpCaption, */
              1        /*   [in]           UINT    uType      */
             );
    return 0;
}
