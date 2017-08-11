#ifndef ESI_H
#define ESI_H
#include <stdio.h>
#include <stdint.h>
#include <byteswap.h>
#include <stdbool.h>
#include <strings.h>
#include <math.h>
#include <stdlib.h>
#include <printf.h>
#include <string.h>

struct esi_image {
    bool grayscale;
    uint32_t width;
    uint32_t height;
    uint16_t bdepth;
    uint8_t *data;
};

extern int save_esi_image(FILE *file, struct esi_image *image);

extern struct esi_image* load_esi_image(FILE *file);


#endif // ESI_H
