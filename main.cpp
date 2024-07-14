#include <iostream>
#include <vector>
#include <raylib.h>
#include <string>
#include <sstream>
#include <cstring>
#include <utility>

#define screen_width 800
#define screen_height 800

#define grid_width 600
#define grid_height 600

#define cell_margin 0.5
#define font_size 10


struct cell {
    Color color;
    Vector2 pos;
    bool painted;
    cell() : color(BLACK), painted(false) {};
    cell(Color c) : color(c), painted(false) {};
};


class Grid {
    private:
        // dimension of the 10 x 10 cells
        float atomic_cell_side = 10;
        int num_atomicCells_1d = (grid_width / atomic_cell_side);
         // dimension of the 20 x 20 cells
        float cells_medium = 20;
        int num_cells_medium_1d = (grid_width / cells_medium);
        // dimension of the 40 x 40 cells
        float cells_large = 40;
        int num_cells_large_1d = (grid_width / cells_large);

        // the primary grid holds the complete information of each cell
        std::vector<std::vector<cell*>> grid;   // grid that contains the primary(atomic) cells (2.5 x 2.5)
        
        // the secondary grids is an overlay for the 20 x 20 and 40 x 40 cells, they would correspondingly 
        // hold the cell IDs of the cells that they accumulate
        std::vector<std::vector<std::pair<int, int>>> sec_grid1;    // grid for 20 x 20 cells
        std::vector<std::vector<std::pair<int, int>>> sec_grid2;    // grid for 40 x 40 cells
        Color default_color = BLACK;
        

    public:
        Grid() {
            Vector2 pos = {0, 0};
            grid.resize(num_atomicCells_1d, std::vector<cell*>(num_atomicCells_1d, NULL));
            for (int i = 0; i < num_atomicCells_1d; i++) {
                pos.x = 0;
                for (int j = 0; j < num_atomicCells_1d; j++) {
                    grid[i][j] = new cell();
                    grid[i][j]->pos = pos;
                    pos.x += atomic_cell_side;
                }
                pos.y += atomic_cell_side;
            }

            sec_grid1.resize(num_cells_medium_1d, std::vector<std::pair<int, int>>(num_cells_medium_1d, {-1, -1}));

            for (int i = 0; i < num_cells_medium_1d; i++) {
                for (int j = 0; j < num_cells_medium_1d; j++) {
                    sec_grid1[i][j] = {i * 2, j * 2};
                }
            }

            sec_grid2.resize(num_cells_large_1d, std::vector<std::pair<int, int>>(num_cells_large_1d, {-1, -1}));
            for (int i = 0; i < num_cells_large_1d; i++) {
                for (int j = 0; j < num_cells_large_1d; j++) {
                    sec_grid2[i][j] = {i * 4, j * 4};
                }
            }
        }

        void run(Color color, int cell_side_choice) {
            draw(color, cell_side_choice);
            if (IsKeyPressed(KEY_C)) {
                clear();
            }
        }

        void clear() {
            for (int i = 0; i < num_atomicCells_1d; i++) {
                for (int j = 0; j < num_atomicCells_1d; j++) {
                    grid[i][j]->painted = false;
                    grid[i][j]->color = default_color;
                }
            }
        }

