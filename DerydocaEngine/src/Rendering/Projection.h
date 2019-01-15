#pragma once
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace DerydocaEngine::Rendering
{

	/* Types of projection modes */
	enum ProjectionMode {
		/* Orthographic projection */
		Orthographic,
		/* Perspective projection */
		Perspective
	};

	struct Projection
	{
	public:
		glm::mat4 getInverseViewProjectionMatrix(glm::mat4 const& transformModelMatrix) const { return m_projectionMatrix * glm::inverse(transformModelMatrix); }
		glm::mat4 getViewMatrix(glm::mat4 const& transformModelMatrix) const { return glm::inverse(transformModelMatrix); }
		glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }
		inline glm::mat4 getViewProjectionMatrix(glm::mat4 const& transformModelMatrix) const { return m_projectionMatrix * transformModelMatrix; }

		/*
		Gets a matrix representing the camera's rotation and projection.

		@return Rotation projection matrix
		*/
		glm::mat4 getRotationProjection(glm::fquat const& quaternionRotation) const { return m_projectionMatrix * glm::inverse(glm::mat4_cast(quaternionRotation)); }

		void recalculateProjectionMatrix();
		void setProjectionMode(ProjectionMode const& mode) { m_projectionMode = mode; }

		void setFov(float const& fov) { m_fov = fov; }
		void setAspectRatio(float const& aspectRatio) { m_aspect = aspectRatio; }
		void setAspectRatio(int const& width, int const& height) { m_aspect = (float)width / (float)(height != 0 ? height : 1); }
		void setZNear(float const& zNear) { m_zNear = zNear; }
		void setZFar(float const& zFar) { m_zFar = zFar; }

		float& getFov() { return m_fov; }
		float getFov() const { return m_fov; }
		float getAspectRatio() const { return m_aspect; }
		float& getZNear() { return m_zNear; }
		float getZNear() const { return m_zNear; }
		float& getZFar() { return m_zFar; }
		float getZFar() const { return m_zFar; }
	private:
		glm::mat4 m_projectionMatrix;
		ProjectionMode m_projectionMode = Perspective;
		float m_fov;
		float m_aspect;
		float m_zNear;
		float m_zFar;
	};

}
