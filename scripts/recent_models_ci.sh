#!/usr/bin/env bash
#
# CI driver for regenerating recent-models.md.
#
# This is where ALL the recent-models logic lives. The workflow that invokes
# it, .github/workflows/recent-models.yml, is a thin wrapper that only checks
# out the branch and sets up Python.
#
# Unlike citation-watch, this workflow belongs on gh-pages rather than master:
# it triggers on pushes to gh-pages, and push events run the workflow file from
# the branch that was pushed. master used to carry a second copy that could
# never fire, and the two drifted -- master's had fixes (an existence guard,
# and `git add -A` so deletions are staged) that the copy actually running here
# lacked. That copy is gone; this script is the one place to edit.
set -euo pipefail

cd "$(dirname "$0")/.."

sudo apt-get update
sudo apt-get install -y graphviz

pip install bionetgen networkx pyyaml

python3 scripts/generate_recent_models.py

git config user.name "github-actions[bot]"
git config user.email "github-actions[bot]@users.noreply.github.com"
git add recent-models.md
# -A so regenerated diagrams that disappear are staged as deletions too.
if [ -e assets/recent-models ]; then
  git add -A assets/recent-models
fi
if git diff --cached --quiet; then
  echo "No change to recent-models.md."
  exit 0
fi
git commit -m "Regenerate recent-models.md [skip ci]"
git push origin HEAD:gh-pages
