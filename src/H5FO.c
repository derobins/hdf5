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

#include "H5Eprivate.h"  /* Error handling                          */
#include "H5Fprivate.h"  /* File access                             */
#include "H5FLprivate.h" /* Free lists                              */
#include "H5FOprivate.h" /* File objects                            */
#include "H5Oprivate.h"  /* Object headers                          */

/* Information about open objects in a file */
typedef struct H5FO_open_obj_t {
    haddr_t addr;      /* Address of object header for object */
    void   *obj;       /* Pointer to the object */
    hbool_t deleted;   /* Flag to indicate that the object was deleted from the file */
    UT_hash_handle hh; /* Hash table handle (must be LAST) */
} H5FO_open_obj_t;

/* Information about counted objects in a file */
typedef struct H5FO_obj_count_t {
    haddr_t addr;      /* Address of object header for object */
    hsize_t count;     /* Number of times object is opened */
    UT_hash_handle hh; /* Hash table handle (must be LAST) */
} H5FO_obj_count_t;

/* The open objects */
struct H5FO_objects {
    H5FO_open_obj_t *hash_table; /* Hash table pointer open objects */
};

/* The open object counts */
struct H5FO_counts {
    H5FO_obj_count_t *hash_table; /* Hash table pointer for open object counts */
};

H5FL_DEFINE_STATIC(H5FO_open_obj_t);
H5FL_DEFINE_STATIC(H5FO_obj_count_t);
H5FL_DEFINE_STATIC(H5FO_objects_t);
H5FL_DEFINE_STATIC(H5FO_counts_t);

