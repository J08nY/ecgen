# ![](docs/full.png)

[![Test](https://github.com/J08nY/ecgen/actions/workflows/test.yml/badge.svg)](https://github.com/J08nY/ecgen/actions/workflows/test.yml) [![GitHub release](https://img.shields.io/github/release/J08nY/ecgen.svg)](https://github.com/J08nY/ecgen/releases/latest) [![codecov](https://codecov.io/gh/J08nY/ecgen/branch/master/graph/badge.svg)](https://codecov.io/gh/J08nY/ecgen)

Tool for generating Elliptic curve domain parameters.

### Usage

	ecgen --fp/--f2m BITS

#### Field specification

 - `--f2m`					Binary field.
 - `--fp`					Prime field.

#### Generation methods

 - `--anomalous`            Generate an anomalous curve (of trace one, with field order equal to curve order).
 - `--supersingular`        Generate a supersingular curve.
 - `-i / --invalid`			Generate a set of invalid curves, for a given curve (using Invalid curve algorithm).
 - `-n / --order=ORDER`		Generate a curve with given `ORDER` (using Complex Multiplication), order can be a comma-separated(no spaces) list of factors.
 - `-s / --ansi[=SEED]`		Generate a curve from `SEED` (ANSI X9.62 verifiable procedure).
 - `-b / --brainpool[=SEED]`Generate a curve using the Brainpool verifiably pseudorandom algorithm from the original paper.
 - `--brainpool-rfc[=SEED]` Generate a curve using the Brainpool verifiably pseudorandom algorithm as per RFC 5639.
 - `-F / --family=FAMILY`   Generate a pairing friendly curve from a curve family (e.g. "BN", "BLS12", "BLS24").
 - `--nums`                 Generate a curve using the NUMS procedure (as per draft-black-numscurves-02).
 - `--twist`                Generate a twist of a given curve.

#### Generation options

 - `-c / --count=COUNT`		Generate multiple curves.
 - `-k / --cofactor=VALUE`	Generate a curve with cofactor of `VALUE`.
 - `-B / --smooth=BOUND`    Generate a smooth order curve with bit-lengths of factors bounded by `BOUND`.
 - `-K / --koblitz[=A]`		Generate a Koblitz curve (a in {0, 1}, b = 1).
 - `-p / --prime`			Generate a curve with prime order.
 - `--points=TYPE`			Generate points of given `TYPE` (random/prime/all/nonprime/none).
 - `-r / --random`			Generate a random curve (using Random approach).
 - `-u / --unique`			Generate a curve with only one generator.
 - `--hex-check=HEX`        Generate a curve with `HEX` string in the hex expansion of its parameters.
 - `--metadata`				Compute the curve metadata (j-invariant, discriminant, trace of Frobenius, CM discriminant, embedding degree)

#### IO options

 - `-f / --input=FILE`		Input from `FILE`.
 - `-o / --output=FILE`		Output into `FILE`. Overwrites any existing file!
 - `-a / --append`			Append to output file (don't overwrite).
 - `-v / --verbose[=FILE]`	Verbose logging (to stdout or `FILE`).

#### Other

 - `-d / --data-dir=DIR`	Set PARI/GP data directory (containing seadata package).
 - `-m / --memory=SIZE`		Use PARI stack of `SIZE` (can have suffix k/m/g).
 - `--threads=NUM`			Use `NUM` threads.
 - `--thread-stack=SIZE`	Use PARI stack of `SIZE` (per thread, can have suffix k/m/g).
 - `--timeout=TIME`			Timeout computation of a curve parameter after `TIME` (can have suffix s/m/h/d).

#### Examples

Generate a prime field, uniquely generated, prime order, 192-bit curve, don't ask for input try random values:

    > ecgen --fp -u -p -r 192
    
Generate 5 random, binary field, 163-bit curves:

    > ecgen --f2m -r -c5 163
    
Generate invalid curves to a file, for a given prime field 192-bit curve:

    > ecgen --fp -i --output=invalid.json 192
    p: <input prime>
    a: <input a param>
    b: <input b param>

Generate a prime field, uniquely generated, prime order curve, don't ask for input try random values, verbose output *(shown with example output here)*:

    > ecgen --fp -u -p -r -v 128
    +++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-+++++++
    {
        "field": {
            "p": "0xa5393890f26881d94ad670171ce67b79"
        },
        "a": "0x9c083973bdca36ea71078bbaabab4947",
        "b": "0x3d986a0206bfbe1ba62c858df54385e9",
        "order": "0xa5393890f26881d9394aece3bc2d9b47",
        "subgroups": [
            {
                "x": "0x5acc17d6a44e8f8d30e877f4fef8712f",
                "y": "0x6864dd64e80609abd1797c8de1febb9f",
                "order": "0xa5393890f26881d9394aece3bc2d9b47",
                "cofactor": "0x1",
                "points": [
                    {
                        "x": "0x9c7878930ddf5bfb705102f652754e7",
                        "y": "0x4b15a7bb808cb3579fd4c2ce42f628de",
                        "order": "0xa5393890f26881d9394aece3bc2d9b47"
                    }
                ]
            }
        ]
    }

Generate a prime field curve with given order (given as a list of factors) of size 128 bits and do not output any points on subgroups:

	> ecgen --fp -n 7,43,83,503,653,823,863,887,1019,1279,2011,2311,2381,2957 --points=none 128
	[{
	    "field": {
	        "p": "0xa5dad2cb65b65ab89ed2248be2971c0d"
	    },
	    "a": "0x2407ec02a74565af4952f552fee14588",
	    "b": "0x5e80456d719dcb64df00923e8b7aa726",
	    "order": "0xa5dad2cb65b65ab704d75427e1e4f81b",
	    "subgroups": [
	        {
	            "x": "0x3219e70cb3b557dd8f8ddf9e6264a28d",
	            "y": "0x0aa1274c0272f13411833fd7eb7e86d8",
	            "order": "0xa5dad2cb65b65ab704d75427e1e4f81b",
	            "cofactor": "0x1"
	        }
	    ]
	}]
	

Generate a prime field, uniquely generated random curve, of size 192 bits, also display its metadata:

	> ecgen --fp -r -u --metadata 192
	[{
	    "field": {
	        "p": "0xf0886302f64dfdd33035064e0f0a346a74c0cf0c22e22a79"
	    },
	    "a": "0x54c517a28dd47c296842f20273a0ec1840e576e6c06bb4e8",
	    "b": "0x3ee3025dc7d8c26104e6fb3e11f3bdaabc15c73b9df27c8f",
	    "order": "0xf0886302f64dfdd33035064df1a3dc1e3c4577ad590bfaa6",
	    "subgroups": [
	        {
	            "x": "0x9e886fbe1f155acdd790183bf1ceb5c92cfa112e693d34ef",
	            "y": "0x9723f733f48229287d173ce4a5147cd947de0637def3f3ed",
	            "order": "0xf0886302f64dfdd33035064df1a3dc1e3c4577ad590bfaa6",
	            "cofactor": "0x1",
	            "points": [
	                {
	                    "x": "0x1f7b95e47d5224164880e9809082c60194152901d1bc613e",
	                    "y": "0x000000000000000000000000000000000000000000000000",
	                    "order": "0x2"
	                },
	                {
	                    "x": "0xce3e0c542060d6e2b244ba5850e7ccdda413854454b464ff",
	                    "y": "0x38c193df5bb67817128fbf9f63d4c7af55535110553d247f",
	                    "order": "0x3"
	                },
	                {
	                    "x": "0x73d0035006dc8c9e63c65d76f96cbb5da7ab9735980ece74",
	                    "y": "0x420a85069882811d44458106bba9b2aedd4820f25ca7761c",
	                    "order": "0x9d"
	                },
	                {
	                    "x": "0x68f998d413b99520fe2ba1501f8ea22db0357cf88e4ab80f",
	                    "y": "0x6321a63274a60d2d0db49800e806d8d7923a5eb8bcd990a6",
	                    "order": "0x1ca3"
	                },
	                {
	                    "x": "0xd81d2b8eb95474be0e456dfd3e85d17d7832775a9e867ad4",
	                    "y": "0x6dc2c4c566616b1b959935192e14d403d7cfcaac4a9f0b90",
	                    "order": "0x67a21"
	                },
	                {
	                    "x": "0x919460438753800b0332685fc9294d0ae44d61fd8d7d4403",
	                    "y": "0xefa307fb3d2946939461fa3b6ac2a1c658821e69837d0122",
	                    "order": "0x1053aa57a15ae784f7"
	                },
	                {
	                    "x": "0xce1773b3b404b78923f0e8ba5493e806c088ef527c712dfb",
	                    "y": "0x451aa2ca72326983381c77a1965517787333ffe8c036073d",
	                    "order": "0x5869ef97eaf9885325d01"
	                }
	            ]
	        }
	    ],
	    "meta": {
	        "j": "1634327697577408300542081333151383474217426785000945527408",
	        "discriminant": "4412636550389189038392233746786780978750224170644136456036",
	        "embedding_degree": "529827387818798217347035546078799382319810259857040",
	        "frobenius": "9098792692692684052427583444",
	        "cm_discriminant": "-23508596506131498450876986604994502392929597678959240749652",
	        "conductor": "1"
	    }
	}]


> [!NOTE]  
> The command-line interface is quite tricky and **will** ignore options somewhat silently,
> meaning that the computation will go on and some properties of the output curve will be wrong.
> 
> If this happens, you will get a warning such as:
> > Warning: Ignored command-line argument prime (-p/--prime).


### Docs

See [docs](docs/readme.md). Also:

    make docs

for Doxygen.

### Generation methods

Five different EC curve parameters generation methods are implemented.

[Efficient Algorithms for Generating Elliptic Curves over Finite Fields Suitable for Use in Cryptography - [Baier]](https://www.cdc.informatik.tu-darmstadt.de/reports/reports/harald_baier.diss.pdf)

[Generation Methods of Elliptic Curves - [Baier, Buchmann]](https://www.ipa.go.jp/security/enc/CRYPTREC/fy15/doc/1030_Buchmann.evaluation.pdf)

#### Random approach

 - Generates field and equation parameters:
    - randomly
    - using ANSI X9.62 verifiably random method(from seed), until a curve with requested properties appears.
    - given input
 - Can generate curves repeatedly until one satisfies requested properties:
    - `-p / --prime` generates curves until a prime order curve is found.
    - `-K / --koblitz` generates a Koblitz curve.
    - `-u / --unique` generates a uniquely generated curve (with one generator/cyclic group).
    - `--twist` generates a curve and then produces it quadratic twist.
    - etc..

#### Invalid curve generation

 - Generates *invalid* curves for a given curve.
 - These curves have the same field, and *A* parameter in the short Weierstrass equation.
 - Multiplication using some(most?) scalar multiplication algorithm proceeds the same way multiplication on the input curve would.
 - Used with the `-i / --invalid` option.
 - These curves are **NOT SECURE** and are useful for implementation testing.
 - [Validation of Elliptic Curve Public Keys - [Antipa, Brown, Menezes, Struik, Vanstone]](https://www.iacr.org/archive/pkc2003/25670211/25670211.pdf)
 - [Differential Fault Attacks on Elliptic Curve Cryptosystems - [Biehl, Mayer, Muller]](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.107.3920&rep=rep1&type=pdf)
 - [Practical Invalid Curve Attacks on TLS-ECDH - [Jager, Schwenk, Somorovksy]](http://euklid.org/pdf/ECC_Invalid_Curve.pdf)

#### Complex multiplication

 - Capable of generating a curve of a given (prime) order.
 - Only works over a prime field.
 - Used with the `-n / --order` option.
 - Uses generation method from: [Constructing elliptic curves of prime order - [Broker, Stevenhagen]](https://arxiv.org/abs/0712.2022) (see Algorithm 2.2)
 - Creates the class polynomials using the built in PARI/GP `polclass()` function or the implemented class polynomial algorithm from IEEE P1363
 - [Constructing elliptic curves of prescribed order - [Broker (thesis)]](https://openaccess.leidenuniv.nl/bitstream/handle/1887/4425/Thesis.pdf)
 - [Generating Elliptic Curves of Prime Order - [Savas, Schmidt, Koc]](http://people.oregonstate.edu/~schmidtt/ourPapers/SavasKoc/ches01curve.pdf)

#### Supersingular curves

 - Generates curves of order equal to prime + 1 (trace of Frobenius equal to zero).
 - Used with the `--supersingular` option.
 - These curves are **NOT SECURE** and are useful for implementation testing.
 - [CONSTRUCTING SUPERSINGULAR ELLIPTIC CURVES - [Broker]](https://pdfs.semanticscholar.org/56c5/5b9cf0b218f93b8d263cc9f64ccb5fb97f52.pdf)

#### Anomalous curve generation

 - Generates curves of order equal to field order (trace of Frobenius equal to 1).
 - Used with the `--anomalous` option.
 - These curves are **NOT SECURE** and are useful for implementation testing.
 - [Elliptic curves over F_p suitable for cryptosystems - [Miyaji]](https://dspace.jaist.ac.jp/dspace/bitstream/10119/4464/1/73-61.pdf)
 - TODO: Extend with [Generating Anomalous Elliptic Curves - [Leprevost, Monnerat, Varrette, Vaydenay]](https://www.monnerat.info/publications/anomalous.pdf)

#### Curve families

 - Generates pairing friendly curves.
 - Used with the `-F / --family` option. Select from BN, BLS12 or BLS24 currently. 

### Build

ecgen can be built using Make or CMake. ecgen uses git submodules for testing at:

 - `test/lib/assert.sh` pointing at <https://github.com/J08nY/assert.sh>
 - `test/lib/JSON.sh` pointing at <https://github.com/J08nY/JSON.sh>
 - `test/lib/criterion` pointing at <https://github.com/Snaipe/Criterion>, its build requires meson.

these need to be initialized for `make test` to work.

#### Make

	git clone https://github.com/J08nY/ecgen.git
	cd ecgen
	git submodule update --init
	make

#### CMake

	git clone https://github.com/J08nY/ecgen.git
	cd ecgen
	git submodule update --init
	mkdir build && cd build && cmake .. && make

### Requirements

 - PARI/GP
 - gcc / clang
 - [posix-macos-addons](https://github.com/stanislaw/posix-macos-addons) for macOS

ecgen uses the [PARI/GP](http://pari.math.u-bordeaux.fr/) library for elliptic
curve arithmetic and it's SEA point counting algorithm implementation. It also requires the 
additional [seadata](http://pari.math.u-bordeaux.fr/packages.html) package (seadata and seadata-big recommended for large curves).

ecgen uses some extensions supported by the gcc and clang compilers, namely `__thread`-local storage.

### Libraries

 - `lib/parson` *©MIT*
 - `lib/sha1` *©MPL / GPLv2 or later*

[parson](https://github.com/kgabis/parson) is used to input and output JSON and is included in the `lib/` directory.

A [SHA-1](lib/sha1/sha1.c) implementation by Paul Kocher, based on the SHA 180-1 Reference Implementation (for ANSI X9.62 algorithm) is used and also included in the `lib/` directory.

### License

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
    USA.

*© Eastern Seaboard Phishing Authority*
