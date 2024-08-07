
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

static mp_obj_t user_square(mp_obj_t arg1, mp_obj_t arg2, mp_obj_t arg3, mp_obj_t arg4) {

    ndarray_obj_t *r = MP_OBJ_TO_PTR(arg1);
    ndarray_obj_t *g = MP_OBJ_TO_PTR(arg2);
    ndarray_obj_t *b = MP_OBJ_TO_PTR(arg3);
    mp_obj_array_t *array = MP_OBJ_TO_PTR(arg4);

    uint8_t *rarray = (uint8_t *)r->array;
    uint8_t *garray = (uint8_t *)g->array;
    uint8_t *barray = (uint8_t *)b->array;
    for(size_t i=0; i < ndarray->len; i++, rarray++, garray++, barray++) {
        mp_binary_set_val_array('I', array->items, i, mp_obj_new_int((*garray << 16) + (*rarray << 8) + *barray));
    }

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(user_square_obj, 4, 4, user_square);

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

