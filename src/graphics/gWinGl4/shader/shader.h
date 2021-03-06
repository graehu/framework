#ifndef SHADER_H
#define SHADER_H

class shader 
{
public:
    shader(const char *vsFile, const char *gsFile, const char *fsFile);
    ~shader(void);
    void bind();
    void unbind();
    unsigned int id();
    
private:

	  void init(const char *vsFile, const char *gsFile, const char *fsFile);

    unsigned int shader_id; // The shader program identifier
    unsigned int shader_vp; // The vertex shader identifier
	unsigned int shader_gp; // The geometry shader identifier
    unsigned int shader_fp; // The fragment shader identifier
    
    bool inited; // Whether or not we have initialized the shader
};
#endif//SHADER_H