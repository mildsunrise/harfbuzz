/*
 * Copyright (C) 2007,2008,2009  Red Hat, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
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

#ifndef HB_OT_LAYOUT_PRIVATE_HH
#define HB_OT_LAYOUT_PRIVATE_HH

#include "hb-private.h"

#include "hb-ot-layout.h"
#include "hb-ot-head-private.hh"

#include "hb-font-private.h"
#include "hb-buffer-private.hh"

HB_BEGIN_DECLS


/* XXX */
#define component() var1.u16[0]
#define lig_id() var1.u16[1]
#define back() var.u16[0] /* number of glyphs to go back for drawing current glyph */
#define cursive_chain() var.i16[1] /* character to which this connects, may be positive or negative */

typedef unsigned int hb_ot_layout_class_t;

/*
 * hb_ot_layout_t
 */

struct hb_ot_layout_t
{
  hb_blob_t *gdef_blob;
  hb_blob_t *gsub_blob;
  hb_blob_t *gpos_blob;

  const struct GDEF *gdef;
  const struct GSUB *gsub;
  const struct GPOS *gpos;

  struct
  {
    unsigned char *klasses;
    unsigned int len;
  } new_gdef;
};

struct hb_ot_layout_context_t
{
  hb_face_t *face;
  hb_font_t *font;

  /* Convert from font-space to user-space */
  inline hb_position_t scale_x (int16_t v) { return scale (v, this->font->x_scale); }
  inline hb_position_t scale_y (int16_t v) { return scale (v, this->font->y_scale); }

  private:
  inline hb_position_t scale (int16_t v, unsigned int scale) { return v * (int64_t) scale / this->face->head_table->get_upem (); }
};


HB_INTERNAL hb_ot_layout_t *
_hb_ot_layout_new (hb_face_t *face);

HB_INTERNAL void
_hb_ot_layout_free (hb_ot_layout_t *layout);


/*
 * GDEF
 */

HB_INTERNAL hb_bool_t
_hb_ot_layout_has_new_glyph_classes (hb_face_t *face);

HB_INTERNAL void
_hb_ot_layout_set_glyph_property (hb_face_t      *face,
				  hb_codepoint_t  glyph,
				  unsigned int    property);

HB_INTERNAL void
_hb_ot_layout_set_glyph_class (hb_face_t                  *face,
			       hb_codepoint_t              glyph,
			       hb_ot_layout_glyph_class_t  klass);

HB_INTERNAL hb_bool_t
_hb_ot_layout_check_glyph_property (hb_face_t    *face,
				    hb_glyph_info_t *ginfo,
				    unsigned int  lookup_flags,
				    unsigned int *property);

HB_INTERNAL hb_bool_t
_hb_ot_layout_skip_mark (hb_face_t    *face,
			 hb_glyph_info_t *ginfo,
			 unsigned int  lookup_flags,
			 unsigned int *property);


HB_END_DECLS

#endif /* HB_OT_LAYOUT_PRIVATE_HH */
