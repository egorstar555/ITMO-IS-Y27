#include <iostream>
#include <cstdint>
#include <cstring>
#include <fstream>

const int8_t kMaxSand = 4;

struct Cell {
    int16_t x;
    int16_t y;
    uint64_t sand;
};

struct Cells {
    Cell* data;
    size_t capacity;
    size_t size;
};

struct Arguments {
    const char* file_name = nullptr;
    const char* dir_name = nullptr;
    int max_iteration = 0;
    int frequency = 0;
    bool is_error = false;
};

struct GridParam {
    int16_t row;
    int16_t col;
    uint64_t cells;
    bool is_resize;
    bool is_top;
    bool is_bottom;
    bool is_left;
    bool is_right;
};

int CalculatePaddingWidth(int width) {
    int padding_width = width;
    padding_width += padding_width % 2; // должно быть четным т.к. в один байт пишется 2 пикселя
    padding_width /= 2; // в один байт пишется 2 пикселя
    padding_width = padding_width + ((4 - padding_width % 4) % 4); //  Ряды кратны четырём байтам
    return padding_width;
}

void SaveBMP(const char* path, const uint64_t*& grid, GridParam& grid_param) {
    std::ofstream f(path, std::ios::out | std::ios::binary);

    if (!f.is_open()) {
        std::cerr << "File cold not be opened\n";
        return;
    }

    int image_height = grid_param.row;
    int image_width = grid_param.col;
    unsigned char bmp_pad[3] = {0, 0, 0};
    const int kPaddingWidth = CalculatePaddingWidth(image_width); // размер строки в байтах
    const int kPaddingSize = kPaddingWidth - (image_width + image_width % 2) / 2; // дополнение строки в байтах
    const int kFileHeaderSize = 14; // определено форматом
    const int kInformationHeaderSize = 40; // версия 3 BMP
    const int kNumColor = 5;
    const int kbiBitCount = 4;
    const int kColorTableSize = kNumColor * kbiBitCount;
    const int kFileSize = kFileHeaderSize + kInformationHeaderSize + kColorTableSize + kPaddingWidth * image_height;
    unsigned char file_header[kFileHeaderSize] = {0};

    file_header[0] = 'B';
    file_header[1] = 'M';
    file_header[2] = kFileSize;
    file_header[3] = kFileSize >> 8;
    file_header[4] = kFileSize >> 16;
    file_header[5] = kFileSize >> 24;
    file_header[10] = kFileHeaderSize + kInformationHeaderSize + kColorTableSize; //bfOffBits начало массива цветов

    unsigned char information_header[kInformationHeaderSize] = {0};
    information_header[0] = kInformationHeaderSize; // 40 версия 3
    information_header[4] = image_width;
    information_header[5] = image_width >> 8;
    information_header[6] = image_width >> 16;
    information_header[7] = image_width >> 24;

    information_header[8] = image_height;
    information_header[9] = image_height >> 8;
    information_header[10] = image_height >> 16;
    information_header[11] = image_height >> 24;
    information_header[12] = 1; // biPlanes
    information_header[14] = kbiBitCount; // biBitCount

    information_header[32] = kNumColor; // biClrUsed
    information_header[36] = kNumColor; // biClrImportant

    f.write(reinterpret_cast<char*>(file_header), kFileHeaderSize);
    f.write(reinterpret_cast<char*>(information_header), kInformationHeaderSize);

    uint8_t kWhite[4] = {255, 255, 255,
                         0}; // формат RGBQUAD rgbBlue (синий), rgbGreen (зелёный), rgbRed (красный) и rgbReserved
    uint8_t kGreen[4] = {0, 255, 0, 0};
    uint8_t kYellow[4] = {0, 255, 255, 0};
    uint8_t kPurple[4] = {255, 0, 255, 0};
    uint8_t kBlack[4] = {0, 0, 0, 0};

    f.write(reinterpret_cast<char*>(kWhite), sizeof(kWhite));
    f.write(reinterpret_cast<char*>(kGreen), sizeof(kGreen));
    f.write(reinterpret_cast<char*>(kYellow), sizeof(kYellow));
    f.write(reinterpret_cast<char*>(kPurple), sizeof(kPurple));
    f.write(reinterpret_cast<char*>(kBlack), sizeof(kBlack));

    uint8_t color[1];
    uint8_t lo;
    uint8_t hi;
    for (int x = 0; x < image_height; ++x) {
        for (int y = 0; y < image_width; y += 2) {
            hi = grid[x * image_width + y];
            hi = hi > 4 ? 4 : hi;
            lo = 0;
            if (y + 1 < image_width) {
                lo = grid[x * image_width + (y + 1)];
                lo = lo > 4 ? 4 : lo;
            }
            color[0] = lo + (hi << 4);
            f.write(reinterpret_cast<char*>(color), 1);
        }
        f.write(reinterpret_cast<char*>(bmp_pad), kPaddingSize);
    }
}

