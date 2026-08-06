#!/usr/bin/env python3
"""
Generate recent-models.md: a richer showcase of the N most recent entries in
_data/publications.yaml, each with a short citation, a link to its RuleHub
model (when one can be matched), and an auto-generated contact-map diagram
for matched models.

Requires (not stdlib): PyYAML, the `bionetgen` package (PyBioNetGen),
`networkx`, and the graphviz `dot` binary on PATH. RuleHub is fetched via a
shallow git clone into a temp dir unless RULEHUB_ROOT is set to an existing
checkout.

Usage:
    python3 scripts/generate_recent_models.py [--count 10] [--rulehub-root PATH]
"""
import argparse
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import yaml

REPO_ROOT = Path(__file__).resolve().parent.parent
PUBLICATIONS_YAML = REPO_ROOT / "_data" / "publications.yaml"
OUTPUT_MD = REPO_ROOT / "recent-models.md"
IMAGE_DIR = REPO_ROOT / "assets" / "recent-models"
RULEHUB_URL = "https://github.com/RuleWorld/RuleHub.git"


def parse_published_entries():
    records = yaml.safe_load(PUBLICATIONS_YAML.read_text()) or []
    # publications.yaml is already sorted newest-first.
    entries = []
    for r in records:
        surname = re.split(r"[\s,]", r["authors"].rstrip("."), maxsplit=1)[0]
        entries.append({
            "authors": r["authors"],
            "title": r["title"],
            "doi": r["doi"],
            "journal": r["journal"],
            "year": str(r["year"]),
            "surname": surname,
            "rulehub": r.get("rulehub"),
        })
    return entries


def ensure_rulehub(root: Path | None) -> Path:
    if root and root.exists():
        return root
    tmp = Path(tempfile.mkdtemp(prefix="rulehub_"))
    subprocess.run(
        ["git", "clone", "--depth", "1", RULEHUB_URL, str(tmp)],
        check=True, capture_output=True,
    )
    return tmp


def match_rulehub_dir(entry, published_dirs):
    surname = entry["surname"].lower()
    year = entry["year"]
    candidates = [d for d in published_dirs if surname in d.name.lower()]
    if not candidates:
        return None, "no candidate"

    with_year = [d for d in candidates if year in d.name]
    if len(with_year) == 1:
        return with_year[0], "surname+year in dirname"

    if len(candidates) == 1:
        # Cross-check year against metadata.yaml `name` field or README title.
        d = candidates[0]
        for fname in ("metadata.yaml", "README.md"):
            f = d / fname
            if f.exists() and year in f.read_text(errors="ignore"):
                return d, f"surname in dirname, year confirmed in {fname}"
        return None, f"surname-only match '{d.name}', year not confirmed — needs manual review"

    # Multiple ambiguous candidates: pick best title-similarity match if any
    # README clearly contains the year, else give up.
    year_matches = []
    for d in candidates:
        readme = d / "README.md"
        if readme.exists() and year in readme.read_text(errors="ignore"):
            year_matches.append(d)
    if len(year_matches) == 1:
        return year_matches[0], "disambiguated by year in README"
    return None, f"ambiguous ({len(candidates)} candidates) — needs manual review"


def find_bngl_file(model_dir: Path):
    bngl_files = list(model_dir.glob("*.bngl"))
    if len(bngl_files) == 1:
        return bngl_files[0]
    if len(bngl_files) > 1:
        # Prefer a file whose stem resembles the directory name.
        for f in bngl_files:
            if f.stem.lower().replace("_", "") in model_dir.name.lower():
                return f
        return bngl_files[0]
    return None


def graphml_to_svg(graphml_path: Path, svg_path: Path):
    import networkx as nx

    g = nx.read_graphml(graphml_path)
    lines = [
        "graph G {",
        "  overlap=false;",
        "  splines=true;",
        '  node [shape=ellipse, style=filled, fillcolor="#dbeafe", '
        'color="#3b82f6", fontname="Helvetica", fontsize=11];',
        '  edge [color="#94a3b8"];',
    ]
    for nid, data in g.nodes(data=True):
        label = data.get("label")
        if not label:
            continue
        safe = label.replace('"', "'")
        lines.append(f'  "{nid}" [label="{safe}"];')
    labeled = {n for n, d in g.nodes(data=True) if d.get("label")}
    for u, v in g.edges():
        if u in labeled and v in labeled:
            lines.append(f'  "{u}" -- "{v}";')
    lines.append("}")

    dot_path = svg_path.with_suffix(".dot")
    dot_path.write_text("\n".join(lines))
    subprocess.run(
        ["dot", "-Tsvg", "-Kneato", str(dot_path), "-o", str(svg_path)],
        check=True, capture_output=True,
    )
    dot_path.unlink()