        void fill_cells(int cell_side_choice, Color color, Vector2 mouse_pos) {
            int col, row;
            if (cell_side_choice == 1) {
                col = mouse_pos.x / atomic_cell_side;
                row = mouse_pos.y / atomic_cell_side;
                if (col < num_atomicCells_1d && row < num_atomicCells_1d) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        std::cout << "Mouse click at: [" << mouse_pos.x << ", " << mouse_pos.y << "]" << std::endl;
                        std::cout << "Cell number: [" << col << ", " << row << "]" << std::endl;
                        grid[row][col]->painted = true;
                        grid[row][col]->color = color;
                    }
                    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                        std::cout << "Mouse click at: [" << mouse_pos.x << ", " << mouse_pos.y << "]" << std::endl;
                        std::cout << "Cell number: [" << col << ", " << row << "]" << std::endl;
                        grid[row][col]->painted = false;
                        grid[row][col]->color = default_color;
                    }
                }
            }

            else if (cell_side_choice == 2) {
                col = mouse_pos.x / cells_medium;
                row = mouse_pos.y / cells_medium;
                if (col < num_cells_medium_1d && row < num_cells_medium_1d) {
                    int start_ind_i = sec_grid1[row][col].first;
                    int end_ind_i = start_ind_i + 2;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        std::cout << "Mouse click at: [" << mouse_pos.x << ", " << mouse_pos.y << "]" << std::endl;
                        std::cout << "Cell number: [" << col << ", " << row << "]" << std::endl;
                        for ( ; start_ind_i < end_ind_i; start_ind_i++) {
                            int start_ind_j = sec_grid1[row][col].second;
                            int end_ind_j = start_ind_j + 2;
                            for ( ; start_ind_j < end_ind_j; start_ind_j++) {
                                grid[start_ind_i][start_ind_j]->painted = true;
                                grid[start_ind_i][start_ind_j]->color = color;
                            }
                        }
                    }
                    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                        std::cout << "Mouse click at: [" << mouse_pos.x << ", " << mouse_pos.y << "]" << std::endl;
                        std::cout << "Cell number: [" << col << ", " << row << "]" << std::endl;
                        for ( ; start_ind_i < end_ind_i; start_ind_i++) {
                            int start_ind_j = sec_grid1[row][col].second;
                            int end_ind_j = start_ind_j + 2;
                            for ( ; start_ind_j < end_ind_j; start_ind_j++) {
                                grid[start_ind_i][start_ind_j]->painted = false;
                                grid[start_ind_i][start_ind_j]->color = default_color;
                            }
                        }
                    }
                }
            }

            else if (cell_side_choice == 3) {
                col = mouse_pos.x / cells_large;
                row = mouse_pos.y / cells_large;
                if (col < num_cells_large_1d && row < num_cells_large_1d) {
                    int start_ind_i = sec_grid2[row][col].first;
                    int end_ind_i = start_ind_i + 4;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        std::cout << "Mouse click at: [" << mouse_pos.x << ", " << mouse_pos.y << "]" << std::endl;
                        std::cout << "Cell number: [" << col << ", " << row << "]" << std::endl;
                        for ( ; start_ind_i < end_ind_i; start_ind_i++) {
                            int start_ind_j = sec_grid2[row][col].second;
                            int end_ind_j = start_ind_j + 4;
                            for ( ; start_ind_j < end_ind_j; start_ind_j++) {
                                grid[start_ind_i][start_ind_j]->painted = true;
                                grid[start_ind_i][start_ind_j]->color = color;
                            }
                        }
                    }
                    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                        std::cout << "Mouse click at: [" << mouse_pos.x << ", " << mouse_pos.y << "]" << std::endl;
                        std::cout << "Cell number: [" << col << ", " << row << "]" << std::endl;
                        for ( ; start_ind_i < end_ind_i; start_ind_i++) {
                            int start_ind_j = sec_grid2[row][col].second;
                            int end_ind_j = start_ind_j + 4;
                            for ( ; start_ind_j < end_ind_j; start_ind_j++) {
                                grid[start_ind_i][start_ind_j]->painted = false;
                                grid[start_ind_i][start_ind_j]->color = default_color;
                            }
                        }
                    }
                }
            }
        }

        void draw(Color color, int cell_side_choice) {
            for (int i = 0; i < num_atomicCells_1d; i++) {
                for (int j = 0; j < num_atomicCells_1d; j++) {
                    cell* curr = grid[i][j];
                    Rectangle curr_rect = {curr->pos.x, curr->pos.y, atomic_cell_side, atomic_cell_side};
                    if (!curr->painted) {
                        DrawRectangleLinesEx(curr_rect, cell_margin, GRAY);
                    }
                    else {
                        DrawRectangle(curr->pos.x, curr->pos.y, atomic_cell_side, atomic_cell_side, curr->color);
                    }
                    
                }
            }
            Vector2 mouse_pos = GetMousePosition();
            fill_cells(cell_side_choice, color, mouse_pos);
        }
};


