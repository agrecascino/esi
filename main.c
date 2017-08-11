#include "esi.h"

int save_esi_image(FILE *file, struct esi_image *image) {
    if((image->bdepth % 8) != 0){
       return false;
    }
    uint64_t bytes_to_write = (image->width * image->height) * ((image->bdepth / 8) * (image->grayscale ? 1 : 3));
    uint16_t width = __bswap_16(image->width);
    uint16_t bdepth = image->bdepth;
    uint32_t zero = 0;
    uint64_t magic = 0x31697365;
    bdepth = bdepth << 2;
    bdepth |= image->grayscale ? 0b0000000000000000 : 0b000000010000000;
    fwrite(&magic, 4, 1, file);
    fwrite(&width, 2, 1, file);
    fwrite(&bdepth, 2, 1, file);
    fwrite(&zero, 4, 1, file);
    fwrite(image->data, bytes_to_write, 1, file);
    return true;
}

struct esi_image* load_esi_image(FILE *file) {
    struct esi_image *esi;
    uint64_t file_size;
    uint32_t magic;
    uint64_t bytes_left;
    if(!file)
        return NULL;
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    if(file_size < 12)
        return NULL;
    fread(&magic, 4, 1, file);
    if(magic != 0x31697365) {
        return NULL;
    }
    esi = malloc(sizeof(struct esi_image));
    fread(&esi->width, 2, 1, file);
    fread(&esi->bdepth, 2, 1, file);
    esi->width = __bswap_16(esi->width);
    if(!(esi->bdepth & 0b0000000010000000))
        esi->grayscale = true;
    esi->bdepth = (esi->bdepth & ~0b0000000010000000) >> 2;
    if((esi->bdepth % 8) != 0){
       free(esi);
       return NULL;
    }
    for(int i = 0; i < 4; i++)
        fgetc(file);
    esi->height = ((((file_size - 12) / (esi->grayscale ? 1 : 3)) / (esi->bdepth / 8)) / esi->width);
    bytes_left = esi->width * esi->height * (esi->bdepth / 8) * (esi->grayscale ? 1 : 3);
    if(bytes_left > (file_size - 12)) {
        bytes_left = sqrt(file_size - 12);
    }
    esi->data = malloc(bytes_left);
    for(uint64_t i = 0; i < bytes_left; i++){
        esi->data[i] = fgetc(file);
    }
    return esi;
 }
