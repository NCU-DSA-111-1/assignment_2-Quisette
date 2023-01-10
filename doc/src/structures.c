// `每種棋駒的各項性質，包含名稱、中文與棋譜輸出代號`
struct Attr {
    char sfen;
    char *displayChar[2];
    char *name[2];
};
// `主棋盤上的棋駒`
struct Piece {
    char type;
    bool promoted;
};
// `駒台儲存該玩家持有的持駒，編號以` enum `表示`
struct Komadai {
    u_int8_t komaList[8];
};
// `完整遊戲所需的所有資料（主盤面、先後手駒台、手數、手番）` 
struct Board {
    Piece_t pieces[9][9]; 
    Komadai_t senteKomadai, goteKomadai;
    int moveNumber;
    bool turn;
};
// `SFEN 棋譜處理後之資料儲存結構`
struct SfenData {
    char *matrix[9];
    char turn;  // turn
    char *mochiKomaList;
    int moveNumber;
};
// `棋駒之座標結構，可用於表示位置或向量`
struct Location {
    int X;
    int Y;
};
// `使用者輸入之結構`
struct UserInput {
    char type[10];
    Location_t init;
    Location_t final;
};