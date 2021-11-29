#ifndef AENTITY_H
#define AENTITY_H

#include <vector>
#include <iostream>
#include <stack>
#include <glew.h>
#include <gl.h>
#include <glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>

class AEntity
{
    public:
        virtual void beginDraw() = 0;
        virtual void endDraw() = 0;
        virtual ~AEntity(){}
        //stack of matrix to calcule model matrix
		static std::stack<glm::mat4>& matrixStack(){
			static std::stack<glm::mat4> stack;
			return stack;
		}

		//model matrix (obj)
		static glm::mat4& modelMatrix(){
			static glm::mat4 model;
			return model;
		}

		//view matrix (camera transforms)
		static glm::mat4& viewMatrix(){
			static glm::mat4 view;
			return view;
		}

		//projection matrix (saved on the camera)
		static glm::mat4& projectionMatrix(){
			static glm::mat4 proyec;
			return proyec;
		}

		//light matrix
		static glm::mat4& lightMatrix(){
			static glm::mat4 light;
			return light;
		}

};

#endif // AENTITY_H