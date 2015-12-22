///***********************************************************************************

/// Title: Grade Records and Calculator

/// Description: 

/// Reads and writes to a file in order to keep a record of assignments, grade

/// received, and average grade. Should be able to update records any time.

///		All files will hold in the following order:
///			(I) 	The number of entries that it holds.
///			(II) 	The total amount of points earned.
///			(III)	The total amount of points possible.
///			(IV)	The entries in sequence.

/// Methods: 

/// Should have a method to be able create a file incase I want to start a new 

/// record for a different class. 										Done

/// Should have a method to select specific class' records to open. 	Done

/// Should have a method to add assignments.							Done

/// Should have a method to display all assignments and scores.			Done

/// Should have a method to undo all changes.

/// Should have a method to save changes.

/// Author: Willy Ma

/// COMSC 165 Section 8326

/// Date: December 3, 2015

/// Status : Incomplete

/// 

/// 

///***********************************************************************************
using namespace std;

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdio>
#include <cstdlib>


bool 	addDoc( string );
bool 	openDoc( string , fstream & );
bool 	isExist( string );

void	addAsgnmt( fstream & );
void	displayall( fstream & );
void	OpenOrCreate();
void	showMenu();

int 	getEntryNum( fstream & );

bool	hasAlpha( const char * );
bool	noDec( const char * );
bool 	manyDec( const char * );
bool 	hasSpace( const char * );

bool 	pointsValidation( const char * );

struct Entry
{

	int entryNum;
	string title;
	double score;
	double total;
	/* char *percentage; Wont need this. It offers opportunity for stale data. */

};

const int FIRST_INDEX = sizeof( int ) + ( 2 * sizeof( double ) );
int main()
{

	int firstInput = -1;
    // First menu. Open or create a record.

    fstream file;

    while ( firstInput != 3 )
    {
    	OpenOrCreate();
    	cin >> firstInput;
    	cin.ignore();

    	while ( ( firstInput < 1 ) || ( firstInput > 3 ) )
    	{
    		cout << "Please enter a valid input." << endl;
    		cin >> firstInput;
    		cin.ignore();
    	}

    	if ( firstInput == 1 )
    	{

    		string recordName;
    		cout << "Please enter the name of the class whose records you wish to access: ";
    		getline( cin , recordName );
    		openDoc( recordName , file );
    		displayall( file );
    		
    		int secondInput = -1;

    		while ( secondInput != 2 )
    		{
    			showMenu();
    			cin >> secondInput;
    			cin.ignore();

		    	while ( ( secondInput < 1 ) || ( secondInput > 2 ) )
		    	{
		    		cout << "Please enter a valid input." << endl;
		    		cin >> secondInput;
		    		cin.ignore();
		    	}

		    	if ( secondInput == 1 )
		    	{
		    		addAsgnmt( file );
		    	}
    		}
    	}

    	if ( firstInput == 2 )
    	{
    		string newRecordName;
    		cout << "Please enter the name of the class for which you want to create a record for: ";
    		getline( cin , newRecordName );
    		while ( addDoc( newRecordName ) == false )
    		{
    			cout << "That record already exists. Please try again: ";
    			getline( cin , newRecordName );
    		}

    	}


    }








	return 0;
}

bool addDoc( string docName )	// Creates a document with the name passed
{
	// Check for the existence of a file with that name.
	if ( isExist( docName ) )
	{
		return false;
	}

	else
	{
		fstream addFile;
		addFile.open( docName.c_str() , ios::out | ios::binary );
		int initialnum = 0;
		double initialPts = 0.00;

		// Storing first 3 chunks of data. 
		addFile.write( reinterpret_cast<char *>( &initialnum ) , sizeof( initialnum ) ); 
		addFile.write( reinterpret_cast<char *>( &initialPts ) , sizeof( initialPts ) ); 
		addFile.write( reinterpret_cast<char *>( &initialPts ) , sizeof( initialPts ) );

		addFile.close();
		return true;
	}

}


bool openDoc( string docName , fstream &inFile)
{

	if ( isExist( docName ) )
	{
		inFile.open( docName.c_str() , ios::in | ios::out | ios::binary );

		if ( inFile.fail() )
		{
			return false;
		}
		else
		{
			return true;
		}

	}

	else
	{
		return false;
	}

}


bool isExist( string docName )
{
	fstream test;
	test.open( docName.c_str() , ios::in );

	if ( test.fail() == true )
	{
		test.close();
		return true;
	}
	else
	{
		test.close();
		return false;
	}

}

