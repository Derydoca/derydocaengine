#pragma once
#include "ovr/OVR_CAPI.h"
#include "ovr/OVR_CAPI_GL.h"
#include "ovr/OVR_CAPI_Keys.h"
#include "GameComponent.h"
#include "RenderTexture.h"
#include "GameObject.h"
#include "Camera.h"

class Oculus : public GameComponent
{
public:
	GENINSTANCE(Oculus);
	Oculus();
	~Oculus();

	void init();
	
	void render(Camera* camera, GameObject* root);

private:
	ovrSession m_session;
	ovrGraphicsLuid m_graphicsLuid;
	ovrHmdDesc m_hmdDesc;
	ovrTrackingState m_trackingState;
	RenderTexture* m_leftEyeTexture;
	RenderTexture* m_rightEyeTexture;
	RenderTexture* m_leftEyeDepthTexture;
	RenderTexture* m_rightEyeDepthTexture;

	bool m_initialized = false;
};
