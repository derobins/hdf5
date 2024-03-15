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

/** \cond INTERNAL */

/** Information about open objects in a file */
typedef struct H5FO_open_obj_t {
    haddr_t addr;        /** Address of object header for object */
    void   *obj;         /** Pointer to the object */
    bool    delete_flag; /** Flag to indicate that the object was deleted from the file */
    UT_hash_handle hh;   /** Hash table handle (must be LAST) */
} H5FO_open_obj_t;

/** Information about counted objects in a file */
typedef struct H5FO_obj_count_t {
    haddr_t addr;      /** Address of object header for object */
    hsize_t count;     /** Number of times object is opened */
    UT_hash_handle hh; /** Hash table handle (must be LAST) */
} H5FO_obj_count_t;

/** The open objects */
struct H5FO_objects {
    H5FO_open_obj_t *hash_table; /** Hash table pointer open objects */
};

/** The open object counts */
struct H5FO_counts {
    H5FO_obj_count_t *hash_table; /** Hash table pointer for open object counts */
};

H5FL_DEFINE_STATIC(H5FO_open_obj_t);
H5FL_DEFINE_STATIC(H5FO_obj_count_t);
H5FL_DEFINE_STATIC(H5FO_objects_t);
H5FL_DEFINE_STATIC(H5FO_counts_t);

/**
 * \brief Creates a new open object collection
 *
 * \return A pointer to a valid collection on success, NULL on failure
 */
