
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

    //mp_int_t val = 13;
    uint8_t *rarray = (uint8_t *)ndarray->array;
    for(size_t i=0; i < ndarray->len; i++, rarray++) {
        mp_binary_set_val_array('I', array->items, i, mp_obj_new_int(*rarray));
    }
    //mp_obj_new_int(*rarray++)

//
//     "        uint8_t *array = (uint8_t *)ndarray->array;\n",
//        "        uint8_t *rarray = (uint8_t *)results->array;\n",
//        "        for(size_t i=0; i < ndarray->len; i++, array++) {\n",
//        "            *rarray++ = (*array) * (*array);\n",
//        "        }\n",
//




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

