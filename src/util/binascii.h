/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#ifndef ECGEN_BINASCII_H
#define ECGEN_BINASCII_H

#include <pari/pari.h>
#include "gen/types.h"

/**
 * @brief
 * @param i
 * @return
 */
size_t binascii_blen(GEN i);

/**
 * @brief
 * @param i
 * @param endianity
 * @return
 */
char *binascii_itob(GEN i, endian_e endianity);

/**
 * @brief
 * @param bytes
 * @param endianity
 * @return
 */
GEN binascii_btoi(const char *bytes, size_t len, endian_e endianity);

#endif  // ECGEN_BINASCII_H
