
GLuint texture_white[16];
GLuint texture_black[16];
GLuint texture_button[15];
GLuint texture_background[2];
char* file_black[16] =
{
	"pieces/pieces_black/blank.bmp",
	"pieces/pieces_black/pawn_white.bmp",
	"pieces/pieces_black/king_white.bmp",
	"pieces/pieces_black/knight_white.bmp",
	"pieces/pieces_black/blank.bmp",
	"pieces/pieces_black/bishop_white.bmp",
	"pieces/pieces_black/rook_white.bmp",
	"pieces/pieces_black/queen_white.bmp",
	"pieces/pieces_black/blank.bmp",
	"pieces/pieces_black/pawn_black.bmp",
	"pieces/pieces_black/king_black.bmp",
	"pieces/pieces_black/knight_black.bmp",
	"pieces/pieces_black/blank.bmp",
	"pieces/pieces_black/bishop_black.bmp",
	"pieces/pieces_black/rook_black.bmp",
	"pieces/pieces_black/queen_black.bmp"
};
char* file_white[16] =
{
	"pieces/pieces_white/blank.bmp",
	"pieces/pieces_white/pawn_white.bmp",
	"pieces/pieces_white/king_white.bmp",
	"pieces/pieces_white/knight_white.bmp",
	"pieces/pieces_white/blank.bmp",
	"pieces/pieces_white/bishop_white.bmp",
	"pieces/pieces_white/rook_white.bmp",
	"pieces/pieces_white/queen_white.bmp",
	"pieces/pieces_white/blank.bmp",
	"pieces/pieces_white/pawn_black.bmp",
	"pieces/pieces_white/king_black.bmp",
	"pieces/pieces_white/knight_black.bmp",
	"pieces/pieces_white/blank.bmp",
	"pieces/pieces_white/bishop_black.bmp",
	"pieces/pieces_white/rook_black.bmp",
	"pieces/pieces_white/queen_black.bmp"
};

char* file_button[23] =
{
	"pieces/buttons/new_base.bmp",
	"pieces/buttons/new_click.bmp",
	"pieces/buttons/cb_base.bmp",
	"pieces/buttons/cb_click.bmp",
	"pieces/buttons/cc_base.bmp",
	"pieces/buttons/cc_click.bmp",
	"pieces/buttons/undo_base.bmp",
	"pieces/buttons/undo_click.bmp",
	"pieces/buttons/black_turn.bmp",
	"pieces/buttons/white_turn.bmp",
	"pieces/buttons/black_wins.bmp",
	"pieces/buttons/white_wins.bmp",
	"pieces/buttons/draw.bmp",
	"pieces/buttons/check.bmp",
	"pieces/buttons/no_check.bmp",
	"pieces/buttons/about_base.bmp",
	"pieces/buttons/about_click.bmp",
	"pieces/buttons/back_base.bmp",
	"pieces/buttons/back_click.bmp",
	"pieces/buttons/play_base.bmp",
	"pieces/buttons/play_click.bmp",
	"pieces/buttons/home_base.bmp",
	"pieces/buttons/home_click.bmp"


};

char* file_background[3] =
{
	"pieces/background.bmp",
	"pieces/background2.bmp",
	"pieces/about.bmp"
};
void drawstring(float x, float y, const char *s)
{
	int i;
	glRasterPos2f(x, y);

	for (i = 0; i < s[i] != '\0'; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[i]);
	}
}
void drawstringg(float x, float y, const char *s)
{
	int i;
	glRasterPos2f(x, y);

	for (i = 0; i < s[i] != '\0'; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
	}
}


