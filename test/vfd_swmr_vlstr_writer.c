/*
 * Copyright by The HDF Group.
 * Copyright by the Board of Trustees of the University of Illinois.
 * All rights reserved.
 *
 * This file is part of HDF5.  The full HDF5 copyright notice, including
 * terms governing use, modification, and redistribution, is contained in
 * the COPYING file, which can be found at the root of the source code
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.
 * If you do not have access to either file, you may request a copy from
 * help@hdfgroup.org.
 */

#define H5C_FRIEND /*suppress error about including H5Cpkg   */
#define H5F_FRIEND /*suppress error about including H5Fpkg   */

#include "hdf5.h"

#include "H5Cpkg.h"
#include "H5Fpkg.h"
#include "H5HGprivate.h"
#include "H5VLprivate.h"

#include "testhdf5.h"
#include "vfd_swmr_common.h"

enum _step { CREATE = 0, LENGTHEN, SHORTEN, DELETE, NSTEPS } step_t;

static hbool_t caught_out_of_bounds = FALSE;

static void
write_vl_dset(hid_t dset, hid_t type, hid_t space, char *data)
{
    if (H5Dwrite(dset, type, space, space, H5P_DEFAULT, &data) < 0) {
        HDfprintf(stderr, "%s: H5Dwrite\n", __func__);
        HDexit(EXIT_FAILURE);
    }
    if (H5Dflush(dset) < 0) {
        HDfprintf(stderr, "%s: H5Dflush\n", __func__);
        HDexit(EXIT_FAILURE);
    }
}

#if 0
static hid_t
initialize_dset(hid_t file, hid_t type, hid_t space, const char *name,
    void *data)
{
    hid_t dset;

    dset = H5Dcreate2(file, name, type, space, H5P_DEFAULT, H5P_DEFAULT,
        H5P_DEFAULT);

    if (dset == H5I_INVALID_HID) {
        HDfprintf(stderr, "H5Dcreate2\n");
        HDexit(EXIT_FAILURE);
    }

    if (H5Dwrite(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, data) < 0) {
        HDfprintf(stderr, "H5Dwrite\n");
        HDexit(EXIT_FAILURE);
    }

    if (H5Dflush(dset) < 0) {
        HDfprintf(stderr, "%s: H5Dflush\n", __func__);
        HDexit(EXIT_FAILURE);
    }

    return dset;
}

static void
rewrite_dset(hid_t dset, hid_t type, char *data)
{
    if (H5Dwrite(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, data) < 0) {
        HDfprintf(stderr, "%s: H5Dwrite\n", __func__);
        HDexit(EXIT_FAILURE);
    }
    if (H5Dflush(dset) < 0) {
        HDfprintf(stderr, "%s: H5Dflush\n", __func__);
        HDexit(EXIT_FAILURE);
    }
}
#endif

