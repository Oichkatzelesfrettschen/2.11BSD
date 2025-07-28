# License Audit for 2.11BSD Repository

This document summarizes the license-related material identified in the repository.

## Root of Repository

No dedicated LICENSE or COPYING file exists at the repository root. Typical BSD tools are provided as standalone binaries or archives. No files with SPDX license tags were found.

```
find . -iname '*license*' -o -iname '*copying*' -type f
```
produced no matches.

## BSD-Style Licenses

Many system source files contain the classic 4-clause BSD license header referencing the Regents of the University of California. Example:

- [`usr/sys/sys/kern_prot2.c`](usr/sys/sys/kern_prot2.c)【F:usr/sys/sys/kern_prot2.c†L1-L30】

Other kernel and system files follow a similar pattern.

## Digital Equipment Corporation License

Files such as [`usr/sys/pdp/mch_fpsim.s`](usr/sys/pdp/mch_fpsim.s) contain a copyright and license notice from Digital Equipment Corporation:

```
Licensed from Digital Equipment Corporation
...
```

## Third Party / Custom Notices

- [`usr/new/jove/extend.c`](usr/new/jove/extend.c) provides a custom notice from Jonathan Payne allowing redistribution with the notice intact.
- [`usr/new/kermit5.188/ckcmai.c`](usr/new/kermit5.188/ckcmai.c) includes a lengthy license statement from Columbia University.
- [`usr/new/rcs/src/partime.c`](usr/new/rcs/src/partime.c) is under a quasi-public license prohibiting sale or use in licensed software without permission.

## Carnegie Mellon References

Comments referencing Carnegie Mellon are present, but no explicit license is provided. Example:

- [`usr/bin/tcsh/tc.os.c`](usr/bin/tcsh/tc.os.c) contains historical comments mentioning "Carnegie-Mellon University" but not a license.

## Absence of SPDX Tags

A repository-wide search found no files containing `SPDX-License-Identifier` markers:

```
grep -r "SPDX" -n
```

returned no results.

## Project Structure

High-level directories under `usr` include:

```
usr/adm
usr/bin
usr/crash
usr/dict
usr/doc
usr/etc
usr/games
usr/hosts
usr/include
usr/ingres
usr/lib
usr/libdata
usr/libexec
usr/local
usr/man
usr/msgs
usr/new
usr/old
usr/pub
```

This structure contains system binaries, libraries, include files, and various utilities.

## Conclusion

The repository primarily contains 4-clause BSD-style licenses and a variety of vendor-specific notices. No dedicated license file or SPDX headers were found.

## Search Methodology

All regular files under version control were scanned recursively using `grep` for license-related keywords and references to Carnegie Mellon University. The archived sets in `file6.tar.gz`, `file7.tar.gz`, and `file8.tar.gz` were extracted to `/tmp/decomp` and scanned in the same manner. Disk images `root.afio.gz` and `root.dump` could not be examined because extraction utilities (`afio`, `restore`) are unavailable in the environment.

## Files Mentioning Carnegie Mellon

The following files contain references to Carnegie‑Mellon University in comments or documentation. None of them include an explicit license granted by CMU:

- `usr/bin/tcsh/tc.os.c` – historical comment crediting Richard Draves at CMU.
- `usr/doc/usd/31.bib/bibdoc.ms`
- `usr/doc/usd/28.tbl/tbl`
- `usr/doc/smm/14.fastfs/6.t`
- `usr/doc/ps2/09.lisp/ch0.n`
- `usr/doc/ps2/09.lisp/ch15.n`
- `usr/doc/misc/gprof/refs.me`
- `usr/new/rcs/doc/rcs.ms`
- `usr/man/man0/preface.ms`

## Top-Level Directory Layout

```
.
├── boot
├── HOWTO
├── README
├── VERSION
├── usr/
└── ... (build scripts and disk images)
```

The `usr` directory expands into the traditional BSD hierarchy with subdirectories such as `bin`, `lib`, `src`, and `doc`.

