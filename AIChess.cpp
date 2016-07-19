#include"definitions.h"
#include"prototypes.h"
#include"timer.h"
#include"move_structure.h"
#include"move_tree.h"
#include"board.h"
//#include"fen_string.h"
#include"bit_operations.h"
#include"move_generator.h"
#include"move_display.h"
#include"data.h"
#include"user_input.h"
user_input input;
#include"search.h"
//#include"gui.h"
#include"gui2.h"
int main()
{


	cout << "________________________________________________________________________________"<<endl;
	cout <<setw(45)<< "A.I. CHESS" << endl;
	cout << "________________________________________________________________________________" << endl;
	cout << "\n\n\n\n\n\n";
	//cout << "PRESS ANY KEY TO CONTINUE...";
	//getch();
	dataInit(); 
	glutInitWindowSize(600, 600);
	glutCreateWindow("CHESS");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMainLoop();
	cout << "\n\nTHANK YOU FOR PLAYING!";
	getch();
	return 0;
}