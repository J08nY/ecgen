## ecgen

Tool for generating Elliptic curve domain parameters.

Currently only supports curves over prime fields.

### Usage

#### ecgen
The main binary of the tool, does the hard work.
```
ecgen [-l|-s|-p]
	-l for point with the largest prime order on the curve
	-s for point with the smallest prime order on the curve
	-p for a generator of the whole curve as well as a smallest prime order point(pubkey)
```
ecgen then reads three lines of input:
`p`
`a`
`b`
the curve is then specified as `E(Fp): y^2 = x^3 + a x + b mod p`

#### gen.sh
Utility script, uses openssl to generate the *p*, *a*, *b* parameters
required for ecgen.
```gen.sh [-l|-s|-p] [num_curves] [prime_size] [timeout]```
*p* is `prime_size` bits prime number, *a* and *b* are `prime_size` bits
random numbers.
This script will generate `num_curves` curves with random parameters.
It requires a timeout value, as the curve order computation can get really
slow for some unfortunate curve parameters, it is therefore more effective
to skip those after some timeout.
