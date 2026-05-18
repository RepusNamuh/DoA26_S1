"""
visualize_data.py
─────────────────
Reads test.txt, parses experiment results, and produces two heatmaps:
  • False Positives (FP)
  • True Negatives  (TN)

Usage:
    python visualize_data.py                  # reads test.txt in current dir
    python visualize_data.py path/to/test.txt # custom path
"""

import sys
import re
import pandas as pd
import matplotlib.pyplot as plt


# ── Config ────────────────────────────────────────────────────────────────────

DEFAULT_INPUT  = "test.txt"
OUTPUT_IMAGE   = "fp_tn_heatmaps.png"
FIGURE_SIZE    = (18, 7)
DPI            = 150
COLORMAP_FP    = "RdYlGn_r"   # red = high FP (bad)
COLORMAP_TN    = "RdYlGn"     # green = high TN (good)
ANNOTATION_FONTSIZE = 7


# ── Parsing ───────────────────────────────────────────────────────────────────

def parse_file(filepath: str) -> pd.DataFrame:
    """Read and parse the raw text file into a DataFrame."""
    with open(filepath, "r") as fh:
        text = fh.read()

    records = []
    blocks = re.split(r"\n{2,}", text.strip())
    for block in blocks:
        lines = [l.strip() for l in block.splitlines() if l.strip()]
        if len(lines) < 5:
            continue
        try:
            n_tables = int(re.search(r"=\s*(\d+)", lines[0]).group(1))
            size     = int(re.search(r"=\s*(\d+)", lines[1]).group(1))
            tp       = int(re.search(r":\s*(\d+)", lines[2]).group(1))
            fp       = int(re.search(r":\s*(\d+)", lines[3]).group(1))
            tn       = int(re.search(r":\s*(\d+)", lines[4]).group(1))
            records.append(dict(n_tables=n_tables, size=size, TP=tp, FP=fp, TN=tn))
        except (AttributeError, IndexError, ValueError):
            continue  # skip malformed blocks

    if not records:
        raise ValueError(f"No valid records found in '{filepath}'.")

    return pd.DataFrame(records)


# ── Pivot helpers ─────────────────────────────────────────────────────────────

def make_pivot(df: pd.DataFrame, value_col: str) -> pd.DataFrame:
    """Pivot the DataFrame so rows=n_tables, columns=size."""
    return df.pivot(index="n_tables", columns="size", values=value_col)


# ── Heatmap drawing ───────────────────────────────────────────────────────────

def draw_heatmap(ax, pivot: pd.DataFrame, title: str, cmap: str, vmin: float, vmax: float):
    """Render a single annotated heatmap onto *ax*."""
    im = ax.imshow(
        pivot.values,
        aspect="auto",
        cmap=cmap,
        vmin=vmin,
        vmax=vmax,
        origin="lower",
    )

    ax.set_xticks(range(len(pivot.columns)))
    ax.set_xticklabels(pivot.columns, rotation=45, ha="right")
    ax.set_yticks(range(len(pivot.index)))
    ax.set_yticklabels(pivot.index)
    ax.set_xlabel("Table Size", fontsize=11)
    ax.set_ylabel("Number of Tables", fontsize=11)
    ax.set_title(title, fontsize=13, fontweight="bold", pad=10)

    for row_i, row_val in enumerate(pivot.index):
        for col_i, col_val in enumerate(pivot.columns):
            val = pivot.loc[row_val, col_val]
            norm = (val - vmin) / (vmax - vmin) if (vmax - vmin) else 0
            txt_colour = "white" if norm < 0.25 or norm > 0.75 else "black"
            ax.text(
                col_i, row_i, f"{int(val):,}",
                ha="center", va="center",
                fontsize=ANNOTATION_FONTSIZE,
                color=txt_colour,
            )

    plt.colorbar(im, ax=ax, fraction=0.046, pad=0.04)


# ── Figure assembly ───────────────────────────────────────────────────────────

def build_figure(df: pd.DataFrame, output_path: str):
    """Create and save the dual-heatmap figure."""
    pivot_fp = make_pivot(df, "FP")
    pivot_tn = make_pivot(df, "TN")

    global_max = max(pivot_fp.values.max(), pivot_tn.values.max())

    fig, axes = plt.subplots(1, 2, figsize=FIGURE_SIZE)
    fig.suptitle(
        "FP and TN counts by Table Count × Table Size",
        fontsize=15, fontweight="bold", y=1.02,
    )

    draw_heatmap(axes[0], pivot_fp, "False Positives (FP)", COLORMAP_FP, 0, global_max)
    draw_heatmap(axes[1], pivot_tn, "True Negatives  (TN)", COLORMAP_TN, 0, global_max)

    plt.tight_layout()
    plt.savefig(output_path, dpi=DPI, bbox_inches="tight")
    plt.show()
    print(f"Saved → {output_path}")


# ── Entry point ───────────────────────────────────────────────────────────────

def main():
    input_path = sys.argv[1] if len(sys.argv) > 1 else DEFAULT_INPUT
    print(f"Reading: {input_path}")

    df = parse_file(input_path)
    print(f"Parsed {len(df)} records  |  "
          f"n_tables: {sorted(df['n_tables'].unique())}  |  "
          f"sizes: {sorted(df['size'].unique())}")

    build_figure(df, OUTPUT_IMAGE)


if __name__ == "__main__":
    main()