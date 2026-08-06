#!/usr/bin/env python3
"""
Find new publications that cite the core BioNetGen papers and are not yet
listed in _data/publications.yaml, so they can be triaged as candidates.

Requires PyYAML (not stdlib).

Usage:
    python3 scripts/citation_watch.py --bootstrap   # mark all current citations as seen, no issue body
    python3 scripts/citation_watch.py                # print/emit only genuinely new candidates
"""
import argparse
import difflib
import json
import re
import urllib.request
from pathlib import Path

import yaml

REPO_ROOT = Path(__file__).resolve().parent.parent
PUBLICATIONS_YAML = REPO_ROOT / "_data" / "publications.yaml"
SEEN_PATH = REPO_ROOT / "_data" / "citation_seen.json"

# OpenAlex work IDs for the core BioNetGen papers we track citations to.
CORE_PAPERS = {
    "W2163493106": "Blinov et al. 2004, BioNetGen: software for rule-based modeling",
    "W2303100661": "Harris et al. 2016, BioNetGen 2.2: advances in rule-based modeling",
    "W2175715045": "Faeder et al. 2009, Rule-Based Modeling of Biochemical Systems with BioNetGen",
}

OPENALEX_URL = (
    "https://api.openalex.org/works"
    "?filter=cites:{ids}"
    "&sort=publication_date:desc"
    "&per-page=100&cursor={cursor}"
    "&select=id,doi,title,display_name,publication_year,publication_date,primary_location,authorships,ids"
    "&mailto=bionetgen.help@gmail.com"
)

TITLE_SIMILARITY_THRESHOLD = 0.92


def normalize_title(title: str) -> str:
    return re.sub(r"[^a-z0-9]+", "", (title or "").lower())


def fetch_all_citing_works() -> list[dict]:
    ids = "|".join(CORE_PAPERS)
    results = []
    cursor = "*"
    while cursor:
        url = OPENALEX_URL.format(ids=ids, cursor=cursor)
        with urllib.request.urlopen(url, timeout=30) as resp:
            data = json.load(resp)
        results.extend(data["results"])
        cursor = data.get("meta", {}).get("next_cursor")
        if not data["results"]:
            break
    return results


def existing_identifiers_from_page() -> tuple[set, set, set]:
    """Return (pmids, dois, normalized_titles) already present in publications.yaml."""
    records = yaml.safe_load(PUBLICATIONS_YAML.read_text()) or []
    pmids = {str(r["pmid"]) for r in records if r.get("pmid")}
    dois = {r["doi"].lower() for r in records if r.get("doi")}
    titles = {normalize_title(r["title"]) for r in records if r.get("title")}
    return pmids, dois, titles


def load_seen() -> set:
    if SEEN_PATH.exists():
        return set(json.loads(SEEN_PATH.read_text()).get("seen_ids", []))
    return set()


def save_seen(ids: set) -> None:
    SEEN_PATH.parent.mkdir(exist_ok=True)
    SEEN_PATH.write_text(json.dumps({"seen_ids": sorted(ids)}, indent=2) + "\n")


def work_short_id(work: dict) -> str:
    return work["id"].rsplit("/", 1)[-1]


def dedupe_preprints(works: list[dict]) -> list[dict]:
    """Collapse preprint/published-version pairs that both cite BioNetGen,
    preferring the peer-reviewed (non-repository) version when present."""
    groups: list[list[dict]] = []
    for w in works:
        title_norm = normalize_title(w.get("display_name") or w.get("title") or "")
        placed = False
        for group in groups:
            group_title = normalize_title(group[0].get("display_name") or "")
            if difflib.SequenceMatcher(None, title_norm, group_title).ratio() >= TITLE_SIMILARITY_THRESHOLD:
                group.append(w)
                placed = True
                break
        if not placed:
            groups.append([w])

    deduped = []
    for group in groups:
        if len(group) == 1:
            deduped.append(group[0])
            continue
        non_repo = [
            w for w in group
            if ((w.get("primary_location") or {}).get("source") or {}).get("type") != "repository"
        ]
        deduped.append(non_repo[0] if non_repo else group[0])
    return deduped


def format_candidate(work: dict) -> str:
    year = work.get("publication_year", "?")
    venue = (work.get("primary_location") or {}).get("source") or {}
    journal = venue.get("display_name") or "venue unknown"
    authors = work.get("authorships", [])
    first_author = authors[0]["author"]["display_name"] if authors else "?"
    author_str = f"{first_author} et al." if len(authors) > 1 else first_author
    doi = work.get("doi")
    link = doi or work["id"]
    title = work.get("display_name") or work.get("title") or "(untitled)"
    return f"- [ ] **{title}** — {author_str}, *{journal}*, {year}. [{link}]({link})"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--bootstrap", action="store_true",
                         help="Mark all currently-citing works as seen without emitting candidates.")
    parser.add_argument("--out", type=Path, default=None,
                         help="Write the candidate list (markdown) to this file instead of stdout.")
    args = parser.parse_args()

    works = fetch_all_citing_works()
    works = dedupe_preprints(works)

    seen = load_seen()
    pmids, dois, titles = existing_identifiers_from_page()

    if args.bootstrap:
        all_ids = {work_short_id(w) for w in works}
        save_seen(all_ids | seen)
        print(f"Bootstrapped {len(all_ids)} citing works as seen. No candidates emitted.")
        return

    candidates = []
    for w in works:
        wid = work_short_id(w)
        if wid in seen:
            continue
        doi = (w.get("doi") or "").replace("https://doi.org/", "").lower()
        pmid = None
        wids = w.get("ids", {})
        if wids.get("pmid"):
            pmid = wids["pmid"].rsplit("/", 1)[-1]
        title_norm = normalize_title(w.get("display_name") or "")

        on_page = (doi and doi in dois) or (pmid and pmid in pmids) or any(
            difflib.SequenceMatcher(None, title_norm, t).ratio() >= TITLE_SIMILARITY_THRESHOLD
            for t in titles
        )
        if not on_page:
            candidates.append(w)

    new_seen = seen | {work_short_id(w) for w in works}
    save_seen(new_seen)

    if not candidates:
        body = "No new candidate citations since the last check."
    else:
        lines = [
            f"Found **{len(candidates)}** new work(s) citing a core BioNetGen paper that aren't yet in "
            f"[_data/publications.yaml](../blob/gh-pages/_data/publications.yaml). Review each: check the box "
            f"and add a properly formatted record (year, authors, title, journal, doi, system, and pmid/rulehub "
            f"if available) for anything that applies BioNetGen to a specific biological system; leave "
            f"unchecked (and close) items that are just tool/method citations.\n",
        ]
        lines += [format_candidate(w) for w in candidates]
        body = "\n".join(lines)

    if args.out:
        args.out.write_text(body + "\n")
    else:
        print(body)


if __name__ == "__main__":
    main()
