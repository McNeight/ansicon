/*
  debugstr.c - Auxiliary debug functionality.
*/

#include "ansicon.h"

#if (MYDEBUG > 1)
char tempfile[MAX_PATH];
#endif

#if (MYDEBUG > 0)
void DEBUGSTR( LPTSTR szFormat, ... ) // sort of OutputDebugStringf
{
  TCHAR szBuffer[1024], szEscape[1024];
  va_list pArgList;

#if (MYDEBUG > 1)
  if (*tempfile == '\0')
    _snprintf( tempfile, MAX_PATH, "%s\\ansicon.log", getenv( "TEMP" ) );
  if (szFormat == NULL)
  {
    DeleteFileA( tempfile );
    return;
  }
#endif

  va_start( pArgList, szFormat );
  _vsnwprintf( szBuffer, lenof(szBuffer), szFormat, pArgList );
  va_end( pArgList );

  szFormat = szBuffer;
  if (*szFormat == '\\')
  {
    BOOL first = TRUE;
    LPTSTR pos = szEscape;
    while (*++szFormat != '\0' && pos < szEscape + lenof(szEscape) - 4)
    {
      if (*szFormat < 32)
      {
	*pos++ = '\\';
	switch (*szFormat)
	{
	  case '\a': *pos++ = 'a'; break;
	  case '\b': *pos++ = 'b'; break;
	  case '\t': *pos++ = 't'; break;
	  case '\r': *pos++ = 'r'; break;
	  case '\n': *pos++ = 'n'; break;
	  case	27 : *pos++ = 'e'; break;
	  default:
	    pos += _snwprintf( pos, 32, L"%.*o",
			     (szFormat[1] >= '0' && szFormat[1] <= '7') ? 3 : 1,
			     *szFormat );
	}
      }
      else if (*szFormat == '"')
      {
	if (first)
	  first = FALSE;
	else if (szFormat[1] == '\0')
	  ;
	else
	  *pos++ = '\\';
	*pos++ = '"';
      }
      else
      {
	*pos++ = *szFormat;
      }
    }
    *pos = '\0';
    szFormat = szEscape;
  }
#if (MYDEBUG > 1)
  {
  FILE* file = fopen( tempfile, "at" ); // _fmode might be binary
  if (file != NULL)
  {
    fwprintf( file, L"%s\n", szFormat );
    fclose( file );
  }
  }
#else
  OutputDebugString( szFormat );
#endif
}
#endif
