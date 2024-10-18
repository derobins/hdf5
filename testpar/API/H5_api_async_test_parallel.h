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

#ifndef H5_API_ASYNC_TEST_PARALLEL_H_
#define H5_API_ASYNC_TEST_PARALLEL_H_

#include "H5_api_test_parallel.h"

int H5_api_async_test_parallel(void);

/********************************************************
 *                                                      *
 *           API parallel async test defines            *
 *                                                      *
 ********************************************************/

#define PAR_ASYNC_API_TEST_FILE        "H5_api_async_test_parallel.h5"
#define PAR_ASYNC_API_TEST_FILE_PRINTF "H5_api_async_test_parallel_%d.h5"

#endif /* H5_API_ASYNC_TEST_PARALLEL_H_ */
