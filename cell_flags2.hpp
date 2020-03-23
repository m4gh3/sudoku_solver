#include <iostream>
#include <cstring>
#include <string>
#include <limits>
#include <exception>
#include <csignal>
#include <cstdio>

#include "sudoku_cell_groups.hpp"

class binary_base
{
	public:
	class bad_bits : public std::exception
	{
		virtual const char* what() const throw()
		{
			return "Number of Bits != Number of digits!";
		}
	} bad_bits;

	friend std::ostream& operator<<(std::ostream& stream, const binary_base& bin)
	{
		return stream << bin.digits;
	}

	protected:
	std::string digits;
};

template<typename T> class binary:public binary_base
{
	public:
	binary(const T& n)
	{
		const int bits = std::numeric_limits<T>::digits;
		for (int i = 0; i < bits; i++)
			digits.insert(digits.begin(), (n & (1 << i)) ? '1' : '0');

		if (digits.size() != bits)
			throw bad_bits;
	}
};

short int solve_score[9] = {0,0,0,0,0,0,0,0,0};
short int create_check_mask_count = 0;
short int check_mask;

bool solved_flag;

void increase_score(short int bitmask)
{
	
	short int select_bit = 1;
	
	for( int i = 0; i < 9; i++,select_bit<<=1 )
	{
		if( bitmask & select_bit )
			solve_score[i]++;
	}
	
}

void breaking_badly()
{
}

void print_solve_score()
{
	std::cout << "solve_score:\n\t";
	
	for(int i = 0; i < 9; i++ )
		std::cout << i+1 << ": " << solve_score[i] << "  ";
		
	std::cout << '\n';
	//getchar();
	
}

int get_solutions_n(short int bitmask)
{

	int solutions_n = 0;
	short int select_bit = 1;
	
	for( int i = 0; i < 9; i++,select_bit<<=1 )
	{
		if( bitmask & select_bit )
			solutions_n++;
	}
	
	return solutions_n;
	
}


void create_check_mask_a(size_t index)
{
	
	short int retval;
	short int select_bit;
	
	//std::cout << "\t\tcreating check_mask_a:\n";
	
	//for(int i = 0,j; i < 9; i++ )
	//{
	int i = 8;
	int j;
	
	while( i-- > 0 )
	{
		
		j = 0;
		retval = 0;
		select_bit = 1;
		
		for(int k = 0; ( k <= ( 9 - i + j - 1 ) ) && ( j < i+2 ); k++,select_bit<<=1 )
		{
			
			if( solve_score[k] == i+1 )
			{
				j++;
				retval |= select_bit;
				//std::cout << "retval(i: " << i << "): " << binary<short int>( retval & 0x1ff ) << '\n';
			}
			
		}
		
		if( j-1 == i )
		{
			
			create_check_mask_count = get_solutions_n(retval);
			//occhio
			
			for( int k = 0; k < 9; k++ )
				((cells[k] & retval) == retval) ? create_check_mask_count-- : 0;
			
			if( create_check_mask_count == 0 )
			{
				check_mask = retval;
				return;
			}
			
		}
		
	}
	
}

short int solution_n(unsigned short int pos )
{
	//std::cout << binary<short int>(1<<pos) << '\n';
	return 1<<pos;
}

short int get_solution_pos(short int solution_n)
{

	short int bitmask = 1;
	short int retval = 0;
	
	for(int i = 0; i < 9; i++,bitmask<<=1 )
	{
		if( bitmask & solution_n )
		{
			if( retval != 0 )
				return 0;
			retval = i+1;
		}
	}
	
	return retval;
	
}

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define KNRM  "\x1B[0m"

#define CLS   "\x1B[2J\x1B[H"


char *grid_colors[] = { MAG ,MAG ,RED, YEL, GRN, CYN, BLU, WHT ,WHT };

void something_is_wrong();

