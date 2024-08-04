
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

// x range: [-PI,PI]
float fast_sine(float x) {
    const float PI = 3.14159265358f;
    const float B = 4.0f / PI;
    const float C = -4.0f / (PI * PI);
    const float P = 0.225f;

    float y = B * x + C * x * (x < 0 ? -x : x);
    return P * (y * (y < 0 ? -y : y) - y) + y;
}

// x range: [-PI, PI]
float fast_cosine(float x) {
    const float PI = 3.14159265358f;

    x = (x > 0) ? -x : x;
    x += PI/2;

    return fast_sine(x);
}

static mp_obj_t user_gradient(size_t n_args, const mp_obj_t *args) {

    // Create a 1D gradient.
    // Resulting array should be of shape (3, 256)
    // This is because we want to be able to perform np.take on the result, and
    // there needs to be 256 values to do the lookup.
    // Resulting array will be of type int.
    // Some cumulative float error happening here since we calculate step as a
    // float but cast to int every time.

    // TODO: Off by 1 error - 255 element isn't being filled.
    ndarray_obj_t *results = ndarray_new_linear_array(256, NDARRAY_UINT8);

    mp_int_t pos = 0;
    mp_int_t val = 0;

    for(size_t a=0; a < n_args; a++) {

        // Ensure the arg is a tuple.
        if(!mp_obj_is_type(args[a], &mp_type_tuple)) {
            mp_raise_TypeError(MP_ERROR_TEXT("Stop must be of type tuple"));
        }

        // Ensure the tuple is of size 2.
        mp_obj_tuple_t *stop = MP_OBJ_TO_PTR(args[a]);
        if(stop->len != 2) {
            mp_raise_TypeError(MP_ERROR_TEXT("Stop must be of length 2"));
        }

        mp_int_t next_pos = mp_obj_get_int(stop->items[0]);
        mp_int_t next_val = mp_obj_get_int(stop->items[1]);
        mp_int_t val_step = (next_val - val) / (next_pos - pos);

        // Write values into results array.
        for(size_t i=pos; i < next_pos; i++, (val) += (val_step)) {
            ndarray_set_value(NDARRAY_UINT8, results->array, i, mp_obj_new_int(val));
        }

        pos = next_pos;
        val = next_val;
    }

    return MP_OBJ_FROM_PTR(results);
}

static mp_obj_t user_fast_sin(mp_obj_t arg) {

    ndarray_obj_t *ndarray = MP_OBJ_TO_PTR(arg);
    ndarray_obj_t *results = ndarray_new_dense_ndarray(ndarray->ndim, ndarray->shape, ndarray->dtype);

    mp_float_t *array = (mp_float_t *)ndarray->array;
    mp_float_t (*func1)(void *) = ndarray_get_float_function(ndarray->dtype);
    mp_float_t *rarray = (mp_float_t *)results->array;
    for(size_t i=0; i < ndarray->len; i++) {
        *rarray++ = fast_sine(func1(array));
        array++;
    }

    return MP_OBJ_FROM_PTR(results);
}

static mp_obj_t user_fast_cos(mp_obj_t arg) {

    ndarray_obj_t *ndarray = MP_OBJ_TO_PTR(arg);
    ndarray_obj_t *results = ndarray_new_dense_ndarray(ndarray->ndim, ndarray->shape, ndarray->dtype);

    mp_float_t *array = (mp_float_t *)ndarray->array;
    mp_float_t (*func1)(void *) = ndarray_get_float_function(ndarray->dtype);
    mp_float_t *rarray = (mp_float_t *)results->array;
    for(size_t i=0; i < ndarray->len; i++) {
        *rarray++ = fast_cosine(func1(array));
        array++;
    }

    return MP_OBJ_FROM_PTR(results);
}

static mp_obj_t user_quake_uvs(mp_obj_t rhs, mp_obj_t lhs) {
    ndarray_obj_t *ndarray = ndarray_binary_op(MP_BINARY_OP_MULTIPLY, rhs, lhs);

    ndarray_obj_t *results = ndarray_new_dense_ndarray(ndarray->ndim, ndarray->shape, ndarray->dtype);

    mp_float_t *array = (mp_float_t *)ndarray->array;
    mp_float_t (*func1)(void *) = ndarray_get_float_function(ndarray->dtype);
    mp_float_t *rarray = (mp_float_t *)results->array;
    for(size_t i=0; i < ndarray->len; i++) {
        *rarray++ = func1(array);
        array++;
    }

    return MP_OBJ_FROM_PTR(results);
//    float scaledTime = iTime * speed;
//    vec2 scaledCoords = coords * frequency;
//    float x = sin(scaledTime + scaledCoords.x) * amplitude;
//    float y = sin(scaledTime + scaledCoords.y) * amplitude;
//    return coords + vec2(y, x);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(user_gradient_obj, 1, 4, user_gradient);
MP_DEFINE_CONST_FUN_OBJ_1(user_fast_sin_obj, user_fast_sin);
MP_DEFINE_CONST_FUN_OBJ_1(user_fast_cos_obj, user_fast_cos);
MP_DEFINE_CONST_FUN_OBJ_2(user_quake_uvs_obj, user_quake_uvs);

static const mp_rom_map_elem_t ulab_user_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_user) },
    { MP_ROM_QSTR(MP_QSTR_gradient), MP_ROM_PTR(&user_gradient_obj) },
    { MP_ROM_QSTR(MP_QSTR_fast_sin), MP_ROM_PTR(&user_fast_sin_obj) },
    { MP_ROM_QSTR(MP_QSTR_fast_cos), MP_ROM_PTR(&user_fast_cos_obj) },
    { MP_ROM_QSTR(MP_QSTR_quake_uvs), MP_ROM_PTR(&user_quake_uvs_obj) },
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

