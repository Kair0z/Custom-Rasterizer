#include "pch.h"
#include "Camera.h"
#include "CatchInput.h"

#pragma region Constructors
Camera::Camera(const FPoint3& startPos, float FOV_Angle, float speed, const Frustrum& frustrum)
	: Camera(startPos, FVector3{0.f, 0.f, -1.f}, FOV_Angle, speed, frustrum)
{

}
Camera::Camera(const FPoint3& startPos, const FPoint3& lookTarget, float FOV_Angle, float speed, const Frustrum& frustrum)
	: Camera(startPos, FVector3{lookTarget - startPos}, FOV_Angle, speed, frustrum)
{

}
Camera::Camera(const FPoint3& startPos, const FVector3& lookAt, float FOV_Angle, float speed, const Frustrum& frustrum)
	: m_FOV_Angle{Elite::ToRadians(FOV_Angle)}
	, m_Speed{speed}
	, m_Frustrum{frustrum}
	, m_MouseState{false, false}
{
	// the axis' forward is the inverse of what the user defines as 'looking at' in rhs
	FVector3 axisForward{-lookAt};
	FVector3 axisRight{};
	FVector3 axisUp{};

	Elite::Normalize(axisForward);
	
	axisRight = Elite::Cross(FVector3{ 0.f, 1.f, 0.f }, axisForward);
	axisUp = Elite::Cross(axisForward, axisRight);

	Elite::Normalize(axisRight);
	Elite::Normalize(axisUp);

	m_OBN_Original = m_OBN = Elite::FMatrix4{ axisRight, axisUp, axisForward, FVector4{startPos.x, startPos.y, startPos.z, 1.f} };
}
#pragma endregion

#pragma region Positioning
void Camera::SetPosition(const FPoint3& newPos)
{
	m_OBN[3] = FVector4{ FVector3{newPos}, 1.f };
}

FPoint3 Camera::GetPosition() const
{
	return FPoint3{ m_OBN[3].xyz };
}

void Camera::ResetPosition()
{
	m_OBN = m_OBN_Original;
}
#pragma endregion

FMatrix4 Camera::GetONB() const
{
	return m_OBN;
}

FMatrix4 Camera::GetProj(float AR) const
{
	float FOV = tanf(m_FOV_Angle / 2);

	float inBetween{ m_Frustrum.m_Near - m_Frustrum.m_Far };

	float a{ m_Frustrum.m_Far / inBetween };
	float b{ (m_Frustrum.m_Far * m_Frustrum.m_Near) / inBetween };

	return FMatrix4
	{
		1.f / (FOV * AR),	0.f,		0.f,		0.f,
		0.f,				1.f / FOV,	0.f,		0.f,
		0.f,				0.f,		a,			b,
		0.f,				0.f,		-1.f,		0.f
		// store og-z component in w
	};
}

Frustrum Camera::GetFrustrum() const
{
	return m_Frustrum;
}

#pragma region FOV
void Camera::SetFOV(float newAngle)
{
	m_FOV_Angle = newAngle;
}

float Camera::GetFOVAngle() const
{
	return m_FOV_Angle;
}

void Camera::IncFOV(float angleIncrement)
{
	m_FOV_Angle += angleIncrement;
}
#pragma endregion

#pragma region Transformation
void Camera::Move(const FVector3& translation)
{
	m_OBN *= Elite::MakeTranslation(translation);
}

void Camera::LerpMove(const FVector3& translation, float t)
{
	m_OBN *= Elite::MakeTranslation(Utils::Lerp<FVector3>(m_OBN[3].xyz, m_OBN[3].xyz + translation, t));
}

void Camera::Rotate(const FVector3& axis, float amount)
{
	m_OBN *= FMatrix4{ Elite::MakeRotation(amount, axis) };
}
#pragma endregion

void Camera::ProcessInput(float elapsedSec)
{
	IVector2 mouseTrans{};
	const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseTrans.x, &mouseTrans.y);

	// Both buttons:
	if ((SDL_BUTTON(SDL_BUTTON_RIGHT) & mouseState) && (SDL_BUTTON(SDL_BUTTON_LEFT) & mouseState))
	{
		Move(FVector3{ float(mouseTrans.x), -float(mouseTrans.y), 0.f } * elapsedSec * m_Speed);
		return;
	}

	// Left button:
	if (SDL_BUTTON(SDL_BUTTON_LEFT) & mouseState)
	{
		if (abs(mouseTrans.x) > abs(mouseTrans.y))
		{
			// Rotate around Global y-axis:
			Rotate(FVector3{ 0.f, 1.f, 0.f }, -float(mouseTrans.x) * elapsedSec * m_Speed / 12.f);
			return;
		}
		else
		{
			// Move forward:
			Move(FVector3{ 0.f, 0.f, float(mouseTrans.y) * elapsedSec * m_Speed});
			return;
		}
	}

	// Right button:
	if (SDL_BUTTON(SDL_BUTTON_RIGHT) & mouseState)
	{
		// Rotate 
		Rotate(FVector3{ 1.f, 0.f, 0.f }, float(mouseTrans.y) * elapsedSec * m_Speed / 12.f);
		return;
	}
}

#pragma region Input
void Camera::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDL_Keycode(Action::MoveCamDown):
		Move(FVector3{ 0.f, -1.f, 0.f } * m_Speed);
		break;

	case SDL_Keycode(Action::MoveCamLeft):
		Move(FVector3{ -1.f, 0.f, 0.f } * m_Speed);
		break;

	case SDL_Keycode(Action::MoveCamUp):
		Move(FVector3{ 0.f, 1.f, 0.f } * m_Speed);
		break;

	case SDL_Keycode(Action::MoveCamRight):
		Move(FVector3{ 1.f, 0.f, 0.f } * m_Speed);
		break;
	}
}
void Camera::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{}
void Camera::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{

}
void Camera::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	
}
void Camera::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	
}
#pragma endregion