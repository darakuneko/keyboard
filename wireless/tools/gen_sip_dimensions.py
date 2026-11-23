#!/usr/bin/env python3
"""Dimension drawing and STL of the HY0020 SIP (HY0020 fitted) for people who design their own board.

  python tools/gen_sip_dimensions.py        (needs matplotlib and cadquery)
Writes hardware/hy0020_sip_dimensions.pdf and hardware/hy0020_sip.stl.

Frame (the same in both files): origin = centre of pin 1, X along the pin row towards pin 7, +Y towards
the HY0020 module, Z up from the board's back face (the face with the pin names, which lies on the host
board). The HY0020 is on the front face. Numbers come from the KiCad board (hy0020_sip v1.1) and the
HY0020 datasheet (module 10 x 3.5 x 1.0).
"""
import os
import textwrap

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.patches import Circle, Rectangle

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT_PDF = os.path.join(ROOT, "hardware", "hy0020_sip_dimensions.pdf")
OUT_STL = os.path.join(ROOT, "hardware", "hy0020_sip.stl")

BOARD = dict(x0=-1.27, x1=16.51, y0=-1.2, y1=6.0)          # 17.78 x 7.2
T = 1.6                                                    # board thickness
PITCH, PINS = 2.54, ["DFU", "RST", "IRQ", "RX", "TX", "GND", "3V3"]
HOLE_D, LAND_D = 0.85, 1.5
MODULE = dict(x0=5.73, x1=15.73, y0=1.65, y1=5.15)         # HY0020, 10 x 3.5, on the front face
MODULE_H = 1.0
SWD = {"DIO": (14.33, 2.9), "CLK": (14.33, 4.7)}            # 1.0 mm test pads on the back face

INK = "#1d1d1b"
DIM = "#2f6f4f"
FILL = "#e9eee9"
PART = "#c9d6cc"
NOTE = "#6b6b66"


def rect(ax, x0, y0, x1, y1, fc=PART, ec=INK, lw=0.8, ls="-", z=2, label=None, fs=6):
    ax.add_patch(Rectangle((x0, y0), x1 - x0, y1 - y0, fc=fc, ec=ec, lw=lw, ls=ls, zorder=z))
    if label:
        ax.text((x0 + x1) / 2, (y0 + y1) / 2, label, ha="center", va="center", fontsize=fs, color=INK, zorder=z + 1)


def dim(ax, p0, p1, off, text=None, fs=6.5, ext=True):
    """linear dimension between p0 and p1, drawn `off` away (perpendicular, + = left of p0->p1)"""
    p0, p1 = np.array(p0, float), np.array(p1, float)
    d = p1 - p0
    n = np.array([-d[1], d[0]]) / np.linalg.norm(d)
    a, b = p0 + n * off, p1 + n * off
    if ext:
        for p, q in ((p0, a), (p1, b)):
            e = q + n * 0.3 * np.sign(off)
            ax.plot([p[0], e[0]], [p[1], e[1]], color=DIM, lw=0.4, zorder=4)
    ax.annotate("", xy=b, xytext=a, arrowprops=dict(arrowstyle="<->", color=DIM, lw=0.6, shrinkA=0, shrinkB=0), zorder=4)
    t = text if text is not None else "%g" % round(float(np.linalg.norm(d)), 2)
    m = (a + b) / 2 + n * 0.45 * (1 if off >= 0 else -1)
    ang = np.degrees(np.arctan2(d[1], d[0]))
    if ang > 90 or ang <= -90:
        ang += 180
    ax.text(m[0], m[1], t, ha="center", va="center", rotation=ang, fontsize=fs, color=DIM, zorder=5,
            bbox=dict(fc="white", ec="none", pad=0.3))


def setup(ax, title, xlim, ylim, axes=("X", "Y")):
    ax.set_aspect("equal")
    ax.set_xlim(*xlim)
    ax.set_ylim(*ylim)
    ax.axis("off")
    ax.set_title(title, fontsize=10, loc="left", color=INK)
    ax.plot([xlim[0] + 0.4, xlim[0] + 1.6], [ylim[0] + 0.4, ylim[0] + 0.4], color=NOTE, lw=0.6)
    ax.plot([xlim[0] + 0.4, xlim[0] + 0.4], [ylim[0] + 0.4, ylim[0] + 1.6], color=NOTE, lw=0.6)
    ax.text(xlim[0] + 1.8, ylim[0] + 0.4, axes[0], fontsize=6, color=NOTE, va="center")
    ax.text(xlim[0] + 0.4, ylim[0] + 1.9, axes[1], fontsize=6, color=NOTE, ha="center")


