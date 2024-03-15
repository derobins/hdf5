/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://www.hdfgroup.org/licenses.               *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef H5FOprivate_H
#define H5FOprivate_H

/** \cond INTERNAL */

/* Open object information
 *
 * Tracks the objects currently open in a file, for various internal mechanisms
 * which need to be aware of such things.
 */

#include "H5private.h" /* Generic Functions                       */

/* Forward declarations */
struct H5F_t;

/** The open objects */
typedef struct H5FO_objects H5FO_objects_t;

/** The open object counts */
typedef struct H5FO_counts H5FO_counts_t;

/* API calls that manipulate the H5FO objects in H5F_shared_t */
H5_DLL H5FO_objects_t *H5FO_create(void);
H5_DLL void           *H5FO_opened(H5FO_objects_t *objects, haddr_t addr);
H5_DLL herr_t          H5FO_insert(H5FO_objects_t *objects, haddr_t addr, void *obj, bool delete_flag);
H5_DLL herr_t          H5FO_delete(H5FO_objects_t *objects, struct H5F_t *f, haddr_t addr);
H5_DLL herr_t          H5FO_mark(H5FO_objects_t *objects, haddr_t addr, bool delete_flag);
H5_DLL bool            H5FO_marked(H5FO_objects_t *objects, haddr_t addr);
H5_DLL herr_t          H5FO_dest(H5FO_objects_t *objects);

/* API calls that manipulate the H5FO object counts in H5F_t */
H5_DLL H5FO_counts_t *H5FO_top_create(void);
H5_DLL herr_t         H5FO_top_incr(H5FO_counts_t *counts, haddr_t addr);
H5_DLL herr_t         H5FO_top_decr(H5FO_counts_t *counts, haddr_t addr);
H5_DLL hsize_t        H5FO_top_count(H5FO_counts_t *counts, haddr_t addr);
H5_DLL herr_t         H5FO_top_dest(H5FO_counts_t *counts);

/** \endcond */

#endif /* H5FOprivate_H */
