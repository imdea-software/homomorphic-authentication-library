HAL: A Library for Homomorphic Authentication
================================================================================

--------------------------------------------------------------------------------
Overview
--------------------------------------------------------------------------------

HAL is a software library that implements __homomorphic
authenticator__ (HA) schemes. An HA is a cryptographic primitive that
allows for proving and verifying the integrity of computations on
authenticated data. Informally speaking, this means that a party can
use a secret key to authenticate a value _x_ obtaining an
authentication tag _s_<sub>_x_</sub>; next, anyone can compute an
authentication tag _s_<sub>_f,y_</sub> that certifies the value
_y=f(x)_ as output of function _f_; _s_<sub>_f,y_</sub> can be
eventually verified by anyone with knowledge of an appropriate (public
or secret) verification key. This verification will ensure that _y_ is
indeed _f(x)_, for an _x_ correctly authenticated using the
corresponding secret key.

### Homomorphic Authenticators
Going a bit more in detail, an HA scheme includes the following
algorithms:

- __KeyGen__, which generates a public evaluation key _ek_, a secret
  authentication key _sk_, and a verification key _vk_. If _vk_ can be
  publicly released the HA is called a _homomorphic signature_,
  otherwise the scheme is called a _homomorphic MAC_.
- __Auth(___sk_, _D_, _L_, _x_ __)__-->_s_, which on input the secret
authentication key _sk_, a dataset name _D_, an input label _L_ (a
string), and a message _x_, returns an authentication tag _s_.
- __Eval(___f_, _s1_,...,_sn_ __)__-->_s_, which on input the
  description of an n-to-1 function _f_, and _n_ authentication tags
  _s1_,...,_sn_, returns a new authentication tag _s_ for the result.
- __Verify(___vk_, _f_, _L1, ..., _Ln_, _D_, _x_, _s_ __)__, which on input the
  verification key _vk_, the description of function _f_ with input
  labels _L1_, ..., _Ln_, a dataset name _D_, a message  _x_ and an
  authentication tag  _s_, returns _accept_ or _reject_.

A tuple of algorithms such the ones described above is an homomorphic
authenticator if it offers the following properties.

-   __Correctness:__
applying __Eval__ on valid tags _s_<sub>_x1_</sub>, ...,
_s_<sub>_xn_</sub> corresponding to dataset _D_, inputs _x1, ..., xn_
and labels _L1, ..., Ln_ respectively, yields a tag
_s_<sub>_f,y_</sub> that verifies against function _f_, labels  _L1,
..., Ln_, dataset _D_, and message _f(x1, ..., xn)_.
-   __Succinctness:__
the size of authentication tags is short and independent of the input size.
-   __Security:__
without the scheme's secrets it is computationally infeasible to
create valida tags for false computations results.

For formal definitions and theoretical discussions about these, see
\[GW13], \[CF13], \[BFR13], and references therein.

#### Homomorphic Authenticators with Efficient Verification
An interesting class of HA schemes (considered in the library) are
those that offer _efficient verification_. The difference is that the
__Verify__ algorithm can be "split" in two algorithms:

- __VerifyPrep(__ _vk_, _f_, _L1, ..., _Ln_ __)__--> _vk_<sub>_f_</sub>,
  which returns a verification key bound to the function _f_ and its
  input labels.
- __Verify(__ _vk_<sub>_f_</sub>, _D_, _x_, _s_ __)__, which on
  input the verification key generated as above, a dataset name _D_,
  a message  _x_ and an authentication tag  _s_, returns _accept_ or _reject_.

Essentially, __VerifyPrep__ creates a verification key that is
independent of the dataset name _D_ and thus can be reused to verify
computations of the same function _f_ on different datasets _D1_,
_D2_, etc. Notably, this way the running of __Verify__ becomes
independent of _f_ and, in most constructions, constant in the input
size.

## Implemented schemes
The HAL library currently provides an implementation of:

1.  __fgp__. An homomorphic MAC with efficient verification for
degree-2 arithmetic circuits over polynomials (see below for more
details). This is the scheme implicitly described in \[FGP14].

	The message space of this scheme is the polynomial ring _Z[X,Y]/qZ_,
	where _q_ is a prime number that is the order of the bilinear groups
	used by the scheme. "Fresh messages" (i.e., those taken by __Auth__)
	have degree at most _1_ in _Y_ and degree at most _d_ in _X_. The
	messages taken by __Verify__ are expected to have degree at most _2_
	in _Y_ and _2*d_ in _X_. The class of functions _f_ supported by this
	scheme are arithmetic circuits of multiplicative depth _1_
	(degree_2_) where additions and multiplications are over the ring
	_Z[X,Y]/qZ_. The special case in which the message values are
	constant polynomials (i.e., of degree 0 in both X and Y), the
	scheme almost matches the homomorphic MAC described in \[BFR13].

2.  A collection of utilities that can be common to several schemes, such
as the implementation of a PRF mapping binary strings into integers in a
finite ring, and a tool that allows to write a (degree-2) algorithm
and compiles into C code that corresponds to the homomorphic
computation over tha authentication tags, the verification keys, and
the messages.