def top_view(ax):
    b, m = BOARD, MODULE
    setup(ax, "Front view (X–Y, HY0020 side)", (-5.5, 21.5), (-5.2, 10.2))
    rect(ax, b["x0"], b["y0"], b["x1"], b["y1"], fc=FILL, lw=1.2, z=1)
    rect(ax, m["x0"], m["y0"], m["x1"], m["y1"], label="HY0020", fs=7)
    for i, name in enumerate(PINS):
        x = i * PITCH
        if i == 0:
            rect(ax, x - LAND_D / 2, -LAND_D / 2, x + LAND_D / 2, LAND_D / 2, fc="#d8c48a", lw=0.5, z=2)
        else:
            ax.add_patch(Circle((x, 0), LAND_D / 2, fc="#d8c48a", ec=INK, lw=0.5, zorder=2))
        ax.add_patch(Circle((x, 0), HOLE_D / 2, fc="white", ec=INK, lw=0.5, zorder=3))
        ax.text(x, -1.75, "%d\n%s" % (i + 1, name), ha="center", va="top", fontsize=5.5, color=INK, linespacing=1.2)
    for name, (x, y) in SWD.items():
        ax.add_patch(Circle((x, y), 0.5, fc="none", ec=NOTE, lw=0.6, ls="--", zorder=4))
        ax.text(b["x1"] + 0.4, y, "%s (back)" % name, fontsize=5.5, va="center", color=NOTE)
        ax.plot([x + 0.5, b["x1"] + 0.3], [y, y], color=NOTE, lw=0.4, zorder=4)
    ax.plot([0], [0], marker="+", color=DIM, ms=7, mew=0.8, zorder=6)
    ax.text(-0.35, 0.75, "origin", fontsize=5.5, color=DIM, ha="right")
    dim(ax, (b["x0"], b["y1"]), (b["x1"], b["y1"]), 2.6, "17.78")
    dim(ax, (m["x0"], m["y1"]), (m["x1"], m["y1"]), 1.2, "10")
    dim(ax, (b["x0"], b["y1"]), (m["x0"], b["y1"]), 1.2, "7")
    dim(ax, (b["x0"], b["y0"]), (b["x0"], b["y1"]), 1.6, "7.2")
    dim(ax, (b["x0"], b["y0"]), (b["x0"], 0), 0.6, "1.2", fs=5.5)
    dim(ax, (m["x1"], m["y0"]), (m["x1"], m["y1"]), -3.6, "3.5")
    dim(ax, (m["x1"], 0), (m["x1"], m["y0"]), -3.6, "1.65", fs=5.5)
    yl = -3.2                                           # below the pin names
    dim(ax, (0, yl), (PITCH, yl), -0.5, "2.54")
    dim(ax, (0, yl), (6 * PITCH, yl), -1.5, "15.24 (6 × 2.54)")
    dim(ax, (b["x0"], yl), (0, yl), -0.5, "1.27", fs=5.5)


def side_view(ax):
    b, m = BOARD, MODULE
    setup(ax, "Side view (Y–Z, seen from pin 7)", (-5.5, 21.5), (-3.2, 5.2), axes=("Y", "Z"))
    # horizontal = Y, vertical = Z; the host board is below Z 0
    ax.plot([-4, 10], [0, 0], color=NOTE, lw=0.6, ls="--")
    ax.text(10.3, 0, "host board (Z 0)", fontsize=5.5, va="center", color=NOTE)
    rect(ax, b["y0"], 0, b["y1"], T, fc=FILL, lw=1.2, label="board", fs=5.5)
    rect(ax, m["y0"], T, m["y1"], T + MODULE_H, label="HY0020", fs=5.5)
    ax.plot([0, 0], [-2.4, T + 1.6], color=INK, lw=1.0, zorder=3)
    ax.text(0, -2.75, "pin", fontsize=5.5, ha="center", color=INK)
    dim(ax, (b["y1"] + 0.6, 0), (b["y1"] + 0.6, T), -0.8, "1.6")
    dim(ax, (b["y1"] + 2.6, 0), (b["y1"] + 2.6, T + MODULE_H), -0.8, "2.6")
    dim(ax, (b["y0"], T + MODULE_H), (b["y1"], T + MODULE_H), 1.2, "7.2")


