# Qamera 🎥

A built-in camera / recording rig for **Beat Saber on Quest** — like having a tiny OBS inside the game. Free detached camera, adjustable FOV, smoothing, headset-mirror toggle, and saved presets, all controlled from an in-game settings menu.

- **Target game:** Beat Saber `1.37.0`
- **Modloader:** Scotland2 (`paper2_scotland2`) + Paperlog
- **ABI:** `arm64-v8a`
- **Install via:** ModsBeforeFriday (MBF)

## Features

| Feature | What it does |
|---|---|
| **Free / detached camera** | Place a recording camera anywhere in the world via X/Y/Z position + rotation offsets. |
| **Adjustable FOV** | 30°–120° field of view on the recording camera. |
| **Smoothing / damping** | Independent position & rotation smoothing for buttery, cinematic motion. |
| **Headset mirror** | Toggle the recording feed to exactly mirror your headset view. |
| **Presets** | Save/apply named camera setups; persisted to disk. |
| **Config persistence** | Everything is saved automatically and restored on next launch. |

All settings live in **Settings → Mods → Qamera** in-game (BSML menu).

---

## Building the `.qmod` with no PC (GitHub Actions)

This is a **source mod** — there's no prebuilt binary in this repo, because the native code has to be compiled against headers reverse-engineered from your exact game version (1.37.0). That compile step needs the Android NDK and the QPM package registry, both of which need real internet access during the build. The steps below do that compile in the cloud, so all you need is a browser (works fine on the Quest's own browser or your phone).

1. Create a free GitHub account if you don't have one (github.com, from your phone or Quest browser).
2. Create a new repository and upload everything in this project folder to it (GitHub's web UI has an "Add file → Upload files" button — no git command line needed).
3. Go to the **Actions** tab of your repo → you should see the **"Build Qamera qmod"** workflow → click **Run workflow**.
4. Wait a few minutes for it to finish (green check ✅).
5. Click into the finished run → under **Artifacts**, download `qamera-qmod` (it's a zip containing `qamera.qmod`).
6. Unzip it (or use a phone file manager / zip app) to get `qamera.qmod`.

### One thing to check on the first run

Open the build log for the `qpm restore` step. If it errors or the resulting mod crashes Beat Saber on launch, the most likely cause is the `bs-cordl` dependency in `qpm.json` — that package's version is tied to a *specific* Beat Saber build, and the placeholder in this repo (`versionRange: "*"`) just grabs whatever's newest, which usually targets a newer game version than 1.37.0. Check the BSMG mod list for 1.37.0 mods' shared dependency files (search "bs-cordl 1.37.0 qpm") or ask in the BSMG Discord `#quest-mod-dev` channel for the exact version string, then edit that one line in `qpm.json` and re-run the workflow. This is a normal first step for *any* source mod targeting a specific game build — it's not specific to this project.

---

## Installing the `.qmod` (ModsBeforeFriday)

1. On your Quest, open the browser and go to **`mbf.bsquest.xyz`**.
2. Let MBF mod **Beat Saber 1.37.0** (installs Scotland2 + core mods).
3. In MBF → **Mods → Add Mod** → pick the `qamera.qmod` you downloaded above.
4. MBF auto-downloads the dependencies (custom-types, bsml) listed in the manifest. beatsaber-hook, bs-cordl, and config-utils are linked into the compiled `.so` directly.
5. Launch Beat Saber → **Settings → Mods → Qamera**.

---

## Project layout

```
qamera/
├── qpm.json                       # QPM manifest + dependencies (1.37.0)
├── mod.template.json              # -> compiled into mod.json inside the qmod
├── CMakeLists.txt                 # build config -> libqamera.so
├── build.sh                       # NDK cross-compile (arm64-v8a)
├── copy-and-restart.sh            # adb deploy helper (only useful if you do have a PC)
├── .github/workflows/build.yml    # cloud build -- this is how you get a real qmod with no PC
├── include/
│   ├── main.hpp                   # logging + globals
│   ├── config.hpp                 # persistent config (config-utils)
│   ├── camera_manager.hpp         # recording camera owner
│   └── presets.hpp                # named preset store
└── src/
    ├── main.cpp                   # setup() / late_load() entry points
    ├── hooks.cpp                  # game hooks driving the camera
    ├── camera_manager.cpp         # free-cam / mirror / FOV / smoothing logic
    ├── presets.cpp                # save/load presets to disk
    └── settings_menu.cpp          # in-game BSML settings tab
```

---

## Notes / caveats

- This is a **source mod**; the per-version compile must run against the real 1.37.0 codegen headers (handled by `qpm restore`, see the build note above about pinning `bs-cordl`).
- Hooked types (`MainSystemInit`, `VRRenderingParamsSetup`) have been stable hook points across the 1.3x–1.4x line, but bs-cordl auto-generates their exact method signatures per game version — if the cloud build fails on one of these, it's almost always a one-line signature fix in `src/hooks.cpp`, not a logic problem.
- Capturing the actual video feed to a file is done by Quest's native screen recorder or a connected capture tool; Qamera controls **what the camera sees** (angle, FOV, smoothing). A render-to-file pipeline can be added later if you want fully in-mod recording.