H5FO_objects_t *
H5FO_create(void)
{
    H5FO_objects_t *ret_value = NULL;

    FUNC_ENTER_NOAPI(NULL)

    if ((ret_value = H5FL_CALLOC(H5FO_objects_t)) == NULL)
        HGOTO_ERROR(H5E_FILE, H5E_NOSPACE, NULL, "memory allocation failed");

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Return a previously opened object (if any) at a given address
 *
 * \param[in] objects The collection of open file objects
 * \param[in] addr Object's address in the file
 *
 * \return A pointer to a valid object on success, NULL on failure or if
 *          not found
 *
 * \note The library ignores errors from this function and treats them like
 *       the "not found" case
 */
void *
H5FO_opened(H5FO_objects_t *objects, haddr_t addr)
{
    H5FO_open_obj_t *open_obj = NULL;
    void            *ret_value;

    FUNC_ENTER_NOAPI(NULL)

    assert(objects);
    assert(H5F_addr_defined(addr));

    /* Get the object from the collection */
    HASH_FIND(hh, objects->hash_table, &addr, sizeof(haddr_t), open_obj);

    if (NULL != open_obj) {
        ret_value = open_obj->obj;
        if (NULL == ret_value)
            HGOTO_ERROR(H5E_FILE, H5E_CANTGET, NULL, "no object stored in the hash table entry");
    }
    else
        ret_value = NULL;

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Insert an open object into the collection
 *
 * \param[in] objects The collection of open file objects
 * \param[in] addr Object's address in the file
 * \param[in] obj The object to insert
 * \param[in] delete_flag Whether to immediately mark this object for deletion
 *
 * \return \herr_t
 */
herr_t
H5FO_insert(H5FO_objects_t *objects, haddr_t addr, void *obj, bool delete_flag)
{
    H5FO_open_obj_t *open_obj;
    herr_t           ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    assert(objects);
    assert(H5F_addr_defined(addr));
    assert(obj);

    /* Allocate new open object information structure */
    if ((open_obj = H5FL_MALLOC(H5FO_open_obj_t)) == NULL)
        HGOTO_ERROR(H5E_FILE, H5E_NOSPACE, FAIL, "memory allocation failed");

    /* Assign information */
    open_obj->addr        = addr;
    open_obj->obj         = obj;
    open_obj->delete_flag = delete_flag;

    /* Insert into collection */
    HASH_ADD(hh, objects->hash_table, addr, sizeof(haddr_t), open_obj);

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Remove an open object from the collection
 *
 * \param[in] objects The collection of open file objects
 * \param[in] f The file in which the object has been opened
 * \param[in] addr Object's address in the file
 *
 * \note The H5F_t parameter is only needed in order to call
 *       H5O_delete() on the object. This could be refactored out
 *       if that responsibility were transferred to the caller.
 *
 * \return \herr_t
 */
herr_t
H5FO_delete(H5FO_objects_t *objects, struct H5F_t *f, haddr_t addr)
{
    H5FO_open_obj_t *open_obj  = NULL;
    herr_t           ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    assert(objects);
    assert(H5F_addr_defined(addr));

    /* Remove from container */
    HASH_FIND(hh, objects->hash_table, &addr, sizeof(haddr_t), open_obj);
    HASH_DELETE(hh, objects->hash_table, open_obj);

    /* Delete the object if it was marked for deletion */
    if (open_obj->delete_flag) {
        if (H5O_delete(f, addr) < 0)
            HGOTO_ERROR(H5E_FILE, H5E_CANTDELETE, FAIL, "can't delete object from file");
    }

    /* Release the object information */
    open_obj = H5FL_FREE(H5FO_open_obj_t, open_obj);

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Mark an object for deletion when it is closed
 *
 * \param[in] objects The collection of open file objects
 * \param[in] addr Object's address in the file
 * \param[in] delete_flag Whether to mark (or unmark) the object for deletion
 *
 * \return \herr_t
 */
herr_t
H5FO_mark(H5FO_objects_t *objects, haddr_t addr, bool delete_flag)
{
    H5FO_open_obj_t *open_obj  = NULL; /* Information about open object */
    herr_t           ret_value = SUCCEED;

    FUNC_ENTER_NOAPI_NOERR

    assert(objects);
    assert(H5F_addr_defined(addr));

    /* Get the object node from the container */
    HASH_FIND(hh, objects->hash_table, &addr, sizeof(haddr_t), open_obj);

    if (NULL != open_obj)
        open_obj->delete_flag = delete_flag;
    else
        ret_value = FAIL;

    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Determine if an object has been marked 'delete on close'
 *
 * \param[in] objects The collection of open file objects
 * \param[in] addr Object's address in the file
 *
 * \return true/false
 *
 * \details Also implicitly checks that the object is in the list of
 *          objects. Not being in the list is not considered an error.
 */
bool
H5FO_marked(H5FO_objects_t *objects, haddr_t addr)
{
    H5FO_open_obj_t *open_obj  = NULL; /* Information about open object */
    bool             ret_value = FALSE;

    FUNC_ENTER_NOAPI_NOERR

    assert(objects);
    assert(H5F_addr_defined(addr));

    /* Get the object node from the container */
    HASH_FIND(hh, objects->hash_table, &addr, sizeof(haddr_t), open_obj);

    if (NULL != open_obj)
        ret_value = open_obj->delete_flag;

    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Destroy an open object info collection
 *
 * \param[in] objects The collection of open file objects (CANNOT BE NULL)
 *
 * \return \herr_t
 */
herr_t
H5FO_dest(H5FO_objects_t *objects)
{
    herr_t ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    assert(objects);

    /* NOTE: Use HDONE_ERROR to push errors and keep going */

    /* Check if the object info set is empty */
    if (HASH_COUNT(objects->hash_table) != 0)
        HDONE_ERROR(H5E_FILE, H5E_CANTRELEASE, FAIL, "objects still in open object info set");

    /* Release memory */
    H5FL_FREE(H5FO_objects_t, objects);

    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Creates a new open object counts collection
 *
 * \return A pointer to a valid collection on success, NULL on failure
 */
H5FO_counts_t *
H5FO_top_create(void)
{
    H5FO_counts_t *ret_value = NULL;

    FUNC_ENTER_NOAPI(NULL)

    if ((ret_value = H5FL_CALLOC(H5FO_counts_t)) == NULL)
        HGOTO_ERROR(H5E_FILE, H5E_NOSPACE, NULL, "memory allocation failed");

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Increment the reference count on the top-level file pointer for
 *        an object in a file
 *
 * \param[in] counts The collection of open file object counts
 * \param[in] addr Object's address in the file
 *
 * \note There is no insert API call. If the object at addr has not yet been
 *       added to the collection, this call will add it.
 *
 * \return \herr_t
 */
herr_t
H5FO_top_incr(H5FO_counts_t *counts, haddr_t addr)
{
    H5FO_obj_count_t *obj_count = NULL;
    herr_t            ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    assert(counts);
    assert(H5F_addr_defined(addr));

    /* Get the object node from the container */
    HASH_FIND(hh, counts->hash_table, &addr, sizeof(haddr_t), obj_count);

    if (NULL != obj_count) {
        (obj_count->count)++;
    }
    else {
        /* Allocate new opened object information structure */
        if (NULL == (obj_count = H5FL_MALLOC(H5FO_obj_count_t)))
            HGOTO_ERROR(H5E_FILE, H5E_NOSPACE, FAIL, "memory allocation failed");

        /* Assign information */
        obj_count->addr  = addr;
        obj_count->count = 1;

        /* Insert into container */
        HASH_ADD(hh, counts->hash_table, addr, sizeof(haddr_t), obj_count);
    }

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Decrement the reference count on the top-level file pointer for
 *        an object in a file
 *
 * \param[in] counts The collection of open file object counts
 * \param[in] addr Object's address in the file
 *
 * \note When the reference count drops to zero, the object's counts will
 *       be removed from the collection.
 *
 * \return \herr_t
 */
herr_t
H5FO_top_decr(H5FO_counts_t *counts, haddr_t addr)
{
    H5FO_obj_count_t *obj_count = NULL; /* Ref. count for object */
    herr_t            ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    assert(counts);
    assert(H5F_addr_defined(addr));

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
        HGOTO_ERROR(H5E_FILE, H5E_NOTFOUND, FAIL, "can't decrement ref. count");

done:
    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Get the reference count on the top-level file pointer
 *
 * \param[in] counts The collection of open file object counts
 * \param[in] addr Object's address in the file
 *
 * \return The refernce count for the object at address addr
 */
hsize_t
H5FO_top_count(H5FO_counts_t *counts, haddr_t addr)
{
    H5FO_obj_count_t *obj_count = NULL; /* Ref. count for object */
    hsize_t           ret_value;

    FUNC_ENTER_NOAPI_NOINIT_NOERR

    assert(counts);
    assert(H5F_addr_defined(addr));

    /* Get the object node from the container */
    HASH_FIND(hh, counts->hash_table, &addr, sizeof(haddr_t), obj_count);

    if (NULL != obj_count)
        ret_value = obj_count->count;
    else
        ret_value = 0;

    FUNC_LEAVE_NOAPI(ret_value)
}

/**
 * \brief Destroy a collection of open file object counts
 *
 * \param[in] counts The collection of open file object counts
 *
 * \return \herr_t
 */
herr_t
H5FO_top_dest(H5FO_counts_t *counts)
{
    herr_t ret_value = SUCCEED;

    FUNC_ENTER_NOAPI(FAIL)

    assert(counts);

    /* NOTE: Use HDONE_ERROR to push errors and keep going */

    /* Check if the object count set is empty */
    if (HASH_COUNT(counts->hash_table) != 0)
        HDONE_ERROR(H5E_FILE, H5E_CANTRELEASE, FAIL, "objects still in open object counts list");

    /* Release memory */
    H5FL_FREE(H5FO_counts_t, counts);

    FUNC_LEAVE_NOAPI(ret_value)
}

/** \endcond */