//inline void mySleep(clock_t sec) // clock_t is a like typedef unsigned int clock_t
//{
//	clock_t start_time = clock();
//	clock_t end_time = sec * 150 + start_time;
//	while (clock() != end_time);
//}
void mySleep2(clock_t sec) // clock_t is a like typedef unsigned int clock_t
{
	clock_t start_time = clock();
	clock_t end_time = sec*60 + start_time;
	//cout << start_time;
	//cout << endl << end_time;
	for (; clock() < end_time;);
}
//void undo_button(float x, float y, float z)
//{
//	//undo button
//	glBegin(GL_QUADS);
//	glColor3f(x, y, z);
//	glVertex2f(50, 520);
//	glColor3f(x - 0.3, y - 0.3, z - 0.3);
//	glVertex2f(120, 520);
//	glColor3f(x, y, z);
//	glVertex2f(120, 490);
//	glColor3f(x - 0.3, y - 0.3, z - 0.3);
//	glVertex2f(50, 490);
//	glEnd();
//	glColor3f(0, 0, 0);
//	drawstring(68, 500, "UNDO");
//	glFlush();
//}
//void comp_move_button(float x, float y, float z)
//{
//	//comp move button
//	glBegin(GL_QUADS);
//	glColor3f(x, y, z);
//	glVertex2f(140, 520);
//	glColor3f(x - 0.3, y - 0.3, z - 0.3);
//	glVertex2f(240, 520);
//	glColor3f(x, y, z);
//	glVertex2f(240, 490);
//	glColor3f(x - 0.3, y - 0.3, z - 0.3);
//	glVertex2f(140, 490);
//	glEnd();
//	glColor3f(0, 0, 0);
//	drawstring(150, 500, "COMP BLACK");
//	glFlush();
//}
//void comp_vs_comp_button(float x, float y, float z)
//{
//	//comp move button
//	glBegin(GL_QUADS);
//	glColor3f(x, y, z);
//	glVertex2f(260, 520);
//	glColor3f(x - 0.3, y - 0.3, z - 0.3);
//	glVertex2f(380, 520);
//	glColor3f(x, y, z);
//	glVertex2f(380, 490);
//	glColor3f(x - 0.3, y - 0.3, z - 0.3);
//	glVertex2f(260, 490);
//	glEnd();
//	glColor3f(0, 0, 0);
//	drawstring(270, 500, "COMP VS COMP");
//	glFlush();
//}
//void new_game_button(float x, float y, float z)
//{
//	glBegin(GL_QUADS);
//	glColor3f(x, y, z);
//	glVertex2f(400, 520);
//	glColor3f(x - 0.1, y - 0.1, z - 0.1);
//	glVertex2f(440, 520);
//	glColor3f(x, y, z);
//	glVertex2f(440, 490);
//	glColor3f(x - 0.1, y - 0.1, z - 0.1);
//	glVertex2f(400, 490);
//	glEnd();
//	glColor3f(0, 0, 0);
//	drawstring(405, 500, "NEW");
//	glFlush();
//}

struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image;
#define checkImageWidth 64
#define checkImageHeight 64
GLubyte checkImage[checkImageWidth][checkImageHeight][3];

Image *image_black[16];
Image *image_white[16];
Image *image_button[23];
Image *background[2];

