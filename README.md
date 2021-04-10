# ![](docs/full.png)

[![Build Status](https://travis-ci.org/J08nY/ecgen.svg?branch=master)](https://travis-ci.org/J08nY/ecgen) [![GitHub release](https://img.shields.io/github/release/J08nY/ecgen.svg)](https://github.com/J08nY/ecgen/releases/latest) [![codecov](https://codecov.io/gh/J08nY/ecgen/branch/master/graph/badge.svg)](https://codecov.io/gh/J08nY/ecgen)

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
 - `-n / --order=ORDER`		Generate a curve with given `ORDER` (using Complex Multiplication).
 - `-s / --ansi[=SEED]`		Generate a curve from `SEED` (ANSI X9.62 verifiable procedure).
 - `-b / --brainpool[=SEED]`Generate a curve using the Brainpool verifiably pseudorandom algorithm from the original paper.
 - `--brainpool-rfc[=SEED]` Generate a curve using the Brainpool verifiably pseudorandom algorithm as per RFC 5639.
 - `--twist`                Generate a twist of a given curve.

#### Generation options

 - `-c / --count=COUNT`		Generate multiple curves.
 - `-k / --cofactor=VALUE`	Generate a curve with cofactor of `VALUE`.
 - `-K / --koblitz[=A]`		Generate a Koblitz curve (a in {0, 1}, b = 1).
 - `-p / --prime`			Generate a curve with prime order.
 - `--points=TYPE`			Generate points of given `TYPE` (random/prime/all/nonprime/none).
 - `-r / --random`			Generate a random curve (using Random approach).
 - `-u / --unique`			Generate a curve with only one generator.
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

Generate a prime field, uniquely generated random curve, of size 192 bits, also display its metadata:

	> ecgen --fp -r -u --metadata 192
	[{
	    "field": {
	        "p": "0xc61cc69c570f03c690df1177fa1ac8c5b295587f876df1bf"
	    },
	    "a": "0xaa67a35fbec93655b4a9dc2fa84e2deff18077a4fa12d2ef",
	    "b": "0x4477eabe9abbcfccaa3f996ed03d58f32b149c42cb22d1b9",
	    "order": "0xc61cc69c570f03c690df1178443ec606114599ad0f6d8790",
	    "subgroups": [
	        {
	            "x": "0x909b1f3b6378fb77a2c1d2117fe7229c329a4fa400605a62",
	            "y": "0x2da7e3fafdd4c3b4b149cf6693151dd559eebf8b081435fd",
	            "order": "0xc61cc69c570f03c690df1178443ec606114599ad0f6d8790",
	            "cofactor": "0x1",
	            "points": [
	                {
	                    "x": "0x6831806c80fbd016fb59c2eb20700716eeb392c601deda25",
	                    "y": "0x000000000000000000000000000000000000000000000000",
	                    "order": "0x2"
	                },
	                {
	                    "x": "0x6206b8e9d56a22485c1b2baa6c61292ff51aa0848097e823",
	                    "y": "0xbd0a1bda7ba13c81bb3ecaf4abfd34c054d9e4a41aa00733",
	                    "order": "0xa3"
	                },
	                {
	                    "x": "0x9e196e607959b78f821600fdf263834d4ad2b10046b5921c",
	                    "y": "0x26f2c864a472bacd4bbb3e513b0a1c1f59a919a187fa4b47",
	                    "order": "0xbe3b5"
	                },
	                {
	                    "x": "0x0220c948fbb6403608563efc5cf16a630953281155e62d47",
	                    "y": "0x3a630cd359f8028da08b27df562ab8749e3c2430b8a9f3a9",
	                    "order": "0x202205798a8a981b"
	                },
	                {
	                    "x": "0x76f971b3722e4b8bfc284c7c39adb395ade0baa61b1e6820",
	                    "y": "0xb7304431b86e6a06576005a6da2afcd96b43e1ce62056724",
	                    "order": "0xd07e2df2cc0790a9a06ccfa45"
	                }
	            ]
	        }
	    ],
	    "meta": {
	        "j": "2147872786867739581244581376066320780390424557746041800843",
	        "discriminant": "1437087699540380117015716490177070508119149165688061690066",
	        "embedding_degree": "4857702049745916698354329148965234205490318980592629024655",
	        "frobenius": "-22945399076600858943676192208",
	        "cm_discriminant": "4726079215049797810784721872708419089129671954601412853631"
	    }
	}]

### Docs

See [docs](docs/readme.md). Also:

    make docs

for Doxygen.

### Generation methods

Four different EC curve parameters generation methods are implemented.

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

 - [CONSTRUCTING SUPERSINGULAR ELLIPTIC CURVES - [Broker]](https://pdfs.semanticscholar.org/56c5/5b9cf0b218f93b8d263cc9f64ccb5fb97f52.pdf)

#### Anomalous curve generation

 - Generates curves of order equal to field order.
 - Used with the `--anomalous` option.
 - These curves are **NOT SECURE** and are useful for implementation testing.
 - [Elliptic curves over F_p suitable for cryptosystems - [Miyaji]](https://dspace.jaist.ac.jp/dspace/bitstream/10119/4464/1/73-61.pdf)

### Build

ecgen can be built using Make or CMake. ecgen uses git submodules for testing at:

 - `test/lib/assert.sh` pointing at <https://github.com/J08nY/assert.sh>
 - `test/lib/JSON.sh` pointing at <https://github.com/jimklimov/JSON.sh>
 - `test/lib/criterion` pointing at <https://github.com/Snaipe/Criterion>

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
