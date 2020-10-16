#pragma once

struct Frustrum
{
	float m_Near;
	float m_Far;
};

struct MouseState
{
	bool m_LeftIsDown;
	bool m_RightIsDown;
};

using namespace Elite;

class Camera
{
public:
	Camera(const FPoint3& startPos, float FOV_Angle = 90.f, float speed = 10.f, const Frustrum& frustrum = Frustrum{0.1f, 100.f});
	Camera(const FPoint3& startPos, const FPoint3& lookTarget, float FOV_Angle = 90.f, float speed = 10.f, const Frustrum& frustrum = Frustrum{ 0.1f, 100.f });
	Camera(const FPoint3& startPos, const FVector3& lookAt, float FOV_Angle = 90.f, float speed = 10.f, const Frustrum& frustrum = Frustrum{ 0.1f, 100.f });

	// Position:
	void SetPosition(const FPoint3& newPos);
	FPoint3 GetPosition() const;
	void ResetPosition();

	void Move(const FVector3& translation);
	void LerpMove(const FVector3& translation, float t);
	void Rotate(const FVector3& axis, float amount);

	// Matrices:
	FMatrix4 GetONB() const;
	FMatrix4 GetProj(float AR) const;

	// Frustum:
	Frustrum GetFrustrum() const;

	// FOV:
	void SetFOV(float newAngle);
	float GetFOVAngle() const;
	void IncFOV(float angleIncrement);

	// Input (elapsedSec):
	void ProcessInput(float elapsedSec);

	// Input (without elapsedSec):
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

private:
	// Matrices:
	FMatrix4 m_OBN;
	FMatrix4 m_OBN_Original;

	Frustrum m_Frustrum;
	float m_FOV_Angle;
	float m_Speed;

	MouseState m_MouseState;
};

