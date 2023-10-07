#include "be_constobj.h"

static be_define_const_map_slots(be_class_bytes_map) {
    { be_const_key(resize, 27), be_const_func(m_resize) },
    { be_const_key(_X3D_X3D, -1), be_const_func(m_equal) },
    { be_const_key(setfloat, -1), be_const_func(m_setfloat) },
    { be_const_key(clear, -1), be_const_func(m_clear) },
    { be_const_key(fromb64, -1), be_const_func(m_fromb64) },
    { be_const_key(_change_buffer, 32), be_const_func(m_change_buffer) },
    { be_const_key(_X21_X3D, -1), be_const_func(m_nequal) },
    { be_const_key(_X2Ep, 31), be_const_var(0) },
    { be_const_key(_X2B, 4), be_const_func(m_merge) },
    { be_const_key(_X2Esize, 6), be_const_var(1) },
    { be_const_key(size, -1), be_const_func(m_size) },
    { be_const_key(getbits, -1), be_const_closure(getbits_closure) },
    { be_const_key(add, 29), be_const_func(m_add) },
    { be_const_key(geti, -1), be_const_func(m_geti) },
    { be_const_key(tob64, -1), be_const_func(m_tob64) },
    { be_const_key(_X2Elen, -1), be_const_var(2) },
    { be_const_key(_X2E_X2E, 5), be_const_func(m_connect) },
    { be_const_key(_buffer, -1), be_const_func(m_buffer) },
    { be_const_key(deinit, 12), be_const_func(m_deinit) },
    { be_const_key(set, -1), be_const_func(m_set) },
    { be_const_key(asstring, -1), be_const_func(m_asstring) },
    { be_const_key(ismapped, -1), be_const_func(m_is_mapped) },
    { be_const_key(copy, 17), be_const_func(m_copy) },
    { be_const_key(fromhex, 11), be_const_func(m_fromhex) },
    { be_const_key(setitem, 2), be_const_func(m_setitem) },
    { be_const_key(item, 30), be_const_func(m_item) },
    { be_const_key(tostring, -1), be_const_func(m_tostring) },
    { be_const_key(init, -1), be_const_func(m_init) },
    { be_const_key(reverse, 13), be_const_func(m_reverse) },
    { be_const_key(tohex, -1), be_const_func(m_tohex) },
    { be_const_key(tobool, -1), be_const_func(m_tobool) },
    { be_const_key(setbits, -1), be_const_closure(setbits_closure) },
    { be_const_key(seti, 34), be_const_func(m_set) },
    { be_const_key(setbytes, -1), be_const_func(m_setbytes) },
    { be_const_key(get, -1), be_const_func(m_getu) },
    { be_const_key(fromstring, -1), be_const_func(m_fromstring) },
    { be_const_key(getfloat, -1), be_const_func(m_getfloat) },
};

static be_define_const_map(
    be_class_bytes_map,
    37
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_bytes,
    3,
    NULL,
    bytes
);
