#!/usr/bin/env bash
set -euo pipefail

# Build a standalone macOS executable for image_gray_check_v201.py.
# Run this script on macOS only.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="${SCRIPT_DIR}"

APP_NAME="${APP_NAME:-image_gray_check_v201}"
ENTRY_FILE="${REPO_ROOT}/image_gray_check_v201.py"
DIST_DIR="${REPO_ROOT}/dist-macos-v201"
WORK_DIR="${REPO_ROOT}/build-macos-v201/pyinstaller"
VENV_DIR="${REPO_ROOT}/build-macos-v201/venv-pyinstaller"
PYTHON_BIN="${PYTHON_BIN:-python3}"
WINDOWED="${WINDOWED:-0}"

if [[ "$(uname -s)" != "Darwin" ]]; then
    echo "Error: this script must be run on macOS (Darwin)."
    echo "Current system: $(uname -s)"
    exit 1
fi

if [[ ! -f "${ENTRY_FILE}" ]]; then
    echo "Error: entry file not found: ${ENTRY_FILE}"
    exit 1
fi

echo "[1/4] Create virtual environment: ${VENV_DIR}"
"${PYTHON_BIN}" -m venv "${VENV_DIR}"
source "${VENV_DIR}/bin/activate"

echo "[2/4] Install build dependencies"
python -m pip install --upgrade pip wheel
python -m pip install pyinstaller pillow openpyxl

echo "[3/4] Build executable with PyInstaller"
rm -rf "${DIST_DIR}" "${WORK_DIR}"

PYI_ARGS=(
    --noconfirm
    --clean
    --name "${APP_NAME}"
    --distpath "${DIST_DIR}"
    --workpath "${WORK_DIR}"
    --hidden-import "PIL.ImageTk"
    --collect-submodules "openpyxl"
    --strip
)

if command -v upx >/dev/null 2>&1 && [[ "${SMALLER:-0}" == "1" ]]; then
    PYI_ARGS+=(--upx-dir "$(command -v upx)")
fi

if [[ "${ONEDIR:-0}" == "1" ]]; then
    PYI_ARGS+=(--onedir)
else
    PYI_ARGS+=(--onefile)
fi

if [[ "${WINDOWED}" == "1" ]]; then
    PYI_ARGS+=(--windowed)
fi

pyinstaller "${PYI_ARGS[@]}" "${ENTRY_FILE}"

echo "[4/4] Done"
if [[ "${ONEDIR:-0}" == "1" ]]; then
    chmod +x "${DIST_DIR}/${APP_NAME}/${APP_NAME}"
    echo "Output: ${DIST_DIR}/${APP_NAME}/${APP_NAME}"
    echo "Tip: run with: ${DIST_DIR}/${APP_NAME}/${APP_NAME} --help"
else
    chmod +x "${DIST_DIR}/${APP_NAME}"
    echo "Output: ${DIST_DIR}/${APP_NAME}"
    echo "Tip: run with: ${DIST_DIR}/${APP_NAME} --help"
fi
