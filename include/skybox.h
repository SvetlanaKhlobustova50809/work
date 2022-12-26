#ifndef H_SKY
#define H_SKY
#include <vector>
#include <string>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
using std::cerr;
using std::endl;
using std::string;
using std::vector;
class SkyBox{
    GLuint VBOvertices,VBOindices;
    unsigned int cubemapTexture;

    unsigned int loadCubemap(vector<string> faces, const string sky)
    {
        unsigned int textureID=0;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

       /* string path = string("../textures/skybox/") + sky + string("/");
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            string file = path + faces[i];
            int width, height, nrChannels;
            cerr << file << endl;
            GLubyte* data = stbi_load(file.c_str(), &width, &height, &nrChannels, STBI_rgb);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                ); 
            	 glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
                stbi_image_free(data);
                 glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }*/
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }  
    void Init(const float size, const string sky){
        GLfloat sky_vertices[] = {
            size,  size,  size,  
            size, -size,  size, 
            size,  size, -size,
            size, -size, -size,
           -size, -size, -size,
           -size,  size, -size, 
           -size, -size,  size,  
           -size,  size,  size   
        };

        GLuint sky_indices[] = {  
            0, 1, 3,  
            3, 2, 0,
            0, 1, 7,
            7, 6, 1,
            1, 3, 6,
            6, 4, 3,
            3, 2, 4,
            4, 2, 5,
            5, 4, 6,
            6, 5, 7,
            7, 5, 2,
            2, 0, 7
        };  
        vector<string> faces
        {
            "right.tga",
            "left.tga",
            "top.tga",
            "bottom.tga",
            "back.tga",
            "front.tga"
        };
	    glGenVertexArrays(1, &vao);
	    glGenBuffers(1, &VBOvertices);
	    glGenBuffers(1, &VBOindices);
    
        cubemapTexture = loadCubemap(faces, sky);
	    glBindVertexArray(vao); 
		
		//передаем в шейдерную программу атрибут координат вершин
	    glBindBuffer(GL_ARRAY_BUFFER, VBOvertices); 
	    glBufferData(GL_ARRAY_BUFFER, sizeof(sky_vertices) ,sky_vertices, GL_STATIC_DRAW); 
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	    glEnableVertexAttribArray(0); 

		//передаем в шейдерную программу индексы
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VBOindices); 
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sky_indices), sky_indices, GL_STATIC_DRAW); 

	    glBindBuffer(GL_ARRAY_BUFFER, 0);

	    glBindVertexArray(0);
    }

    public:
        GLuint vao=0;

        SkyBox(const float size = 1, const string skybox = "1"){
            Init(size, skybox);
        };

        void Draw(const ShaderProgram& shader, const float4x4& projection,const float4x4& view, const float3& campos){
            shader.StartUseShader();
            shader.SetUniform("projection", projection);
            shader.SetUniform("view", view);
            shader.SetUniform("campos", campos);

            glBindVertexArray(vao);
           
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            shader.SetUniform("skybox", 4);
            
            glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_INT, nullptr); 
            glBindVertexArray(0); 
            shader.StopUseShader();
        }


};



#endif