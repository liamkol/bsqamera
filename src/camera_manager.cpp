#include "camera_manager.hpp"
#include "main.hpp"
#include "config.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Mathf.hpp"

using namespace UnityEngine;

namespace Qamera {

    void CameraManager::Init(Transform* headsetTransform) {
        headset = headsetTransform;

        auto& cfg = getConfig();
        SetTargetPose(
            {cfg.PosX.GetValue(), cfg.PosY.GetValue(), cfg.PosZ.GetValue()},
            {cfg.RotX.GetValue(), cfg.RotY.GetValue(), cfg.RotZ.GetValue()}
        );
        SetFov(cfg.Fov.GetValue());
        SetSmoothing(cfg.PositionSmoothing.GetValue(), cfg.RotationSmoothing.GetValue());
        SetMirrorHeadset(cfg.MirrorHeadset.GetValue());

        RebuildCamera();
    }

    void CameraManager::RebuildCamera() {
        if (recordingCamera) {
            UnityEngine::Object::Destroy(recordingCamera->get_gameObject());
        }

        auto* go = GameObject::New_ctor("QameraRecordingCamera");
        recordingCamera = go->AddComponent<Camera*>();
        recordingCameraTransform = go->get_transform();

        recordingCamera->set_fieldOfView(fov);
        recordingCameraTransform->set_position(targetPos);
        recordingCameraTransform->set_rotation(targetRot);

        LOG_INFO("Recording camera (re)built, fov=" + std::to_string(fov));
    }

    void CameraManager::Tick(float deltaTime) {
        if (!recordingCameraTransform) return;

        if (mirrorHeadset && headset) {
            recordingCameraTransform->set_position(headset->get_position());
            recordingCameraTransform->set_rotation(headset->get_rotation());
            return;
        }

        // Exponential smoothing toward target pose. positionSmoothing /
        // rotationSmoothing are "time to mostly catch up" in seconds;
        // 0 == snap instantly, larger == more cinematic lag.
        float posT = positionSmoothing <= 0.0f
            ? 1.0f
            : 1.0f - Mathf::Exp(-deltaTime / positionSmoothing);
        float rotT = rotationSmoothing <= 0.0f
            ? 1.0f
            : 1.0f - Mathf::Exp(-deltaTime / rotationSmoothing);

        auto current = recordingCameraTransform->get_position();
        auto lerped = Vector3::Lerp(current, targetPos, posT);
        recordingCameraTransform->set_position(lerped);

        auto currentRot = recordingCameraTransform->get_rotation();
        auto slerped = Quaternion::Slerp(currentRot, targetRot, rotT);
        recordingCameraTransform->set_rotation(slerped);
    }

    void CameraManager::SetFov(float newFov) {
        fov = Mathf::Clamp(newFov, 30.0f, 120.0f);
        if (recordingCamera) {
            recordingCamera->set_fieldOfView(fov);
        }
    }

    void CameraManager::SetSmoothing(float posSmoothing, float rotSmoothing) {
        positionSmoothing = posSmoothing;
        rotationSmoothing = rotSmoothing;
    }

    void CameraManager::SetTargetPose(Vector3 pos, Vector3 eulerRot) {
        targetPos = pos;
        targetRot = Quaternion::Euler(eulerRot.x, eulerRot.y, eulerRot.z);
    }
}
