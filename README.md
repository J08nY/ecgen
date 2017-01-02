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
* `p` - a prime number, in hex, without hex prefix
* `a` - a random number, in hex, without hex prefix
* `b` - a random number, in hex, without hex prefix
the curve is then specified as `E(Fp): y^2 = x^3 + a x + b mod p`

#### gen.sh
Utility script, uses OpenSSL to generate the *p*, *a*, *b* parameters
required for ecgen.
```
gen.sh [-l|-s|-p] [num_curves] [prime_size] [timeout]
```
This script will generate `num_curves` curves with random `prime_size` bit size parameters.
It requires a timeout value, as the curve order computation can get really
slow for some unfortunate curve parameters, it is therefore more effective
to skip those after some timeout.
The `-l -s -p` options correspond to ecgen options.

### Build
```
git clone https://github.com/J08nY/ecgen.git
cd ecgen
make
```

### Requirements
ecgen uses the [PARI/GP](http://pari.math.u-bordeaux.fr/) library for elliptic
curve arithmetic and it's SEA point counting algorithm implementation.

gen.sh script uses [OpenSSL](https://www.openssl.org/) to generate the random domain parameters,
although any library/language can be used here, ecgen does not require OpenSSL.



*© Eastern Seaboard Phishing Authority*
