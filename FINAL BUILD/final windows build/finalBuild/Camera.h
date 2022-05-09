#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>


namespace GE {
	class Camera {
	public:
		Camera(glm::vec3 cam_pos, glm::vec3 cam_target, glm::vec3 camupDir, float fov, float ar, float near, float far) {
			pos = cam_pos;
			target = cam_target;
			up = camupDir;

			fovy = fov;
			aspectRatio = ar;
			nearClip = near;
			farClip = far;

			updateCamMatrices();
			SDL_GetMouseState(&oldMouseX, &oldMouseY);
		}
		~Camera() {

		}

		// ------- all gets to get information about the camera 
		float getPosX() {
			return pos.x;
		}

		float getPosY() {
			return pos.y;
		}

		float getPosZ() {
			return pos.z;

		}

		glm::vec3 getPos() {
			return pos;
		}

		glm::vec3 getTarget() {
			return target;
		}

		glm::vec3 getUpDir() {
			return up;
		}

		float getPitch() {
			return pitch;
		}

		float getYaw() {
			return yaw;
		}

		float getOldMouseX() {
			return oldMouseX;
		}

		float getOldMouseY() {
			return oldMouseY;
		}

		// used by draw to send view matrix to vertex shader
		glm::mat4 getViewMatrix() {
			return viewMat;
		}

		// used by draw to send view matrix to fragment shader
		glm::mat4 getProjectionMatrix() {
			return projectionMat;
		}

		// -- these are sets to be able to update the camrea

		void setPosX(float newX) {
			pos = glm::vec3(newX, pos.y, pos.z);

			updateCamMatrices();
		}

		void setPosY(float newY) {
			pos = glm::vec3(pos.x, newY, pos.z);

			updateCamMatrices();
		}

		void setPosZ(float newZ) {
			pos = glm::vec3(pos.x, pos.y, newZ);

			updateCamMatrices();
		}

		void setPos(float newX, float newY, float newZ) {
			pos = glm::vec3(newX, newY, newZ);
			updateCamMatrices();
		}

		void setPos(glm::vec3 newPos) {
			pos = newPos;
			updateCamMatrices();
		}

		void setTarget(glm::vec3 newTarget) {
			target = newTarget;

			updateCamMatrices();
		}

		void setUpDir(glm::vec3 newUp) {
			up = newUp;

			updateCamMatrices();
		}

		void setPitch(float newPitch) {
			pitch = newPitch;
			//clamp the pitch values
			if (pitch > 70.0f) pitch = 70.0f;
			if (pitch < -70.0f) pitch = -70.0f;
		}

		void setYaw(float newYaw) {
			yaw = newYaw;
		}

		void setOldMouseX(float newMouseX) {
			oldMouseX = newMouseX;
		}

		void setOldMouseY(float newMouseY) {
			oldMouseY = newMouseY;
		}

		void setFov(float newFov) {
			fovy = newFov;

			updateCamMatrices();
		}

		void setAspectRatio(float newAR) {
			aspectRatio = newAR;

			updateCamMatrices();
		}

		void setNearClip(float newNearClip) {
			nearClip = newNearClip;

			updateCamMatrices();
		}

		void setFarClip(float newFarClip) {
			farClip = newFarClip;

			updateCamMatrices();
		}

		void updateCamMatrices() {
			viewMat = glm::lookAt(pos, pos + target, up);
			projectionMat = glm::perspective(glm::radians(fovy), aspectRatio, nearClip, farClip);
		}

	private:

		glm::vec3  pos;
		glm::vec3 target;
		glm::vec3 up;

		float fovy;
		float aspectRatio;
		float nearClip;
		float farClip;

		// camrea pitch and yaw values
		float pitch = 0.0f;
		float yaw = -90.0f;

		// mouse x and y
		int oldMouseX, oldMouseY;

		glm::mat4 viewMat;
		glm::mat4 projectionMat;

		// update cam matrix, done when somthign is updated/changed



	};

}