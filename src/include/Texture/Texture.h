#ifndef _TEXTURE_LOADER_H_
#define _TEXTURE_LOADER_H_

#include <stb/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>
#include <string>

using std::cout;
using std::endl;
using std::exception;
using std::string;

class TextureFailedToLoadException : public exception {
private:
    string path;

public:
    TextureFailedToLoadException(const char* path): path { path } {}

    const char* what() const noexcept override {
        string msg { "ERROR::TEXTURE::FAILED_TO_LOAD: Unable to load texture from " };
        msg += this->path;
        const char* msg_str { msg.c_str() };
        return msg_str;
    }
};

class Texture {
private:
    unsigned int id;
    const char* path;
    GLenum unit;
    GLenum format;

public:
    Texture(
        const char* path,
        GLenum format,
        GLenum unit
    ): path { path }, unit { unit }, format { format } {
        glGenTextures(1, &(this->id));
        this->activate();

        // Set texture wrapping/filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void load() {
        // Loading texture
        int width, height, nr_channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(
            this->path,
            &width,
            &height,
            &nr_channels,
            0
        );

        if (!data) throw TextureFailedToLoadException(this->path); 

        glTexImage2D(
            /* texture target */                  GL_TEXTURE_2D,
            /* mipmapping level */                0,
            /* texture format */                  GL_RGB,
            /* texture dimension */               width, height,
            /* legacy stuff... */                 0,
            /* format, data type of source img */ this->format, GL_UNSIGNED_BYTE,
            /* actual image */                    data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        // clear the image data after texture being generated
        stbi_image_free(data);
    }

    void activate() const {
        glActiveTexture(this->unit);
        glBindTexture(GL_TEXTURE_2D, this->id);
    }
};

#endif
