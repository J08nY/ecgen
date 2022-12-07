/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2021 J08nY
 */
/**
 * @file compat.h
 */
#ifndef ECGEN_MISC_COMPAT_H
#define ECGEN_MISC_COMPAT_H

#define PARI_VERSION_PATCH (PARI_VERSION_CODE & 0xff)
#define PARI_VERSION_MINOR (((PARI_VERSION_CODE) >> 8) & 0xff)
#define PARI_VERSION_MAJOR (PARI_VERSION_CODE >> 16)

#define PARI_VERSION_GT(a,b,c) ((PARI_VERSION_MAJOR == a && PARI_VERSION_MINOR == b && PARI_VERSION_PATCH > c) || (PARI_VERSION_MAJOR == a && PARI_VERSION_MINOR > b) || (PARI_VERSION_MAJOR > a))
#define PARI_VERSION_EQ(a,b,c) (PARI_VERSION_MAJOR == a && PARI_VERSION_MINOR == b && PARI_VERSION_PATCH == c)
#define PARI_VERSION_GE(a,b,c) (PARI_VERSION_GT(a,b,c) || PARI_VERSION_EQ(a,b,c))
#define PARI_VERSION_LT(a,b,c) !(PARI_VERSION_GE(a,b,c))
#define PARI_VERSION_LE(a,b,c) !(PARI_VERSION_GT(a,b,c))

#if PARI_VERSION_LT(2,12,1)
#define polisirreducible isirreducible
#endif

#if PARI_VERSION_LT(2,15,0)
#define znorder(x, o) order(x);
#endif

#endif  // ECGEN_MISC_COMPAT_H
