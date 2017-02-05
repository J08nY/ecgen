/*
 * ecgen, tool for generating Elliptic curve domain parameters
 * Copyright (C) 2017 J08nY
 */
#include "poly.h"
#include <stdlib.h>

polynomial_t ansi_trinomials[] = {
    {161, 18},   {162, 27},   {166, 37},   {167, 6},    {169, 34},
    {170, 11},   {172, 1},    {174, 13},   {175, 6},    {177, 8},
    {178, 31},   {180, 3},    {182, 81},   {183, 56},   {185, 24},
    {186, 11},   {191, 9},    {193, 15},   {194, 87},   {196, 3},
    {198, 9},    {199, 34},   {201, 14},   {202, 55},   {204, 27},
    {207, 43},   {209, 6},    {210, 7},    {212, 105},  {214, 73},
    {215, 23},   {217, 45},   {218, 11},   {220, 7},    {223, 33},
    {225, 32},   {228, 113},  {231, 26},   {233, 74},   {234, 31},
    {236, 5},    {238, 73},   {239, 36},   {241, 70},   {242, 95},
    {244, 111},  {247, 82},   {249, 35},   {250, 103},  {252, 15},
    {253, 46},   {255, 52},   {257, 12},   {258, 71},   {260, 15},
    {263, 93},   {265, 42},   {266, 47},   {268, 25},   {270, 53},
    {271, 58},   {273, 23},   {274, 67},   {276, 63},   {278, 5},
    {279, 5},    {281, 93},   {282, 35},   {284, 53},   {286, 69},
    {287, 71},   {289, 21},   {292, 37},   {294, 33},   {295, 48},
    {297, 5},    {300, 5},    {302, 41},   {303, 1},    {305, 102},
    {308, 15},   {310, 93},   {313, 79},   {314, 15},   {316, 63},
    {318, 45},   {319, 36},   {321, 31},   {322, 67},   {324, 51},
    {327, 34},   {329, 50},   {330, 99},   {332, 89},   {333, 2},
    {337, 55},   {340, 45},   {342, 125},  {343, 75},   {345, 22},
    {346, 63},   {348, 103},  {350, 53},   {351, 34},   {353, 69},
    {354, 99},   {358, 57},   {359, 68},   {362, 63},   {364, 9},
    {366, 29},   {367, 21},   {369, 91},   {370, 139},  {372, 111},
    {375, 16},   {377, 41},   {378, 43},   {380, 47},   {382, 81},
    {383, 90},   {385, 6},    {386, 83},   {388, 159},  {390, 9},
    {391, 28},   {393, 7},    {394, 135},  {396, 25},   {399, 26},
    {401, 152},  {402, 171},  {404, 65},   {406, 141},  {407, 71},
    {409, 87},   {412, 147},  {414, 13},   {415, 102},  {417, 107},
    {418, 199},  {420, 7},    {422, 149},  {423, 25},   {425, 12},
    {426, 63},   {428, 105},  {431, 120},  {433, 33},   {436, 165},
    {438, 65},   {439, 49},   {441, 7},    {444, 81},   {446, 105},
    {447, 73},   {449, 134},  {450, 47},   {455, 38},   {457, 16},
    {458, 203},  {460, 19},   {462, 73},   {463, 93},   {465, 31},
    {468, 27},   {470, 9},    {471, 1},    {473, 200},  {474, 191},
    {476, 9},    {478, 121},  {479, 104},  {481, 138},  {484, 105},
    {486, 81},   {487, 94},   {489, 83},   {490, 219},  {492, 7},
    {494, 17},   {495, 76},   {497, 78},   {498, 155},  {500, 27},
    {503, 3},    {505, 156},  {506, 23},   {508, 9},    {510, 69},
    {511, 10},   {513, 26},   {514, 67},   {516, 21},   {518, 33},
    {519, 79},   {521, 32},   {522, 39},   {524, 167},  {526, 97},
    {527, 47},   {529, 42},   {532, 1},    {534, 161},  {537, 94},
    {538, 195},  {540, 9},    {543, 16},   {545, 122},  {550, 193},
    {551, 135},  {553, 39},   {556, 153},  {558, 73},   {559, 34},
    {561, 71},   {564, 163},  {566, 153},  {567, 28},   {569, 77},
    {570, 67},   {574, 13},   {575, 146},  {577, 25},   {580, 237},
    {582, 85},   {583, 130},  {585, 88},   {588, 35},   {590, 93},
    {593, 86},   {594, 19},   {596, 273},  {599, 30},   {601, 201},
    {602, 215},  {604, 105},  {606, 165},  {607, 105},  {609, 31},
    {610, 127},  {612, 81},   {614, 45},   {615, 211},  {617, 200},
    {618, 295},  {620, 9},    {622, 297},  {623, 68},   {625, 133},
    {626, 251},  {628, 223},  {631, 307},  {633, 101},  {634, 39},
    {636, 217},  {639, 16},   {641, 11},   {642, 119},  {646, 249},
    {647, 5},    {649, 37},   {650, 3},    {651, 14},   {652, 93},
    {654, 33},   {655, 88},   {657, 38},   {658, 55},   {660, 11},
    {662, 21},   {663, 107},  {665, 33},   {668, 147},  {670, 153},
    {671, 15},   {673, 28},   {676, 31},   {679, 66},   {682, 171},
    {684, 209},  {686, 197},  {687, 13},   {689, 14},   {690, 79},
    {692, 299},  {694, 169},  {695, 177},  {697, 267},  {698, 215},
    {700, 75},   {702, 37},   {705, 17},   {708, 15},   {711, 92},
    {713, 41},   {714, 23},   {716, 183},  {718, 165},  {719, 150},
    {721, 9},    {722, 231},  {724, 207},  {726, 5},    {727, 180},
    {729, 58},   {730, 147},  {732, 343},  {735, 44},   {737, 5},
    {738, 347},  {740, 135},  {742, 85},   {743, 90},   {745, 258},
    {746, 351},  {748, 19},   {750, 309},  {751, 18},   {753, 158},
    {754, 19},   {756, 45},   {758, 233},  {759, 98},   {761, 3},
    {762, 83},   {767, 168},  {769, 120},  {772, 7},    {774, 185},
    {775, 93},   {777, 29},   {778, 375},  {780, 13},   {782, 329},
    {783, 68},   {785, 92},   {791, 30},   {793, 253},  {794, 143},
    {798, 53},   {799, 25},   {801, 217},  {804, 75},   {806, 21},
    {807, 7},    {809, 15},   {810, 159},  {812, 29},   {814, 21},
    {815, 333},  {817, 52},   {818, 119},  {820, 123},  {822, 17},
    {823, 9},    {825, 38},   {826, 255},  {828, 189},  {831, 49},
    {833, 149},  {834, 15},   {838, 61},   {839, 54},   {841, 144},
    {842, 47},   {844, 105},  {845, 2},    {846, 105},  {847, 136},
    {849, 253},  {850, 111},  {852, 159},  {855, 29},   {857, 119},
    {858, 207},  {860, 35},   {861, 14},   {862, 349},  {865, 1},
    {866, 75},   {868, 145},  {870, 301},  {871, 378},  {873, 352},
    {876, 149},  {879, 11},   {881, 78},   {882, 99},   {884, 173},
    {887, 147},  {889, 127},  {890, 183},  {892, 31},   {894, 173},
    {895, 12},   {897, 113},  {898, 207},  {900, 1},    {902, 21},
    {903, 35},   {905, 117},  {906, 123},  {908, 143},  {911, 204},
    {913, 91},   {916, 183},  {918, 77},   {919, 36},   {921, 221},
    {924, 31},   {926, 365},  {927, 403},  {930, 31},   {932, 177},
    {935, 417},  {937, 217},  {938, 207},  {942, 45},   {943, 24},
    {945, 77},   {948, 189},  {951, 260},  {953, 168},  {954, 131},
    {956, 305},  {959, 143},  {961, 18},   {964, 103},  {966, 201},
    {967, 36},   {969, 31},   {972, 7},    {975, 19},   {977, 15},
    {979, 178},  {982, 177},  {983, 230},  {985, 222},  {986, 3},
    {988, 121},  {990, 161},  {991, 39},   {993, 62},   {994, 223},
    {996, 65},   {998, 101},  {999, 59},   {1001, 17},  {1007, 75},
    {1009, 55},  {1010, 99},  {1012, 115}, {1014, 385}, {1015, 186},
    {1020, 135}, {1022, 317}, {1023, 7},   {1025, 294}, {1026, 35},
    {1028, 119}, {1029, 98},  {1030, 93},  {1031, 68},  {1033, 108},
    {1034, 75},  {1036, 411}, {1039, 21},  {1041, 412}, {1042, 439},
    {1044, 41},  {1047, 10},  {1049, 141}, {1050, 159}, {1052, 291},
    {1054, 105}, {1055, 24},  {1057, 198}, {1058, 27},  {1062, 49},
    {1063, 168}, {1065, 463}, {1071, 7},   {1078, 361}, {1079, 230},
    {1081, 24},  {1082, 407}, {1084, 189}, {1085, 62},  {1086, 189},
    {1087, 112}, {1089, 91},  {1090, 79},  {1092, 23},  {1094, 57},
    {1095, 139}, {1097, 14},  {1098, 83},  {1100, 35},  {1102, 117},
    {1103, 65},  {1105, 21},  {1106, 195}, {1108, 327}, {1110, 417},
    {1111, 13},  {1113, 107}, {1116, 59},  {1119, 283}, {1121, 62},
    {1122, 427}, {1126, 105}, {1127, 27},  {1129, 103}, {1130, 551},
    {1134, 129}, {1135, 9},   {1137, 277}, {1138, 31},  {1140, 141},
    {1142, 357}, {1145, 227}, {1146, 131}, {1148, 23},  {1151, 90},
    {1153, 241}, {1154, 75},  {1156, 307}, {1158, 245}, {1159, 66},
    {1161, 365}, {1164, 19},  {1166, 189}, {1167, 133}, {1169, 114},
    {1170, 27},  {1174, 133}, {1175, 476}, {1177, 16},  {1178, 375},
    {1180, 25},  {1182, 77},  {1183, 87},  {1185, 134}, {1186, 171},
    {1188, 75},  {1190, 233}, {1191, 196}, {1193, 173}, {1196, 281},
    {1198, 405}, {1199, 114}, {1201, 171}, {1202, 287}, {1204, 43},
    {1206, 513}, {1207, 273}, {1209, 118}, {1210, 243}, {1212, 203},
    {1214, 257}, {1215, 302}, {1217, 393}, {1218, 91},  {1220, 413},
    {1223, 255}, {1225, 234}, {1226, 167}, {1228, 27},  {1230, 433},
    {1231, 105}, {1233, 151}, {1234, 427}, {1236, 49},  {1238, 153},
    {1239, 4},   {1241, 54},  {1242, 203}, {1246, 25},  {1247, 14},
    {1249, 187}, {1252, 97},  {1255, 589}, {1257, 289}, {1260, 21},
    {1263, 77},  {1265, 119}, {1266, 7},   {1268, 345}, {1270, 333},
    {1271, 17},  {1273, 168}, {1276, 217}, {1278, 189}, {1279, 216},
    {1281, 229}, {1282, 231}, {1284, 223}, {1286, 153}, {1287, 470},
    {1289, 99},  {1294, 201}, {1295, 38},  {1297, 198}, {1298, 399},
    {1300, 75},  {1302, 77},  {1305, 326}, {1306, 39},  {1308, 495},
    {1310, 333}, {1311, 476}, {1313, 164}, {1314, 19},  {1319, 129},
    {1321, 52},  {1324, 337}, {1326, 397}, {1327, 277}, {1329, 73},
    {1332, 95},  {1334, 617}, {1335, 392}, {1337, 75},  {1338, 315},
    {1340, 125}, {1343, 348}, {1345, 553}, {1348, 553}, {1350, 237},
    {1351, 39},  {1353, 371}, {1354, 255}, {1356, 131}, {1358, 117},
    {1359, 98},  {1361, 56},  {1362, 655}, {1364, 239}, {1366, 1},
    {1367, 134}, {1369, 88},  {1372, 181}, {1374, 609}, {1375, 52},
    {1377, 100}, {1380, 183}, {1383, 130}, {1385, 12},  {1386, 219},
    {1388, 11},  {1390, 129}, {1391, 3},   {1393, 300}, {1396, 97},
    {1398, 601}, {1399, 55},  {1401, 92},  {1402, 127}, {1404, 81},
    {1407, 47},  {1409, 194}, {1410, 383}, {1412, 125}, {1414, 429},
    {1415, 282}, {1417, 342}, {1420, 33},  {1422, 49},  {1423, 15},
    {1425, 28},  {1426, 103}, {1428, 27},  {1430, 33},  {1431, 17},
    {1433, 387}, {1434, 363}, {1436, 83},  {1438, 357}, {1441, 322},
    {1442, 395}, {1444, 595}, {1446, 421}, {1447, 195}, {1449, 13},
    {1452, 315}, {1454, 297}, {1455, 52},  {1457, 314}, {1458, 243},
    {1460, 185}, {1463, 575}, {1465, 39},  {1466, 311}, {1468, 181},
    {1470, 49},  {1471, 25},  {1473, 77},  {1476, 21},  {1478, 69},
    {1479, 49},  {1481, 32},  {1482, 411}, {1486, 85},  {1487, 140},
    {1489, 252}, {1490, 279}, {1492, 307}, {1495, 94},  {1497, 49},
    {1500, 25},  {1503, 80},  {1505, 246}, {1508, 599}, {1510, 189},
    {1511, 278}, {1513, 399}, {1514, 299}, {1516, 277}, {1518, 69},
    {1519, 220}, {1521, 229}, {1524, 27},  {1526, 473}, {1527, 373},
    {1529, 60},  {1530, 207}, {1534, 225}, {1535, 404}, {1537, 46},
    {1540, 75},  {1542, 365}, {1543, 445}, {1545, 44},  {1548, 63},
    {1550, 189}, {1551, 557}, {1553, 252}, {1554, 99},  {1556, 65},
    {1558, 9},   {1559, 119}, {1561, 339}, {1562, 95},  {1564, 7},
    {1566, 77},  {1567, 127}, {1569, 319}, {1570, 667}, {1572, 501},
    {1575, 17},  {1577, 341}, {1578, 731}, {1580, 647}, {1582, 121},
    {1583, 20},  {1585, 574}, {1586, 399}, {1588, 85},  {1590, 169},
    {1591, 15},  {1593, 568}, {1596, 3},   {1599, 643}, {1601, 548},
    {1602, 783}, {1604, 317}, {1606, 153}, {1607, 87},  {1609, 231},
    {1612, 771}, {1615, 103}, {1617, 182}, {1618, 211}, {1620, 27},
    {1623, 17},  {1625, 69},  {1628, 603}, {1630, 741}, {1631, 668},
    {1633, 147}, {1634, 227}, {1636, 37},  {1638, 173}, {1639, 427},
    {1641, 287}, {1642, 231}, {1647, 310}, {1649, 434}, {1650, 579},
    {1652, 45},  {1655, 53},  {1657, 16},  {1660, 37},  {1663, 99},
    {1665, 176}, {1666, 271}, {1668, 459}, {1671, 202}, {1673, 90},
    {1674, 755}, {1676, 363}, {1678, 129}, {1679, 20},  {1681, 135},
    {1687, 31},  {1689, 758}, {1692, 359}, {1694, 501}, {1695, 29},
    {1697, 201}, {1698, 459}, {1700, 225}, {1703, 161}, {1705, 52},
    {1708, 93},  {1710, 201}, {1711, 178}, {1713, 250}, {1716, 221},
    {1719, 113}, {1721, 300}, {1722, 39},  {1724, 261}, {1726, 753},
    {1729, 94},  {1734, 461}, {1735, 418}, {1737, 403}, {1738, 267},
    {1740, 259}, {1742, 869}, {1743, 173}, {1745, 369}, {1746, 255},
    {1748, 567}, {1750, 457}, {1751, 482}, {1753, 775}, {1756, 99},
    {1759, 165}, {1764, 105}, {1767, 250}, {1769, 327}, {1770, 279},
    {1772, 371}, {1774, 117}, {1775, 486}, {1777, 217}, {1778, 635},
    {1780, 457}, {1782, 57},  {1783, 439}, {1785, 214}, {1788, 819},
    {1790, 593}, {1791, 190}, {1793, 114}, {1798, 69},  {1799, 312},
    {1801, 502}, {1802, 843}, {1804, 747}, {1806, 101}, {1807, 123},
    {1809, 521}, {1810, 171}, {1814, 545}, {1815, 163}, {1817, 479},
    {1818, 495}, {1820, 11},  {1823, 684}, {1825, 9},   {1828, 273},
    {1830, 381}, {1831, 51},  {1833, 518}, {1836, 243}, {1838, 53},
    {1839, 836}, {1841, 66},  {1844, 339}, {1846, 901}, {1847, 180},
    {1849, 49},  {1854, 885}, {1855, 39},  {1857, 688}, {1860, 13},
    {1862, 149}, {1863, 260}, {1865, 53},  {1866, 11},  {1870, 121},
    {1871, 261}, {1873, 199}, {1878, 253}, {1879, 174}, {1881, 370},
    {1884, 669}, {1886, 833}, {1887, 353}, {1889, 29},  {1890, 371},
    {1895, 873}, {1900, 235}, {1902, 733}, {1903, 778}, {1905, 344},
    {1906, 931}, {1908, 945}, {1911, 67},  {1913, 462}, {1918, 477},
    {1919, 105}, {1921, 468}, {1924, 327}, {1926, 357}, {1927, 25},
    {1929, 31},  {1932, 277}, {1934, 413}, {1935, 103}, {1937, 231},
    {1938, 747}, {1940, 113}, {1943, 11},  {1945, 91},  {1946, 51},
    {1948, 603}, {1950, 9},   {1951, 121}, {1953, 17},  {1956, 279},
    {1958, 89},  {1959, 371}, {1961, 771}, {1962, 99},  {1964, 21},
    {1966, 801}, {1967, 26},  {1969, 175}, {1974, 165}, {1975, 841},
    {1977, 238}, {1980, 33},  {1983, 113}, {1985, 311}, {1986, 891},
    {1988, 555}, {1990, 133}, {1991, 546}, {1993, 103}, {1994, 15},
    {1996, 307}, {1999, 367}};

