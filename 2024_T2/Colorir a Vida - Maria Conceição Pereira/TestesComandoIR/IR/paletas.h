#define NUM_PALETTES 11

DEFINE_GRADIENT_PALETTE( browngreen_gp ) {
  0,    6,  255,    0,     //green
  71,    0,  255,  153,     //bluegreen
  122,  200,  200,  200,     //gray
  181,  110,   61,    6,     //brown
  255,    6,  255,    0      //green
};
CRGBPalette16 brownGreenPalette = browngreen_gp;


DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,      0,    0,    0,  // black
  128,  255,    0,    0,  // red
  200,  255,  255,    0,  // bright yellow
  255,  255,  255,  255,  // full white
};
CRGBPalette16 heatPalette = heatmap_gp;

DEFINE_GRADIENT_PALETTE( greenblue_gp ) {
  0,   0,  255, 245,
  46,  0,  21,  255,
  179, 12, 250, 0,
  255, 0,  255, 245
};
CRGBPalette16 greenbluePalette = greenblue_gp;

DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
  0, 120,  0,  0,
  22, 179, 22,  0,
  51, 255, 104,  0,
  85, 167, 22, 18,
  135, 100,  0, 103,
  198,  16,  0, 130,
  255,   0,  0, 160
};
CRGBPalette16 sunsetPalette = Sunset_Real_gp;

CRGBPalette16 purplePalette = CRGBPalette16 (
                                CRGB::DarkViolet,
                                CRGB::DarkViolet,
                                CRGB::DarkViolet,
                                CRGB::DarkViolet,

                                CRGB::Magenta,
                                CRGB::Magenta,
                                CRGB::Linen,
                                CRGB::Linen,

                                CRGB::Magenta,
                                CRGB::Magenta,
                                CRGB::DarkViolet,
                                CRGB::DarkViolet,

                                CRGB::DarkViolet,
                                CRGB::DarkViolet,
                                CRGB::Linen,
                                CRGB::Linen
                              );

DEFINE_GRADIENT_PALETTE( fireandice_gp ) {
  0,  80,  2,  1,
  51, 206, 15,  1,
  101, 242, 34,  1,
  153,  16, 67, 128,
  204,   2, 21, 69,
  255,   1,  2,  4
};
CRGBPalette16 fireandicePalette = fireandice_gp;

DEFINE_GRADIENT_PALETTE( bhw2_turq_gp ) {
  0,   1, 33, 95,
  38,   1, 107, 37,
  76,  42, 255, 45,
  127, 255, 255, 45,
  178,  42, 255, 45,
  216,   1, 107, 37,
  255,   1, 33, 95
};
CRGBPalette16 turqPalette = bhw2_turq_gp;

DEFINE_GRADIENT_PALETTE( autumnrose_gp ) {
  0,  71,  3,  1,
  45, 128,  5,  2,
  84, 186, 11,  3,
  127, 215, 27,  8,
  153, 224, 69, 13,
  188, 229, 84,  6,
  226, 242, 135, 17,
  255, 247, 161, 79
};
CRGBPalette16 autumnrosePalette = autumnrose_gp;

DEFINE_GRADIENT_PALETTE( bhw1_06_gp ) {
  0, 184,  1, 128,
  160,   1, 193, 182,
  219, 153, 227, 190,
  255, 255, 255, 255
};
CRGBPalette16 bhw1_06Palette = bhw1_06_gp;

DEFINE_GRADIENT_PALETTE( bhw2_xmas_gp ) {
  0,   0, 12,  0,
  40,   0, 55,  0,
  66,   1, 117,  2,
  77,   1, 84,  1,
  81,   0, 55,  0,
  119,   0, 12,  0,
  153,  42,  0,  0,
  181, 121,  0,  0,
  204, 255, 12,  8,
  224, 121,  0,  0,
  244,  42,  0,  0,
  255,  42,  0,  0
};
CRGBPalette16 xmasPalette = bhw2_xmas_gp;

DEFINE_GRADIENT_PALETTE( bhw1_justducky_gp ) {
  0,  47, 28,  2,
  76, 229, 73,  1,
  163, 255, 255,  0,
  255, 229, 73,  1
};
CRGBPalette16 justduckyPalette = bhw1_justducky_gp;
