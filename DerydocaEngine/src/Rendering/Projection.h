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
		Projection() :
			m_ProjectionMatrix(),
			m_ProjectionMode(Perspective),
			m_FOV(60.0f),
			m_Aspect(1.0f),
			m_ZNear(0.01f),
			m_ZFar(1000.0f)
		{
		}

		glm::mat4 getInverseViewProjectionMatrix(glm::mat4 const& transformModelMatrix) const { return m_ProjectionMatrix * glm::inverse(transformModelMatrix); }
		glm::mat4 getViewMatrix(glm::mat4 const& transformModelMatrix) const { return glm::inverse(transformModelMatrix); }
		glm::mat4 getProjectionMatrix() const { return m_ProjectionMatrix; }
		inline glm::mat4 getViewProjectionMatrix(glm::mat4 const& transformModelMatrix) const { return m_ProjectionMatrix * transformModelMatrix; }

		/*
		Gets a matrix representing the camera's rotation and projection.

		@return Rotation projection matrix
		*/
		glm::mat4 getRotationProjection(glm::fquat const& quaternionRotation) const { return m_ProjectionMatrix * glm::inverse(glm::mat4_cast(quaternionRotation)); }

		void recalculateProjectionMatrix();
		void setProjectionMode(ProjectionMode const& mode) { m_ProjectionMode = mode; }

		void setFov(float const& fov) { m_FOV = fov; }
		void setAspectRatio(float const& aspectRatio) { m_Aspect = aspectRatio; }
		void setAspectRatio(int const& width, int const& height) { m_Aspect = (float)width / (float)(height != 0 ? height : 1); }
		void setZNear(float const& zNear) { m_ZNear = zNear; }
		void setZFar(float const& zFar) { m_ZFar = zFar; }

		float& getFov() { return m_FOV; }
		float getFov() const { return m_FOV; }
		float getAspectRatio() const { return m_Aspect; }
		float& getZNear() { return m_ZNear; }
		float getZNear() const { return m_ZNear; }
		float& getZFar() { return m_ZFar; }
		float getZFar() const { return m_ZFar; }

		SERIALIZE_FUNC(
			SERIALIZE(m_ProjectionMode),
			SERIALIZE(m_FOV),
			SERIALIZE(m_ZNear),
			SERIALIZE(m_ZFar)
		);
	private:
		ProjectionMode m_ProjectionMode;
		float m_FOV;
		float m_ZNear;
		float m_ZFar;

		float m_Aspect;
		glm::mat4 m_ProjectionMatrix;
	};

}
