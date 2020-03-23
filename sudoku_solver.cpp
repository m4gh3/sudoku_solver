#include "cell_flags2.hpp"
#include <fstream>
#include <ctime>

int main(int argc, char *argv[])
{
	
	unsigned short int input_n;
	std::fstream sudoku_file;
	
	if( argc < 2 )
	{
		std::cerr << RED << "Usage: sudoku_solver [sudokufile]\n" << KNRM;
		exit(EXIT_FAILURE);
	}
	else
	{
		try
		{
			sudoku_file.open(argv[1]);
		}
		catch(std::fstream::failure f)
		{
			std::cerr << RED << f.what() << KNRM << '\n';
			exit(EXIT_FAILURE);
		}
	}
	//woah
	memset(grid, 0xff, sizeof(grid) );
	
	//ask for input
	for(int i = 0; i < 9; i++ )
	{
		for(int j = 0;j < 9; j++ )
		{
			sudoku_file >> input_n;
			if( input_n != 0 )
				grid[i][j] = solution_n(input_n-1);
		}
	}
	
	clock_t t;
	t = clock();
	
	do
	{
		
		print_grid();
		std::cout << '\n';
		
		solved_flag = true;
		
		for(char mode = 0; mode < 3; mode++ )
		{
			cells.select_mode(mode);
			
			for(size_t selected_zone = 0; selected_zone < 9; selected_zone++ )
			{
				cells.select_zone(selected_zone);
				dO_thE_thinG();
			}
			
			//if( solved_flag )
				//break;
			
		}
	
	if( solved_flag )
		break;
	
	//std::cout << '\n';
	//print_grid();
	//getchar();
	}
	while( !solved_flag );
	
	t = clock() - t;
	
	std::cout << "It took me " << ( ((double) t)/CLOCKS_PER_SEC ) << "s\n";
	
	return 0;
	
}
