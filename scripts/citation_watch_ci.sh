#!/usr/bin/env bash
#
# CI driver for the monthly citation watch.
#
# This is where ALL the citation-watch logic lives. The workflow that invokes
# it, .github/workflows/citation-watch.yml, exists only on master (GitHub fires
# `schedule` solely from the default branch) and is a thin wrapper: it checks
# out gh-pages, sets up Python, and runs this script. Keeping the logic here
# means there is exactly one copy to edit, on the same branch as the data it
# operates on -- the two branches drifting is what silently broke the
# 2026-09-01 scheduled run (master's copy was missing `pip install pyyaml`).
#
# Edit this file, not the workflow.
#
# Expects: GH_TOKEN in the environment, and a gh-pages checkout as cwd.
set -euo pipefail

cd "$(dirname "$0")/.."

pip install pyyaml

python3 scripts/citation_watch.py --out candidates.md

gh label create "citation-candidates" --color "0e8a16" \
  --description "New BioNetGen citations awaiting triage for published.md" || true

if grep -q "No new candidate citations" candidates.md; then
  echo "Nothing new, skipping issue creation."
else
  gh issue create \
    --title "New BioNetGen citations to review - $(date +%Y-%m-%d)" \
    --label "citation-candidates" \
    --body-file candidates.md
fi

git config user.name "github-actions[bot]"
git config user.email "github-actions[bot]@users.noreply.github.com"
git add _data/citation_seen.json
if git diff --cached --quiet; then
  echo "No change to seen-list."
  exit 0
fi
git commit -m "Update citation_seen.json [skip ci]"
git push origin HEAD:gh-pages