def notes(ax):
    ax.axis("off")
    rows = [
        ("Origin", "centre of pin 1, X towards pin 7, +Y towards the HY0020, Z = back face of the board (0)"),
        ("STL", "hy0020_sip.stl: same frame (mm), HY0020 fitted, no pins"),
        ("Board", "17.78 × 7.2 × 1.6. With the HY0020: 2.6 high"),
        ("Pins", "1 × 7, 2.54 pitch. Holes φ0.85, lands φ1.5 (pin 1 square).\n"
                 "1 DFU (P0.20) / 2 RST (P0.21) / 3 IRQ (P0.07) / 4 RX (P0.08) / 5 TX (P0.06) / 6 GND / 7 3V3"),
        ("HY0020", "10 × 3.5 × 1.0 on the front face, X 5.73 … 15.73, Y 1.65 … 5.15"),
        ("SWD pads", "back face, φ1.0: DIO at (14.33, 2.9), CLK at (14.33, 4.7).\n"
                     "They face the host board: keep copper out under them"),
        ("Host board", "holes for the thin pin header: φ0.8, lands φ1.7.\n"
                       "The SIP lies flat on the host board, back face down"),
        ("Antenna", "the antenna is on the HY0020 itself. Keep metal and copper pours away from the module;\n"
                    "let it reach over the edge of the host board if possible (see the HY0020 application note)"),
        ("Supply", "3V3: 1.7 – 3.6 V. Put a 100 nF capacitor next to pin 7 on the host board (the SIP has none)"),
    ]
    y = 1.0
    for k, v in rows:
        lines = [l for part in v.split("\n") for l in textwrap.wrap(part, 54)]
        ax.text(0.0, y, k, fontsize=7, va="top", color=INK, weight="bold", transform=ax.transAxes)
        ax.text(0.0, y - 0.026, "\n".join(lines), fontsize=6.5, va="top", color=INK, linespacing=1.35,
                transform=ax.transAxes)
        y -= 0.05 + 0.024 * len(lines)
    ax.text(0.0, y - 0.02, "All dimensions in mm. HY0020 SIP v1.1.", fontsize=6, color=NOTE,
            va="top", transform=ax.transAxes)


def write_stl():
    import cadquery as cq
    b, m = BOARD, MODULE
    board = (cq.Workplane("XY").box(b["x1"] - b["x0"], b["y1"] - b["y0"], T, centered=False)
             .translate((b["x0"], b["y0"], 0)))
    holes = cq.Workplane("XY").pushPoints([(i * PITCH, 0) for i in range(len(PINS))]).circle(HOLE_D / 2).extrude(T)
    module = (cq.Workplane("XY").box(m["x1"] - m["x0"], m["y1"] - m["y0"], MODULE_H, centered=False)
              .translate((m["x0"], m["y0"], T)))
    cq.exporters.export(board.cut(holes).union(module), OUT_STL, tolerance=0.02, angularTolerance=0.2)
    print("wrote", OUT_STL)


def main():
    os.makedirs(os.path.dirname(OUT_PDF), exist_ok=True)
    plt.rcParams["font.family"] = ["Hiragino Sans", "Arial Unicode MS", "DejaVu Sans"]
    with PdfPages(OUT_PDF) as pdf:
        fig = plt.figure(figsize=(11.69, 8.27))                       # A4 landscape
        fig.suptitle("HY0020 SIP v1.1 — dimensions", x=0.04, ha="left", fontsize=13, color=INK)
        top_view(fig.add_axes([0.02, 0.36, 0.60, 0.56]))
        side_view(fig.add_axes([0.02, 0.03, 0.60, 0.30]))
        notes(fig.add_axes([0.64, 0.06, 0.34, 0.84]))
        pdf.savefig(fig)
        plt.close(fig)
    print("wrote", OUT_PDF)
    write_stl()


if __name__ == "__main__":
    main()
