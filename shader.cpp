#include "shader.h"

static GLuint CreateShader(const std::string& text, GLenum shaderType);
static std::string LoadShader(const std::string& fileName);
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);


Shader::Shader(const std::string& fileName, unsigned const int displayHeight, unsigned const int displayWidth){

    height = displayHeight;
    width = displayWidth;

    program = glCreateProgram();
    shaders[0] = CreateShader(LoadShader(fileName + ".vs"),GL_VERTEX_SHADER);
    shaders[1] = CreateShader(LoadShader(fileName + ".fs"),GL_FRAGMENT_SHADER);

    for(unsigned int i = 0; i < NUM_SHADERS; i++)
        glAttachShader(program,shaders[i]);

    glBindAttribLocation(program,0,"pos");
    glBindAttribLocation(program,1,"texCoord");

    glLinkProgram(program);
    CheckShaderError(program,GL_LINK_STATUS,true,"Error in shader, linking failed: ");   

    glValidateProgram(program);
    CheckShaderError(program,GL_VALIDATE_STATUS,true,"Error in shader, validation failed: ");   
}

Shader::~Shader(){
    for(unsigned int i = 0; i < NUM_SHADERS; i++){
        glDetachShader(program,shaders[i]);
        glDeleteShader(shaders[i]);
    }
    glDeleteProgram(program);
}

void Shader::Bind(){
    glUseProgram(program);
    GLint resolutionUniformLocation = glGetUniformLocation(program,"systemResolution");
    glUniform2f(resolutionUniformLocation,width,height);
}

void Shader::UpdateTime(unsigned const int timeTicks){
    GLint timeUniformLocation = glGetUniformLocation(program,"systemTime");
    //printf("Setting current uniform value to %d.\n",timeTicks);
    glUniform1f(timeUniformLocation,timeTicks);
}

static GLuint CreateShader(const std::string& text, GLenum shaderType){
    GLuint shader = glCreateShader(shaderType);

    if(shader == 0)
        std::cerr << "Shader creation failed." << std::endl;
    
    const GLchar* shaderSourceStrings[1];
    GLint shaderSourceStringLengths[1];

    shaderSourceStrings[0] = text.c_str();
    shaderSourceStringLengths[0] = text.length();

    glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths); //Send source code to opengl
    glCompileShader(shader);

    CheckShaderError(shader,GL_COMPILE_STATUS,false,"Error in shader compilation: ");

    return shader;
}

//load shader from file
static std::string LoadShader(const std::string& fileName){
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open()){
        while(file.good()){
            getline(file,line);
            output.append(line + "\n");
        }
    } else {
        std::cerr << "Unable to load shader: " << fileName << std::endl;
    }
    return output;
}

//report shader errors
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage){
    GLint success = 0;
    GLchar error[1024] = {0};

    if(isProgram)
        glGetProgramiv(shader,flag,&success);
    else
        glGetShaderiv(shader,flag,&success);
    
    if(success == GL_FALSE){
        if(isProgram)
            glGetProgramInfoLog(shader,sizeof(error),NULL,error);
        else
            glGetShaderInfoLog(shader,sizeof(error),NULL,error);
    }
}