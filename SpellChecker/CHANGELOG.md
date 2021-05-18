# Changelog
Changelog of project Nuspell.

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [4.0.1] - 2020-11-02
### Fixed
- Fix handling CRLF line endings in .dic. Fixes #89.
- Fix CMake scripts for unusual install prefixes.
- Improve hash-table insertion, avoid temporaries. This improves loading times.

### Changed
- Switch man-page generator from Ronn to Pandoc. This changes the dependencies
  for building. See the README.

## [4.0.0] - 2020-10-19
### Fixed
- Fix lifetime issue (segmentation fault) in ngram-based suggestions. Fixes #84.
- Fix internal usage of `std::codecvt` in edge case in function `to_wide()` and
  `to_narrow()`
- Fix not propagating recursion depth in spell_break(), which caused hangup.
  Fixes #85.
- Fix overwriting of PDB files when compiling with Visual Studio. This prevented
  debugging nuspell.dll. Fixes #86.
- Fix output of CLI tool to be more compatible to ispell and hunspell.

### Changed
- Boost is no longer a dependency of the library at all. It is still a
  dependency of the CLI tool. Closes #79. Fixes #71.
- Use `std::string_view` in the public API instead of `const std::string&`. Also
  use it internally at various places.
- Build as shared library by default. Building as static is still possible via
  the CMake's variable `BUILD_SHARED_LIBS`.
- The public API is annotated with dll-export/visibility attributes, and also
  some private functions are annotated needed for testing. This reduces the file
  size. Closes #65.
- Use Unicode text segmentation by default in the CLI tool. Simple segmentation
  on white-space is still available via CLI option.

### Removed
- Delete deprecated functions in the class Finder.
- Drop under-designed and undocumented feature in the CLI tool about personal
  dictionaries.

## [3.1.2] - 2020-07-01
### Fixed
- Internal refactorings that reduce function templates in `utils` and `finder`.

### Changed
- File `utils.hxx` is not installed any more. It is a private header.

### Deprecated
- Some functions for searching dictionaries in class `Finder` are deprecated as
  they were searching in directories that no longer contain dictionaries.

## [3.1.1] - 2020-05-04
### Changed
- Updated description in README. Packagers are encouraged to update it in their
  packages.
- Moved Hunspell files from the directory "src" into the directory named
  "external" to clarify that it is third-party dependency. Previously, some
  packagers confused Hunspell's license notices as part of Nuspell. Hunspell is
  used only for testing and it is not part of the main program.

### Fixed
- Fixed bugs on FreeBSD and other BSDs related to encoding conversions. Now
  all tests pass on FreeBSD and the support for it is not experimental anymore.
- Fixed compiling on NetBSD.

## [3.1.0] - 2020-04-07
### Added
- Add so called hidden homonym feature. This feature enables words in mixed case
  in the dictionary to be accepted when met in uppercase in the input text. Also
  enables uppercase word to get lowercase suffix, and the whole derived word be
  accepted in all uppercase in some input text.
- Improve suggestions to handle words in various casing.
- Add new complex method for giving suggestions based on ngram similarity of the
  wrong word with the words in the dictionary.

## [3.0.0] - 2019-11-23
### Added
- Added compounding features: CHECKCOMPOUNDREP, FORCEUCASE, COMPOUNDWORDMAX.
- Added compounding features specific only to Hungarian language:
  COMPOUNDROOT, COMPOUNDSYLLABLE, SYLLABLENUM. These three basically are
  extension to COMPOUNDWORDMAX.
- Added six new simple suggestion methods.

### Changed
- Building and using the library requires a compiler with C++17 support.
- The functions of the public API now accept strings encoded in UTF-8 by
  default. You should not call the function `imbue()` and you should not use
  `locale` and `codecvt` objects at all if you need UTF-8 strings. Use `imbue()`
  only if you need API that accepts strings in other encoding.

### Fixed
- Major improvement in speed. The best case is almost 3x faster than Hunspell,
  and the worst case is now matching and exceeding Hunspell's speed by a
  few percent. Previously, the worst case was usually triggered with incorrect
  words and was major bottleneck, it was slower than Hunspell.
- Fixed loading Dutch dictionary, a regression introduced in 2.3.0.

## [2.3.0] - 2019-08-08
### Added
- Support for macOS
- Support for building with MSVC on Windows
- Support for building with pre-installed Catch 2
- Continuous integration/testing for all three major operating systems
- In the CLI tool, Unicode text segmentation now can be combined with all modes.

### Changed
- In Cmake the exported target has namespace, e.g. Nuspell::nuspell

### Fixed
- Building from a tarball. Previously only a git clone worked.
- Small internal fixes in Unicode transformations on Windows (because wchar_t
  is 16 bits there).
- Major improvements in aff parser brings better error handling.

## [2.2.0] - 2019-03-19
### Added
- Added build System CMake. Supports building as shared library.

### Changed
- Public API changed again, last for v2:
    - `Dictionary::suggest()` return data inside simple `vector<string>`.
      `List_Strings` is not used anymore.
    - Constructors of class `Dictionary` like `Dictionary::load_from_path()`
      throw `Dictionary_Loading_Error` on error. Previously they were throwing
      `ios_base::failure`.
- Boost::Locale is not dependency of library Nuspell anymore. It is still a
  dependency of the CLI tool. The library depends directly on ICU. Internally,
  all string now are in Unicode (UTF-8 or UTF-32, it depends of the need).

### Removed
- Removed old Autotools build system.
- Removed `NOSUGGEST_MODE` in CLI tool. It was very similar to
  `MISSPELLED_WORDS_MODE`.
- Class `Finder` does not search for Myspell dictionaries on the file-system
  anymore.

### Fixed
- Support compiling with GCC 5. Previously GCC 7 was needed.
- Faster dictionary loading and better internal error handling when parsing a
  dictionary file.
- Faster spellchecking as a consequence of faster case classification, which
  in turn, is a consequence of all string being Unicode and directly using ICU.

## [2.1.0] - 2019-01-03
### Changed
- Public API classes are inside inline namespace v2
- `List_Strings<char>` is renamed to just `List_Strings`. Affects client code.

### Fixed
- Improve public API docs

## [2.0.0] - 2018-11-22
### Added
- First public release
- Spelling error detection (checking) is closely matching Hunspell
- Support for spelling error correction (suggestions)

[Unreleased]: https://github.com/nuspell/nuspell/compare/v4.0.1...HEAD
[4.0.1]: https://github.com/nuspell/nuspell/compare/v4.0.0...v4.0.1
[4.0.0]: https://github.com/nuspell/nuspell/compare/v3.1.2...v4.0.0
[3.1.2]: https://github.com/nuspell/nuspell/compare/v3.1.1...v3.1.2
[3.1.1]: https://github.com/nuspell/nuspell/compare/v3.1.0...v3.1.1
[3.1.0]: https://github.com/nuspell/nuspell/compare/v3.0.0...v3.1.0
[3.0.0]: https://github.com/nuspell/nuspell/compare/v2.3.0...v3.0.0
[2.3.0]: https://github.com/nuspell/nuspell/compare/v2.2.0...v2.3.0
[2.2.0]: https://github.com/nuspell/nuspell/compare/v2.1.0...v2.2.0
[2.1.0]: https://github.com/nuspell/nuspell/compare/v2.0.0...v2.1.0
[2.0.0]: https://github.com/nuspell/nuspell/compare/v1.6.2...v2.0.0
