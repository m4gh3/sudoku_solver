#include <cstdlib>

class sudoku_cell_groups
{
	public:
		sudoku_cell_groups( short int (*grid)[9][9] )
		{
			this->grid = grid;
		}
		short int &operator[](size_t i)
		{
			last_i = i;
			switch(mode)
			{
				case 0:
					last_col = i;
					last_row = selected;
					return (*grid)[selected][i];
				break;
				case 1:
					last_col = selected;
					last_row = i;
					return (*grid)[i][selected];
				break;
				case 2:
					div_t rc;
					size_t col = 0, row = 0;
					rc = div( selected, 3 );
					row += rc.quot * 3;
					col += rc.rem * 3;
					rc = div( i, 3 );
					row += rc.quot;
					col += rc.rem;
					last_col = col;
					last_row = row;
					return (*grid)[row][col];
				break;
			}
		}
		void select_zone(size_t i)
		{
			selected = i;
		}
		void select_mode(char selected_mode)
		{
			mode = selected_mode;
		}
		size_t get_last_row() { return last_row; }
		size_t get_last_col() { return last_col; }
		size_t get_last_selected() { return selected; }
		size_t get_last_index() { return last_i; }
		char get_mode() { return mode; }
	private:
		char mode;
		size_t selected;
		short int (*grid)[9][9];
		size_t last_col;
		size_t last_row;
		size_t last_i;
};

short int grid[9][9];

sudoku_cell_groups cells(&grid);
