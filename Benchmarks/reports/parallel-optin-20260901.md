# Native parallel batch mode

## Scope

The follow-up to the original `codex/parallel-optin-20260901` benchmark branch
adds a user-facing native batch mode to `bng_cpp`. The mode is opt-in and is
intended for independent model files:

```sh
bng_cpp --parallel 2 --parallel-dir ./parallel-output \
  model-a.bngl model-b.bngl
```

The existing single-model and multi-input sequential invocation paths are
unchanged unless `--parallel` (or its `--jobs` alias) is supplied.

## Mechanism

The parent process validates the inputs, creates one empty output root, and
stages each model into a numbered job directory. A bounded set of worker
threads launches one child `bng_cpp` process per job. Each child changes to its
job directory before execution, so generated `.net`, `.cdat`, `.gdat`, and
related files remain separate. The parent prints the output directory for each
job and returns failure if any child exits unsuccessfully.

This is process-level parallelism by design. It does not run multiple models
inside one native engine instance, whose graph and parser state is not
thread-safe. `--check` and `--verbose` are forwarded to every child. If
`--parallel-dir` is omitted, the output root is created under the platform
temporary directory and its path is printed on completion. Requested output
roots must be empty to prevent accidental overwrites.

The staged input is a private copy of the model file. Models that depend on
additional relative files should be prepared as self-contained model files or
run through the existing benchmark-side launcher, which has the same staging
boundary.

## Validation

The executable-level test `test_batch_runner` covers:

* two independent models producing `.net` files in separate job directories;
* private copies of both input model files; and
* propagation of a malformed child model failure while a valid sibling still
  completes.

Focused command:

```sh
cmake --build build-codex --target bng_cpp --parallel 4
cmake --build build-codex --target test_batch_runner --parallel 4
ctest --test-dir build-codex -R 'batch' --output-on-failure
```

Observed result: both focused tests passed on macOS. The native Release build
completed with the repository's existing SUNDIALS deprecation warnings.
