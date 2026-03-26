# Changelog

## 6.2.3

- Add a native host test target so core formatting behavior can be checked without hardware.
- Add Pixi-managed PlatformIO and release-check tooling for repeatable local builds.
- Refresh README and package metadata to document current examples and workflows.
- Fix `_WIDTH` digit counting for minimum signed integer values.
- Avoid by-value copies in the comma streaming operator.
- Add a dedicated formatting helper example for `_FMT`, `_WIDTHZ`, and `_FLOATW`.
- Add GitHub Actions coverage for the native host test target.
- Add a shared `.clang-format` and Pixi formatting tasks for source, tests, and examples.

## 6.2.0

- Add comma-operator streaming support.