def generate_contact_map(bngl_file: Path, out_stem: str, workdir: Path):
    subprocess.run(
        ["bionetgen", "visualize", "-i", str(bngl_file), "-t", "contactmap", "-o", str(workdir)],
        check=True, capture_output=True, text=True,
    )
    graphml_candidates = list(workdir.glob("*_contactmap.graphml"))
    if not graphml_candidates:
        return None
    svg_path = IMAGE_DIR / f"{out_stem}.svg"
    IMAGE_DIR.mkdir(parents=True, exist_ok=True)
    graphml_to_svg(graphml_candidates[0], svg_path)
    return svg_path


def render_page(rows, count):
    lines = [
        "---",
        "title: Recent Models",
        "layout: default",
        "permalink: /recent-models",
        "parent: Uses of BioNetGen",
        "nav_order: 3",
        "---",
        "",
        "# Recent BioNetGen Models",
        "",
        f"A closer look at the {count} most recent publications from "
        "[Published BioNetGen Models](applications), with a link to the model "
        "code in [RuleHub](https://github.com/RuleWorld/RuleHub) where one has "
        "been curated. *Auto-generated — do not edit by hand.*",
        "",
    ]
    for r in rows:
        lines.append(f"## {r['title']}")
        lines.append("")
        lines.append(
            f"*{r['authors']} — {r['journal']}, {r['year']}. "
            f"[Read the paper](https://doi.org/{r['doi']}).*"
        )
        lines.append("")
        if r.get("image_path"):
            rel = r["image_path"].relative_to(REPO_ROOT)
            lines.append(f"![Contact map for {r['title']}](/{rel})")
            lines.append("")
        if r.get("rulehub_path"):
            rh_rel = r["rulehub_path"].name
            lines.append(
                f"[View model code on RuleHub](https://github.com/RuleWorld/RuleHub/tree/main/Published/{rh_rel})"
            )
        else:
            lines.append(
                "*Not yet archived in RuleHub. "
                "[Help add it](https://github.com/RuleWorld/RuleHub/blob/main/AddingModels.md).*"
            )
        lines.append("")
    OUTPUT_MD.write_text("\n".join(lines))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--count", type=int, default=10)
    parser.add_argument("--rulehub-root", type=Path, default=None)
    args = parser.parse_args()

    entries = parse_published_entries()[: args.count]
    rulehub_root = ensure_rulehub(args.rulehub_root)
    published_dirs = [
        d for d in (rulehub_root / "Published").iterdir()
        if d.is_dir() and not d.name.startswith(".")
    ]

    dirs_by_name = {d.name: d for d in published_dirs}

    rows = []
    review_needed = []
    with tempfile.TemporaryDirectory() as tmpdir:
        for entry in entries:
            row = dict(entry)
            if entry.get("rulehub") and entry["rulehub"] in dirs_by_name:
                match, reason = dirs_by_name[entry["rulehub"]], "cached in publications.yaml"
            else:
                match, reason = match_rulehub_dir(entry, published_dirs)
            if reason and "needs manual review" in reason:
                review_needed.append((entry["title"], reason))
            if match:
                row["rulehub_path"] = match
                bngl = find_bngl_file(match)
                if bngl:
                    try:
                        svg = generate_contact_map(bngl, match.name, Path(tmpdir))
                        if svg:
                            row["image_path"] = svg
                    except subprocess.CalledProcessError as e:
                        print(f"WARNING: visualize failed for {match.name}: {e.stderr}", file=sys.stderr)
            rows.append(row)

    render_page(rows, len(rows))
    matched = sum(1 for r in rows if r.get("rulehub_path"))
    imaged = sum(1 for r in rows if r.get("image_path"))
    print(f"Wrote {OUTPUT_MD} — {matched}/{len(rows)} matched to RuleHub, {imaged} with a generated diagram.")
    if review_needed:
        print("\nCandidates needing manual review (not auto-linked):")
        for title, reason in review_needed:
            print(f"  - {title}: {reason}")

    if args.rulehub_root is None:
        shutil.rmtree(rulehub_root, ignore_errors=True)


if __name__ == "__main__":
    main()