void makeCheckImage(void)
{
	int i, j, c;
	for (i = 0; i < checkImageWidth; i++) {
		for (j = 0; j < checkImageHeight; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
		}
	}
}
int ImageLoad(char *filename, Image *image) {
	FILE *file;
	unsigned long size; // size of the image in bytes.
	unsigned long i; // standard counter.
	unsigned short int plane; // number of planes in image

	unsigned short int bpp; // number of bits per pixel
	char temp; // temporary color storage for
	// make sure the file is there.
#pragma warning(disable:4996)
	if ((file = fopen(filename, "rb")) == NULL)
	{
		printf("File Not Found : %s\n", filename);
		return 0;
	}
	// seek through the bmp header, up to the width/height:
	fseek(file, 18, SEEK_CUR);
	// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}
	//printf("Width of %s: %lu\n", filename, image->sizeX);
	// read the height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}
	//printf("Height of %s: %lu\n", filename, image->sizeY);
	// calculate the size (assuming 24 bits or 3 bytes per pixel).
	size = image->sizeX * image->sizeY * 3;
	// read the planes
	if ((fread(&plane, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}
	if (plane != 1) {
		printf("Planes from %s is not 1: %u\n", filename, plane);
		return 0;
	}
	// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);

		return 0;
	}
	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}
	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);
	// read the data.
	image->data = (char *)malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}
	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}
	for (i = 0; i<size; i += 3) { // reverse all of the colors. (bgr -> rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	// we're done.
	return 1;
}
void loadTexture()
{
	int i;
	for (i = 0; i < 16; i++)
	{
		// allocate space for texture
		image_black[i] = (Image *)malloc(sizeof(Image));
		if (image_black[i] == NULL) {
			printf("Error allocating space for image");
			exit(0);
		}
		//pic.bmp is a 64x64 picture
		if (!ImageLoad(file_black[i], image_black[i])) {
			exit(1);
		}
	}

	for (i = 0; i < 16; i++)
	{
		// allocate space for texture
		image_white[i] = (Image *)malloc(sizeof(Image));
		if (image_white[i] == NULL) {
			printf("Error allocating space for image");
			exit(0);
		}
		//pic.bmp is a 64x64 picture
		if (!ImageLoad(file_white[i], image_white[i])) {
			exit(1);
		}
	}

	for (i = 0; i < 23; i++)
	{
		// allocate space for texture
		image_button[i] = (Image *)malloc(sizeof(Image));
		if (image_button[i] == NULL) {
			printf("Error allocating space for image");
			exit(0);
		}
		//pic.bmp is a 64x64 picture
		if (!ImageLoad(file_button[i], image_button[i])) {
			exit(1);
		}
	}

}
void texture_load_black(int index)
{
	glClearColor(1, 1, 1, 1);


	/*if (image1 == NULL)
	{
	printf("Image was not returned from loadTexture\n");
	exit(0);
	}*/
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	glGenTextures(16, texture_black);
	glBindTexture(GL_TEXTURE_2D, texture_black[index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image_black[index]->sizeX, image_black[index]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image_black[index]->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
}
void texture_load_white(int index)
{
	glClearColor(1, 1, 1, 1);
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	glGenTextures(16, texture_white);
	glBindTexture(GL_TEXTURE_2D, texture_white[index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image_white[index]->sizeX, image_white[index]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image_white[index]->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
}

void texture_load_button(int index)
{
	glClearColor(1, 1, 1, 1);
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	glGenTextures(21, texture_button);
	glBindTexture(GL_TEXTURE_2D, texture_button[index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image_button[index]->sizeX, image_button[index]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image_button[index]->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
}


void draw_square_black(float x, float y, float width, int index)
{
	texture_load_black(index);
//	glBindTexture(GL_TEXTURE_2D, texture_black[index]);
	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(x, y);

	glTexCoord2f(1, 0);
	glVertex2f(x + width, y);

	glTexCoord2f(1, 1);
	glVertex2f(x + width, y + width);

	glTexCoord2f(0, 1);
	glVertex2f(x, y + width);

	glEnd();
	//	glFlush();
}
void draw_square_white(float x, float y, float width, int index)
{
	texture_load_white(index);

//x	glBindTexture(GL_TEXTURE_2D, texture_white[index]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(x, y);

	glTexCoord2f(1, 0);
	glVertex2f(x + width, y);

	glTexCoord2f(1, 1);
	glVertex2f(x + width, y + width);

	glTexCoord2f(0, 1);
	glVertex2f(x, y + width);

	glEnd();
	//	glFlush();
}

void draw_button(float x1, float x2, int index,int ya=490, int yb=520)
{
	texture_load_button(index);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(x1, ya);

	glTexCoord2f(1, 0);
	glVertex2f(x2, ya);

	glTexCoord2f(1, 1);
	glVertex2f(x2, yb);

	glTexCoord2f(0, 1);
	glVertex2f(x1, yb);

	glEnd();
	//	glFlush();
}


void whose_turn(int turn)
{
	int i;
	if (turn == 1)
	{
		draw_button(475, 586, 9, 220, 420);
	}
	else
	{
		draw_button(475, 586, 8,220, 420);
	}
	glFlush();
}
void whose_turn_display()
{
	if (board.get_nextMove() == WHITE_MOVE)
	{
		whose_turn(1);

	}
	else
	{
		whose_turn(2);
	}
}

void is_king_check(float x, float y, float z)
{
	draw_button(474, 585, 13, 100, 180);

}
void clear_king_check(float x, float y, float z)
{
	draw_button(474, 585, 14, 100, 180);

}



void display_board()
{
	
	for (i = 0; i<maxx; i++)
		x[i] = x0 + i*dx;
	for (j = 0; j<maxy; j++)
		y[j] = yk + j*dy;
	int k = 0, l = 0, ind = 0;
	int file, rank;
		for (rank = 8; rank >= 1; rank--)
	{
		for (file = 1; file <= 8; file++)
		{
			if (l > 10)l = 0;
			ind = board.square[BOARDINDEX[file][rank]];
			if (k % 2 == 0)
			{
				draw_square_black(x[file] - dx, y[rank] - dx, dx, ind);
				glDeleteTextures(16, texture_black);

			}
			else
			{
				draw_square_white(x[file] - dx, y[rank] - dx, dx, ind);
				glDeleteTextures(16, texture_white);

			}
			k++;

		}
		k++;
	}
		glFlush();
		if (onep == true)
		{

			if (board.get_nextMove() == BLACK_MOVE)
			{
				draw_button(475, 586, 8, 220, 420);
				glFlush();
				mySleep2(2);
				board.searchDepth = 3;
				sprintf(gui_ip, "go");
				input.read_command(gui_ip); 
			}
		}

	if (isOwnKingAttacked())
	{
		is_king_check(1, 0, 0);
	}
	else
		clear_king_check(0.5, 0.2, 0);
	glFlush();

//freeTexture();
}

void draw_background(int index)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	background[index] = (Image *)malloc(sizeof(Image));
	if (background[index] == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	//pic.bmp is a 64x64 picture
	if (!ImageLoad(file_background[index], background[index])) {
		getchar();
		exit(1);
	}

	makeCheckImage();


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Create Texture
	glGenTextures(1, &texture_background[index]);
	glBindTexture(GL_TEXTURE_2D, texture_background[index]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, background[index]->sizeX, background[index]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, background[index]->data);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 0);
	glVertex2f(0, 0);

	glTexCoord2f(1, 0);
	glVertex2f(600, 0);

	glTexCoord2f(1, 1);
	glVertex2f(600, 600);

	glTexCoord2f(0, 1);
	glVertex2f(0, 600);
	glEnd();

	glDeleteTextures(1, &texture_background[index]);
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	loadTexture();

	//glFlush();
	//getchar();
	if (main_menu == true)
	{
		draw_background(1);
		draw_button(250, 340, 19, 360, 390);
		draw_button(250, 340, 15, 290, 320);
		glFlush();
	}
	else if (game == true)
	{
		draw_background(0);
		glFlush();
		display_board();
		draw_button(40, 130, 21, 550, 580);
		draw_button(40, 130, 0);
		draw_button(150, 240, 2);
		draw_button(260, 350, 4);
		draw_button(370, 460, 6);
		whose_turn_display();
		//glFlush();
	}
	else if (about_menu == true)
	{
		draw_background(2);
		draw_button(250, 340, 17, 100, 130);

		glFlush();

	}
	for (int i = 0; i < 23; i++)
	{
		glDeleteTextures(1, &texture_button[i]);
	}
	glColor3f(1, 1, 1);


}

void mouse(int button, int status, int px, int py)
{
	py -= gh;
	py = -py;
	int k = 0;
#pragma warning (disable:4996)

	if (main_menu == true)
	{
		if (button == GLUT_LEFT_BUTTON && status == GLUT_DOWN)
		{
			if ((px > 250) && (px < 340) && (py > 360) && (py < 390))
			{
				draw_button(250, 340, 20, 360, 390);
				glFlush();
				mySleep2(3);
				draw_button(250, 340, 19, 360, 390);
				glFlush();
				mySleep2(1);
				//glFlush();
				main_menu = false;
				game = true;
				glutPostRedisplay();
			}
			else if ((px > 250) && (px < 340) && (py > 290) && (py < 320))
			{
				draw_button(250, 340, 16, 290, 320);
				glFlush();
				mySleep2(3);
				draw_button(250, 340, 15, 290, 320);
				glFlush();
				mySleep2(1);
				//glFlush();
				main_menu = false;
				about_menu = true;
				glutPostRedisplay();
			}
		}
	}
	else if (about_menu == true)
	{
		if (button == GLUT_LEFT_BUTTON && status == GLUT_DOWN)
		{
			draw_button(250, 340, 18, 100, 130);
			glFlush();
			mySleep2(1);
			about_menu = false;
			main_menu = true;
			glutPostRedisplay();

		}
	}
	else
	{
		if (button == GLUT_LEFT_BUTTON && status == GLUT_DOWN)
		{
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				if ((px > x[j]) && (px < x[j + 1]) && (py > y[i]) && (py < y[i + 1]))
				{

					if (flag % 2 == 0)
					{
						sprintf(buf[0], "%s", SQUARENAME[j + (8 * k)]);
					}
					else
					{
						sprintf(buf[1], "%s", SQUARENAME[j + (8 * k)]);
					}
					flag++;
					flag = flag % 2;	
				}
				k++;
		
			}
			if ((px > 40) && (px < 130) && (py > 490) && (py < 520))
			{
				draw_button(40, 130, 1);
				board.searchDepth = 3;
				sprintf(gui_ip, "new");
				input.read_command(gui_ip);
				mySleep2(2);
				draw_button(40, 130, 0);



			}

			else if ((px > 150) && (px < 240) && (py > 490) && (py < 520))
			{
				draw_button(150, 240, 3);
				board.searchDepth = 3;
				if (onep == false)
					onep = true;
				else
				{
					onep = false;
					draw_button(150, 240, 2);
				}
				display_board();
				//mySleep2(2);

			}

			else if ((px > 260) && (px < 350) && (py > 490) && (py < 520))
			{
				draw_button(260, 350, 5);
				board.searchDepth = 2;
				sprintf(gui_ip, "cc");
				input.read_command(gui_ip);
				mySleep2(2);
				//			mySleep2(3);
				draw_button(260, 350, 4);

			}

			else if ((px > 40) && (px < 130) && (py > 550) && (py < 580))
			{
				draw_button(40, 130, 22, 550, 580);
				glFlush();
				mySleep2(3);
				draw_button(40, 130, 21, 550, 580);
				glFlush();
				game = false;
				main_menu = true;
				glutPostRedisplay();
			}
			else if ((px > 370) && (px < 460) && (py > 490) && (py < 520))
			{
				draw_button(370, 460, 7);
				sprintf(gui_ip, "undo");
				input.read_command(gui_ip);
				mySleep2(2);
				draw_button(370, 460, 6);
				whose_turn_display();

			}

			else if (flag % 2 == 0)
			{
				if ((buf[0][0] == '\0') && (buf[1][0] == '\0'))printf("OUTSIDE THE BOARD!\n");
				else if ((buf[0][0] == buf[1][0]) && (buf[0][1] == buf[1][1]))
				{
					printf("Same square clicked!\n");			
				}
				else
				{
					sprintf(gui_ip, "move %sq\n", buf[0]);
					input.read_command(gui_ip);
					buf[0][0] = '\0';
					buf[1][0] = '\0';
					buf[0][1] = '\0';
					buf[1][1] = '\0';

				}
				//printf("%s", gui_ip);

			}
			else
			{
				mySleep2(3);
			}
			/*display_blank_board();
			populate_board();*/
			// 	display_board();
			//		freeTexture();
			//glColor3f(1, 1, 1);
			//	whose_turn_display();
			glFlush();

		}
	}
}


void reshape(int w, int h)
{
	gw = w;
	gh = h;
	//glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 600, 0, 600);
	glMatrixMode(GL_MODELVIEW);
	glutReshapeWindow(600, 600);
}
