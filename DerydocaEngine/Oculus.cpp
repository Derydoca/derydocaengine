#include "Oculus.h"
#include <stdio.h>

Oculus::Oculus()
{
}

Oculus::~Oculus()
{
	if (m_leftEyeTexture) { delete m_leftEyeTexture; }
	if (m_rightEyeTexture) { delete m_rightEyeTexture; }
}

void Oculus::init()
{
	printf("Initializing Oculus headset.\n");
	ovrResult initResult = ovr_Initialize(nullptr);
	if (initResult == ovrSuccess)
	{
		printf("Initialization successful. Creating session.\n");
		ovrResult result = ovr_Create(&m_session, &m_graphicsLuid);

		if (result == ovrSuccess)
		{
			printf("Success!.\n");

			m_hmdDesc = ovr_GetHmdDesc(m_session);

			ovrSizei lEye = ovr_GetFovTextureSize(m_session, ovrEye_Left, m_hmdDesc.DefaultEyeFov[0], 1.0);
			m_leftEyeTexture->initializeTexture(lEye.w, lEye.h);

			m_initialized = true;
		}
	}
	else
	{
		printf("Unable to initialize the Oculus headset.\n");
	}
}

void Oculus::render(Camera* camera, GameObject* root)
{
	//m_trackingState = ovr_GetTrackingState(m_session, 0, true);
	m_leftEyeTexture->bindAsRenderTexture();
	camera->renderRoot(root);
	m_rightEyeTexture->bindAsRenderTexture();
	camera->renderRoot(root);

	//ovr_BeginFrame(m_session);
}
