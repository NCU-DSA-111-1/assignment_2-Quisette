// `初始化本程式並讀入棋駒之各項資料`
void initialize();
// `讀入SFEN棋譜`
void readKifu(FILE *file);
// `將現有盤面轉為SFEN字串`
void exportToSFEN(char *str);
// `將移動後的資訊輸出於螢幕上`
void renderBoard();
// `瀏覽棋譜功能`
void scrollKifu(bool function, Node *current);
// `處理使用者輸入`
int userInput();
// `將輸入SFEN字串轉為本程式之資料結構`
bool SFENParse(char *sfen);
// `將轉化後的 `SFENData_t `結構化為 `Board` 結構`
void SFENLoad(SfenData_t data);
// `將SFEN棋駒代號轉為本程式的編號` (enum PieceType)
int getPieceNumber(char c);
// `將一般座標轉化為矩陣座標`
char coordTransfer(char axis, char input);
// `根據傳入之座標取得盤面上該座標之棋駒`
Piece_t *getPieceBycoord(Location_t loc);
// `根據棋駒取得該棋駒之名稱`
char *getPieceName(Piece_t piece);
// `移動棋駒(含昇變)`
void makeMove(Location_t init, Location_t final, bool promote);
// `根據代號輸出該棋駒的擁有者`
char owner(char pieceType);
// `確認移動是否合乎金將(或相同走法之棋駒)之移動方式`
bool kinMove(Location_t loc, bool owner);
// `確認移動是否合乎各棋駒之移動方式` 
bool validMove(Location_t init, Location_t final);
// `確認移動是否合乎玉將之移動方式` 
bool gyokuMove(Location_t loc);
// `根據棋駒之羅馬拼音輸出該棋駒之編號`
int getPieceNumByName(char *str);
// `根據棋駒與移動位置是否合乎金將(或相同走法之棋駒)之移動方式`
bool kinDetection(char type, Location_t diff);
// `確認移動是否合乎角形之移動方式` 
bool kakuMove(Location_t diff, Location_t init);
// `確認移動是否合乎飛車之移動方式`  
bool hisyaMove(Location_t diff, Location_t init);
// `根據紀錄的鏈結串列產生棋譜`
void generateKifu(LinkedList list);
// `確認是否該棋駒可以進行昇變`
bool canPromote();
// `悔棋`
bool revert();
// `初始化盤面`
void initializeBoard();
// `強制昇變`
bool forcePromote();
