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

#if !(((PARI_VERSION_MAJOR == 2) && (PARI_VERSION_MINOR == 12) && (PARI_VERSION_PATCH >= 2)) || ((PARI_VERSION_MAJOR >= 2) && (PARI_VERSION_MINOR >= 12)))
// pari < 2.12.2
#define polisirreducible isirreducible
#endif


#endif  // ECGEN_MISC_COMPAT_H
