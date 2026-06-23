#pragma once

#include "UnityEngine/Camera.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/Quaternion.hpp"

namespace Qamera {

    // Owns the detached recording camera GameObject and drives its
    // transform every frame based on config + smoothing state.
    //
    // NOTE: the exact Camera / rig hookup (which Transform the headset
    // camera lives under, what CameraManager.MainCamera is named in
    // 1.37.0's codegen) is the part most likely to need a small fix
    // once real bs-cordl headers for 1.37.0 are pulled in by CI --
    // the il2cpp type names below match the layout used on 1.40.8 and
    // recent versions, which has been stable across the 1.3x line too,
    // but if the build fails on a missing member, that's expected and
    // just needs the corresponding codegen name swapped in.
    class CameraManager {
    public:
        static CameraManager& get() {
            static CameraManager instance;
            return instance;
        }

        // Called once a valid scene/headset camera is available.
        void Init(UnityEngine::Transform* headsetTransform);

        // Called every frame (hooked off the game's render-params update).
        void Tick(float deltaTime);

        // Recreates the recording camera GameObject from scratch,
        // applying current FOV / position / rotation from config.
        void RebuildCamera();

        void SetMirrorHeadset(bool mirror) { mirrorHeadset = mirror; }
        bool IsMirroring() const { return mirrorHeadset; }

        void SetFov(float fov);
        void SetSmoothing(float posSmoothing, float rotSmoothing);
        void SetTargetPose(UnityEngine::Vector3 pos, UnityEngine::Vector3 eulerRot);

    private:
        CameraManager() = default;

        UnityEngine::Camera* recordingCamera = nullptr;
        UnityEngine::Transform* recordingCameraTransform = nullptr;
        UnityEngine::Transform* headset = nullptr;

        UnityEngine::Vector3 targetPos{};
        UnityEngine::Quaternion targetRot{};

        float fov = 60.0f;
        float positionSmoothing = 0.15f;
        float rotationSmoothing = 0.15f;
        bool mirrorHeadset = false;
    };
}