void addAsgnmt( fstream &file )
{
	Entry assignment;
	char cancel;

	const int maxChar = 200;
	char title[ maxChar + 1 ];

	const int sigFigs = 10;
	char totalPts[ sigFigs + 2 ];

	char myScore[ sigFigs + 2 ];

	cout << "Please enter the name of the assignment that you wish to add.\n";
	cout << "Title: ";
	cin.getline( title , maxChar );
	while ( cin.fail() )
	{
		cin.clear();
		cout << "The title is too long!\n";
		cout << "Title: ";
		cin.getline( title , maxChar );
	}

	cout << "Please enter the your score, including the decimal: ";
	cin.getline( myScore , sigFigs );
	cout << endl;

	while ( cin.fail() || pointsValidation( myScore ) )
	{

		if ( cin.fail() )
		{
			cin.clear();
			cout << "TOO MANY SIG FIGS!! Please round to 10 digits.\n";
		}           

		if ( hasAlpha( myScore ) )
		{
			cout << "Error! There are letters in your input.\n";
		}

		if ( noDec( myScore ) )
		{
			cout << "Error! There isn't a decimal in your input.\n";
		}

		if ( manyDec( myScore ) )
		{
			cout << "Error! There is more than one decimal in your input.\n";
		}

		if ( hasSpace( myScore ) )
		{
			cout << "Error! There is a space in your input.\n";
		}

		cout << "Please enter the your score, including the decimal: ";
		cin.getline( myScore , sigFigs );
		cout << endl;
	}

	cout << "Please enter the total points possible, including the decimal: ";
	cin.getline( totalPts , sigFigs );
	cout << endl;

	while ( cin.fail() || pointsValidation( totalPts ) )
	{

		if ( cin.fail() )
		{
			cin.clear();
			cout << "TOO MANY SIG FIGS!! Please round to 10 digits.\n";
		}           

		if ( hasAlpha( totalPts ) )
		{
			cout << "Error! There are letters in your input.\n";
		}

		if ( noDec( totalPts ) )
		{
			cout << "Error! There isn't a decimal in your input.\n";
		}

		if ( manyDec( totalPts ) )
		{
			cout << "Error! There is more than one decimal in your input.\n";
		}

		if ( hasSpace( totalPts ) )
		{
			cout << "Error! There is a space in your input.\n";
		}

		cout << "Please enter the total points possible, including the decimal: ";
		cin.getline( totalPts , sigFigs );
		cout << endl;
	}

	assignment.entryNum = ( getEntryNum( file ) + 1);
	assignment.title = static_cast<string>( title );
	assignment.score = atof( myScore );
	assignment.total = atof( totalPts );

	double percentage = ( assignment.score / assignment.total ) * 100.0;


	cout << "--------------------Entry--------------------" << endl;
	cout << "Title: " << title << endl;
	cout << "Score: " << myScore << " out of " << totalPts;
	cout << " (" << percentage << "%)" << endl;
	
	cout << "Is this okay? Enter C to CANCEL. Enter anything else to continue: ";
	cin.get( cancel );
	cin.ignore(1000);

	

	if ( cancel != 'C' )
	{
		file.seekp( 0 , ios::end );
		file.write( reinterpret_cast<char *>( &assignment ) , sizeof( assignment ) );
		cout << "Added assignment to record." << endl;

		int updateNum = ( assignment.entryNum );
		file.seekp( 0 , ios::beg );
		file.write( reinterpret_cast<char *>( &updateNum ) , sizeof( updateNum ) );
	}

	else
		cout << "Cancelled." << endl;
}

void displayall( fstream &file )
{
	int *numEntries = NULL;
	file.seekg( 0 , ios::beg );
	file.read( reinterpret_cast<char *>( numEntries ) , sizeof( int ) );

	if ( *numEntries > 0 )
	{
		cout << "-----------------------Assignments-----------------------";
		for ( int i = 0; i < (*numEntries); i++ )
		{
			Entry *entryPtr = NULL;
			file.seekg( ( FIRST_INDEX + ( i * sizeof( Entry ) ) ) , ios::beg );
			file.read( reinterpret_cast<char *>( entryPtr ) , sizeof( Entry ) );

			double percentage = ( entryPtr->score / entryPtr->total ) * 100.0;

			cout << entryPtr->entryNum << ". ";
			cout << entryPtr->title << " -- ";
			cout << entryPtr->score << " out of " << entryPtr->total;
			cout << " (" << percentage << "%)" << endl;
		}
		cout << "---------------------------------------------------------";
	}

	else
	{
		cout << "There are no entries in this file." << endl;
	}




}




bool hasAlpha( const char *str )
{

	for ( int i = 0 ; i < strlen( str ) ; i++ )
	{
		if ( isalpha( *( str + i ) ) )
		{
			return true;
		}
	}

	return false;

}

bool noDec( const char *str )
{

	for ( int i = 0 ; i < strlen( str ) ; i++ )
	{
		if (  *( str + i ) == '.' )
		{
			return false;
		}
	}

	return true;

}

bool manyDec( const char *str )
{
	if ( noDec( str ) )
	{
		return false;
	}

	int count = 0;

	for ( int i = 0 ; i < strlen( str ) ; i++ )
	{
		if (  *( str + i ) == '.' )
		{
			count = count + 1;
		}

		if ( count > 1 )
		{
			return true;
		}
	}

	return false;

}

bool hasSpace( const char *str )
{
	for ( int i = 0 ; i < strlen( str ) ; i++ )
	{
		if ( isspace( *( str + i ) ) )
		{
			return true;
		}
	}

	return false;

}

bool pointsValidation( const char * str )
{

	if ( hasAlpha( str ) || noDec( str ) || manyDec( str ) || hasSpace( str ) )
	{
		return false;
	}

	else
	{
		return true;
	}
}


int getEntryNum( fstream &file )
{
	int *numEntries;
	file.seekg( 0 , ios::beg );
	file.read( reinterpret_cast<char *>( numEntries ) , sizeof( int ) );

	return ( *numEntries );
}

void OpenOrCreate()
{
	cout << "--------------------------MENU---------------------------" << endl;
	cout << "1. Open a record." << endl;
	cout << "2. Create a new record." << endl;
	cout << "3. QUIT" << endl;
	cout << "---------------------------------------------------------" << endl;

}

void showMenu()
{
	cout << "--------------------------MENU---------------------------" << endl;
	cout << "1. Add an assignment." << endl;
	cout << "2. QUIT" << endl;
	cout << "---------------------------------------------------------" << endl;
}