class Screen {
    private:
        Grid* grid = new Grid();
        Color available_colors = {};
        Color default_color = WHITE;
        int max_input_chars = 6;
        int font_size_hex = 15;
        std::string hex_val;

        // choice for cell dimensions
        int cell_side_choice = 1;

    public:

        bool mouse_over_box(Vector2 mouse_pos, Rectangle box) {
            return ((mouse_pos.x > box.x && mouse_pos.y < (box.y + box.height) && mouse_pos.y > box.y && mouse_pos.x < (box.x + box.width)));
        }

        Color hex_to_color(const std::string &hex) {
            if (hex.size() < 6 || hex.size() > 6) {
                return default_color;
            }
            unsigned int rgb;
            std::stringstream ss;
            ss << std::hex << hex;
            ss >> rgb;
            
            Color color;
            color.r = (rgb >> 16) & 0xFF;
            color.g = (rgb >> 8) & 0xFF;
            color.b = (rgb & 0xFF);
            color.a = 255;

            return color;
        }

        Color change_color(int frame_counter) {
            // text box to type in hex value for color
            DrawText("Enter hex value for color: ", 30, 650, 25, RED);
            Vector2 color_picker_box_pos = {100, 700};
            Vector2 color_picker_box_size = {100, 30};
            Rectangle color_picker_box = {color_picker_box_pos.x, color_picker_box_pos.y, color_picker_box_size.x, color_picker_box_size.y};
            float box_border = 2.5;
            Color border_col = GRAY;

            Vector2 mouse_pos = GetMousePosition();
           
            if (mouse_over_box(mouse_pos, color_picker_box)) {
                border_col = RED;
                ++frame_counter;
                char key = GetCharPressed();
                while (key > 0 && hex_val.size() < max_input_chars) {
                    if ((key >= 32) && (key <= 125) && (hex_val.size() < max_input_chars)) {
                        hex_val += key;
                    }
                    if (((frame_counter / 20) % 2) == 0) {
                        DrawText("_", color_picker_box_pos.x + 5 + MeasureText(hex_val.c_str(), font_size_hex), color_picker_box_pos.y + 5, font_size_hex, default_color);
                    }
                    key = GetCharPressed();
                }
                if (!hex_val.empty() && IsKeyPressed(KEY_BACKSPACE)) {
                    hex_val.pop_back();
                }
            }
            else {
                frame_counter = 0;
            }
            DrawRectangle(color_picker_box.x, color_picker_box.y, color_picker_box.width, color_picker_box.height, BLACK);
            DrawRectangleLinesEx(color_picker_box, box_border, border_col);

            Color color = hex_to_color(hex_val);
            DrawText(hex_val.c_str(), color_picker_box_pos.x + 5, color_picker_box_pos.y + 5, font_size_hex, color);
            
            // show color in a circle alongside text box
            Vector2 color_circle_pos = {50, 710};
            float color_circle_rad = 30;
            DrawCircleV(color_circle_pos, color_circle_rad, color);

            Color color_circle_line_col = WHITE;
            DrawCircleLinesV(color_circle_pos, color_circle_rad, color_circle_line_col);

            
            return color;
        }

        void screen_manager(int frame_counter) {
            Color color = change_color(frame_counter);
            if (IsKeyPressed(KEY_KP_1))
                cell_side_choice = 1;
            else if (IsKeyPressed(KEY_KP_2))
                cell_side_choice = 2;
            else if (IsKeyPressed(KEY_KP_3))
                cell_side_choice = 3;

            grid->run(color, cell_side_choice);
        }

};


int main()
{
    InitWindow(screen_width, screen_height, "Pixel Art");
    SetTargetFPS(60);
    int frame_counter = 0;

    Screen* screen = new Screen();   

    while (!WindowShouldClose()) {       
        // Draw
        BeginDrawing();
            ClearBackground(BLACK);
            screen->screen_manager(frame_counter);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}