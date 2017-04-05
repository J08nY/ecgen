## ecgen [![Build Status](https://travis-ci.org/J08nY/ecgen.svg?branch=master)](https://travis-ci.org/J08nY/ecgen) [![GitHub release](https://img.shields.io/github/release/J08nY/ecgen.svg)](https://github.com/J08nY/ecgen/releases/latest)

Tool for generating Elliptic curve domain parameters.

### Usage

	ecgen --fp/--f2m BITS

#### Field specification

 - `--f2m`                  Binary field.
 - `--fp`                   Prime field.

#### Generation options

 - `-c`, `--count=COUNT`    Generate multiple curves.
 - `-i`, `--invalid`        Generate a set of invalid curves, for a given curve (using Invalid curve algorithm).
 - `-k`, `--cofactor=BOUND` Generate a curve with cofactor up to `BOUND`
 - `-K`, `--koblitz`        Generate a Koblitz curve (a = 0).
 - `-n`, `--order=ORDER`    Generate a curve with given `ORDER` (using Complex Multiplication).
 - `-p`, `--prime`          Generate a curve with prime order.
 - `--points=TYPE`          Generate points of given `TYPE` (random/prime/none).
 -`r`, `--random`           Generate a random curve (using Random approach).
 -`s`, `--seed[=SEED]`      Generate a curve from `SEED` (ANSI X9.62 verifiable procedure).
 -`u`, `--unique`           Generate a curve with only one generator.

#### IO options

 - `-a`, `--append`         Append to output file (don't overwrite).
 - `-f`, `--input=FILE`     Input from `FILE`.
 - `-o`, `--output=FILE`    Output into `FILE`. Overwrites any existing file!
 - `-t`, `--format=FORMAT`  Format to output in. One of [csv,json], default is json.
 - `-v`, `--verbose[=FILE]` Verbose logging (to stdout or `FILE`).

#### Other

 - `-d`, `--data-dir=DIR`   Set PARI/GP data directory (containing seadata package).
 - `-m`, `--memory=SIZE`    Use PARI stack of `SIZE` (can have suffix k/m/g).

#### Examples

Generate a prime field, uniquely generated 192-bit curve, don't ask for input try random values:

    > ecgen --fp -r -p -u 192
    
Generate 5 random, binary field, 163-bit koblitz curves:

    > ecgen --f2m -r -k -c5 163
    
Generate invalid curves to a file, for a given prime field 192-bit curve:

    > ecgen --fp -i --output=invalid.json 192
    p: <input prime>
    a: <input a param>
    b: <input b param>

Generate a prime field, uniquely generated, prime order curve, don't ask for input try random values, verbose output *(show with example output here)*:

    > ecgen --fp -r -p -u -v 128
    +++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-++++-+++++++
    {
        "field": {
            "p": "0xa5393890f26881d94ad670171ce67b79"
        },
        "a": "0x9c083973bdca36ea71078bbaabab4947",
        "b": "0x3d986a0206bfbe1ba62c858df54385e9",
        "order": "0xa5393890f26881d9394aece3bc2d9b47",
        "generators": [
            {
                "x": "0x5acc17d6a44e8f8d30e877f4fef8712f",
                "y": "0x6864dd64e80609abd1797c8de1febb9f",
                "order": "0xa5393890f26881d9394aece3bc2d9b47",
                "cofactor": "0x1"
            }
        ],
        "points": [
            {
                "x": "0x9c7878930ddf5bfb705102f652754e7",
                "y": "0x4b15a7bb808cb3579fd4c2ce42f628de",
                "order": "0xa5393890f26881d9394aece3bc2d9b47"
            }
        ]
    }


### Generation methods

Three different EC curve parameters generation methods are implemented.

[Efficient Algorithms for Generating Elliptic Curves over Finite Fields Suitable for Use in Cryptography - [Baier]](https://www.cdc.informatik.tu-darmstadt.de/reports/reports/harald_baier.diss.pdf)

[Generation Methods of Elliptic Curves - [Baier, Buchmann]](https://www.ipa.go.jp/security/enc/CRYPTREC/fy15/doc/1030_Buchmann.evaluation.pdf)

##### Random approach

 - Generates field and equation parameters:
   - randomly
   - using ANSI X9.62 verifiably random method(from seed), until a curve with requested properties appears.
   - given input
 - Can generate curves repeatedly until one satisfies requested properties:
   - `-p/--prime` generates curves until a prime order curve is found.
   - `-k/--koblitz` generates a curve with fixed *A = 0* parameter.
   - `-u/--unique` generates a uniquely generated curve (with one generator/cyclic group).

##### Invalid curve generation

 - Generates *invalid* curves for a given curve.
 - These curves have the same field, and *A* parameter in the short Weierstrass equation.
 - Multiplication using some(most?) scalar multiplication algorithm proceeds the same way multiplication on the input curve would.
 - Used with the `-i / --invalid` option
 - [Validation of Elliptic Curve Public Keys - [Antipa, Brown, Menezes, Struik, Vanstone]](https://www.iacr.org/archive/pkc2003/25670211/25670211.pdf)
 - [Differential Fault Attacks on Elliptic Curve Cryptosystems - [Biehl, Mayer, Muller]](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.107.3920&rep=rep1&type=pdf)
 - [Practical Invalid Curve Attacks on TLS-ECDH - [Jager, Schwenk, Somorovksy]](http://euklid.org/pdf/ECC_Invalid_Curve.pdf)

##### Complex multiplication

 - Capable of generating a curve of a given (prime) order.
 - Generates a subset of all Elliptic Curves over a given field.
 - Used with the `-n / --order` option
 - [Constructing elliptic curves of prime order - [Broker, Stevenhagen]](https://arxiv.org/abs/0712.2022)
 - [Generating Elliptic Curves of Prime Order - [Savas, Schmidt, Koc]](http://people.oregonstate.edu/~schmidtt/ourPapers/SavasKoc/ches01curve.pdf)


### Build

	git clone https://github.com/J08nY/ecgen.git
	cd ecgen
	make

### Requirements

 - PARI/GP

ecgen uses the [PARI/GP](http://pari.math.u-bordeaux.fr/) library for elliptic
curve arithmetic and it's SEA point counting algorithm implementation. It also requires the 
additional [seadata](http://pari.math.u-bordeaux.fr/packages.html) package (seadata and seadata-big recommended for large curves).


### License

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.


*© Eastern Seaboard Phishing Authority*
