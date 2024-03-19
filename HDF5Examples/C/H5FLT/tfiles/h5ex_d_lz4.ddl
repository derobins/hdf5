HDF5 "h5ex_d_lz4.h5" {
GROUP "/" {
   DATASET "DS1" {
      DATATYPE  H5T_STD_I32LE
      DATASPACE  SIMPLE { ( 32, 64 ) / ( 32, 64 ) }
      STORAGE_LAYOUT {
         CHUNKED ( 4, 8 )
         SIZE 19968 (0.410:1 COMPRESSION)
      }
      FILTERS {
         USER_DEFINED_FILTER {
            FILTER_ID 32004
            COMMENT HDF5 lz4 filter; see https://github.com/HDFGroup/hdf5_plugins/blob/master/docs/RegisteredFilterPlugins.md
            PARAMS { 3 }
         }
      }
      FILLVALUE {
         FILL_TIME H5D_FILL_TIME_IFSET
         VALUE  H5D_FILL_VALUE_DEFAULT
      }
      ALLOCATION_TIME {
         H5D_ALLOC_TIME_INCR
      }
      DATA {
      (0,0): 0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14,
      (0,15): -15, -16, -17, -18, -19, -20, -21, -22, -23, -24, -25, -26,
      (0,27): -27, -28, -29, -30, -31, -32, -33, -34, -35, -36, -37, -38,
      (0,39): -39, -40, -41, -42, -43, -44, -45, -46, -47, -48, -49, -50,
      (0,51): -51, -52, -53, -54, -55, -56, -57, -58, -59, -60, -61, -62,
      (0,63): -63,
      (1,0): 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      (1,21): 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      (1,42): 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      (1,63): 0,
      (2,0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
      (2,18): 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,
      (2,34): 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
      (2,50): 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
      (3,0): 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32,
      (3,17): 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64,
      (3,33): 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94, 96,
      (3,49): 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122,
      (3,62): 124, 126,
      (4,0): 0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48,
      (4,17): 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 90, 93, 96,
      (4,33): 99, 102, 105, 108, 111, 114, 117, 120, 123, 126, 129, 132, 135,
      (4,46): 138, 141, 144, 147, 150, 153, 156, 159, 162, 165, 168, 171,
      (4,58): 174, 177, 180, 183, 186, 189,
      (5,0): 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64,
      (5,17): 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120,
      (5,31): 124, 128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168,
      (5,43): 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216,
      (5,55): 220, 224, 228, 232, 236, 240, 244, 248, 252,
      (6,0): 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75,
      (6,16): 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140,
      (6,29): 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200,
      (6,41): 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255, 260,
      (6,53): 265, 270, 275, 280, 285, 290, 295, 300, 305, 310, 315,
      (7,0): 0, 6, 12, 18, 24, 30, 36, 42, 48, 54, 60, 66, 72, 78, 84, 90,
      (7,16): 96, 102, 108, 114, 120, 126, 132, 138, 144, 150, 156, 162, 168,
      (7,29): 174, 180, 186, 192, 198, 204, 210, 216, 222, 228, 234, 240,
      (7,41): 246, 252, 258, 264, 270, 276, 282, 288, 294, 300, 306, 312,
      (7,53): 318, 324, 330, 336, 342, 348, 354, 360, 366, 372, 378,
      (8,0): 0, 7, 14, 21, 28, 35, 42, 49, 56, 63, 70, 77, 84, 91, 98, 105,
      (8,16): 112, 119, 126, 133, 140, 147, 154, 161, 168, 175, 182, 189,
      (8,28): 196, 203, 210, 217, 224, 231, 238, 245, 252, 259, 266, 273,
      (8,40): 280, 287, 294, 301, 308, 315, 322, 329, 336, 343, 350, 357,
      (8,52): 364, 371, 378, 385, 392, 399, 406, 413, 420, 427, 434, 441,
      (9,0): 0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120,
      (9,16): 128, 136, 144, 152, 160, 168, 176, 184, 192, 200, 208, 216,
      (9,28): 224, 232, 240, 248, 256, 264, 272, 280, 288, 296, 304, 312,
      (9,40): 320, 328, 336, 344, 352, 360, 368, 376, 384, 392, 400, 408,
      (9,52): 416, 424, 432, 440, 448, 456, 464, 472, 480, 488, 496, 504,
      (10,0): 0, 9, 18, 27, 36, 45, 54, 63, 72, 81, 90, 99, 108, 117, 126,
      (10,15): 135, 144, 153, 162, 171, 180, 189, 198, 207, 216, 225, 234,
      (10,27): 243, 252, 261, 270, 279, 288, 297, 306, 315, 324, 333, 342,
      (10,39): 351, 360, 369, 378, 387, 396, 405, 414, 423, 432, 441, 450,
      (10,51): 459, 468, 477, 486, 495, 504, 513, 522, 531, 540, 549, 558,
      (10,63): 567,
      (11,0): 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140,
      (11,15): 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260,
      (11,27): 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 380,
      (11,39): 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500,
      (11,51): 510, 520, 530, 540, 550, 560, 570, 580, 590, 600, 610, 620,
      (11,63): 630,
      (12,0): 0, 11, 22, 33, 44, 55, 66, 77, 88, 99, 110, 121, 132, 143, 154,
      (12,15): 165, 176, 187, 198, 209, 220, 231, 242, 253, 264, 275, 286,
      (12,27): 297, 308, 319, 330, 341, 352, 363, 374, 385, 396, 407, 418,
      (12,39): 429, 440, 451, 462, 473, 484, 495, 506, 517, 528, 539, 550,
      (12,51): 561, 572, 583, 594, 605, 616, 627, 638, 649, 660, 671, 682,
      (12,63): 693,
      (13,0): 0, 12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 132, 144, 156,
      (13,14): 168, 180, 192, 204, 216, 228, 240, 252, 264, 276, 288, 300,
      (13,26): 312, 324, 336, 348, 360, 372, 384, 396, 408, 420, 432, 444,
      (13,38): 456, 468, 480, 492, 504, 516, 528, 540, 552, 564, 576, 588,
      (13,50): 600, 612, 624, 636, 648, 660, 672, 684, 696, 708, 720, 732,
      (13,62): 744, 756,
      (14,0): 0, 13, 26, 39, 52, 65, 78, 91, 104, 117, 130, 143, 156, 169,
      (14,14): 182, 195, 208, 221, 234, 247, 260, 273, 286, 299, 312, 325,
      (14,26): 338, 351, 364, 377, 390, 403, 416, 429, 442, 455, 468, 481,
      (14,38): 494, 507, 520, 533, 546, 559, 572, 585, 598, 611, 624, 637,
      (14,50): 650, 663, 676, 689, 702, 715, 728, 741, 754, 767, 780, 793,
      (14,62): 806, 819,
      (15,0): 0, 14, 28, 42, 56, 70, 84, 98, 112, 126, 140, 154, 168, 182,
      (15,14): 196, 210, 224, 238, 252, 266, 280, 294, 308, 322, 336, 350,
      (15,26): 364, 378, 392, 406, 420, 434, 448, 462, 476, 490, 504, 518,
      (15,38): 532, 546, 560, 574, 588, 602, 616, 630, 644, 658, 672, 686,
      (15,50): 700, 714, 728, 742, 756, 770, 784, 798, 812, 826, 840, 854,
      (15,62): 868, 882,
      (16,0): 0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195,
      (16,14): 210, 225, 240, 255, 270, 285, 300, 315, 330, 345, 360, 375,
      (16,26): 390, 405, 420, 435, 450, 465, 480, 495, 510, 525, 540, 555,
      (16,38): 570, 585, 600, 615, 630, 645, 660, 675, 690, 705, 720, 735,
      (16,50): 750, 765, 780, 795, 810, 825, 840, 855, 870, 885, 900, 915,
      (16,62): 930, 945,
      (17,0): 0, 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208,
      (17,14): 224, 240, 256, 272, 288, 304, 320, 336, 352, 368, 384, 400,
      (17,26): 416, 432, 448, 464, 480, 496, 512, 528, 544, 560, 576, 592,
      (17,38): 608, 624, 640, 656, 672, 688, 704, 720, 736, 752, 768, 784,
      (17,50): 800, 816, 832, 848, 864, 880, 896, 912, 928, 944, 960, 976,
      (17,62): 992, 1008,
      (18,0): 0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221,
      (18,14): 238, 255, 272, 289, 306, 323, 340, 357, 374, 391, 408, 425,
      (18,26): 442, 459, 476, 493, 510, 527, 544, 561, 578, 595, 612, 629,
      (18,38): 646, 663, 680, 697, 714, 731, 748, 765, 782, 799, 816, 833,
      (18,50): 850, 867, 884, 901, 918, 935, 952, 969, 986, 1003, 1020, 1037,
      (18,62): 1054, 1071,
      (19,0): 0, 18, 36, 54, 72, 90, 108, 126, 144, 162, 180, 198, 216, 234,
      (19,14): 252, 270, 288, 306, 324, 342, 360, 378, 396, 414, 432, 450,
      (19,26): 468, 486, 504, 522, 540, 558, 576, 594, 612, 630, 648, 666,
      (19,38): 684, 702, 720, 738, 756, 774, 792, 810, 828, 846, 864, 882,
      (19,50): 900, 918, 936, 954, 972, 990, 1008, 1026, 1044, 1062, 1080,
      (19,61): 1098, 1116, 1134,
      (20,0): 0, 19, 38, 57, 76, 95, 114, 133, 152, 171, 190, 209, 228, 247,
      (20,14): 266, 285, 304, 323, 342, 361, 380, 399, 418, 437, 456, 475,
      (20,26): 494, 513, 532, 551, 570, 589, 608, 627, 646, 665, 684, 703,
      (20,38): 722, 741, 760, 779, 798, 817, 836, 855, 874, 893, 912, 931,
      (20,50): 950, 969, 988, 1007, 1026, 1045, 1064, 1083, 1102, 1121, 1140,
      (20,61): 1159, 1178, 1197,
      (21,0): 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260,
      (21,14): 280, 300, 320, 340, 360, 380, 400, 420, 440, 460, 480, 500,
      (21,26): 520, 540, 560, 580, 600, 620, 640, 660, 680, 700, 720, 740,
      (21,38): 760, 780, 800, 820, 840, 860, 880, 900, 920, 940, 960, 980,
      (21,50): 1000, 1020, 1040, 1060, 1080, 1100, 1120, 1140, 1160, 1180,
      (21,60): 1200, 1220, 1240, 1260,
      (22,0): 0, 21, 42, 63, 84, 105, 126, 147, 168, 189, 210, 231, 252, 273,
      (22,14): 294, 315, 336, 357, 378, 399, 420, 441, 462, 483, 504, 525,
      (22,26): 546, 567, 588, 609, 630, 651, 672, 693, 714, 735, 756, 777,
      (22,38): 798, 819, 840, 861, 882, 903, 924, 945, 966, 987, 1008, 1029,
      (22,50): 1050, 1071, 1092, 1113, 1134, 1155, 1176, 1197, 1218, 1239,
      (22,60): 1260, 1281, 1302, 1323,
      (23,0): 0, 22, 44, 66, 88, 110, 132, 154, 176, 198, 220, 242, 264, 286,
      (23,14): 308, 330, 352, 374, 396, 418, 440, 462, 484, 506, 528, 550,
      (23,26): 572, 594, 616, 638, 660, 682, 704, 726, 748, 770, 792, 814,
      (23,38): 836, 858, 880, 902, 924, 946, 968, 990, 1012, 1034, 1056,
      (23,49): 1078, 1100, 1122, 1144, 1166, 1188, 1210, 1232, 1254, 1276,
      (23,59): 1298, 1320, 1342, 1364, 1386,
      (24,0): 0, 23, 46, 69, 92, 115, 138, 161, 184, 207, 230, 253, 276, 299,
      (24,14): 322, 345, 368, 391, 414, 437, 460, 483, 506, 529, 552, 575,
      (24,26): 598, 621, 644, 667, 690, 713, 736, 759, 782, 805, 828, 851,
      (24,38): 874, 897, 920, 943, 966, 989, 1012, 1035, 1058, 1081, 1104,
      (24,49): 1127, 1150, 1173, 1196, 1219, 1242, 1265, 1288, 1311, 1334,
      (24,59): 1357, 1380, 1403, 1426, 1449,
      (25,0): 0, 24, 48, 72, 96, 120, 144, 168, 192, 216, 240, 264, 288, 312,
      (25,14): 336, 360, 384, 408, 432, 456, 480, 504, 528, 552, 576, 600,
      (25,26): 624, 648, 672, 696, 720, 744, 768, 792, 816, 840, 864, 888,
      (25,38): 912, 936, 960, 984, 1008, 1032, 1056, 1080, 1104, 1128, 1152,
      (25,49): 1176, 1200, 1224, 1248, 1272, 1296, 1320, 1344, 1368, 1392,
      (25,59): 1416, 1440, 1464, 1488, 1512,
      (26,0): 0, 25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300,
      (26,13): 325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 575, 600,
      (26,25): 625, 650, 675, 700, 725, 750, 775, 800, 825, 850, 875, 900,
      (26,37): 925, 950, 975, 1000, 1025, 1050, 1075, 1100, 1125, 1150, 1175,
      (26,48): 1200, 1225, 1250, 1275, 1300, 1325, 1350, 1375, 1400, 1425,
      (26,58): 1450, 1475, 1500, 1525, 1550, 1575,
      (27,0): 0, 26, 52, 78, 104, 130, 156, 182, 208, 234, 260, 286, 312,
      (27,13): 338, 364, 390, 416, 442, 468, 494, 520, 546, 572, 598, 624,
      (27,25): 650, 676, 702, 728, 754, 780, 806, 832, 858, 884, 910, 936,
      (27,37): 962, 988, 1014, 1040, 1066, 1092, 1118, 1144, 1170, 1196,
      (27,47): 1222, 1248, 1274, 1300, 1326, 1352, 1378, 1404, 1430, 1456,
      (27,57): 1482, 1508, 1534, 1560, 1586, 1612, 1638,
      (28,0): 0, 27, 54, 81, 108, 135, 162, 189, 216, 243, 270, 297, 324,
      (28,13): 351, 378, 405, 432, 459, 486, 513, 540, 567, 594, 621, 648,
      (28,25): 675, 702, 729, 756, 783, 810, 837, 864, 891, 918, 945, 972,
      (28,37): 999, 1026, 1053, 1080, 1107, 1134, 1161, 1188, 1215, 1242,
      (28,47): 1269, 1296, 1323, 1350, 1377, 1404, 1431, 1458, 1485, 1512,
      (28,57): 1539, 1566, 1593, 1620, 1647, 1674, 1701,
      (29,0): 0, 28, 56, 84, 112, 140, 168, 196, 224, 252, 280, 308, 336,
      (29,13): 364, 392, 420, 448, 476, 504, 532, 560, 588, 616, 644, 672,
      (29,25): 700, 728, 756, 784, 812, 840, 868, 896, 924, 952, 980, 1008,
      (29,37): 1036, 1064, 1092, 1120, 1148, 1176, 1204, 1232, 1260, 1288,
      (29,47): 1316, 1344, 1372, 1400, 1428, 1456, 1484, 1512, 1540, 1568,
      (29,57): 1596, 1624, 1652, 1680, 1708, 1736, 1764,
      (30,0): 0, 29, 58, 87, 116, 145, 174, 203, 232, 261, 290, 319, 348,
      (30,13): 377, 406, 435, 464, 493, 522, 551, 580, 609, 638, 667, 696,
      (30,25): 725, 754, 783, 812, 841, 870, 899, 928, 957, 986, 1015, 1044,
      (30,37): 1073, 1102, 1131, 1160, 1189, 1218, 1247, 1276, 1305, 1334,
      (30,47): 1363, 1392, 1421, 1450, 1479, 1508, 1537, 1566, 1595, 1624,
      (30,57): 1653, 1682, 1711, 1740, 1769, 1798, 1827,
      (31,0): 0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330, 360,
      (31,13): 390, 420, 450, 480, 510, 540, 570, 600, 630, 660, 690, 720,
      (31,25): 750, 780, 810, 840, 870, 900, 930, 960, 990, 1020, 1050, 1080,
      (31,37): 1110, 1140, 1170, 1200, 1230, 1260, 1290, 1320, 1350, 1380,
      (31,47): 1410, 1440, 1470, 1500, 1530, 1560, 1590, 1620, 1650, 1680,
      (31,57): 1710, 1740, 1770, 1800, 1830, 1860, 1890
      }
   }
}
}
