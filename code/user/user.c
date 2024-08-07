
/*
 * This file is part of the micropython-ulab project,
 *
 * https://github.com/v923z/micropython-ulab
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020-2021 Zoltán Vörös
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "py/obj.h"
#include "py/runtime.h"
#include "py/misc.h"
#include "user.h"

#if ULAB_HAS_USER_MODULE

//| """This module should hold arbitrary user-defined functions."""
//|

static mp_obj_t user_square(mp_obj_t arg1, mp_obj_t arg2) {

    ndarray_obj_t *ndarray = MP_OBJ_TO_PTR(arg1);
    mp_obj_array_t *array = MP_OBJ_TO_PTR(arg2);

    //mp_int_t (*func1)(void *) = ndarray_get_float_function(ndarray->dtype);

    mp_int_t val = 13;

    for(size_t i=0; i < ndarray->len; i++) {
        mp_binary_set_val_array('I', array->items, i++, mp_obj_new_int(val));
    }
//
//
//
//    ndarray_obj_t *ndarray = MP_OBJ_TO_PTR(arg1);
//    mp_obj_array_t *array = MP_OBJ_TO_PTR(arg2);
//
//    // if the input is a dense array, create `results` with the same number of
//    // dimensions, shape, and dtype
//    ndarray_obj_t *results = ndarray_new_dense_ndarray(ndarray->ndim, ndarray->shape, ndarray->dtype);
//
//    for(size_t i=0; i < results->len; i++) {
//        uint8_t adv_item_len  = ((uint8_t * )data->items)[byte_index];
//    }

//
//
//
//     // TODO: check if self->data is set
//    mp_obj_array_t * data = MP_OBJ_TO_PTR(self->data);
//
//    uint16_t byte_index = 0;
//
//    while (byte_index < data->len) {
//        mp_obj_tuple_t *t = MP_OBJ_TO_PTR(mp_obj_new_tuple(3, NULL));
//
//        uint8_t adv_item_len  = ((uint8_t * )data->items)[byte_index];
//
//
////    // Write values into results array.
////    for(size_t i=0; i < results->len; i++) {
////        //ndarray_set_value(NDARRAY_UINT8, results->array, i, mp_binary_get_val_array(array->typecode, array->items, i));
////    }

    // at the end, return a micrppython object
    //return MP_OBJ_FROM_PTR(results);
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_2(user_square_obj, user_square);

static const mp_rom_map_elem_t ulab_user_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_user) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_square), (mp_obj_t)&user_square_obj },
};

static MP_DEFINE_CONST_DICT(mp_module_ulab_user_globals, ulab_user_globals_table);

const mp_obj_module_t ulab_user_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_ulab_user_globals,
};
#if CIRCUITPY_ULAB
MP_REGISTER_MODULE(MP_QSTR_ulab_dot_user, ulab_user_module);
#endif
#endif

