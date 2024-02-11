
const int sobel_Gx[9] = {-1, 0, 1,
                   -2, 0, 2,
                   -1, 0, 1};

// Mask for AxeY
const int sobel_Gy[9] = {-1, -2, -1,
                   0, 0, 0,
                   1, 2, 1};

// Mask for experimentation
const int sobel_Gz[9] = {0, 1, 2,
                   -1, 0, 1,
                   -2, -1, 0};

// Mask for experimentation
const int prewitt_Gx[9] = {-1, 0, 1,
                   -1, 0, 1,
                   -1, 0, 1};

// Mask for experimentation
const int prewitt_Gy[9] = {-1, -1, -1,
                      0, 0, 0,
                      1, 1, 1};
// Mask for experimentation
const int kirch_Gx[9] = {   -3, -3, 5,
                      -3,  0, 5,
                      -3, -3, 5};

// Mask for experimentation
const int kirch_Gy[9] = {   -3, -3, -3,
                      -3,  0, -3,
                       5,  5,  5};

// Gaussian blur mask 5x5
const int Gaussian[25] = {   2,   4,   5,  4, 2,
                      4,   9,  12,  9, 4,
                      5,  12,  15, 12, 5,
                      4,   9,  12,  9, 4,
                      2,   4,   5,  4, 2    };
// Gaussian blur mask 5x5
const int Gaussian_by_five[25] = {   1,    4,   7,   4, 1,
                                 4,   16,  26,  16, 4,
                                 7,   26,  41,  26, 7,
                                 4,   16,  26,  16, 4,
                                 1,    4,   7,   4, 1    };
// Gaussian blur mask 7x7
const int Gaussian_by_seven[49] = {   0,    0,   1,   2,  1,  0, 0,
                                 0,    3,  13,  22, 13,  3, 0,
                                 1,   13,  59,  97, 59, 13, 1,
                                 2,   22,  97, 159, 97, 22, 2,
                                 1,   13,  59,  97, 59, 13 ,1,
                                 0,    3,  13,  22, 13,  3 ,0,
                                 0,    0,   1,   2,  1,  0 ,0    };

