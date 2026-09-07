#!/usr/bin/env python3
"""Run independent BioNetGen models in isolated processes.

This is intentionally a process-level, opt-in launcher.  Native BioNetGen
execution has mutable/static graph state, so models are not run concurrently
inside one bng_cpp process.  Each job receives its own working directory and
therefore keeps the generated .net/.cdat/.gdat/etc. files independent.
"""

import argparse
import concurrent.futures
import hashlib
import json
import os
import pathlib
import shutil
import subprocess
import sys
import tempfile
import time

try:
    import resource
except ImportError:  # pragma: no cover - Windows has no resource module
    resource = None


EXCLUDED_FILES = {"manifest.json", "stdout.log", "stderr.log"}


def sha256(path):
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for block in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def run_job(index, executable, model, output_root, bng_args, timeout):
    model_path = pathlib.Path(model).resolve()
    job_dir = output_root / f"{index:03d}-{model_path.stem}"
    job_dir.mkdir()
    local_model = job_dir / model_path.name
    shutil.copy2(model_path, local_model)
    command = [executable, *bng_args, str(local_model)]

    started = time.perf_counter()
    try:
        completed = subprocess.run(
            command,
            cwd=job_dir,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=timeout,
        )
        returncode = completed.returncode
        stdout = completed.stdout
        stderr = completed.stderr
    except subprocess.TimeoutExpired as exc:
        returncode = 124
        stdout = exc.stdout or ""
        stderr = (exc.stderr or "") + "\nparallel batch timeout\n"

    elapsed = time.perf_counter() - started
    (job_dir / "stdout.log").write_text(stdout)
    (job_dir / "stderr.log").write_text(stderr)

    files = sorted(
        path for path in job_dir.iterdir()
        if path.is_file() and path.name != local_model.name and path.name not in EXCLUDED_FILES
    )
    return {
        "index": index,
        "model": str(model_path),
        "model_sha256": sha256(model_path),
        "command": command,
        "returncode": returncode,
        "wall_seconds": elapsed,
        "output_root": str(job_dir),
        "output_hashes": {path.name: sha256(path) for path in files},
        "output_sizes": {path.name: path.stat().st_size for path in files},
        "output_bytes": sum(path.stat().st_size for path in files),
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--executable", required=True, help="bng_cpp executable")
    parser.add_argument("--model", action="append", required=True, dest="models")
    parser.add_argument("--jobs", type=int, default=1, help="maximum concurrent child processes")
    parser.add_argument("--output-root", help="empty directory for isolated job outputs")
    parser.add_argument("--timeout", type=float, default=300.0)
    parser.add_argument(
        "--bng-arg",
        action="append",
        default=[],
        help="argument passed to every bng_cpp invocation (repeatable)",
    )
    args = parser.parse_args()

    if args.jobs < 1:
        parser.error("--jobs must be at least 1")

    executable = str(pathlib.Path(args.executable).resolve())
    if not os.access(executable, os.X_OK):
        parser.error(f"executable is not runnable: {executable}")

    models = [str(pathlib.Path(model).resolve()) for model in args.models]
    for model in models:
        if not pathlib.Path(model).is_file():
            parser.error(f"model does not exist: {model}")

    temporary_root = args.output_root is None
    output_root = pathlib.Path(args.output_root).resolve() if args.output_root else pathlib.Path(
        tempfile.mkdtemp(prefix="bng-parallel-")
    )
    output_root.mkdir(parents=True, exist_ok=True)
    if any(output_root.iterdir()):
        parser.error(f"output directory must be empty: {output_root}")

    started = time.perf_counter()
    before = resource.getrusage(resource.RUSAGE_CHILDREN) if resource else None
    with concurrent.futures.ThreadPoolExecutor(max_workers=min(args.jobs, len(models))) as pool:
        futures = [
            pool.submit(run_job, index, executable, model, output_root, args.bng_arg, args.timeout)
            for index, model in enumerate(models)
        ]
        results = [future.result() for future in futures]
    after = resource.getrusage(resource.RUSAGE_CHILDREN) if resource else None

    manifest = {
        "executable": executable,
        "executable_sha256": sha256(pathlib.Path(executable)),
        "models": models,
        "jobs": args.jobs,
        "timeout_seconds": args.timeout,
        "bng_args": args.bng_arg,
        "wall_seconds": time.perf_counter() - started,
        "cpu_user_seconds": (after.ru_utime - before.ru_utime) if resource else None,
        "cpu_system_seconds": (after.ru_stime - before.ru_stime) if resource else None,
        "max_rss": after.ru_maxrss if resource else None,
        "temporary_output_root": temporary_root,
        "results": results,
    }
    (output_root / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n")
    print(json.dumps(manifest, indent=2))
    return 0 if all(result["returncode"] == 0 for result in results) else 1


if __name__ == "__main__":
    sys.exit(main())
