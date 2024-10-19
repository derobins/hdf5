/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the LICENSE file, which can be found at the root of the source code       *
 * distribution tree, or in https://www.hdfgroup.org/licenses.               *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Purpose: This file deals with Windows compatibility. MSVC is largely C99
 *          compliant now, but we still need work-arounds for some POSIX
 *          things and MinGW.
 *
 *          This file must be included before the H5private.h HD mappings, so
 *          the definitions here will supersede them.
 */

#ifdef H5_HAVE_WIN32_API

/* Win32 platform-independent definition for struct stat. For POSIX, see
 * H5private.h.
 */
typedef struct _stati64 h5_stat_t;

#ifdef H5_HAVE_VISUAL_STUDIO
struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};
#endif

#define HDcreat(S, M)        Wopen(S, O_CREAT | O_TRUNC | O_RDWR, M)
#define HDflock(F, L)        Wflock(F, L)
#define HDfstat(F, B)        _fstati64(F, B)
#define HDftell(F)           _ftelli64(F)
#define HDgetdcwd(D, S, Z)   _getdcwd(D, S, Z)
#define HDgetdrive()         _getdrive()
#define HDgettimeofday(V, Z) Wgettimeofday(V, Z)
#define HDlseek(F, O, W)     _lseeki64(F, O, W)
#define HDlstat(S, B)        _lstati64(S, B)
#define HDmkdir(S, M)        _mkdir(S)

/* We only support the standards conformant preprocessor */
#define HDopen(S, F, ...) Wopen(S, F, ##__VA_ARGS__)

#define HDremove(S)           Wremove(S)
#define HDsetenv(N, V, O)     Wsetenv(N, V, O)
#define HDsetvbuf(F, S, M, Z) setvbuf(F, S, M, (Z > 1 ? Z : 2))
#define HDsleep(S)            Sleep(S * 1000)
#define HDstat(S, B)          _stati64(S, B)
#define HDstrcasecmp(A, B)    _stricmp(A, B)
#define HDstrcasestr(A, B)    Wstrcasestr_wrap(A, B)
#define HDstrndup(S, N)       H5_strndup(S, N)
#define HDstrtok_r(X, Y, Z)   strtok_s(X, Y, Z)
#define HDunsetenv(N)         Wsetenv(N, "", 1)

#ifndef H5_HAVE_MINGW
#define HDftruncate(F, L) _chsize_s(F, L)
#define HDfseek(F, O, W)  _fseeki64(F, O, W)
#endif

#ifdef __cplusplus
extern "C" {
#endif
H5_DLL int      Wgettimeofday(struct timeval *tv, struct timezone *tz);
H5_DLL int      Wsetenv(const char *name, const char *value, int overwrite);
H5_DLL int      Wflock(int fd, int operation);
H5_DLL herr_t   H5_expand_windows_env_vars(char **env_var);
H5_DLL wchar_t *H5_get_utf16_str(const char *s);
H5_DLL int      Wopen(const char *path, int oflag, ...);
H5_DLL int      Wremove(const char *path);
H5_DLL int      H5_get_win32_times(H5_timevals_t *tvs);
H5_DLL char    *H5_strndup(const char *s, size_t n);
H5_DLL char    *Wstrcasestr_wrap(const char *haystack, const char *needle);
#ifdef __cplusplus
}
#endif

#endif /* H5_HAVE_WIN32_API */
