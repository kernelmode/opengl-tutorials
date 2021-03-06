/* 
 * File:   Texture.cpp
 * Author: kdubovikov
 * 
 * Created on October 30, 2014, 6:44 PM
 */

#include "Texture.h"

bool Texture::load(const std::string& imagePath) {
    
    textureId = SOIL_load_OGL_texture(imagePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_DDS_LOAD_DIRECT);
    printf("Texture %s loaded, id - %d\n", imagePath.c_str(), textureId);
    return textureId == 0;
}

bool Texture::customLoadDDS(const std::string& imagePath) {
    printf("Loading texture %s\n", imagePath.c_str());
    unsigned char header[124];

    FILE *fp;

    /* try to open the file */
    fp = fopen(imagePath.c_str(), "rb");
    if (fp == NULL) {
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagePath.c_str());
        getchar();
        return 0;
    }

    /* verify the type of file */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        printf("Texture format not supported\n");
        return 0;
    }

    /* get the surface desc */
    fread(&header, 124, 1, fp);

    unsigned int height = *(unsigned int*) &(header[8 ]);
    unsigned int width = *(unsigned int*) &(header[12]);
    unsigned int linearSize = *(unsigned int*) &(header[16]);
    unsigned int mipMapCount = *(unsigned int*) &(header[24]);
    unsigned int fourCC = *(unsigned int*) &(header[80]);


    unsigned char * buffer;
    unsigned int bufsize;
    /* how big is it going to be including all mipmaps? */
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*) malloc(bufsize * sizeof (unsigned char));
    fread(buffer, 1, bufsize, fp);
    /* close the file pointer */
    fclose(fp);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch (fourCC) {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            printf("Texture format not supported\n");
            free(buffer);
            return 0;
    }

    // Create one OpenGL texture
    glGenTextures(1, &textureId);
    printf("Tex id - %d %s\n", textureId, imagePath.c_str());
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    /* load the mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
        unsigned int size = ((width + 3) / 4)*((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
                0, size, buffer + offset);

        offset += size;
        width /= 2;
        height /= 2;

        // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
        if (width < 1) width = 1;
        if (height < 1) height = 1;

    }

    free(buffer);
    
    return textureId == 0;


}

void Texture::prepareTexture() {
    // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);
}

GLuint Texture::getTextureId() {
    return textureId;
}

Texture::~Texture() {
    glDeleteTextures(1, &textureId);
}