Arguments ParserArguments(int argc, char** argv) {
    Arguments my_arguments;

    if (argc < 2) {
        std::cerr << "No filename";
        my_arguments.is_error = true;

        return my_arguments;
    }
    int i = 0;
    while (i < argc) {
        if ((!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input")) && (i + 1 < argc)) {
            my_arguments.file_name = argv[i + 1];
        }
        if ((!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) && (i + 1 < argc)) {
            my_arguments.dir_name = argv[i + 1];
        }
        if ((!strcmp(argv[i], "-m") || !strcmp(argv[i], "--max-iter")) && (i + 1 < argc)) {
            errno = 0;
            my_arguments.max_iteration = std::strtol(argv[i + 1], nullptr, 10);
            if ((errno == ERANGE && (my_arguments.max_iteration == LONG_MAX || my_arguments.max_iteration == LONG_MIN))
                || (errno != 0 && my_arguments.max_iteration == 0)) {
                std::cerr << "Invalid input: the number of max iteration can only be a positive number" << "\n";
                my_arguments.is_error = true;
                return my_arguments;
            }
        }
        if ((!strcmp(argv[i], "-f") || !strcmp(argv[i], "--freq")) && (i + 1 < argc)) {
            errno = 0;
            my_arguments.frequency = std::strtol(argv[i + 1], nullptr, 10);
            if ((errno == ERANGE && (my_arguments.frequency == LONG_MAX || my_arguments.frequency == LONG_MIN))
                || (errno != 0 && my_arguments.frequency == 0)) {
                std::cerr << "Invalid input: frequency can only be a positive number" << "\n";
                my_arguments.is_error = true;
                return my_arguments;
            }
        }
        i++;
    }

    return my_arguments;
}

size_t GetSize(Cells value) {
    return value.size;
}

void reserve(size_t new_capacity, Cells& value) {
    if (new_capacity <= value.capacity) {
        return;
    }
    Cell* new_data = new Cell[new_capacity];
    memcpy(new_data, value.data, value.size * sizeof(Cell));
    delete[] value.data;
    value.data = new_data;
    value.capacity = new_capacity;
}

void PushBack(const Cell element, Cells& value) {
    if (value.size == value.capacity) {
        size_t new_capacity = (value.capacity == 0) ? 1 : 2 * value.capacity;
        reserve(new_capacity, value);
    }
    value.data[value.size] = element;
    value.size++;
}

Cells LoadInitialState(const char* input_file) {
    Cells initial_state;
    initial_state.data = nullptr;
    initial_state.size = 0;
    initial_state.capacity = 0;
    Cell cell;

    std::ifstream file(input_file);
    if (!file.is_open()) {
        std::cerr << "Invalid file open: " << input_file << std::endl;
        exit(1);
    }
    while (file >> cell.x >> cell.y >> cell.sand) {
        if (file) {
            PushBack(cell, initial_state);
        }
    }

    return initial_state;
}

void CalculateGridSize(const Cells initial_state, int16_t& x, int16_t& y) {
    int16_t max_x = 0;
    int16_t max_y = 0;
    for (int i = 0; i < GetSize(initial_state); i++) {
        max_x = std::max(max_x, initial_state.data[i].x);
        max_y = std::max(max_y, initial_state.data[i].y);
    }
    x = max_x + 1;
    y = max_y + 1;
}

void InitializeGrid(uint64_t grid[], const GridParam grid_param, const Cells& initial_state) {
    for (uint64_t i = 0; i < GetSize(initial_state); i++) {
        grid[initial_state.data[i].x * grid_param.col + initial_state.data[i].y] = initial_state.data[i].sand;
    }
}

void ResizeGrid(uint64_t*& grid, GridParam& grid_param, Cells& new_cells) {
    uint16_t new_rows = grid_param.row + (grid_param.is_top ? 1 : 0) + (grid_param.is_bottom ? 1 : 0);
    uint16_t new_cols = grid_param.col + (grid_param.is_left ? 1 : 0) + (grid_param.is_right ? 1 : 0);
    uint64_t new_cells_num = new_rows * new_cols;
    uint64_t* new_grid = new uint64_t[new_cells_num]{0};
    uint64_t top_margin = grid_param.is_top ? new_cols : 0;
    uint64_t left_margin = grid_param.is_left ? 1 : 0;
    uint64_t rigth_margin = grid_param.is_right ? 1 : 0;
    uint64_t tek_cell;
    uint64_t new_tek_cell;

    for (uint64_t i = 0; i < grid_param.row; i++) {
        for (uint64_t j = 0; j < grid_param.col; j++) {
            tek_cell = i * grid_param.col + j;
            new_tek_cell = tek_cell + top_margin + ((i + 1) * left_margin) + (i * rigth_margin);
            new_grid[new_tek_cell] = grid[tek_cell];
        }
    }

    int32_t x;
    int32_t y;
    for (uint64_t i = 0; i < GetSize(new_cells); i++) {
        x = new_cells.data[i].x;
        x += grid_param.is_top ? 1 : 0;
        y = new_cells.data[i].y;
        y += grid_param.is_left ? 1 : 0;
        new_tek_cell = (x * new_cols + y);
        new_grid[new_tek_cell] += new_cells.data[i].sand;
    }

    delete[] grid;
    grid = new_grid;
    grid_param.col = new_cols;
    grid_param.row = new_rows;
    grid_param.cells = new_cells_num;
}

bool UpdateGrid(uint64_t*& grid, GridParam& grid_param) {
    bool is_stable = true;
    grid_param.is_resize = false;
    uint64_t* new_grid = new uint64_t[grid_param.cells]{};
    uint64_t tek_cell;
    Cells new_cells;
    new_cells.data = nullptr;
    new_cells.capacity = 0;
    new_cells.size = 0;

    Cell cell;

    for (int16_t i = 0; i < grid_param.row; i++) {
        for (int16_t j = 0; j < grid_param.col; j++) {
            tek_cell = i * grid_param.col + j;
            if (grid[tek_cell] >= kMaxSand) {
                is_stable = false;
                new_grid[tek_cell] += grid[tek_cell] - kMaxSand;
                if (i > 0) {
                    ++new_grid[(i - 1) * grid_param.col + j];
                } else {
                    grid_param.is_resize = true;
                    grid_param.is_top = true;
                    cell.x = -1;
                    cell.y = j;
                    cell.sand = 1;
                    PushBack(cell, new_cells);
                }
                if (i < grid_param.row - 1) {
                    ++new_grid[(i + 1) * grid_param.col + j];
                } else {
                    grid_param.is_resize = true;
                    grid_param.is_bottom = true;
                    cell.x = i + 1;
                    cell.y = j;
                    cell.sand = 1;
                    PushBack(cell, new_cells);
                }
                if (j > 0) {
                    ++new_grid[i * grid_param.col + j - 1];
                } else {
                    grid_param.is_resize = true;
                    grid_param.is_left = true;
                    cell.x = i;
                    cell.y = -1;
                    cell.sand = 1;
                    PushBack(cell, new_cells);
                }
                if (j < grid_param.col - 1) {
                    ++new_grid[i * grid_param.col + j + 1];
                } else {
                    grid_param.is_resize = true;
                    grid_param.is_right = true;
                    cell.x = i;
                    cell.y = j + 1;
                    cell.sand = 1;
                    PushBack(cell, new_cells);
                }
            } else {
                new_grid[tek_cell] += grid[tek_cell];
            }
        }
    }

    if (grid_param.is_resize) {
        ResizeGrid(new_grid, grid_param, new_cells);
        grid_param.is_resize = false;
        grid_param.is_left = false;
        grid_param.is_right = false;
        grid_param.is_top = false;
        grid_param.is_bottom = false;
    }
    delete[] grid;
    grid = new_grid;
    delete[] new_cells.data;

    return is_stable;
}

void CreateFileName(char* output_path, const char* output_dir, const uint64_t iteration) {
    snprintf(output_path, 256, "%s\\iteration_%llu.bmp", output_dir, iteration);
}

void SaveGridImage(const uint64_t grid[], GridParam& grid_param, const char* output_dir, const uint64_t iteration) {
    char output_path[256];
    CreateFileName(output_path, output_dir, iteration);
    SaveBMP(output_path, grid, grid_param);
}

void SandpileRun(const Arguments& my_args) {
    Cells initial_state = LoadInitialState(my_args.file_name);

    int16_t x;
    int16_t y;
    CalculateGridSize(initial_state, x, y);

    uint64_t cells_num = x * y;
    uint64_t* grid = new uint64_t[cells_num]{};

    GridParam grid_param{x, y, cells_num, false};

    InitializeGrid(grid, grid_param, initial_state);

    int iteration = 0;
    bool is_stable = false;

    while (iteration < my_args.max_iteration && !is_stable) {
        is_stable = UpdateGrid(grid, grid_param);

        if (my_args.frequency > 0 && (iteration % my_args.frequency == 0 || is_stable)) {
//            SaveGridImage(grid, grid_param, my_args.dir_name, iteration);
        }
        ++iteration;
    }

//    if (my_args.frequency == 0 && iteration <= my_args.max_iteration) {
//        SaveGridImage(grid, grid_param, my_args.dir_name, iteration);
//    }
    delete[] grid;
    delete[] initial_state.data;
}

int main(int argc, char** argv) {
    Arguments my_args = ParserArguments(argc, argv);

    if (!my_args.is_error) {
        SandpileRun(my_args);
    }
}