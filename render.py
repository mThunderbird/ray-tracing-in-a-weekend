"""Build the ray tracer, save its PPM as a PNG, and open the picture.

PowerShell's > operator rewrites program output as UTF-16, which image
viewers reject. This script reads the program's raw stdout instead.

    python render.py
"""

import os
import struct
import subprocess
import sys
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parent
BUILD = ROOT / "build"
PPM_PATH = ROOT / "image.ppm"
PNG_PATH = ROOT / "image.png"


def cmake_build() -> None:
    if not (BUILD / "build.ninja").exists() and not (BUILD / "CMakeCache.txt").exists():
        subprocess.run(["cmake", "-S", str(ROOT), "-B", str(BUILD)], check=True)
    subprocess.run(["cmake", "--build", str(BUILD)], check=True)


def raytracer_path() -> Path:
    exe = BUILD / "raytracer.exe"
    if exe.exists():
        return exe
    exe = BUILD / "raytracer"
    if exe.exists():
        return exe
    sys.exit(f"Could not find the raytracer executable in {BUILD}")


def render_ppm() -> bytes:
    result = subprocess.run([raytracer_path()], stdout=subprocess.PIPE, check=True)
    PPM_PATH.write_bytes(result.stdout)
    return result.stdout


def _tokens(data: bytes):
    i = 0
    n = len(data)
    while i < n:
        c = data[i]
        if c in b" \t\r\n":
            i += 1
            continue
        if c == ord("#"):
            while i < n and data[i] not in b"\r\n":
                i += 1
            continue
        start = i
        while i < n and data[i] not in b" \t\r\n#":
            i += 1
        yield data[start:i]


def parse_ppm(data: bytes) -> tuple[int, int, bytes]:
    if data.startswith((b"\xff\xfe", b"\xfe\xff")):
        sys.exit("image.ppm is UTF-16. Run python render.py instead of redirecting in PowerShell.")

    tokens = _tokens(data)
    try:
        magic = next(tokens)
        width = int(next(tokens))
        height = int(next(tokens))
        maxval = int(next(tokens))
    except (StopIteration, ValueError):
        sys.exit("raytracer stdout is not a PPM image")

    if magic != b"P3":
        sys.exit(f"Unsupported PPM type {magic!r}. This project writes text P3 images.")

    count = width * height * 3
    try:
        values = [int(next(tokens)) for _ in range(count)]
    except (StopIteration, ValueError):
        sys.exit("PPM ended before all pixels were read")

    if maxval != 255:
        values = [min(255, v * 255 // maxval) for v in values]
    return width, height, bytes(values)


def write_png(width: int, height: int, rgb: bytes) -> None:
    raw = bytearray()
    row = width * 3
    for y in range(height):
        raw.append(0)
        raw.extend(rgb[y * row : (y + 1) * row])

    def chunk(tag: bytes, payload: bytes) -> bytes:
        return (
            struct.pack(">I", len(payload))
            + tag
            + payload
            + struct.pack(">I", zlib.crc32(tag + payload) & 0xFFFFFFFF)
        )

    png = b"\x89PNG\r\n\x1a\n"
    png += chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(bytes(raw), 9))
    png += chunk(b"IEND", b"")
    temporary = PNG_PATH.with_suffix(".png.tmp")
    temporary.write_bytes(png)
    try:
        temporary.replace(PNG_PATH)
    except OSError:
        temporary.unlink(missing_ok=True)
        sys.exit(f"Close {PNG_PATH.name} in the editor or image viewer, then run this again.")


def main() -> None:
    os.chdir(ROOT)
    cmake_build()
    width, height, rgb = parse_ppm(render_ppm())
    write_png(width, height, rgb)
    print(f"Wrote {PNG_PATH.name} ({width}x{height})")
    os.startfile(PNG_PATH)


if __name__ == "__main__":
    main()
