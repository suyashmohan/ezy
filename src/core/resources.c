#include "resources.h"
#include "core.h"

#include <string.h>

texture load_image(const char file[]) {
  texture td = (texture){
      .bitmap = NULL,
  };

  int imgW, imgH, nrChannels;
  int desired_channels = 4;
  // stbi_set_flip_vertically_on_load(true);
  unsigned char *bitmap =
      stbi_load(file, &imgW, &imgH, &nrChannels, desired_channels);

  if (!bitmap) {
    return td;
  }

  td.width = imgW;
  td.height = imgH;
  td.channels = nrChannels;
  td.bitmap = bitmap;

  return td;
}

font_texture load_font(const char file[], float line_height) {
  font_texture font = (font_texture){.line_height = line_height,
                                     .tex = {
                                         .bitmap = NULL,
                                         .channels = 4,
                                     }};

  /* load font file */
  long size;
  unsigned char *fontBuffer;

  FILE *fontFile = fopen(file, "rb");
  if (!fontFile) {
    printf("Unable to load font file: %s\n", file);
    exit(EXIT_FAILURE);
    return font;
  }
  fseek(fontFile, 0, SEEK_END);
  size = ftell(fontFile);       /* how long is the file ? */
  fseek(fontFile, 0, SEEK_SET); /* reset */

  fontBuffer = malloc(size);

  fread(fontBuffer, size, 1, fontFile);
  fclose(fontFile);

  /* prepare font */
  stbtt_fontinfo info;
  if (!stbtt_InitFont(&info, fontBuffer, 0)) {
    printf("Unable to init font: %s\n", file);
    exit(EXIT_FAILURE);
    return font;
  }

  char a;
  for (a = 32; a < 127; ++a) {
    font.word[a - 32] = a;
  }
  font.word[a - 32] = '\0';

  float l_h = line_height; /* line height */
  /* calculate font scaling */
  float scale = stbtt_ScaleForPixelHeight(&info, l_h);
  int ascent, descent, lineGap;
  stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
  ascent = (int)roundf(ascent * scale);
  descent = (int)roundf(descent * scale);

  int x = 0;
  int i;

  // Calculate texture width
  for (i = 0; i < strlen(font.word); ++i) {
    /* how wide is this character */
    int ax;
    int lsb;
    stbtt_GetCodepointHMetrics(&info, font.word[i], &ax, &lsb);

    /* advance x */
    x += (int)roundf(ax * scale);

    /* add kerning */
    int kern;
    kern = stbtt_GetCodepointKernAdvance(&info, font.word[i], font.word[i + 1]);
    x += (int)roundf(kern * scale);
  }

  int b_w = x;               /* bitmap width */
  int b_h = (int)ceilf(l_h); /* bitmap height */
  font.tex.width = b_w;
  font.tex.height = b_h;

  /* create a bitmap for the phrase */
  unsigned char *bitmap1 = calloc(b_w * b_h, sizeof(unsigned char));

  x = 0;
  font.word_x[0] = x;
  for (i = 0; i < strlen(font.word); ++i) {
    /* how wide is this character */
    int ax;
    int lsb;
    stbtt_GetCodepointHMetrics(&info, font.word[i], &ax, &lsb);

    /* get bounding box for character (may be offset to account for chars that
     * dip above or below the line */
    int c_x1, c_y1, c_x2, c_y2;
    stbtt_GetCodepointBitmapBox(&info, font.word[i], scale, scale, &c_x1, &c_y1,
                                &c_x2, &c_y2);

    /* compute y (different characters have different heights */
    int y = ascent + c_y1;

    /* render character (stride and offset is important here) */
    int byteOffset = x + (int)roundf(lsb * scale) + (y * b_w);
    stbtt_MakeCodepointBitmap(&info, bitmap1 + byteOffset, c_x2 - c_x1,
                              c_y2 - c_y1, b_w, scale, scale, font.word[i]);

    /* advance x */
    x += (int)roundf(ax * scale);

    /* add kerning */
    int kern;
    kern = stbtt_GetCodepointKernAdvance(&info, font.word[i], font.word[i + 1]);
    x += (int)roundf(kern * scale);
    font.word_x[i + 1] = x;
  }

  free(fontBuffer);

  uint32_t sz = b_w * b_h;
  font.tex.bitmap = (unsigned char *)malloc(sz * 4);
  for (unsigned int i = 0; i < sz; ++i) {
    font.tex.bitmap[i * 4 + 0] = 255;
    font.tex.bitmap[i * 4 + 1] = 255;
    font.tex.bitmap[i * 4 + 2] = 255;
    font.tex.bitmap[i * 4 + 3] = bitmap1[i];
  }
  free(bitmap1);

  return font;
}
