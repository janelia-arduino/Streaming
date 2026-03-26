#!/usr/bin/env python3
"""Run the repo's release-gate checks in a fixed order."""

from __future__ import annotations

import os
import subprocess
from pathlib import Path


def _project_root() -> Path:
    env_root = os.environ.get("PIXI_PROJECT_ROOT")
    if env_root:
        return Path(env_root).resolve()
    return Path(__file__).resolve().parents[1]


def _run(cmd: list[str], *, cwd: Path, env: dict[str, str]) -> int:
    print("+ " + " ".join(cmd))
    completed = subprocess.run(cmd, cwd=str(cwd), env=env, check=False)
    return int(completed.returncode)


def main() -> int:
    root = _project_root()
    env = os.environ.copy()
    env.setdefault("PLATFORMIO_CORE_DIR", str((root / ".platformio").resolve()))

    commands = [
        ["python", "tools/version_sync.py", "check"],
        ["make", "native-test"],
        [
            "python",
            "tools/pio_task.py",
            "build",
            "--example",
            "examples/StreamingExample",
            "--env",
            "teensy41",
        ],
        [
            "python",
            "tools/pio_task.py",
            "build",
            "--example",
            "examples/StreamingTests",
            "--env",
            "teensy41",
        ],
        [
            "python",
            "tools/pio_task.py",
            "build",
            "--example",
            "examples/StreamingExample",
            "--env",
            "uno",
        ],
    ]

    for cmd in commands:
        rc = _run(cmd, cwd=root, env=env)
        if rc != 0:
            return rc
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
