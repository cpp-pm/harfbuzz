/*
 * Copyright (C) 2009  Red Hat, Inc.
 *
 *  This is part of HarfBuzz, an OpenType Layout engine library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Red Hat Author(s): Behdad Esfahbod
 */

#include "hb-private.h"

#include "hb-unicode-private.h"

/*
 * hb_unicode_funcs_t
 */

static hb_codepoint_t hb_unicode_get_mirroring_nil (hb_codepoint_t unicode) { return unicode; }
static hb_category_t hb_unicode_get_general_category_nil (hb_codepoint_t unicode) { return HB_CATEGORY_OTHER_LETTER; }
static hb_script_t hb_unicode_get_script_nil (hb_codepoint_t unicode) { return HB_SCRIPT_UNKNOWN; }
static unsigned int hb_unicode_get_combining_class_nil (hb_codepoint_t unicode) { return 0; }
static unsigned int hb_unicode_get_eastasian_width_nil (hb_codepoint_t unicode) { return 1; }

hb_unicode_funcs_t _hb_unicode_funcs_nil = {
  HB_REFERENCE_COUNT_INVALID, /* ref_count */

  TRUE, /* immutable */

  hb_unicode_get_general_category_nil,
  hb_unicode_get_combining_class_nil,
  hb_unicode_get_mirroring_nil,
  hb_unicode_get_script_nil,
  hb_unicode_get_eastasian_width_nil
};

hb_unicode_funcs_t *
hb_unicode_funcs_create (void)
{
  hb_unicode_funcs_t *ufuncs;

  if (!HB_OBJECT_DO_CREATE (hb_unicode_funcs_t, ufuncs))
    return &_hb_unicode_funcs_nil;

  *ufuncs = _hb_unicode_funcs_nil;
  HB_OBJECT_DO_INIT (ufuncs);

  return ufuncs;
}

hb_unicode_funcs_t *
hb_unicode_funcs_reference (hb_unicode_funcs_t *ufuncs)
{
  HB_OBJECT_DO_REFERENCE (ufuncs);
}

unsigned int
hb_unicode_funcs_get_reference_count (hb_unicode_funcs_t *ufuncs)
{
  HB_OBJECT_DO_GET_REFERENCE_COUNT (ufuncs);
}

void
hb_unicode_funcs_destroy (hb_unicode_funcs_t *ufuncs)
{
  HB_OBJECT_DO_DESTROY (ufuncs);

  free (ufuncs);
}

hb_unicode_funcs_t *
hb_unicode_funcs_copy (hb_unicode_funcs_t *other_ufuncs)
{
  hb_unicode_funcs_t *ufuncs;

  if (!HB_OBJECT_DO_CREATE (hb_unicode_funcs_t, ufuncs))
    return &_hb_unicode_funcs_nil;

  *ufuncs = *other_ufuncs;
  HB_OBJECT_DO_INIT (ufuncs);
  ufuncs->immutable = FALSE;

  return ufuncs;
}

void
hb_unicode_funcs_make_immutable (hb_unicode_funcs_t *ufuncs)
{
  if (HB_OBJECT_IS_INERT (ufuncs))
    return;

  ufuncs->immutable = TRUE;
}


void
hb_unicode_funcs_set_mirroring_func (hb_unicode_funcs_t *ufuncs,
				     hb_unicode_get_mirroring_func_t mirroring_func)
{
  if (ufuncs->immutable)
    return;

  ufuncs->get_mirroring = mirroring_func ? mirroring_func : hb_unicode_get_mirroring_nil;
}

void
hb_unicode_funcs_set_general_category_func (hb_unicode_funcs_t *ufuncs,
					    hb_unicode_get_general_category_func_t general_category_func)
{
  if (ufuncs->immutable)
    return;

  ufuncs->get_general_category = general_category_func ? general_category_func : hb_unicode_get_general_category_nil;
}

void
hb_unicode_funcs_set_script_func (hb_unicode_funcs_t *ufuncs,
				  hb_unicode_get_script_func_t script_func)
{
  if (ufuncs->immutable)
    return;

  ufuncs->get_script = script_func ? script_func : hb_unicode_get_script_nil;
}

