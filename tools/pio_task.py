#!/usr/bin/env python3
"""PlatformIO task wrapper used by pixi tasks."""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path


def _project_root() -> Path:
    env_root = os.environ.get("PIXI_PROJECT_ROOT")
    if env_root:
        return Path(env_root).resolve()
    return Path(__file__).resolve().parents[1]


def _normalize_rel_path(path: str) -> Path:
    return Path(path.replace("\\", "/"))


def _example_src_dir(root: Path, example: str) -> Path:
    rel = _normalize_rel_path(example)
    src_dir = (root / rel).resolve()
    if not src_dir.exists():
        raise FileNotFoundError(f"Example path does not exist: {rel}")
    return src_dir


def _example_build_dir(root: Path, example: str) -> Path:
    rel = _normalize_rel_path(example)
    return (root / ".pio" / "build" / rel).resolve()


def _run(cmd: list[str], *, cwd: Path, env: dict[str, str]) -> int:
    print("+ " + " ".join(cmd))
    try:
        completed = subprocess.run(cmd, cwd=str(cwd), env=env, check=False)
        return int(completed.returncode)
    except FileNotFoundError:
        print("ERROR: 'pio' command not found. Did you run `pixi install`?", file=sys.stderr)
        return 127


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(prog="pio_task.py")
    sub = parser.add_subparsers(dest="command", required=True)

    def add_common_example_env(sp: argparse.ArgumentParser) -> None:
        sp.add_argument(
            "--example",
            default="examples/StreamingExample",
            help="Repo-relative example directory to build.",
        )
        sp.add_argument(
            "--env",
            default="teensy41",
            help="PlatformIO environment name.",
        )

    sp_build = sub.add_parser("build", help="Build an example.")
    add_common_example_env(sp_build)

    sp_clean = sub.add_parser("clean", help="Clean build artifacts for an example/environment.")
    add_common_example_env(sp_clean)

    sp_upload = sub.add_parser("upload", help="Build and upload an example.")
    add_common_example_env(sp_upload)
    sp_upload.add_argument("--port", default=None, help="Upload port (optional).")

    sp_flash = sub.add_parser("flash", help="Clean and upload an example.")
    add_common_example_env(sp_flash)
    sp_flash.add_argument("--port", default=None, help="Upload port (optional).")

    sp_rebuild = sub.add_parser("rebuild", help="Clean and rebuild an example.")
    add_common_example_env(sp_rebuild)

    sp_deep = sub.add_parser("deepclean", help="Delete the isolated build dir for an example.")
    sp_deep.add_argument(
        "--example",
        default="examples/StreamingExample",
        help="Repo-relative example directory.",
    )

    sp_monitor = sub.add_parser("monitor", help="Open a serial monitor.")
    sp_monitor.add_argument("--port", default=None, help="Serial port (optional).")
    sp_monitor.add_argument("--baud", default="115200", help="Baud rate.")

    args, unknown = parser.parse_known_args(argv)

    root = _project_root()
    env = os.environ.copy()
    env.setdefault("PLATFORMIO_CORE_DIR", str((root / ".platformio").resolve()))

    if args.command in ("build", "clean", "upload", "flash", "rebuild"):
        env["PLATFORMIO_SRC_DIR"] = str(_example_src_dir(root, args.example))
        env["PLATFORMIO_BUILD_DIR"] = str(_example_build_dir(root, args.example))

    if args.command == "build":
        return _run(["pio", "run", "-e", args.env, *unknown], cwd=root, env=env)

    if args.command == "clean":
        return _run(["pio", "run", "-e", args.env, "-t", "clean", *unknown], cwd=root, env=env)

    if args.command == "upload":
        cmd = ["pio", "run", "-e", args.env, "-t", "upload"]
        if args.port:
            cmd += ["--upload-port", args.port]
        cmd += unknown
        return _run(cmd, cwd=root, env=env)

    if args.command == "flash":
        rc = _run(["pio", "run", "-e", args.env, "-t", "clean", *unknown], cwd=root, env=env)
        if rc != 0:
            return rc
        cmd = ["pio", "run", "-e", args.env, "-t", "upload"]
        if args.port:
            cmd += ["--upload-port", args.port]
        cmd += unknown
        return _run(cmd, cwd=root, env=env)

    if args.command == "rebuild":
        rc = _run(["pio", "run", "-e", args.env, "-t", "clean", *unknown], cwd=root, env=env)
        if rc != 0:
            return rc
        return _run(["pio", "run", "-e", args.env, *unknown], cwd=root, env=env)

    if args.command == "deepclean":
        build_dir = _example_build_dir(root, args.example)
        print(f"+ rm -rf {build_dir}")
        shutil.rmtree(build_dir, ignore_errors=True)
        return 0

    if args.command == "monitor":
        cmd = ["pio", "device", "monitor"]
        if args.port:
            cmd += ["--port", args.port]
        if args.baud:
            cmd += ["--baud", str(args.baud)]
        cmd += unknown
        return _run(cmd, cwd=root, env=env)

    parser.error("Unknown command")
    return 2


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
