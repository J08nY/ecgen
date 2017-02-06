## ecgen

Tool for generating Elliptic curve domain parameters.

### Usage

#### ecgen
The main binary of the tool, does the hard work.
```
ecgen --fp/--f2m -r BITS
```
 - `--fp`/`--f2m` specifies the field type(prime/binary).
 - `-r` requests a random curve to be generated, currently only supported feature.
 - `-o/--output FILE` writes output to `FILE`.
 - `-i/--input FILE` reads input from `FILE`.
 - `-a/--append` appends to output file(doesn't overwrite it).
 - `-d/--datadir DIR` specifies the PARI/GP datadir containing the `seadata` package.

### Build
```
git clone https://github.com/J08nY/ecgen.git
cd ecgen
make
```

### Requirements
 - PARI/GP
 - parson

ecgen uses the [PARI/GP](http://pari.math.u-bordeaux.fr/) library for elliptic
curve arithmetic and it's SEA point counting algorithm implementation. It also requires the 
additional [seadata](http://pari.math.u-bordeaux.fr/packages.html) package (seadata and seadata-big recommended for large curves).

[parson](https://github.com/kgabis/parson) is used to output curve parameters in JSON format.

### License
```
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
```
*© Eastern Seaboard Phishing Authority*