void print_grid()
{
	
	cells.select_mode(0);
	
	for(int i = 0; i < 9; i++ )
	{
		
		cells.select_zone(i);
		
		for(int j = 0; j < 9; j++ )
		{
			if(get_solutions_n(cells[j]))
			{
				std::cout << grid_colors[ get_solutions_n(cells[j]) - 1 ];
				std::cout << get_solution_pos(cells[j]) << ' ';
			}
			else
			{
				std::cout << WHT"X ";;
			}
		}
		
		std::cout << KNRM "\n";
		
	}
	
}

void print_status()
{
	
	//std::cout << CLS;
	//getchar();
	std::cout << KNRM "\n@ (" << cells.get_last_col() << ',' << cells.get_last_row()
	<< ")\nlast cells status:\n"
		"\tmode: " << (int)cells.get_mode() << "\n"
		"\tselected: " << cells.get_last_selected() << "\n"
		"\tlast_i: " << cells.get_last_index() << std::endl;
	for(int k = 0; k < 9; k++ )
		std::cout << "\tcells[" << k << "]: " << binary<short int>( cells[k] & 0x1ff ) << std::endl;
	print_solve_score();
	print_grid();
}

void print_check_mask()
{
	std::cout << "check_mask: " << binary<short int>( check_mask & 0x1ff ) << '\n';
	//getchar();
}

void something_is_wrong()
{
	std::cout << "Pippo!!!" << std::endl;
	print_status();
	std::raise(SIGTRAP);
	exit(EXIT_FAILURE);
}

void dO_thE_thinG()
{

	for( int i = 0; i < 9; i++ ) // whoops
	{
		
		//print_status();
		
		create_check_mask_count = get_solutions_n(cells[i]);
		
		//solved_flag &= !create_check_mask_count; //check if solved... i guess...
		
		//for( int j = (  ( (i + 1) > 8 ) ? 0 : (i + 1)  ); j!=i; (j==8) ? j=0 : j++ )
		{
			int j = i;
		
			do
			{
			
				if( cells[i] == cells[j] )
					if( create_check_mask_count == 0 )
						something_is_wrong();
					else create_check_mask_count--;
			
				increase_score( cells[i] & cells[j] );
				//std::cout << "create_check_mask_count: " << create_check_mask_count << '\n';
				//print_solve_score();
			
				(j==8) ? j=0 : j++;
			
			} while( j != i );
			
		}
		
		if( cells[i] == 0x1d0 )
			print_solve_score();
		
		if(create_check_mask_count)
		{
			
			create_check_mask_a(i);
			//if( (cells.get_mode() == 2) && (cells.get_last_selected() == 0) && (i == 8) )
				//print_solve_score();
			
			//if( check_mask != 0 )
			//{
				/*if(create_check_mask_count < -1)
				{
					std::cout << "create_check_mask_count was " << create_check_mask_count << '\n';
					std::cout << "retval(i: " << i << "): " << binary<short int>( check_mask & 0x1ff ) << '\n';
					print_status();
				}*/
				if(create_check_mask_count == 0)
				{
					
					int j = i;
					do
					{
						
						if( ( cells[j] & check_mask ) == check_mask )
						{
							solved_flag &= ( cells[j] == check_mask );
							cells[j] = check_mask;
						}
							
						(j==8) ? j=0 : j++;
						
					} while( j != i );
					
				}
				
		}
		else
		{
			
			check_mask = ~cells[i];
			//print_check_mask();
			
			int j = i;
			do
			{
				
				if( cells[j] & check_mask )
				{
					solved_flag &= ( ( cells[j] & check_mask ) == cells[j] );
					cells[j] &= check_mask;
				}
				else
					if( cells[i] != cells[j] )
						something_is_wrong();
						
				(j==8) ? j=0 : j++;
				
			} while( j != i );
			
		}
		
		memset(solve_score, 0, sizeof(solve_score) );
		check_mask = 0;
		
	}
	
}
