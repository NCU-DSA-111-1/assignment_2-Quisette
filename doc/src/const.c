// `判斷是否為正確SFEN之正規表示式`
#define SFEN_REGEX "[+plnsgkrbPLNSGKRB1-9]{1,18}\\/[+plnsgkrbPLNSGKRB1-9]{1,18}\\/[+plnsgkrbPLNSGKRB1-9]{1,18}\\/[+plnsgkrbPLNSGKRB1-9]{1,18}\\/[+plnsgkrbPLNSGKRB1-9]{1,18}\\/[+plnsgkrbPLNSGKRB1-9]{1,18}\\/[+plnsgkrbPLNSGKRB1-9]{1,18}\\/[+plnsgkrbPLNSGKRB1-9]{1,18}\\/[+plnsgkrbPLNSGKRB1-9]{1,18} [wb] [-plnsgkrbPLNSGKRB1-9]+ [1-9]+"

// `判斷是否為正確輸入之正規表示式`
#define INPUT_REGEX "[0-9]{1,2}\\s[1-9]{1,2}"

// `重新命名(簡化)結構體`
typedef struct Attr Attr;
typedef struct Komadai Komadai_t;
typedef struct UserInput UserInput;
typedef struct Location Location_t;
typedef struct Piece Piece_t;
typedef struct Board Board;
typedef struct SfenData SfenData_t;