polynomial_t ansi_pentanomials[] = {
    {160, 1, 2, 117},   {163, 1, 2, 8},     {164, 1, 2, 49},
    {165, 1, 2, 25},    {168, 1, 2, 65},    {171, 1, 3, 42},
    {173, 1, 2, 10},    {176, 1, 2, 43},    {179, 1, 2, 4},
    {181, 1, 2, 89},    {184, 1, 2, 81},    {187, 1, 2, 20},
    {188, 1, 2, 60},    {189, 1, 2, 49},    {190, 1, 2, 47},
    {192, 1, 2, 7},     {195, 1, 2, 37},    {197, 1, 2, 21},
    {200, 1, 2, 81},    {203, 1, 2, 45},    {205, 1, 2, 21},
    {206, 1, 2, 63},    {208, 1, 2, 83},    {211, 1, 2, 165},
    {213, 1, 2, 62},    {216, 1, 2, 107},   {219, 1, 2, 65},
    {221, 1, 2, 18},    {222, 1, 2, 73},    {224, 1, 2, 159},
    {226, 1, 2, 30},    {227, 1, 2, 21},    {229, 1, 2, 21},
    {230, 1, 2, 13},    {232, 1, 2, 23},    {235, 1, 2, 45},
    {237, 1, 2, 104},   {240, 1, 3, 49},    {243, 1, 2, 17},
    {245, 1, 2, 37},    {246, 1, 2, 11},    {248, 1, 2, 243},
    {251, 1, 2, 45},    {254, 1, 2, 7},     {256, 1, 2, 155},
    {259, 1, 2, 254},   {261, 1, 2, 74},    {262, 1, 2, 207},
    {264, 1, 2, 169},   {267, 1, 2, 29},    {269, 1, 2, 117},
    {272, 1, 3, 56},    {275, 1, 2, 28},    {277, 1, 2, 33},
    {280, 1, 2, 113},   {283, 1, 2, 200},   {285, 1, 2, 77},
    {288, 1, 2, 191},   {290, 1, 2, 70},    {291, 1, 2, 76},
    {293, 1, 3, 154},   {296, 1, 2, 123},   {298, 1, 2, 78},
    {299, 1, 2, 21},    {301, 1, 2, 26},    {304, 1, 2, 11},
    {306, 1, 2, 106},   {307, 1, 2, 93},    {309, 1, 2, 26},
    {311, 1, 3, 155},   {312, 1, 2, 83},    {315, 1, 2, 142},
    {317, 1, 3, 68},    {320, 1, 2, 7},     {323, 1, 2, 21},
    {325, 1, 2, 53},    {326, 1, 2, 67},    {328, 1, 2, 51},
    {331, 1, 2, 134},   {334, 1, 2, 5},     {335, 1, 2, 250},
    {336, 1, 2, 77},    {338, 1, 2, 112},   {339, 1, 2, 26},
    {341, 1, 2, 57},    {344, 1, 2, 7},     {347, 1, 2, 96},
    {349, 1, 2, 186},   {352, 1, 2, 263},   {355, 1, 2, 138},
    {356, 1, 2, 69},    {357, 1, 2, 28},    {360, 1, 2, 49},
    {361, 1, 2, 44},    {363, 1, 2, 38},    {365, 1, 2, 109},
    {368, 1, 2, 85},    {371, 1, 2, 156},   {373, 1, 3, 172},
    {374, 1, 2, 109},   {376, 1, 2, 77},    {379, 1, 2, 222},
    {381, 1, 2, 5},     {384, 1, 2, 299},   {387, 1, 2, 146},
    {389, 1, 2, 159},   {392, 1, 2, 145},   {395, 1, 2, 333},
    {397, 1, 2, 125},   {398, 1, 3, 23},    {400, 1, 2, 245},
    {403, 1, 2, 80},    {405, 1, 2, 38},    {408, 1, 2, 323},
    {410, 1, 2, 16},    {411, 1, 2, 50},    {413, 1, 2, 33},
    {416, 1, 3, 76},    {419, 1, 2, 129},   {421, 1, 2, 81},
    {424, 1, 2, 177},   {427, 1, 2, 245},   {429, 1, 2, 14},
    {430, 1, 2, 263},   {432, 1, 2, 103},   {434, 1, 2, 64},
    {435, 1, 2, 166},   {437, 1, 2, 6},     {440, 1, 2, 37},
    {442, 1, 2, 32},    {443, 1, 2, 57},    {445, 1, 2, 225},
    {448, 1, 3, 83},    {451, 1, 2, 33},    {452, 1, 2, 10},
    {453, 1, 2, 88},    {454, 1, 2, 195},   {456, 1, 2, 275},
    {459, 1, 2, 332},   {461, 1, 2, 247},   {464, 1, 2, 310},
    {466, 1, 2, 78},    {467, 1, 2, 210},   {469, 1, 2, 149},
    {472, 1, 2, 33},    {475, 1, 2, 68},    {477, 1, 2, 121},
    {480, 1, 2, 149},   {482, 1, 2, 13},    {483, 1, 2, 352},
    {485, 1, 2, 70},    {488, 1, 2, 123},   {491, 1, 2, 270},
    {493, 1, 2, 171},   {496, 1, 3, 52},    {499, 1, 2, 174},
    {501, 1, 2, 332},   {502, 1, 2, 99},    {504, 1, 3, 148},
    {507, 1, 2, 26},    {509, 1, 2, 94},    {512, 1, 2, 51},
    {515, 1, 2, 73},    {517, 1, 2, 333},   {520, 1, 2, 291},
    {523, 1, 2, 66},    {525, 1, 2, 92},    {528, 1, 2, 35},
    {530, 1, 2, 25},    {531, 1, 2, 53},    {533, 1, 2, 37},
    {535, 1, 2, 143},   {536, 1, 2, 165},   {539, 1, 2, 37},
    {541, 1, 2, 36},    {542, 1, 3, 212},   {544, 1, 2, 87},
    {546, 1, 2, 8},     {547, 1, 2, 165},   {548, 1, 2, 385},
    {549, 1, 3, 274},   {552, 1, 2, 41},    {554, 1, 2, 162},
    {555, 1, 2, 326},   {557, 1, 2, 288},   {560, 1, 2, 157},
    {562, 1, 2, 56},    {563, 1, 4, 159},   {565, 1, 2, 66},
    {568, 1, 2, 291},   {571, 1, 2, 408},   {572, 1, 2, 238},
    {573, 1, 2, 220},   {576, 1, 3, 52},    {578, 1, 2, 138},
    {579, 1, 3, 526},   {581, 1, 2, 138},   {584, 1, 2, 361},
    {586, 1, 2, 14},    {587, 1, 2, 130},   {589, 1, 2, 365},
    {591, 1, 2, 38},    {592, 1, 2, 143},   {595, 1, 2, 9},
    {597, 1, 2, 64},    {598, 1, 2, 131},   {600, 1, 2, 239},
    {603, 1, 2, 446},   {605, 1, 2, 312},   {608, 1, 2, 213},
    {611, 1, 2, 13},    {613, 1, 2, 377},   {616, 1, 2, 465},
    {619, 1, 2, 494},   {621, 1, 2, 17},    {624, 1, 2, 71},
    {627, 1, 2, 37},    {629, 1, 2, 121},   {630, 1, 2, 49},
    {632, 1, 2, 9},     {635, 1, 2, 64},    {637, 1, 2, 84},
    {638, 1, 2, 127},   {640, 1, 3, 253},   {643, 1, 2, 153},
    {644, 1, 2, 24},    {645, 1, 2, 473},   {648, 1, 2, 235},
    {653, 1, 2, 37},    {656, 1, 2, 39},    {659, 1, 2, 25},
    {661, 1, 2, 80},    {664, 1, 2, 177},   {666, 1, 2, 100},
    {667, 1, 2, 161},   {669, 1, 2, 314},   {672, 1, 2, 91},
    {674, 1, 2, 22},    {675, 1, 2, 214},   {677, 1, 2, 325},
    {678, 1, 2, 95},    {680, 1, 2, 91},    {681, 1, 2, 83},
    {683, 1, 2, 153},   {685, 1, 3, 4},     {688, 1, 2, 71},
    {691, 1, 2, 242},   {693, 1, 2, 250},   {696, 1, 2, 241},
    {699, 1, 2, 40},    {701, 1, 2, 466},   {703, 1, 2, 123},
    {704, 1, 2, 277},   {706, 1, 2, 27},    {707, 1, 2, 141},
    {709, 1, 2, 9},     {710, 1, 3, 29},    {712, 1, 2, 623},
    {715, 1, 3, 458},   {717, 1, 2, 320},   {720, 1, 2, 625},
    {723, 1, 2, 268},   {725, 1, 2, 331},   {728, 1, 2, 51},
    {731, 1, 2, 69},    {733, 1, 2, 92},    {734, 1, 2, 67},
    {736, 1, 2, 359},   {739, 1, 2, 60},    {741, 1, 2, 34},
    {744, 1, 2, 347},   {747, 1, 2, 158},   {749, 1, 2, 357},
    {752, 1, 2, 129},   {755, 1, 4, 159},   {757, 1, 2, 359},
    {760, 1, 2, 17},    {763, 1, 2, 17},    {764, 1, 2, 12},
    {765, 1, 2, 137},   {766, 1, 3, 280},   {768, 1, 2, 115},
    {770, 1, 2, 453},   {771, 1, 2, 86},    {773, 1, 2, 73},
    {776, 1, 2, 51},    {779, 1, 2, 456},   {781, 1, 2, 209},
    {784, 1, 2, 59},    {786, 1, 2, 118},   {787, 1, 2, 189},
    {788, 1, 2, 375},   {789, 1, 2, 5},     {790, 1, 2, 111},
    {792, 1, 2, 403},   {795, 1, 2, 137},   {796, 1, 2, 36},
    {797, 1, 2, 193},   {800, 1, 2, 463},   {802, 1, 2, 102},
    {803, 1, 2, 208},   {805, 1, 2, 453},   {808, 1, 3, 175},
    {811, 1, 2, 18},    {813, 1, 2, 802},   {816, 1, 3, 51},
    {819, 1, 2, 149},   {821, 1, 2, 177},   {824, 1, 2, 495},
    {827, 1, 2, 189},   {829, 1, 2, 560},   {830, 1, 2, 241},
    {832, 1, 2, 39},    {835, 1, 2, 350},   {836, 1, 2, 606},
    {837, 1, 2, 365},   {840, 1, 2, 341},   {843, 1, 2, 322},
    {848, 1, 2, 225},   {851, 1, 2, 442},   {853, 1, 2, 461},
    {854, 1, 2, 79},    {856, 1, 2, 842},   {859, 1, 2, 594},
    {863, 1, 2, 90},    {864, 1, 2, 607},   {867, 1, 2, 380},
    {869, 1, 2, 82},    {872, 1, 2, 691},   {874, 1, 2, 110},
    {875, 1, 2, 66},    {877, 1, 2, 140},   {878, 1, 2, 343},
    {880, 1, 3, 221},   {883, 1, 2, 488},   {885, 1, 2, 707},
    {886, 1, 2, 227},   {888, 1, 2, 97},    {891, 1, 2, 364},
    {893, 1, 2, 13},    {896, 1, 2, 19},    {899, 1, 3, 898},
    {901, 1, 2, 581},   {904, 1, 3, 60},    {907, 1, 3, 26},
    {909, 1, 3, 168},   {910, 1, 2, 357},   {912, 1, 2, 569},
    {914, 1, 2, 4},     {915, 1, 2, 89},    {917, 1, 2, 22},
    {920, 1, 3, 517},   {922, 1, 2, 24},    {923, 1, 2, 142},
    {925, 1, 2, 308},   {928, 1, 2, 33},    {929, 1, 2, 36},
    {931, 1, 2, 72},    {933, 1, 2, 527},   {934, 1, 3, 800},
    {936, 1, 3, 27},    {939, 1, 2, 142},   {940, 1, 2, 204},
    {941, 1, 2, 573},   {944, 1, 2, 487},   {946, 1, 3, 83},
    {947, 1, 2, 400},   {949, 1, 2, 417},   {950, 1, 2, 859},
    {952, 1, 3, 311},   {955, 1, 2, 606},   {957, 1, 2, 158},
    {958, 1, 2, 191},   {960, 1, 2, 491},   {962, 1, 2, 18},
    {963, 1, 2, 145},   {965, 1, 2, 213},   {968, 1, 2, 21},
    {970, 1, 2, 260},   {971, 1, 2, 6},     {973, 1, 2, 113},
    {974, 1, 2, 211},   {976, 1, 2, 285},   {978, 1, 2, 376},
    {980, 1, 2, 316},   {981, 1, 2, 383},   {984, 1, 2, 349},
    {987, 1, 3, 142},   {989, 1, 2, 105},   {992, 1, 2, 585},
    {995, 1, 3, 242},   {997, 1, 2, 453},   {1000, 1, 3, 68},
    {1002, 1, 2, 266},  {1003, 1, 2, 410},  {1004, 1, 2, 96},
    {1005, 1, 2, 41},   {1006, 1, 2, 63},   {1008, 1, 2, 703},
    {1011, 1, 2, 17},   {1013, 1, 2, 180},  {1016, 1, 2, 49},
    {1017, 1, 2, 746},  {1018, 1, 2, 27},   {1019, 1, 2, 96},
    {1021, 1, 2, 5},    {1024, 1, 2, 515},  {1027, 1, 2, 378},
    {1032, 1, 2, 901},  {1035, 1, 2, 76},   {1037, 1, 2, 981},
    {1038, 1, 2, 41},   {1040, 1, 2, 429},  {1043, 1, 3, 869},
    {1045, 1, 2, 378},  {1046, 1, 2, 39},   {1048, 1, 3, 172},
    {1051, 1, 3, 354},  {1053, 1, 2, 290},  {1056, 1, 2, 11},
    {1059, 1, 3, 6},    {1061, 1, 2, 166},  {1064, 1, 2, 946},
    {1066, 1, 2, 258},  {1067, 1, 2, 69},   {1068, 1, 2, 223},
    {1069, 1, 2, 146},  {1070, 1, 3, 94},   {1072, 1, 2, 443},
    {1073, 1, 3, 235},  {1074, 1, 2, 395},  {1075, 1, 2, 92},
    {1076, 1, 2, 22},   {1077, 1, 2, 521},  {1080, 1, 2, 151},
    {1083, 1, 2, 538},  {1088, 1, 2, 531},  {1091, 1, 2, 82},
    {1093, 1, 2, 173},  {1096, 1, 2, 351},  {1099, 1, 2, 464},
    {1101, 1, 2, 14},   {1104, 1, 2, 259},  {1107, 1, 2, 176},
    {1109, 1, 2, 501},  {1112, 1, 2, 1045}, {1114, 1, 2, 345},
    {1115, 1, 2, 268},  {1117, 1, 2, 149},  {1118, 1, 2, 475},
    {1120, 1, 3, 386},  {1123, 1, 2, 641},  {1124, 1, 2, 156},
    {1125, 1, 2, 206},  {1128, 1, 3, 7},    {1131, 1, 2, 188},
    {1132, 1, 2, 20},   {1133, 1, 2, 667},  {1136, 1, 2, 177},
    {1139, 1, 2, 45},   {1141, 1, 2, 134},  {1143, 1, 2, 7},
    {1144, 1, 2, 431},  {1147, 1, 2, 390},  {1149, 1, 2, 221},
    {1150, 1, 2, 63},   {1152, 1, 2, 971},  {1155, 1, 2, 94},
    {1157, 1, 2, 105},  {1160, 1, 2, 889},  {1162, 1, 2, 288},
    {1163, 1, 2, 33},   {1165, 1, 2, 494},  {1168, 1, 2, 473},
    {1171, 1, 2, 396},  {1172, 1, 2, 426},  {1173, 1, 2, 673},
    {1176, 1, 2, 19},   {1179, 1, 2, 640},  {1181, 1, 2, 82},
    {1184, 1, 2, 1177}, {1187, 1, 2, 438},  {1189, 1, 2, 102},
    {1192, 1, 3, 831},  {1194, 1, 2, 317},  {1195, 1, 2, 293},
    {1197, 1, 2, 269},  {1200, 1, 3, 739},  {1203, 1, 2, 226},
    {1205, 1, 2, 4},    {1208, 1, 2, 915},  {1211, 1, 2, 373},
    {1213, 1, 2, 245},  {1216, 1, 2, 155},  {1219, 1, 2, 225},
    {1221, 1, 2, 101},  {1222, 1, 2, 215},  {1224, 1, 2, 157},
    {1227, 1, 2, 361},  {1229, 1, 2, 627},  {1232, 1, 2, 225},
    {1235, 1, 2, 642},  {1237, 1, 2, 150},  {1240, 1, 2, 567},
    {1243, 1, 2, 758},  {1244, 1, 2, 126},  {1245, 1, 2, 212},
    {1248, 1, 2, 1201}, {1250, 1, 2, 37},   {1251, 1, 2, 1004},
    {1253, 1, 2, 141},  {1254, 1, 2, 697},  {1256, 1, 2, 171},
    {1258, 1, 2, 503},  {1259, 1, 2, 192},  {1261, 1, 2, 14},
    {1262, 1, 2, 793},  {1264, 1, 2, 285},  {1267, 1, 2, 197},
    {1269, 1, 2, 484},  {1272, 1, 2, 223},  {1274, 1, 2, 486},
    {1275, 1, 2, 25},   {1277, 1, 2, 451},  {1280, 1, 2, 843},
    {1283, 1, 2, 70},   {1285, 1, 2, 564},  {1288, 1, 2, 215},
    {1290, 1, 2, 422},  {1291, 1, 2, 245},  {1292, 1, 2, 78},
    {1293, 1, 2, 26},   {1296, 1, 2, 379},  {1299, 1, 2, 172},
    {1301, 1, 2, 297},  {1303, 1, 2, 306},  {1304, 1, 3, 574},
    {1307, 1, 2, 157},  {1309, 1, 2, 789},  {1312, 1, 2, 1265},
    {1315, 1, 2, 270},  {1316, 1, 2, 12},   {1317, 1, 2, 254},
    {1318, 1, 3, 94},   {1320, 1, 2, 835},  {1322, 1, 2, 538},
    {1323, 1, 2, 1198}, {1325, 1, 2, 526},  {1328, 1, 2, 507},
    {1330, 1, 2, 609},  {1331, 1, 2, 289},  {1333, 1, 2, 276},
    {1336, 1, 2, 815},  {1339, 1, 2, 284},  {1341, 1, 2, 53},
    {1342, 1, 2, 477},  {1344, 1, 2, 469},  {1346, 1, 2, 57},
    {1347, 1, 2, 61},   {1349, 1, 2, 40},   {1352, 1, 2, 583},
    {1355, 1, 2, 117},  {1357, 1, 2, 495},  {1360, 1, 2, 393},
    {1363, 1, 2, 852},  {1365, 1, 2, 329},  {1368, 1, 2, 41},
    {1370, 1, 2, 108},  {1371, 1, 2, 145},  {1373, 1, 2, 613},
    {1376, 1, 2, 1201}, {1378, 1, 2, 362},  {1379, 1, 2, 400},
    {1381, 1, 2, 56},   {1382, 1, 3, 58},   {1384, 1, 2, 1131},
    {1387, 1, 2, 33},   {1389, 1, 2, 41},   {1392, 1, 2, 485},
    {1394, 1, 2, 30},   {1395, 1, 2, 233},  {1397, 1, 2, 397},
    {1400, 1, 2, 493},  {1403, 1, 2, 717},  {1405, 1, 2, 558},
    {1406, 1, 2, 13},   {1408, 1, 3, 45},   {1411, 1, 2, 200},
    {1413, 1, 2, 101},  {1416, 1, 3, 231},  {1418, 1, 2, 283},
    {1419, 1, 2, 592},  {1421, 1, 2, 30},   {1424, 1, 2, 507},
    {1427, 1, 2, 900},  {1429, 1, 2, 149},  {1432, 1, 2, 251},
    {1435, 1, 2, 126},  {1437, 1, 2, 545},  {1439, 1, 2, 535},
    {1440, 1, 3, 1023}, {1443, 1, 2, 413},  {1445, 1, 2, 214},
    {1448, 1, 3, 212},  {1450, 1, 2, 155},  {1451, 1, 2, 193},
    {1453, 1, 2, 348},  {1456, 1, 2, 1011}, {1459, 1, 2, 1032},
    {1461, 1, 2, 446},  {1462, 1, 2, 165},  {1464, 1, 2, 275},
    {1467, 1, 2, 113},  {1469, 1, 2, 775},  {1472, 1, 2, 613},
    {1474, 1, 2, 59},   {1475, 1, 2, 208},  {1477, 1, 2, 1325},
    {1480, 1, 2, 285},  {1483, 1, 2, 1077}, {1484, 1, 2, 61},
    {1485, 1, 2, 655},  {1488, 1, 2, 463},  {1491, 1, 2, 544},
    {1493, 1, 2, 378},  {1494, 1, 2, 731},  {1496, 1, 2, 181},
    {1498, 1, 2, 416},  {1499, 1, 2, 477},  {1501, 1, 2, 60},
    {1502, 1, 2, 111},  {1504, 1, 2, 207},  {1506, 1, 2, 533},
    {1507, 1, 2, 900},  {1509, 1, 2, 209},  {1512, 1, 2, 1121},
    {1515, 1, 2, 712},  {1517, 1, 2, 568},  {1520, 1, 2, 81},
    {1522, 1, 2, 47},   {1523, 1, 2, 240},  {1525, 1, 2, 102},
    {1528, 1, 2, 923},  {1531, 1, 2, 1125}, {1532, 1, 2, 466},
    {1533, 1, 2, 763},  {1536, 1, 2, 881},  {1538, 1, 2, 6},
    {1539, 1, 2, 80},   {1541, 1, 2, 4},    {1544, 1, 2, 99},
    {1546, 1, 2, 810},  {1547, 1, 2, 493},  {1549, 1, 2, 426},
    {1552, 1, 2, 83},   {1555, 1, 2, 254},  {1557, 1, 2, 20},
    {1560, 1, 2, 11},   {1563, 1, 2, 41},   {1565, 1, 2, 18},
    {1568, 1, 2, 133},  {1571, 1, 2, 21},   {1573, 1, 2, 461},
    {1574, 1, 2, 331},  {1576, 1, 2, 147},  {1579, 1, 2, 374},
    {1581, 1, 2, 160},  {1584, 1, 2, 895},  {1587, 1, 2, 433},
    {1589, 1, 2, 882},  {1592, 1, 2, 223},  {1594, 1, 2, 971},
    {1595, 1, 2, 18},   {1597, 1, 2, 42},   {1598, 1, 2, 385},
    {1600, 1, 2, 57},   {1603, 1, 2, 917},  {1605, 1, 2, 46},
    {1608, 1, 2, 271},  {1610, 1, 2, 250},  {1611, 1, 2, 58},
    {1613, 1, 2, 48},   {1614, 1, 2, 1489}, {1616, 1, 2, 139},
    {1619, 1, 2, 289},  {1621, 1, 2, 1577}, {1622, 1, 2, 1341},
    {1624, 1, 2, 1095}, {1626, 1, 2, 191},  {1627, 1, 2, 189},
    {1629, 1, 2, 397},  {1632, 1, 2, 211},  {1635, 1, 2, 113},
    {1637, 1, 2, 234},  {1640, 1, 2, 715},  {1643, 1, 2, 760},
    {1644, 1, 2, 236},  {1645, 1, 2, 938},  {1646, 1, 2, 435},
    {1648, 1, 2, 77},   {1651, 1, 2, 873},  {1653, 1, 2, 82},
    {1654, 1, 3, 201},  {1656, 1, 2, 361},  {1658, 1, 2, 552},
    {1659, 1, 2, 374},  {1661, 1, 2, 84},   {1662, 1, 3, 958},
    {1664, 1, 2, 399},  {1667, 1, 2, 1020}, {1669, 1, 2, 425},
    {1670, 1, 2, 19},   {1672, 1, 2, 405},  {1675, 1, 2, 77},
    {1677, 1, 2, 844},  {1680, 1, 2, 1549}, {1682, 1, 2, 354},
    {1683, 1, 2, 1348}, {1684, 1, 2, 474},  {1685, 1, 2, 493},
    {1686, 1, 2, 887},  {1688, 1, 2, 921},  {1690, 1, 2, 200},
    {1691, 1, 2, 556},  {1693, 1, 2, 137},  {1696, 1, 2, 737},
    {1699, 1, 2, 405},  {1701, 1, 2, 568},  {1702, 1, 2, 245},
    {1704, 1, 3, 55},   {1706, 1, 2, 574},  {1707, 1, 2, 221},
    {1709, 1, 2, 201},  {1712, 1, 2, 445},  {1714, 1, 2, 191},
    {1715, 1, 2, 612},  {1717, 1, 2, 881},  {1718, 1, 2, 535},
    {1720, 1, 2, 525},  {1723, 1, 2, 137},  {1725, 1, 2, 623},
    {1727, 1, 2, 22},   {1728, 1, 2, 545},  {1730, 1, 2, 316},
    {1731, 1, 2, 925},  {1732, 1, 2, 75},   {1733, 1, 2, 285},
    {1736, 1, 2, 435},  {1739, 1, 2, 409},  {1741, 1, 3, 226},
    {1744, 1, 2, 35},   {1747, 1, 2, 93},   {1749, 1, 2, 236},
    {1752, 1, 2, 559},  {1754, 1, 2, 75},   {1755, 1, 2, 316},
    {1757, 1, 2, 21},   {1758, 1, 2, 221},  {1760, 1, 3, 1612},
    {1761, 1, 2, 131},  {1762, 1, 2, 318},  {1763, 1, 2, 345},
    {1765, 1, 2, 165},  {1766, 1, 2, 1029}, {1768, 1, 2, 1403},
    {1771, 1, 2, 297},  {1773, 1, 2, 50},   {1776, 1, 2, 17},
    {1779, 1, 3, 1068}, {1781, 1, 2, 18},   {1784, 1, 2, 1489},
    {1786, 1, 2, 614},  {1787, 1, 2, 457},  {1789, 1, 2, 80},
    {1792, 1, 2, 341},  {1794, 1, 2, 95},   {1795, 1, 2, 89},
    {1796, 1, 2, 829},  {1797, 1, 2, 80},   {1800, 1, 2, 1013},
    {1803, 1, 2, 248},  {1805, 1, 2, 82},   {1808, 1, 2, 25},
    {1811, 1, 2, 117},  {1812, 1, 2, 758},  {1813, 1, 3, 884},
    {1816, 1, 2, 887},  {1819, 1, 2, 116},  {1821, 1, 2, 326},
    {1822, 1, 3, 31},   {1824, 1, 2, 821},  {1826, 1, 2, 298},
    {1827, 1, 2, 154},  {1829, 1, 2, 162},  {1832, 1, 3, 1078},
    {1834, 1, 2, 210},  {1835, 1, 2, 288},  {1837, 1, 2, 200},
    {1840, 1, 2, 195},  {1842, 1, 2, 799},  {1843, 1, 2, 872},
    {1845, 1, 2, 526},  {1848, 1, 2, 871},  {1850, 1, 2, 79},
    {1851, 1, 2, 250},  {1852, 1, 2, 339},  {1853, 1, 2, 705},
    {1856, 1, 2, 585},  {1858, 1, 2, 1368}, {1859, 1, 2, 120},
    {1861, 1, 2, 509},  {1864, 1, 2, 1379}, {1867, 1, 2, 117},
    {1868, 1, 2, 250},  {1869, 1, 2, 617},  {1872, 1, 3, 60},
    {1874, 1, 2, 70},   {1875, 1, 2, 412},  {1876, 1, 2, 122},
    {1877, 1, 2, 796},  {1880, 1, 2, 1647}, {1882, 1, 2, 128},
    {1883, 1, 2, 1062}, {1885, 1, 2, 813},  {1888, 1, 2, 923},
    {1891, 1, 2, 1766}, {1892, 1, 3, 497},  {1893, 1, 2, 461},
    {1894, 1, 3, 215},  {1896, 1, 2, 451},  {1897, 1, 2, 324},
    {1898, 1, 2, 613},  {1899, 1, 2, 485},  {1901, 1, 2, 330},
    {1904, 1, 2, 337},  {1907, 1, 2, 45},   {1909, 1, 2, 225},
    {1910, 1, 3, 365},  {1912, 1, 2, 599},  {1914, 1, 2, 544},
    {1915, 1, 2, 473},  {1916, 1, 2, 502},  {1917, 1, 2, 485},
    {1920, 1, 2, 67},   {1922, 1, 2, 36},   {1923, 1, 4, 40},
    {1925, 1, 2, 576},  {1928, 1, 2, 763},  {1930, 1, 2, 155},
    {1931, 1, 2, 648},  {1933, 1, 2, 971},  {1936, 1, 2, 117},
    {1939, 1, 2, 5},    {1941, 1, 2, 1133}, {1942, 1, 2, 147},
    {1944, 1, 2, 617},  {1947, 1, 2, 1162}, {1949, 1, 2, 621},
    {1952, 1, 3, 65},   {1954, 1, 2, 1226}, {1955, 1, 2, 109},
    {1957, 1, 2, 17},   {1960, 1, 2, 939},  {1963, 1, 2, 1137},
    {1965, 1, 2, 364},  {1968, 1, 3, 922},  {1970, 1, 2, 388},
    {1971, 1, 2, 100},  {1972, 1, 2, 474},  {1973, 1, 2, 438},
    {1976, 1, 3, 1160}, {1978, 1, 2, 158},  {1979, 1, 2, 369},
    {1981, 1, 2, 96},   {1982, 1, 2, 1027}, {1984, 1, 2, 129},
    {1987, 1, 2, 80},   {1989, 1, 2, 719},  {1992, 1, 2, 1241},
    {1995, 1, 2, 37},   {1997, 1, 2, 835},  {1998, 1, 3, 1290},
    {2000, 1, 2, 981}};