--------------------------------------------------------------------------------
Pairing library
--------------------------------------------------------------------------------

For those HA schemes that use pairings (e.g., __fgp__) the HAL library
uses the \[RELIC] library, and in particular its instantiation of a
Barreto-Naehrig curve, providing 128 bits of security.  RELIC is also
used to handle most of _Zq_ arithmetic operations.

--------------------------------------------------------------------------------
Build instructions
--------------------------------------------------------------------------------

HAL is written in C and relies on the following:

- GMP for multi-precision arithmetic
- RELIC for elliptic-curve- and pairing-based schemes
- FLINT for polynomial operations


After GMP, RELIC and FLINT are installed (they are supposed to be in
/usr/local, this can be changed in the Makefile), you can build the
library by running:

$ bash configure

$ make hal-crypto

### Using HAL as a library

If you wish to use HAL as a library, so that you can write your code
in any directory and link it against HAL, run:

$ make lib

This creates a file `libhal-crypto.a` into `./lib`

To install the library, run:

$ make install PREFIX=/installation-path

This will install `hal-crypto.a` into `/installation-path/lib`, and the
required headers into `/installation-path/include/hal-crypto/`. So your
application should be linked using `-L/installation-path/lib -lhal-crypto`,
and compiled using `-I/installation-path/include/hal-crypto`.

By default (if the PREFIX is not set) the library will be installed
in `/usr/local`. Note that in that case, as well as in any case where
the path you give requires super-user privileges, you should run the
command preceded by sudo.


### Portability

We have tested the library on:

* macOS 10.12.1 (Sierra)
* Linux Mint 18 Cinnamon 3.0.7 64-bit

--------------------------------------------------------------------------------
Tutorials
--------------------------------------------------------------------------------

The library includes some usage examples.

###Basic usage

`test/sample_test.c` contains a usage example of the library. The code simulates 
homomorphic encryption by generating random ciphertexts and applying a degree 2
computation on them. The result is then checked.

An executable can be built from that source file by running

$ make test

This will create the `test/sampleTest` executable which should, when run, print
*Verification SUCCESSFUL* to standard output.

###Compiler for homomorphic computations
The library makes available a tool for writing the code of a function
in a generic way, and for compiling this code into C code for
executing this function over the message space, the authentication
tags, and the verification keys (for verification preparation). To use
this utility do the following:

1. Write a generic function in a file using the instruction and model given in the
`test/func_model` file.

2. Run *./createFunction* with the file you just wrote as parameter. Do it from
inside the test folder.

3. Run $ make test2

4. Run the *sampleTest2* executable that was created to test the computation.

If you wish to use the compiled functions in your own code, you can
stop at step 2 and use the code in `test/sample_test2.c` as reference.

--------------------------------------------------------------------------------
Directory structure
--------------------------------------------------------------------------------

The directory structure of the library is as follows:

* src/ --- main C source code, which contains the following modules:
    * fgp/ --- implementation of the __fgp__ homomorphic MAC
    	* tools/ --- implementation of the __fgp__ structures used by the protocol.
	* utils/ --- implementation of some utility functions such as a
      pseudorandom function, conversions between various (big) integer
      types, and error handling.
* include/ --- headers files, following the same structure of src
* test/ --- a test source file as described above
	* func --- a directory with source files and headers associated with the function
	creation options 
* lib/ --- will store the libhal-crypto.a static library
* obj/ --- will contain object files after compilation
* doxygen/ --- doxygen configuration file for the library and a script
  for generating a documentation pdf file.

--------------------------------------------------------------------------------
Authors
--------------------------------------------------------------------------------

The HAL library is developed by Martin Zuber and Dario Fiore, and is released
under the MIT License (see the [LICENSE] file).

Copyright (c) 2016-2017
	Dario Fiore (dario.fiore@imdea.org)
	Martin Zuber (zubersmartin@gmail.com)
	
--------------------------------------------------------------------------------
References
--------------------------------------------------------------------------------

\[BFR13] [
		   _Verifiable Delegation of Computation on Outsourced Data_
		   ](http://eprint.iacr.org/2013/469),
Michael Backes, Dario Fiore, Raphael M. Reischuk,
ACM Conference on Computer and Communications Security (ACM CCS) 2013

\[CF13] [
		   _Practical Homomorphic MACs for Arithmetic Circuits_
		   ](http://eprint.iacr.org/2015/194),
Dario Catalano and Dario Fiore,
EUROCRYPT 2013

\[FGP14] [
		   _Efficiently Verifiable Computation on Encrypted Data_
		   ](https://eprint.iacr.org/2014/202),
Dario Fiore, Rosario Gennaro, and Valerio Pastro,
ACM Conference on Computer and Communications Security (ACM CCS) 2014

\[GW13] [
		   _Fully Homomorphic Message Authenticators_
		   ](http://eprint.iacr.org/2012/290),
Rosario Gennaro and Daniel Wichs,
ASIACRYPT 2013

\[RELIC] [
				_RELIC is an Efficient LIbrary for Cryptography_
				](https://github.com/relic-toolkit/relic),
D. F. Aranha and C. P. L. Gouvea

[LICENSE]: LICENSE (LICENSE file in top directory of HAL distribution)