/*--------------------------------------------------------------------------
 NAME
    H5FO_create
 PURPOSE
    Create an open object info set
 USAGE
    herr_t H5FO_create(f)
        H5F_t *f;       IN/OUT: File to create opened object info set for

 RETURNS
    Returns non-negative on success, negative on failure
 DESCRIPTION
    Create a new open object info set.
--------------------------------------------------------------------------*/
H5FO_objects_t *
H5FO_create(void)
{
    H5FO_objects_t *ret_value = NULL;

    FUNC_ENTER_NOAPI(NULL)

    if ((ret_value = H5FL_CALLOC(H5FO_objects_t)) == NULL)
        HGOTO_ERROR(H5E_FILE, H5E_NOSPACE, NULL, "memory allocation failed")

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_opened
 PURPOSE
    Checks if an object at an address is already open in the file.
 USAGE
    void * H5FO_opened(f,addr)
        const H5F_t *f;         IN: File to check opened object info set
        haddr_t addr;           IN: Address of object to check

 RETURNS
    Returns a pointer to the object on success, NULL on failure
 DESCRIPTION
    Check is an object at an address (the address of the object's object header)
    is already open in the file and return the ID for that object if it is open.
--------------------------------------------------------------------------*/
void *
H5FO_opened(H5FO_objects_t *objects, haddr_t addr)
{
    H5FO_open_obj_t *open_obj = NULL; /* Information about open object */
    void            *ret_value;

    FUNC_ENTER_NOAPI_NOERR

    HDassert(objects);
    HDassert(H5F_addr_defined(addr));

    /* Get the object node from the container */
    HASH_FIND(hh, objects->hash_table, &addr, sizeof(haddr_t), open_obj);

    if (NULL != open_obj) {
        ret_value = open_obj->obj;
        HDassert(ret_value != NULL);
    }
    else
        ret_value = NULL;

    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_insert
 PURPOSE
    Insert a newly opened object/pointer pair into the opened object info set
 USAGE
    herr_t H5FO_insert(f,addr,obj)
        H5F_t *f;               IN/OUT: File's opened object info set
        haddr_t addr;           IN: Address of object to insert
        void *obj;              IN: Pointer to object to insert
        hbool_t delete_flag;    IN: Whether to 'mark' this object for deletion

 RETURNS
    Returns a non-negative on success, negative on failure
 DESCRIPTION
    Insert an object/ID pair into the opened object info set.
--------------------------------------------------------------------------*/
herr_t
H5FO_insert(H5FO_objects_t *objects, haddr_t addr, void *obj, hbool_t delete_flag)
{
    H5FO_open_obj_t *open_obj; /* Information about open object */
    herr_t           ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    HDassert(objects);
    HDassert(H5F_addr_defined(addr));
    HDassert(obj);

    /* Allocate new opened object information structure */
    if ((open_obj = H5FL_MALLOC(H5FO_open_obj_t)) == NULL)
        HGOTO_ERROR(H5E_FILE, H5E_NOSPACE, FAIL, "memory allocation failed")

    /* Assign information */
    open_obj->addr    = addr;
    open_obj->obj     = obj;
    open_obj->deleted = delete_flag;

    /* Insert into container */
    HASH_ADD(hh, objects->hash_table, addr, sizeof(haddr_t), open_obj);

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_delete
 PURPOSE
    Remove an opened object/ID pair from the opened object info set
 USAGE
    herr_t H5FO_delete(f,addr)
        H5F_t *f;               IN/OUT: File's opened object info set
        haddr_t addr;           IN: Address of object to remove

 RETURNS
    Returns a non-negative on success, negative on failure
 DESCRIPTION
    Remove an object/ID pair from the opened object info.
--------------------------------------------------------------------------*/
herr_t
H5FO_delete(H5FO_objects_t *objects, struct H5F_t *f, haddr_t addr)
{
    H5FO_open_obj_t *open_obj  = NULL; /* Information about open object */
    herr_t           ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    HDassert(objects);
    HDassert(H5F_addr_defined(addr));

    /* Remove from container */
    HASH_FIND(hh, objects->hash_table, &addr, sizeof(haddr_t), open_obj);
    HASH_DELETE(hh, objects->hash_table, open_obj);

    /* Check if the object was deleted from the file */
    if (open_obj->deleted) {
        if (H5O_delete(f, addr) < 0)
            HGOTO_ERROR(H5E_FILE, H5E_CANTDELETE, FAIL, "can't delete object from file")
    }

    /* Release the object information */
    open_obj = H5FL_FREE(H5FO_open_obj_t, open_obj);

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_mark
 PURPOSE
    Mark an object to be deleted when it is closed
 USAGE
    herr_t H5FO_mark(f,addr)
        const H5F_t *f;         IN: File opened object is in
        haddr_t addr;           IN: Address of object to delete

 RETURNS
    Returns a non-negative ID for the object on success, negative on failure
 DESCRIPTION
    Mark an opened object for deletion from the file when it is closed.
--------------------------------------------------------------------------*/
herr_t
H5FO_mark(H5FO_objects_t *objects, haddr_t addr, hbool_t deleted)
{
    H5FO_open_obj_t *open_obj  = NULL; /* Information about open object */
    herr_t           ret_value = SUCCEED;

    FUNC_ENTER_NOAPI_NOERR

    HDassert(objects);
    HDassert(H5F_addr_defined(addr));

    /* Get the object node from the container */
    HASH_FIND(hh, objects->hash_table, &addr, sizeof(haddr_t), open_obj);

    if (NULL != open_obj)
        open_obj->deleted = deleted;
    else
        ret_value = FAIL;

    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_marked
 PURPOSE
    Check if an object is marked to be deleted when it is closed
 USAGE
    hbool_t H5FO_marked(f,addr)
        const H5F_t *f;         IN: File opened object is in
        haddr_t addr;           IN: Address of object to delete

 RETURNS
    Returns a TRUE/FALSE on success
 DESCRIPTION
    Checks if the object is currently in the "opened objects" tree and
    whether its marks for deletion from the file when it is closed.
--------------------------------------------------------------------------*/
hbool_t
H5FO_marked(H5FO_objects_t *objects, haddr_t addr)
{
    H5FO_open_obj_t *open_obj  = NULL; /* Information about open object */
    hbool_t          ret_value = FALSE;

    FUNC_ENTER_NOAPI_NOERR

    HDassert(objects);
    HDassert(H5F_addr_defined(addr));

    /* Get the object node from the container */
    HASH_FIND(hh, objects->hash_table, &addr, sizeof(haddr_t), open_obj);

    if (NULL != open_obj)
        ret_value = open_obj->deleted;

    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_dest
 PURPOSE
    Destroy an open object info set
 USAGE
    herr_t H5FO_dest(f)
        H5F_t *f;               IN/OUT: File's opened object info set

 RETURNS
    Returns a non-negative on success, negative on failure
 DESCRIPTION
    Destroy an existing open object info set.
--------------------------------------------------------------------------*/
herr_t
H5FO_dest(H5FO_objects_t *objects)
{
    herr_t ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    HDassert(objects);

    /* NOTE: Use HDONE_ERROR to push errors and keep going */

    /* Check if the object info set is empty */
    if (HASH_COUNT(objects->hash_table) != 0)
        HDONE_ERROR(H5E_FILE, H5E_CANTRELEASE, FAIL, "objects still in open object info set")

    /* Release memory */
    H5FL_FREE(H5FO_objects_t, objects);

    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_top_create
 PURPOSE
    Create the "top" open object count set
 USAGE
    herr_t H5FO_create(f)
        H5F_t *f;       IN/OUT: File to create opened object count set for

 RETURNS
    Returns non-negative on success, negative on failure
 DESCRIPTION
    Create a new open object count set.
--------------------------------------------------------------------------*/
H5FO_counts_t *
H5FO_top_create(void)
{
    H5FO_counts_t *ret_value = NULL;

    FUNC_ENTER_NOAPI(NULL)

    if ((ret_value = H5FL_CALLOC(H5FO_counts_t)) == NULL)
        HGOTO_ERROR(H5E_FILE, H5E_NOSPACE, NULL, "memory allocation failed")

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_top_incr
 PURPOSE
    Increment the "top" reference count for an object in a file
 USAGE
    herr_t H5FO_top_incr(f, addr)
        H5F_t *f;               IN/OUT: File's opened object info set
        haddr_t addr;           IN: Address of object to increment

 RETURNS
    Returns a non-negative on success, negative on failure
 DESCRIPTION
    Increment the reference count for an object in the opened object count set.
--------------------------------------------------------------------------*/
herr_t
H5FO_top_incr(H5FO_counts_t *counts, haddr_t addr)
{
    H5FO_obj_count_t *obj_count = NULL;
    herr_t            ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    HDassert(counts);
    HDassert(H5F_addr_defined(addr));

    /* Get the object node from the container */
    HASH_FIND(hh, counts->hash_table, &addr, sizeof(haddr_t), obj_count);

    if (NULL != obj_count) {
        (obj_count->count)++;
    }
    else {
        /* Allocate new opened object information structure */
        if (NULL == (obj_count = H5FL_MALLOC(H5FO_obj_count_t)))
            HGOTO_ERROR(H5E_FILE, H5E_NOSPACE, FAIL, "memory allocation failed")

        /* Assign information */
        obj_count->addr  = addr;
        obj_count->count = 1;

        /* Insert into container */
        HASH_ADD(hh, counts->hash_table, addr, sizeof(haddr_t), obj_count);
    }

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_top_decr
 PURPOSE
    Decrement the "top" reference count for an object in a file
 USAGE
    herr_t H5FO_top_decr(f, addr)
        H5F_t *f;               IN/OUT: File's opened object info set
        haddr_t addr;           IN: Address of object to decrement

 RETURNS
    Returns a non-negative on success, negative on failure
 DESCRIPTION
    Decrement the reference count for an object in the opened object count set.
--------------------------------------------------------------------------*/
herr_t
H5FO_top_decr(H5FO_counts_t *counts, haddr_t addr)
{
    H5FO_obj_count_t *obj_count = NULL; /* Ref. count for object */
    herr_t            ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    HDassert(counts);
    HDassert(H5F_addr_defined(addr));

    /* Get the object node from the container */
    HASH_FIND(hh, counts->hash_table, &addr, sizeof(haddr_t), obj_count);

    if (NULL != obj_count) {
        /* Decrement the reference count for the object */
        (obj_count->count)--;

        if (obj_count->count == 0) {
            /* Remove from container */
            HASH_DELETE(hh, counts->hash_table, obj_count);

            /* Release the object information */
            obj_count = H5FL_FREE(H5FO_obj_count_t, obj_count);
        }
    }
    else
        HGOTO_ERROR(H5E_FILE, H5E_NOTFOUND, FAIL, "can't decrement ref. count")

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_top_count
 PURPOSE
    Return the "top" reference count for an object in a file
 USAGE
    hsize_t H5FO_top_incr(f, addr)
        H5F_t *f;               IN/OUT: File's opened object info set
        haddr_t addr;           IN: Address of object to increment

 RETURNS
    Returns a non-negative on success, negative on failure
 DESCRIPTION
    Retrieves the reference count for an object in the opened object count set.
--------------------------------------------------------------------------*/
hsize_t
H5FO_top_count(H5FO_counts_t *counts, haddr_t addr)
{
    H5FO_obj_count_t *obj_count = NULL; /* Ref. count for object */
    hsize_t           ret_value;

    FUNC_ENTER_NOAPI_NOINIT_NOERR

    HDassert(counts);
    HDassert(H5F_addr_defined(addr));

    /* Get the object node from the container */
    HASH_FIND(hh, counts->hash_table, &addr, sizeof(haddr_t), obj_count);

    if (NULL != obj_count)
        ret_value = obj_count->count;
    else
        ret_value = 0;

    FUNC_LEAVE_NOAPI(ret_value)
}

/*--------------------------------------------------------------------------
 NAME
    H5FO_top_dest
 PURPOSE
    Destroy an open object info set
 USAGE
    herr_t H5FO_top_dest(f)
        H5F_t *f;               IN/OUT: File's opened object info set

 RETURNS
    Returns a non-negative on success, negative on failure
 DESCRIPTION
    Destroy an existing open object info set.
--------------------------------------------------------------------------*/
herr_t
H5FO_top_dest(H5FO_counts_t *counts)
{
    herr_t ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    HDassert(counts);

    /* NOTE: Use HDONE_ERROR to push errors and keep going */

    /* Check if the object count set is empty */
    if (HASH_COUNT(counts->hash_table) != 0)
        HDONE_ERROR(H5E_FILE, H5E_CANTRELEASE, FAIL, "objects still in open object counts list")

    /* Release memory */
    H5FL_FREE(H5FO_counts_t, counts);

    FUNC_LEAVE_NOAPI(ret_value)
}
