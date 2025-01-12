#ifndef GLIMAGEBUFFEROBJECT_H
#define GLIMAGEBUFFEROBJECT_H

namespace SwwOpenGL {

class GLImageBufferObject
{
public:
    GLImageBufferObject();
public:
    bool ready; //set if the buffer has been allocated and no errors occured

    GLImageBufferObject(unsigned int format);//format eg, GL_RGBA32F , GL_R32UI.
    ~GLImageBufferObject();
    bool resize(int size); //size in bytes
    bool bind(int index, const char* name, unsigned int program, bool read = true, bool write = true);
    static void unbindAll();
    void* map(bool read, bool write);
    bool unmap();
    void copy(GLImageBufferObject& dest, int offsetFrom = 0, int offsetTo = 0, int size = -1);

    int size() {return currentSize;}
    unsigned int getFormat() {return format;}

    //class itself can be used as the buffer object handle
    operator unsigned int() {return bufferObject;}
    unsigned int getTexture() {return textureObject;}

private:
    int currentSize;
    unsigned int format;
    unsigned int bufferObject;
    unsigned int textureObject;

    //disable copying - destructor frees GL memory
    GLImageBufferObject(const GLImageBufferObject& copy) {}
    void operator=(const GLImageBufferObject& copy) {}

    void release();
    void create(int bytes);
    bool allocate(int bytes);

};



}//ns

#endif // GLIMAGEBUFFEROBJECT_H