void
hb_unicode_funcs_set_combining_class_func (hb_unicode_funcs_t *ufuncs,
					   hb_unicode_get_combining_class_func_t combining_class_func)
{
  if (ufuncs->immutable)
    return;

  ufuncs->get_combining_class = combining_class_func ? combining_class_func : hb_unicode_get_combining_class_nil;
}

void
hb_unicode_funcs_set_eastasian_width_func (hb_unicode_funcs_t *ufuncs,
					   hb_unicode_get_eastasian_width_func_t eastasian_width_func)
{
  if (ufuncs->immutable)
    return;

  ufuncs->get_eastasian_width = eastasian_width_func ? eastasian_width_func : hb_unicode_get_eastasian_width_nil;
}


#define LTR HB_DIRECTION_LTR
#define RTL HB_DIRECTION_RTL
const hb_direction_t horiz_dir[] =
{
  LTR,	/* Zyyy */
  LTR,	/* Qaai */
  RTL,	/* Arab */
  LTR,	/* Armn */
  LTR,	/* Beng */
  LTR,	/* Bopo */
  LTR,	/* Cher */
  LTR,	/* Qaac */
  LTR,	/* Cyrl (Cyrs) */
  LTR,	/* Dsrt */
  LTR,	/* Deva */
  LTR,	/* Ethi */
  LTR,	/* Geor (Geon, Geoa) */
  LTR,	/* Goth */
  LTR,	/* Grek */
  LTR,	/* Gujr */
  LTR,	/* Guru */
  LTR,	/* Hani */
  LTR,	/* Hang */
  RTL,	/* Hebr */
  LTR,	/* Hira */
  LTR,	/* Knda */
  LTR,	/* Kana */
  LTR,	/* Khmr */
  LTR,	/* Laoo */
  LTR,	/* Latn (Latf, Latg) */
  LTR,	/* Mlym */
  LTR,	/* Mong */
  LTR,	/* Mymr */
  LTR,	/* Ogam */
  LTR,	/* Ital */
  LTR,	/* Orya */
  LTR,	/* Runr */
  LTR,	/* Sinh */
  RTL,	/* Syrc (Syrj, Syrn, Syre) */
  LTR,	/* Taml */
  LTR,	/* Telu */
  RTL,	/* Thaa */
  LTR,	/* Thai */
  LTR,	/* Tibt */
  LTR,	/* Cans */
  LTR,	/* Yiii */
  LTR,	/* Tglg */
  LTR,	/* Hano */
  LTR,	/* Buhd */
  LTR,	/* Tagb */

  /* Unicode-4.0 additions */
  LTR,	/* Brai */
  LTR,	/* Cprt */
  LTR,	/* Limb */
  LTR,	/* Osma */
  LTR,	/* Shaw */
  LTR,	/* Linb */
  LTR,	/* Tale */
  LTR,	/* Ugar */

  /* Unicode-4.1 additions */
  LTR,	/* Talu */
  LTR,	/* Bugi */
  LTR,	/* Glag */
  LTR,	/* Tfng */
  LTR,	/* Sylo */
  LTR,	/* Xpeo */
  LTR,	/* Khar */

  /* Unicode-5.0 additions */
  LTR,	/* Zzzz */
  LTR,	/* Bali */
  LTR,	/* Xsux */
  RTL,	/* Phnx */
  LTR,	/* Phag */
  RTL,	/* Nkoo */

  /* Unicode-5.1 additions */
  LTR,	/* Kali */
  LTR,	/* Lepc */
  LTR,	/* Rjng */
  LTR,	/* Sund */
  LTR,	/* Saur */
  LTR,	/* Cham */
  LTR,	/* Olck */
  LTR,	/* Vaii */
  LTR,	/* Cari */
  LTR,	/* Lyci */
  LTR	/* Lydi */
};
#undef LTR
#undef RTL

HB_INTERNAL hb_direction_t
_hb_script_get_horizontal_direction (hb_script_t script)
{
  if (HB_UNLIKELY ((unsigned int) script >= ARRAY_LENGTH (horiz_dir)))
    return HB_DIRECTION_LTR;

  return horiz_dir[script];
}
