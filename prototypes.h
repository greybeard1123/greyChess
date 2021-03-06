
unsigned int  bitCnt(BitMap);
unsigned int  firstOne(BitMap);
void          displayBitmap(BitMap);
void          setup();
bool	      isAttacked(BitMap &, const char &);
bool      isOtherKingAttacked();
bool      isOwnKingAttacked();
void          makeBlackPromotion(unsigned int, unsigned int &);
void          makeCapture(unsigned int &, unsigned int &);
void          makeWhitePromotion(unsigned int, unsigned int &);
void          unmakeBlackPromotion(unsigned int, unsigned int &);
void          unmakeCapture(unsigned int &, unsigned int &);
void          unmakeWhitePromotion(unsigned int, unsigned int &);
void			display_board();
void display();
void loadTexture();
void freeTexture();
void texture_load_white(int);
void texture_load_black(int);
void is_king_check(float x, float y, float z);
//void			display_blank_board();
void draw_button(float, float, int, int, int);
void whose_turn_display();
void drawstringg(float, float,const char*);
inline void mySleep2(clock_t sec);
void mouse(int button, int status, int px, int py);