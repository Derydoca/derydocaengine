#pragma once
#include "GameComponent.h"
#include "Texture.h"
#include "MeshRenderer.h"

using namespace std;
using namespace glm;

class ImageProjector : public GameComponent
{
public:
	GENINSTANCE(ImageProjector);

	ImageProjector();
	~ImageProjector();

	virtual void init();
	virtual void deserialize(YAML::Node compNode);
	virtual void update(float deltaTime);

	void inline setFocalPoint(const vec3 focalPoint) { m_focalPoint = focalPoint; m_dirty = true; }
	void inline setUpVector(const vec3 upVector) { m_upVector = upVector; m_dirty = true; }
private:
	bool m_dirty = true;
	vec3 m_focalPoint;
	vec3 m_upVector;
	mat4 m_projectionMatrix;
	float m_fov;
	float m_zNear;
	float m_zFar;
	Texture* m_texture;
	vector<MeshRenderer*> m_meshRenderers;

	void updateProjectionMatrix();
};

