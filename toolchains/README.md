# Bundled MT7621 toolchain

`buildroot-mipsel-uclibc-gcc-6.4.0.tar.xz` is the relocatable host toolchain from the local Buildroot 2018.02.9 build (`output/host`). It contains GCC 6.4.0, binutils 2.29.1, a MIPS little-endian uClibc sysroot, and the Buildroot compiler wrapper. The archive is stored as a regular Git file so GitHub Actions can build from a normal checkout without Git LFS or another toolchain download.

The archive was created from `/home/akito/buildroot-2018.02.9/output/host`. The original tree's generated `/etc/mtab` and `/etc/resolv.conf` symlinks were omitted; neither is needed to compile U-Boot. The Buildroot source tree, including package sources and licenses, is available from [Buildroot 2018.02.9](https://buildroot.org/downloads/buildroot-2018.02.9.tar.gz). This archive contains binaries; see the corresponding upstream source packages for their license terms.

Check and extract from the repository root:

```sh
sha256sum -c toolchains/SHA256SUMS
mkdir -p /tmp/mt7621-toolchain
tar -xJf toolchains/buildroot-mipsel-uclibc-gcc-6.4.0.tar.xz -C /tmp/mt7621-toolchain
export LD_LIBRARY_PATH="/tmp/mt7621-toolchain/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
/tmp/mt7621-toolchain/usr/bin/mipsel-linux-gcc --version
```

The compiler prefix for `make` is `/tmp/mt7621-toolchain/usr/bin/mipsel-linux-`. The workflow extracts the archive into its runner's temporary directory and passes that prefix to every `make` invocation.

The compiler binaries retain the original Buildroot host library RUNPATH. Set `LD_LIBRARY_PATH` to the extracted `lib` directory so cc1 and its dependencies load the bundled MPFR, MPC and GMP libraries. The workflow exports this path to subsequent steps through `GITHUB_ENV` and compiles a small C probe immediately after extraction; `gcc --version` alone does not exercise cc1.
