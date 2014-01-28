#pragma once

#ifndef IGLCOMPONENT_H
#define IGLCOMPONENT_H

#ifndef __APPLE__
#include "GL/glew.h"
#endif

#include "components/SpatialComponent.h"
#include "GLTransform.h"
#include "systems/GLSLShader.h"
#include <unordered_map>
#include <memory>

namespace Sigma {
	// A struct to store which index each of its verts are.
	struct Face {
		Face(unsigned int v1, unsigned int v2, unsigned int v3) : v1(v1), v2(v2), v3(v3) { }
		unsigned int v1, v2, v3;
	};

	// A struct to store the location for each vertex.
	struct Vertex {
		Vertex() : x(0.0f), y(0.0f), z(0.0f) {}
		Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
		Vertex(glm::vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {} 
		float x,y,z;
	};

	// A struct to store the color value for each vertex
	struct Color {
		Color(float r, float g, float b) : r(r), g(g), b(b) { }
		float r,g,b;
	};

	// A struct to store 2D or 1D texture coordinates for each vertex
	struct TexCoord {
        TexCoord(float u, float v = 0.0f) : u(u), v(v) { }
        float u, v;
    };

    // A struct to store material information for each vertex.
    // Wikipedia explains what's going on:
    // http://en.wikipedia.org/wiki/Wavefront_.obj_file#Basic_materials
    struct Material {
        Material() {
            ka[0] = 1.0f; ka[1] = 1.0f; ka[2] = 1.0f;
            kd[0] = 1.0f; kd[1] = 1.0f; kd[2] = 1.0f;
            ks[0] = 1.0f; ks[1] = 1.0f; ks[2] = 1.0f;
            tr = 1.0f;
            hardness = 64.0f;
            illum = 1;
			ambientMap = 0;
			diffuseMap = 0;
			specularMap = 0;
			normalMap = 0;
        }
        float ka[3];
        float kd[3];
        float ks[3];
        float tr; // Aka d
        float hardness;
        int illum;
        
        GLuint ambientMap;
        GLuint diffuseMap;
        GLuint specularMap;
		GLuint normalMap;
    };

	class IGLComponent : public SpatialComponent {
	public:
		SET_COMPONENT_TYPENAME("IGLComponent");

		IGLComponent() 
			: lightingEnabled(true), SpatialComponent(0) {} // Default ctor setting entity ID to 0.
		IGLComponent(const int entityID)
			: lightingEnabled(true), SpatialComponent(entityID) {} // Ctor that sets the entity ID.

        typedef std::unordered_map<std::string, std::shared_ptr<GLSLShader>> ShaderMap;

		/**
		 * \brief Initializes the IGLComponent.
		 *
		 * \param entityID The entity this component belongs to.
		 */
		virtual void InitializeBuffers() = 0;

		/**
		 * \brief Retrieves the specified buffer.
		 *
		 * \param const int index The index of the buffer to retrieve
		 * \return   unsigned int THe ID of the buffer
		 */
		unsigned int GetBuffer(const int index) {
			return this->buffers[index];
		}

		/**
		 * \brief Returns the number of elements to draw for this component.
		 *
		 * \return unsigned int The number of elements to draw.
		 */
		virtual unsigned int MeshGroup_ElementCount(const unsigned int group = 0) const = 0;

		/**
		 * \brief Returns the draw mode for this component.
		 *
		 * \return unsigned int The draw mode (ex. GL_TRIANGLES, GL_TRIANGLE_STRIP).
		 */
		unsigned int DrawMode() const { return this->drawMode; }

		/**
		 * \brief Renders the component.
		 *
		 * \param view The current view matrix
		 * \param proj The current project matrix
		 */
		virtual void Render(glm::mediump_float *view, glm::mediump_float *proj)=0;

		/**
		 * \brief Return the VAO ID of this component.
		 *
		 * \return   unsigned int The VAO ID.
		 */
		unsigned int Vao() const { return this->vao; }

		/**
		 * \brief Sets the face culling mode
		 *
		 */
		virtual void SetCullFace(std::string cull_face) {
			if(cull_face == "back") {
				this->cull_face = GL_BACK;
			}
			else if (cull_face == "front") {
				this->cull_face = GL_FRONT;
			}
			else if (cull_face == "none") {
				this->cull_face = 0;
			}
			else {
				assert(0 && "Invalid cull_face parameter");
			}
		};

        /** \brief load the given shader
         *
         * \param filename the base name of the shader. loads filename.vert and filename.frag.
         *  filename should be a relative path, like "shaders/mesh"
         * \return void
         */
        void LoadShader(const std::string& filename);
		std::shared_ptr<GLSLShader> GetShader() { return this->shader; }

		void SetLightingEnabled(bool enabled) { this->lightingEnabled = enabled; }
		bool IsLightingEnabled() { return this->lightingEnabled; }

		// The index in buffers for each type of buffer.
		int ElemBufIndex;
		int VertBufIndex;
		int UVBufIndex;
		int ColorBufIndex;
		int NormalBufIndex;
		int TangentBufIndex;
		int BiNormalBufIndex;

	protected:
		unsigned int buffers[10]; // The various buffer IDs.
		unsigned int vao; // The VAO that describes this component's data.
		unsigned int drawMode; // The current draw mode (ex. GL_TRIANGLES, GL_TRIANGLE_STRIP).
		GLuint cull_face; // The current culling method for this component.

        std::shared_ptr<GLSLShader> shader; // shaders are shared among components
        // name-->shader map to look up already-loaded shaders (so each can be loaded only once)
        static ShaderMap loadedShaders;

		bool lightingEnabled;
	}; // class IGLComponent
} // namespace Sigma

#endif // IGLCOMPONENT_H