int compare_poly(const void *a, const void *b) {
	const polynomial_t *one = a;
	const polynomial_t *other = b;
	return (one->m - other->m);
}

bool poly_exists(long m) { return m >= 160 && m <= 2000; }

polynomial_t *poly_find(long m) {
	if (!poly_exists(m)) {
		return NULL;
	}
	polynomial_t searched = {(int)m};
	polynomial_t *tri =
	    (polynomial_t *)bsearch(&searched, ansi_trinomials,
	                            sizeof(ansi_trinomials) / sizeof(polynomial_t),
	                            sizeof(polynomial_t), &compare_poly);
	if (tri) {
		return tri;
	} else {
		return (polynomial_t *)bsearch(
		    &searched, ansi_pentanomials,
		    sizeof(ansi_pentanomials) / sizeof(polynomial_t),
		    sizeof(polynomial_t), &compare_poly);
	}
}

GEN poly_find_gen(long m) { return poly_gen(poly_find(m)); }

GEN poly_gen(polynomial_t *polynomial) {
	pari_sp ltop = avma;

	GEN coeffs = gtovec0(gen_0, polynomial->m + 1);
	gel(coeffs, polynomial->m + 1) = gen_1;
	gel(coeffs, polynomial->e1 + 1) = gen_1;
	gel(coeffs, polynomial->e2 + 1) = gen_1;
	gel(coeffs, polynomial->e3 + 1) = gen_1;
	gel(coeffs, 1) = gen_1;

	GEN poly = gmul(gtopolyrev(coeffs, -1), gmodulss(1, 2));
	return gerepilecopy(ltop, ffgen(poly, -1));
}
