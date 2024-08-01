
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

static mp_obj_t user_square(size_t n_args, const mp_obj_t *args) {

    // Create a 1D gradient.
    // Resulting array should be of shape (3, 256)
    // This is because we want to be able to perform np.take on the result, and
    // there needs to be 256 values to do the lookup.
    // Resulting array will be of type int.
    // Some cumulative float error happening here since we calculate step as a
    // float but cast to int every time.

    // TODO: Off by 1 error - 255 element isn't being filled.
    ndarray_obj_t *results = ndarray_new_linear_array(1, NDARRAY_UINT8);

    mp_int_t pos = 0;
    mp_int_t val = 0;

    for(size_t a=0; a < n_args; a++) {

        // Ensure the arg is a tuple.
        if(!mp_obj_is_type(args[a], &mp_type_tuple)) {
            mp_raise_TypeError(MP_ERROR_TEXT("must be a tuple EEEDIOT"));
        }

        // Ensure the tuple is of size 2.
        mp_obj_tuple_t *handle = MP_OBJ_TO_PTR(args[a]);
        if(handle->len != 2) {
            mp_raise_TypeError(MP_ERROR_TEXT("Must be of length 4"));
        }

        mp_int_t next_pos = mp_obj_get_int(handle->items[0]);
        mp_int_t next_val = mp_obj_get_int(handle->items[1]);
        mp_int_t val_step = (next_val - val) / (next_pos - pos);

        // WORKS. How to set array value by index.
        // First handle.
        for(size_t i=pos; i < next_pos; i++, (val) += (val_step)) {
            ndarray_set_value(NDARRAY_UINT8, results->array, i, mp_obj_new_int(val));
        }

        pos = next_pos;
        val = next_val;
    }

    return MP_OBJ_FROM_PTR(results);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(user_square_obj, 1, 4, user_square);

static const mp_rom_map_elem_t ulab_user_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_user) },
    { MP_ROM_QSTR(MP_QSTR_square), MP_ROM_PTR(&user_square_obj) },
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

