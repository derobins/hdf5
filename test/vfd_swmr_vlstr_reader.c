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
// #include "H5Iprivate.h"
#include "H5HGprivate.h"
#include "H5VLprivate.h"

#include "testhdf5.h"
#include "vfd_swmr_common.h"

typedef enum _step { CREATE = 0, LENGTHEN, SHORTEN, DELETE, NSTEPS } step_t;

static hbool_t        caught_out_of_bounds = FALSE;
static hbool_t        read_null            = FALSE;

static bool
read_vl_dset(hid_t dset, hid_t type, char **data)
{
    hbool_t           success;
    estack_state_t es;

    es      = disable_estack();
    success = H5Dread(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, data) >= 0;
    if (*data == NULL) {
        read_null = TRUE;
        return FALSE;
    }
    restore_estack(es);

    return success;
}

static void
usage(const char *progname)
{
    fprintf(stderr, "usage: %s [-W] [-V] [-t (oob|null)] \n", progname);
    fprintf(stderr, "\n  -S: do not use VFD SWMR\n");
    fprintf(stderr, "  -n: number of test steps to perform\n");
    fprintf(stderr, "  -q: be quiet: few/no progress messages\n");
    fprintf(stderr, "  -t (oob|null): select out-of-bounds or NULL test\n");
    exit(EXIT_FAILURE);
}

bool
H5HG_trap(const char *reason)
{
    if (HDstrcmp(reason, "out of bounds") == 0) {
        caught_out_of_bounds = TRUE;
        return TRUE;
    }
    return FALSE;
}

/* TODO: Handle error cleanup properly */
int
main(int argc, char **argv)
{
    hid_t                 fapl, fid, space, type;
    hid_t                 dset[2];
    char *                content[2];
    char                  name[2][96];
    int                   ch, i, ntimes = 100;
    unsigned long         tmp;
    hbool_t                  use_vfd_swmr = TRUE;
    char *                end;
    const long            millisec_in_nanosecs = 1000 * 1000;
    const struct timespec delay                = {.tv_sec = 0, .tv_nsec = millisec_in_nanosecs * 11 / 10};
    testsel_t             sel                  = TEST_NONE;

    assert(H5T_C_S1 != H5I_INVALID_HID);

    while ((ch = getopt(argc, argv, "Sn:qt:")) != -1) {
        switch (ch) {
            case 'S':
                use_vfd_swmr = FALSE;
                break;
            case 'n':
                errno = 0;
                tmp   = HDstrtoul(optarg, &end, 0);
                if (end == optarg || *end != '\0')
                    HDexit(EXIT_FAILURE);
                else if (errno != 0) {
                    HDfprintf(stderr, "couldn't parse `-n` argument `%s`", optarg);
                    HDexit(EXIT_FAILURE);
                }
                else if (tmp > INT_MAX)
                    HDexit(EXIT_FAILURE);
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
        HDexit(EXIT_FAILURE);

    fapl = vfd_swmr_create_fapl(FALSE, sel == TEST_OOB, use_vfd_swmr, "./vlstr-shadow");
    if (fapl < 0)
        goto error;

    fid = H5Fopen("vfd_swmr_vlstr.h5", H5F_ACC_RDONLY, fapl);

    /* Create the VL string datatype and a scalar dataspace */
    if ((type = H5Tcopy(H5T_C_S1)) == H5I_INVALID_HID)
        goto error;

    if (H5Tset_size(type, H5T_VARIABLE) < 0)
        goto error;
    space = H5Screate(H5S_SCALAR);

    if (space == H5I_INVALID_HID)
        goto error;

    if (fid == H5I_INVALID_HID)
        goto error;

    /* content 1 seq 1 short
     * content 1 seq 1 long long long long long long long long
     * content 1 seq 1 medium medium medium
     */
    for (i = 0; !caught_out_of_bounds && i < ntimes; (i % 2 == 0) ? nanosleep(&delay, NULL) : 0, i++) {
        estack_state_t es;
        const int      ndsets = 2;
        const int      which  = i % ndsets;
        int            nconverted;
        struct {
            int  which;
            int  seq;
            char tail[96];
        } scanned_content;

        dbgf(2, "iteration %d which %d", i, which);
        (void)snprintf(name[which], sizeof(name[which]), "dset-%d", which);
        es          = disable_estack();
        dset[which] = H5Dopen(fid, name[which], H5P_DEFAULT);
        restore_estack(es);
        if (caught_out_of_bounds || dset[which] == H5I_INVALID_HID) {
            dbgf(2, ": couldn't open\n");
            continue;
        }
        if (!read_vl_dset(dset[which], type, &content[which])) {
            H5Dclose(dset[which]);
            dbgf(2, ": couldn't read\n");
            continue;
        }
        nconverted = sscanf(content[which], "content %d seq %d %96s", &scanned_content.which,
                            &scanned_content.seq, scanned_content.tail);
        if (nconverted != 3) {
            dbgf(2, ": couldn't scan\n");
            continue;
        }
        dbgf(2, ": read which %d seq %d tail %s\n", scanned_content.which, scanned_content.seq,
             scanned_content.tail);
        H5Dclose(dset[which]);
    }

    if (caught_out_of_bounds)
        HDfprintf(stderr, "caught out of bounds\n");

    if (read_null)
        HDfprintf(stderr, "read NULL\n");

    if (H5Pclose(fapl) < 0)
        goto error;

    if (H5Tclose(type) < 0)
        goto error;

    if (H5Sclose(space) < 0)
        goto error;

    if (H5Fclose(fid) < 0)
        goto error;

    if (sel == TEST_OOB)
        return caught_out_of_bounds ? EXIT_SUCCESS : EXIT_FAILURE;
    else if (sel == TEST_NULL)
        return read_null ? EXIT_SUCCESS : EXIT_FAILURE;

    return EXIT_SUCCESS;

error:
    return EXIT_FAILURE;
}