static hid_t
create_vl_dset(hid_t file, hid_t type, hid_t space, const char *name)
{
    hid_t dset;

    dset = H5Dcreate2(file, name, type, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    return dset;
}

static void
#ifndef H5C_COLLECT_CACHE_STATS
print_cache_hits(H5C_t *cache)
{
    int i;

    for (i = 0; i < H5AC_NTYPES; i++) {
        dbgf(3, "type-%d cache hits %" PRId64 "%s\n", i, cache->hits[i], (i == H5AC_GHEAP_ID) ? " *" : "");
    }
    dbgf(3, "\n");
}
#else
print_cache_hits(H5C_t H5_ATTR_UNUSED *cache)
{
    return;
}
#endif

static void
usage(const char *progname)
{
    HDfprintf(stderr, "usage: %s [-W] [-V]\n", progname);
    HDfprintf(stderr, "\n  -W: do not wait for SIGINT or SIGUSR1\n");
    HDfprintf(stderr, "\n  -S: do not use VFD SWMR\n");
    HDfprintf(stderr, "  -f: use fixed-length string\n");
    HDfprintf(stderr, "      (default: variable-length string)\n");
    HDfprintf(stderr, "  -n: number of test steps to perform\n");
    HDfprintf(stderr, "  -q: be quiet: few/no progress messages\n");
    HDfprintf(stderr, "  -t (oob|null): select out-of-bounds or NULL test\n");
    HDexit(EXIT_FAILURE);
}

bool
H5HG_trap(const char *reason)
{
    if (HDstrcmp(reason, "out of bounds") == 0) {
        caught_out_of_bounds = TRUE;
        return FALSE;
    }
    return TRUE;
}

int
main(int argc, char **argv)
{
    hid_t                 fapl, fcpl, fid, space, type;
    hid_t                 dset[2];
    char                  content[2][96];
    char                  name[2][96];
    H5F_t *               f;
    H5C_t *               cache;
    sigset_t              oldsigs;
    herr_t                ret;
    hbool_t               variable = TRUE, wait_for_signal = TRUE;
    const hsize_t         dims = 1;
    int                   ch, i, ntimes = 100;
    unsigned long         tmp;
    char *                end;
    hbool_t               use_vfd_swmr = TRUE;
    const struct timespec delay        = {.tv_sec = 0, .tv_nsec = 1000 * 1000 * 1000 / 10};
    testsel_t             sel          = TEST_NONE;

    HDassert(H5T_C_S1 != H5I_INVALID_HID);

    while ((ch = getopt(argc, argv, "SWfn:qt:")) != -1) {
        switch (ch) {
            case 'S':
                use_vfd_swmr = FALSE;
                break;
            case 'W':
                wait_for_signal = FALSE;
                break;
            case 'f':
                variable = FALSE;
                break;
            case 'n':
                errno = 0;
                tmp   = HDstrtoul(optarg, &end, 0);
                if (end == optarg || *end != '\0')
                    goto error;
                else if (errno != 0)
                    goto error;
                else if (tmp > INT_MAX)
                    goto error;
                ntimes = (int)tmp;
                break;
            case 'q':
                verbosity = 1;
                break;
            case 't':
                if (strcmp(optarg, "oob") == 0)
                    sel = TEST_OOB;
                else if (strcmp(optarg, "null") == 0)
                    sel = TEST_NULL;
                else
                    usage(argv[0]);
                break;
            default:
                usage(argv[0]);
                break;
        }
    }
    argv += optind;
    argc -= optind;

    if (argc > 0)
        goto error;

    fapl = vfd_swmr_create_fapl(TRUE, sel == TEST_OOB, use_vfd_swmr, "./vlstr-shadow");
    if (fapl < 0)
        goto error;

    if ((fcpl = H5Pcreate(H5P_FILE_CREATE)) < 0)
        goto error;

    ret = H5Pset_file_space_strategy(fcpl, H5F_FSPACE_STRATEGY_PAGE, FALSE, 1);
    if (ret < 0)
        goto error;

    fid = H5Fcreate("vfd_swmr_vlstr.h5", H5F_ACC_TRUNC, fcpl, fapl);

    /* Create the VL string datatype and a scalar dataspace, or a
     * fixed-length string datatype and a simple dataspace.
     */
    if ((type = H5Tcopy(H5T_C_S1)) == H5I_INVALID_HID)
        goto error;

    if (!variable) {
        if (H5Tset_size(type, 32) < 0)
            goto error;
        space = H5Screate_simple(1, &dims, NULL);
    }
    else {
        if (H5Tset_size(type, H5T_VARIABLE) < 0)
            goto error;
        space = H5Screate(H5S_SCALAR);
    }

    if (space == H5I_INVALID_HID)
        goto error;

    if ((f = H5VL_object_verify(fid, H5I_FILE)) == NULL)
        goto error;

    cache = f->shared->cache;

    if (fid == H5I_INVALID_HID)
        goto error;

    block_signals(&oldsigs);

    print_cache_hits(cache);

    /* content 1 seq 1 short
     * content 1 seq 1 long long long long long long long long
     * content 1 seq 1 medium medium medium
     */
    for (i = 0; i < ntimes; i++) {
        const int ndsets = 2;
        const int step   = i % NSTEPS;
        const int which  = (i / NSTEPS) % ndsets;
        const int seq    = i / (ndsets * NSTEPS);
        dbgf(2, "iteration %d which %d step %d seq %d\n", i, which, step, seq);
        switch (step) {
            case CREATE:
                (void)HDsnprintf(name[which], sizeof(name[which]), "dset-%d", which);
                (void)HDsnprintf(content[which], sizeof(content[which]), "content %d seq %d short", which, seq);
                dset[which] = create_vl_dset(fid, type, space, name[which]);
                write_vl_dset(dset[which], type, space, content[which]);
                break;
            case LENGTHEN:
                (void)HDsnprintf(content[which], sizeof(content[which]),
                               "content %d seq %d long long long long long long long long", which, seq);
                write_vl_dset(dset[which], type, space, content[which]);
                break;
            case SHORTEN:
                (void)HDsnprintf(content[which], sizeof(content[which]),
                               "content %d seq %d medium medium medium", which, seq);
                write_vl_dset(dset[which], type, space, content[which]);
                break;
            case DELETE:
                if (H5Dclose(dset[which]) < 0)
                    goto error;
                if (H5Ldelete(fid, name[which], H5P_DEFAULT) < 0) {
                    goto error;
                }
                break;
            default:
                goto error;
        }
        if (caught_out_of_bounds) {
            HDfprintf(stderr, "caught out of bounds\n");
            break;
        }
        HDnanosleep(&delay, NULL);
    }

    if (use_vfd_swmr && wait_for_signal)
        await_signal(fid);

    restore_signals(&oldsigs);

    if (H5Pclose(fapl) < 0)
        goto error;

    if (H5Pclose(fcpl) < 0)
        goto error;

    if (H5Tclose(type) < 0)
        goto error;

    if (H5Sclose(space) < 0)
        goto error;

    if (H5Fclose(fid) < 0)
        goto error;

    return EXIT_SUCCESS;

error:
    return EXIT_FAILURE;
}
