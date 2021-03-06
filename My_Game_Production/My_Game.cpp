//########## ヘッダーファイル読み込み ##########
#include "DxLib.h"
#include "resource.h"
#include <math.h>
//########## マクロ定義 ##########
#define _CRT_SECURE_NO_WARNINGS
#define GAME_WIDTH			1280	//画面の横の大きさ
#define GAME_HEIGHT			720	    //画面の縦の大きさ
#define GAME_COLOR			32	    //画面のカラービット

#define GAME_WINDOW_BAR		0					//タイトルバーはデフォルトにする
#define GAME_WINDOW_NAME	"狂病院からの脱出"  //ウィンドウのタイトル

#define GAME_FPS			60	//FPSの数値	

//マウスのボタン
#define MOUSE_BUTTON_CODE	129	//0x0000〜0x0080まで

//キーボードの種類
#define KEY_CODE_KIND		256	//256種類

//パスの長さ
#define PATH_MAX			255	//255文字まで
#define NAME_MAX			255	//255文字まで

//フォント
#define FONT_ONRYO_PATH			TEXT(".\\FONT\\onryou.TTF")	        //フォントの場所
#define FONT_ONRYO_NAME			TEXT("怨霊")			            //フォントの名前

//エラーメッセージ
#define FONT_INSTALL_ERR_TITLE	TEXT("フォントインストールエラー")
#define FONT_CREATE_ERR_TITLE	TEXT("フォント作成エラー")

//エラーメッセージ
#define IMAGE_LOAD_ERR_TITLE	TEXT("画像読み込みエラー")

//画像のパス
#define IMAGE_TITLE_BACK_PATH   TEXT(".\\IMAGE\\Title_bg.jpg")               //タイトル背景
#define IMAGE_TITLE_ROGO_PATH   TEXT(".\\IMAGE\\Title2.png")                 //タイトルロゴ
#define IMAGE_EASY_ROGO_PATH    TEXT(".\\IMAGE\\Easy_logo.png")              //イージーロゴ
#define IMAGE_EASY_ROGO2_PATH   TEXT(".\\IMAGE\\Easy_logo2.png")             //イージーロゴ（画像上）
#define IMAGE_HARD_ROGO_PATH    TEXT(".\\IMAGE\\Hard_logo.png")              //ハードロゴ
#define IMAGE_HARD_ROGO2_PATH   TEXT(".\\IMAGE\\Hard_logo2.png")             //ハードロゴ（画像上）
#define IMAGE_PLAY_BACK_PATH    TEXT(".\\IMAGE\\Play_bg.jpg")                //プレイ背景
#define IMAGE_RULE_PATH         TEXT(".\\IMAGE\\Rule.png")                   //ルール説明画面背景
#define IMAGE_QUES_BACK_PATH    TEXT(".\\IMAGE\\Play_bg.jpg")                //問題背景
#define IMAGE_QUES_1_PATH       TEXT(".\\IMAGE\\Qusetion1.png")              //問題１画像
#define IMAGE_QUES_2_PATH       TEXT(".\\IMAGE\\Qusetion2.png")              //問題２画像
#define IMAGE_RIGHT_PATH        TEXT(".\\IMAGE\\Rule.png")                   //正解画像
#define IMAGE_WRONG_PATH        TEXT(".\\IMAGE\\play2_bg.jpg")               //不正解画像
#define IMAGE_STORY_BACK_PATH   TEXT(".\\IMAGE\\Title_bg.jpg")               //ストーリー背景
#define IMAGE_NEXT_PATH         TEXT(".\\IMAGE\\Next.png")                   //つぎへボタン
#define IMAGE_NEXT2_PATH        TEXT(".\\IMAGE\\Next2.png")                  //つぎへボタンhover
#define IMAGE_END_BACK_PATH     TEXT(".\\IMAGE\\ゲームオーバーシンプル.png") //ゲームオーバー背景
#define IMAGE_END_CLEAR_PATH     TEXT(".\\IMAGE\\end.jpeg")                  //クリア背景

//画像関連
#define IMAGE_ROGO_ROTA         1.0      //拡大率
#define IMAGE_ROGO_ROTA_MAX     2.0      //拡大率MAX

//エラーメッセージ
#define MUSIC_LOAD_ERR_TITLE	TEXT("音楽読み込みエラー")

//音楽のパス
#define MUSIC_TITLE_PATH        TEXT(".\\MUSIC\\イビツ.mp3")
#define MUSIC_RULE_PATH         TEXT(".\\MUSIC\\不安な果実.mp3")
#define MUSIC_QUES_PATH         TEXT(".\\MUSIC\\不安な果実.mp3")
#define MUSIC_WRONG_PATH        TEXT(".\\MUSIC\\se_zuburi.wav")
#define MUSIC_STORY_PATH        TEXT(".\\MUSIC\\不気味な部屋.mp3")
#define MUSIC_END_PATH          TEXT(".\\MUSIC\\コールドフィッシュ.mp3")
#define MUSIC_END2_PATH         TEXT(".\\MUSIC\\今後ろに誰かが....mp3")

//閉じるボタンを押したとき
#define MSG_CLOSE_TITLE			TEXT("終了メッセージ")
#define MSG_CLOSE_CAPTION		TEXT("ゲームを終了しますか？")

//問題数
#define QUES_MAX                2

//入力タブサイズ
#define BOX_SIZE_X             GAME_WIDTH / 2 - 250
#define BOX_SIZE_Y             GAME_HEIGHT - 120

#define GAME_PI				    3.1415	   //π
#define GAME_HANKEI				300	       //円の半径

//院長との距離カウント
#define COUNTDIST              4

//メッセージの表示
#define MESSAGE_FONT_SIZE      50          //メッセージのフォントの大きさ
#define MESSAGE_MAX_LENGTH     50          //最大文字数
#define MESSAGE_MAX_LINE       20           //最大行数
#define MESSAGE_BOX_X          0          //メッセージボックスのX座標
#define MESSAGE_BOX_Y          0           //メッセージボックスのY座標
#define MESSAGE_BOX_GRAPHIC_FILENAME       "./IMAGE/背景＿黒筋.png"

enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_RULE,
	GAME_SCENE_QUES_EASY,
	GAME_SCENE_QUES_HARD,
	GAME_SCENE_ROOT,
	GAME_SCENE_STORY,
	GAME_SCENE_END,
};	//ゲームのシーン

enum QUES_END {
	QUES_END_RIGHT,	//正解
	QUES_END_WRONG	//不正解
};	//問題の正誤


//int型のPOINT構造体
typedef struct STRUCT_I_POINT
{
	int x = -1;	//座標を初期化
	int y = -1;	//座標を初期化
}iPOINT;

//マウス構造体
typedef struct STRUCT_MOUSE
{
	int InputValue = 0;	//GetMouseInputの値を入れる
	int WheelValue = 0;	//マウスホイールの回転量(奥はプラス値 / 手前はマイナス値)
	iPOINT Point;		//マウスの座標が入る
	iPOINT OldPoint;	//マウスの座標(直前)が入る
	int OldButton[MOUSE_BUTTON_CODE] = { 0 };	//マウスのボタン入力(直前)が入る
	int Button[MOUSE_BUTTON_CODE] = { 0 };	//マウスのボタン入力が入る
}MOUSE;

//フォント構造体
typedef struct STRUCT_FONT
{
	char path[PATH_MAX];		//パス
	char name[NAME_MAX];		//フォント名
	int handle;					//ハンドル
	int size;					//大きさ
	int bold;					//太さ
	int type;					//タイプ
	//※タイプは、https://dxlib.xsrv.jp/function/dxfunc_graph2.html#R17N10　を参照

}FONT;

typedef struct STRUCT_IMAGE
{
	char path[PATH_MAX];		//パス
	int handle;					//ハンドル
	int x;						//X位置
	int y;						//Y位置
	int width;					//幅
	int height;					//高さ
	BOOL IsDraw = FALSE;		//描画できるか

	RECT coll;					//当たり判定
	iPOINT collBeforePt;		//当たる前の座標

}IMAGE;	//画像構造体

typedef struct STRUCT_IMAGE_ROTA
{
	IMAGE image;		//IMAGE構造体
	double angle;		//回転率
	double angleMAX;	//回転率MAX
	double rate;		//拡大率
	double rateMAX;		//拡大率MAX
	double radian;	    //角度(ラジアン)

}IMAGE_ROTA;	//回転拡大する画像の構造体

typedef struct STRUCT_MUSIC
{
	char path[PATH_MAX];		//パス
	int handle;					//ハンドル
}MUSIC;	//音楽構造体


//########## グローバル変数 ##########
//FPS関連
int StartTimeFps;				//測定開始時刻
int CountFps;					//カウンタ
float CalcFps;					//計算結果
int SampleNumFps = GAME_FPS;	//平均を取るサンプル数

//キーボードの入力を取得
char AllKeyState[KEY_CODE_KIND] = { '\0' };		//すべてのキーの状態が入る
char OldAllKeyState[KEY_CODE_KIND] = { '\0' };	//すべてのキーの状態(直前)が入る
char Answer[51];                                //答えの文字配列保存
//char ANSER[5][40] = { "いんちょう","あ","い","う","え"};             //問題の答え

//マウスの座標を取得
MOUSE mouse;

//ゲーム関連
int GameScene;		//ゲームシーンを管理

//背景関連
IMAGE		ImageTitleBack;          //タイトル背景画像
IMAGE		ImagePlayBack;           //ルール背景画像
IMAGE		ImageTitleRogo;          //タイトルロゴ画像
IMAGE		ImageEasyRogo;           //イージーロゴ画像
IMAGE		ImageEasyRogo2;          //イージーロゴ画像hover
IMAGE		ImageHardRogo;           //ハードロゴ画像
IMAGE		ImageHardRogo2;          //ハードロゴ画像hover
IMAGE		ImageRule;               //ルール説明画像
IMAGE_ROTA	ImageQuesBack;           //問題画面背景画像
IMAGE		ImageQues[QUES_MAX];     //問題画像
IMAGE		ImageRight;              //正解
IMAGE		ImageWrong;              //不正解
IMAGE		ImageStoryBack;          //ストーリー画面背景画像
IMAGE		ImageNextBtn;            //つぎへボタン画像
IMAGE		ImageNextBtn2;           //つぎへボタン画像Hover
IMAGE		ImageEndBack;            //ゲームオーバー画像
IMAGE       ImageEndClear;           //クリア画像

//音楽関連
MUSIC BGM_TITLE;			   //タイトルのBGM
MUSIC BGM_RULE;			       //タイトルのBGM
MUSIC BGM_QUES;			       //問題のBGM
MUSIC BGM_WRONG;               //不正解のBGM
MUSIC BGM_STORY;			   //ストーリーのBGM
MUSIC BGM_END;			       //エンドのBGM
MUSIC BGM_END2;			       //エンドのBGM


int QuesKind;					//問題の正誤状態
int CountRight;                 //正解数
int CountWrong;                 //不正解数

int CountQues;                  //問題数カウント

//ストーリー関連
char message[MESSAGE_MAX_LENGTH * MESSAGE_MAX_LINE];        //表示したいメッセージ
char messageBuffer[MESSAGE_MAX_LINE][MESSAGE_MAX_LENGTH];   //メッセージを表示するための仮想バッファ
static int currentCursor = 0;                               //何文字目まで表示しているか
static int currentLineCursor = 0;                            //何行目を表示しているか
static int whiteColor;
static int blackColor;
static int messageBoxGraphHandle;
int isJapanese(unsigned char code);


//########## プロトタイプ宣言 ##########
VOID MY_FPS_UPDATE(VOID);			//FPS値を計測、更新する
VOID MY_FPS_DRAW(VOID);				//FPS値を描画する
VOID MY_FPS_WAIT(VOID);				//FPS値を計測し、待つ

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//キーの入力状態を更新する
BOOL MY_KEY_DOWN(int);				//キーを押しているか、キーコードで判断する
BOOL MY_KEY_UP(int);				//キーを押し上げたか、キーコードで判断する
BOOL MY_KEYDOWN_KEEP(int, int);		//キーを押し続けているか、キーコードで判断する

VOID MY_MOUSE_UPDATE(VOID);			//マウスの入力情報を更新する
BOOL MY_MOUSE_DOWN(int);			//ボタンを押しているか、マウスコードで判断する
BOOL MY_MOUSE_UP(int);				//ボタンを押し上げたか、マウスコードで判断する
BOOL MY_MOUSEDOWN_KEEP(int, int);	//ボタンを押し続けているか、マウスコードで判断する

BOOL MY_FONT_INSTALL_ONCE(VOID);	//フォントをこのソフト用に、一時的にインストール
VOID MY_FONT_UNINSTALL_ONCE(VOID);	//フォントをこのソフト用に、一時的にアンインストール
BOOL MY_FONT_CREATE(VOID);			//フォントを作成する
VOID MY_FONT_DELETE(VOID);			//フォントを削除する

VOID MY_START(VOID);		//スタート画面
VOID MY_START_PROC(VOID);	//スタート画面の処理
VOID MY_START_DRAW(VOID);	//スタート画面の描画

VOID MY_GAME_INIT(VOID);	    //初期化
VOID MY_PLAY_RULE(VOID);	    //ルール説明画面
VOID MY_PLAY_RULE_PROC(VOID);	//ルール説明画面の処理
VOID MY_PLAY_RULE_DRAW(VOID);	//ルール説明画面の描画

VOID MY_PLAY_QUES_EASY(VOID);       //イージー問題画面
VOID MY_PLAY_QUES_EASY_PROC(VOID);  //問題画面の処理
VOID MY_PLAY_QUES_EASY_DRAW(VOID);  //問題画面の描画

VOID MY_PLAY_QUES_HARD(VOID);       //ハード問題画面
VOID MY_PLAY_QUES_HARD_PROC(VOID);  //問題画面の処理
VOID MY_PLAY_QUES_HARD_DRAW(VOID);  //問題画面の描画


VOID MY_PLAY_STORY(VOID);       //ストーリー画面
VOID MY_PLAY_STORY_PROC(VOID);  //ストーリー画面の処理
VOID MY_PLAY_STORY_DRAW(VOID);  //ストーリー画面の描画

VOID MY_PLAY_ROOT(VOID);       //ルート分岐画面
VOID MY_PLAY_ROOT_PROC(VOID);  //ルート分岐画面の処理
VOID MY_PLAY_ROOT_DRAW(VOID);  //ルート分岐画面の描画

VOID MY_END(VOID);			//エンド画面
VOID MY_END_PROC(VOID);		//エンド画面の処理
VOID MY_END_DRAW(VOID);		//エンド画面の描画

BOOL MY_LOAD_IMAGE(VOID);		//画像をまとめて読み込む関数
VOID MY_DELETE_IMAGE(VOID);		//画像をまとめて削除する関数

BOOL MY_LOAD_MUSIC(VOID);		//音楽をまとめて読み込む関数
VOID MY_DELETE_MUSIC(VOID);		//音楽をまとめて削除する関数

VOID writeSubstring(char* message, int start, int len,
	int PosX, int PosY, int color, int bufferLine);
VOID drawMessage(VOID);
VOID setMessage(const char* ms);  //描画したいメッセージをセット

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//指定の数値でウィンドウを表示する
	SetWindowStyleMode(GAME_WINDOW_BAR);				//タイトルバーはデフォルトにする
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));			//ウィンドウのタイトルの文字
	SetAlwaysRunFlag(TRUE);								//非アクティブでも実行する
	SetWindowIconID(IDI_ICON1);							//アイコンファイルを読込
	SetWindowUserCloseEnableFlag(FALSE);				//閉じるボタンで、勝手にウィンドウが閉じないようにする

	if (DxLib_Init() == -1) { return -1; }	//ＤＸライブラリ初期化処理

	//画像を読み込む
	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

	//音楽を読み込む
	if (MY_LOAD_MUSIC() == FALSE) { return -1; }

	//フォントを一時的にインストール
	if (MY_FONT_INSTALL_ONCE() == FALSE) { return -1; }
	//フォントハンドルを作成
	if (MY_FONT_CREATE() == FALSE) { return -1; }

	//初期化
	MY_GAME_INIT();

	GameScene = GAME_SCENE_START;	//ゲームシーンはスタート画面から
	SetDrawScreen(DX_SCREEN_BACK);	//Draw系関数は裏画面に描画

	//無限ループ
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージ処理の結果がエラーのとき、強制終了
		if (ClearDrawScreen() != 0) { break; }	//画面を消去できなかったとき、強制終了

		//画面の閉じるボタンを押されたとき
		if (GetWindowUserCloseFlag(TRUE))
		{
			//終了ダイアログを表示
			int Ret = MessageBox(GetMainWindowHandle(), MSG_CLOSE_CAPTION, MSG_CLOSE_TITLE, MB_YESNO | MB_ICONASTERISK);
			if (Ret == IDYES) { break; }	//YESなら、ゲームを中断する
		}

		MY_ALL_KEYDOWN_UPDATE();				//押しているキー状態を取得

		MY_MOUSE_UPDATE();						//マウスの状態を取得

		MY_FPS_UPDATE();	//FPSの処理[更新]

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();	//スタート画面
			break;
		case GAME_SCENE_RULE:
			MY_PLAY_RULE();	//ルール説明画面
			break;
		case GAME_SCENE_QUES_EASY:
			MY_PLAY_QUES_EASY();	//イージー問題画面
			break;
		//case GAME_SCENE_QUES_HARD:
		//	MY_PLAY_QUES_HARD();	//ハード問題画面
		//	break;
		case GAME_SCENE_STORY:
			MY_PLAY_STORY();	//ストーリー画面
			break;
		case GAME_SCENE_ROOT:
			MY_PLAY_ROOT();	//ルート分岐画面
			break;
		case GAME_SCENE_END:
			MY_END();	//エンド画面
			break;
		}

		MY_FPS_DRAW();		//FPSの処理[描画]

		ScreenFlip();		//モニタのリフレッシュレートの速さで裏画面を再描画

		MY_FPS_WAIT();		//FPSの処理[待つ]
	}

	//フォントハンドルを破棄
	MY_FONT_DELETE();

	//一時的にインストールしたフォントをアンインストール
	MY_FONT_UNINSTALL_ONCE();

	//画像ハンドルを破棄
	MY_DELETE_IMAGE();

	//音楽ハンドルを破棄
	MY_DELETE_MUSIC();

	DxLib_End();	//ＤＸライブラリ使用の終了処理

	return 0;
}

//########## FPS値を計測、更新する関数 ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1フレーム目なら時刻を記憶
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60フレーム目なら平均を計算
	{
		int now = GetNowCount();
		//現在の時間から、0フレーム目の時間を引き、FPSの数値で割る＝1FPS辺りの平均時間が計算される
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

//########## FPS値を描画する関数 ##########
VOID MY_FPS_DRAW(VOID)
{
	//文字列を描画
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

//########## FPS値を計測し、待つ関数 ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//かかった時間
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;	//待つべき時間

	if (waitTime > 0)		//指定のFPS値よりも早い場合
	{
		WaitTimer(waitTime);	//待つ
	}
	return;
}

//########## キーの入力状態を更新する関数 ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{

	//一時的に、現在のキーの入力状態を格納する
	char TempKey[KEY_CODE_KIND];

	//直前のキー入力をとっておく
	for (int i = 0; i < KEY_CODE_KIND; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey); // 全てのキーの入力状態を得る

	for (int i = 0; i < KEY_CODE_KIND; i++)
	{
		if (TempKey[i] != 0)	//押されているキーのキーコードを押しているとき
		{
			AllKeyState[i]++;	//押されている
		}
		else
		{
			AllKeyState[i] = 0;	//押されていない
		}
	}
	return;
}

//キーを押しているか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	//キーコードのキーを押している時
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//キーを押している
	}
	else
	{
		return FALSE;	//キーを押していない
	}
}

//キーを押し上げたか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	//直前は押していて
		&& AllKeyState[KEY_INPUT_] == 0)	//今は押していないとき
	{
		return TRUE;	//キーを押し上げている
	}
	else
	{
		return FALSE;	//キーを押し上げていない
	}
}

//キーを押し続けているか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
//引　数：int：キーを押し続ける時間
BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	//押し続ける時間=秒数×FPS値
	//例）60FPSのゲームで、1秒間押し続けるなら、1秒×60FPS
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}
}

//マウスの入力情報を更新する
VOID MY_MOUSE_UPDATE(VOID)
{
	//マウスの以前の座標を取得
	mouse.OldPoint = mouse.Point;

	//マウスの以前のボタン入力を取得
	for (int i = 0; i < MOUSE_BUTTON_CODE; i++) { mouse.OldButton[i] = mouse.Button[i]; }

	//マウスの座標を取得
	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	//マウスの押しているボタンを取得
	mouse.InputValue = GetMouseInput();

	//左ボタンを押しているかチェック(TRUEは0以外なので、2も4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }		//押している
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }		//押していない

	//中ボタンを押しているかチェック(TRUEは0以外なので、2も4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }	//押している
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }	//押していない

	//右ボタンを押しているかチェック(TRUEは0以外なので、2も4もTRUE)
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }		//押している
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }	//押していない

	//ホイールの回転量を取得
	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

//ボタンを押しているか、マウスーコードで判断する
//引　数：int：マウスコード：MOUSE_INPUT_???
BOOL MY_MOUSE_DOWN(int MOUSE_INPUT_)
{
	//マウスコードのボタンを押している時
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;	//ボタンを押している
	}
	else
	{
		return FALSE;	//ボタンを押していない
	}
}

//ボタンを押し上げたか、マウスコード判断する
//引　数：int：マウスコード：MOUSE_INPUT_???
BOOL MY_MOUSE_UP(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1	//直前は押していて
		&& mouse.Button[MOUSE_INPUT_] == 0)	//今は押していないとき
	{
		return TRUE;	//ボタンを押し上げている
	}
	else
	{
		return FALSE;	//ボタンを押し上げていない
	}
}

//ボタンを押し続けているか、マウスコード判断する
//引　数：int：マウスコード：MOUSE_INPUT_???
//引　数：int：ボタンを押し続ける時間
BOOL MY_MOUSEDOWN_KEEP(int MOUSE_INPUT_, int DownTime)
{
	//押し続ける時間=秒数×FPS値
	//例）60FPSのゲームで、1秒間押し続けるなら、1秒×60FPS
	int UpdateTime = DownTime * GAME_FPS;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}
}

//フォントをこのソフト用に、一時的にインストール
BOOL MY_FONT_INSTALL_ONCE(VOID)
{
	//フォントを一時的に読み込み(WinAPI)
	if (AddFontResourceEx(FONT_ONRYO_PATH, FR_PRIVATE, NULL) == 0)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), FONT_ONRYO_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

//フォントをこのソフト用に、一時的にアンインストール
VOID MY_FONT_UNINSTALL_ONCE(VOID)
{
	//一時的に読み込んだフォントを削除(WinAPI)
	RemoveFontResourceEx(FONT_ONRYO_PATH, FR_PRIVATE, NULL);

	return;
}

//フォントを読み込む関数
//引　数：const char *：読み込むフォントの名前
//引　数：int　：フォントの大きさ
//引　数：int　：フォントの太さ
//引　数：int　：フォントのタイプ
//戻り値：BOOL ：エラー時はFALSE / 正常時はTRUE
BOOL MY_FONT_CREATE(VOID)
{
	return TRUE;
}

//フォントを削除する関数
VOID MY_FONT_DELETE(VOID)
{

	return;
}

//初期化
VOID MY_GAME_INIT(VOID)
{
	whiteColor = GetColor(255, 255, 255);
	blackColor = GetColor(0, 0, 0);
	CountQues = 0;
	CountRight = 0;
	CountWrong = 0;

	messageBoxGraphHandle = LoadGraph(MESSAGE_BOX_GRAPHIC_FILENAME);
}

//スタート画面
VOID MY_START(VOID)
{
	MY_START_PROC();	//スタート画面の処理
	MY_START_DRAW();	//スタート画面の描画


	return;
}

//スタート画面の処理
VOID MY_START_PROC(VOID)
{
	//難易度を押したら、プレイシーンへ移動する
	if (mouse.Point.x >= ImageEasyRogo.x 
		&& mouse.Point.x <= ImageEasyRogo.x+ImageEasyRogo.width
		&& mouse.Point.y >= ImageEasyRogo.y 
		&& mouse.Point.y <= ImageEasyRogo.y+ImageEasyRogo.height)
	{
		if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
		{
			//BGMが流れているなら
			if (CheckSoundMem(BGM_TITLE.handle) != 0)
			{
				StopSoundMem(BGM_TITLE.handle);	//BGMを止める
			}

			//ゲームの終了状態を初期化する
			QuesKind = QUES_END_WRONG;

			//ゲームのシーンをプレイ画面にする
			GameScene = GAME_SCENE_STORY;

			return;
		}

	}

	//else if (mouse.Point.x >= ImageHardRogo.x
	//	&& mouse.Point.x <= ImageHardRogo.x + ImageHardRogo.width
	//	&& mouse.Point.y >= ImageHardRogo.y
	//	&& mouse.Point.y <= ImageHardRogo.y + ImageHardRogo.height)
	//{
	//	if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
	//	{
	//		//BGMが流れているなら
	//		if (CheckSoundMem(BGM_TITLE.handle) != 0)
	//		{
	//			StopSoundMem(BGM_TITLE.handle);	//BGMを止める
	//		}

	//		//ゲームの終了状態を初期化する
	//		QuesKind = QUES_END_WRONG;

	//		//ゲームのシーンをプレイ画面にする
	//		GameScene = GAME_SCENE_QUES_HARD;

	//		return;
	//	}

	//}
		
	//BGMが流れていないなら
	if (CheckSoundMem(BGM_TITLE.handle) == 0)
	{
		//BGMの音量を下げる
		ChangeVolumeSoundMem(255 * 50 / 100, BGM_TITLE.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(BGM_TITLE.handle, DX_PLAYTYPE_LOOP);
	}

	return;
}

//スタート画面の描画
VOID MY_START_DRAW(VOID)
{
	//背景を描画する
	DrawGraph(ImageTitleBack.x, ImageTitleBack.y, ImageTitleBack.handle, TRUE);

	//タイトルロゴを描画する
	DrawGraph(ImageTitleRogo.x, ImageTitleRogo.y, ImageTitleRogo.handle, TRUE);

	//難易度ロゴ描画
	DrawGraph(ImageEasyRogo.x, ImageEasyRogo.y, ImageEasyRogo.handle, TRUE);
	DrawGraph(ImageHardRogo.x, ImageHardRogo.y, ImageHardRogo.handle, TRUE);

	if (mouse.Point.x >= ImageEasyRogo.x
		&& mouse.Point.x <= ImageEasyRogo.x + ImageEasyRogo.width
		&& mouse.Point.y >= ImageEasyRogo.y
		&& mouse.Point.y <= ImageEasyRogo.y + ImageEasyRogo.height)
	{
		DrawGraph(ImageEasyRogo2.x, ImageEasyRogo2.y, ImageEasyRogo2.handle, TRUE);
	}
	/*if (mouse.Point.x >= ImageHardRogo.x
		&& mouse.Point.x <= ImageHardRogo.x + ImageHardRogo.width
		&& mouse.Point.y >= ImageHardRogo.y
		&& mouse.Point.y <= ImageHardRogo.y + ImageHardRogo.height)
	{
		DrawGraph(ImageHardRogo2.x, ImageHardRogo2.y, ImageHardRogo2.handle, TRUE);
	}*/
	return;
}

//ルール画面
VOID MY_PLAY_RULE(VOID)
{
	MY_PLAY_RULE_PROC();	//プレイ画面の処理
	MY_PLAY_RULE_DRAW();	//プレイ画面の描画

	/*DrawString(0, 0, "ルール説明画面", GetColor(255, 255, 255));
	DrawString(0, 20, "プレイ画面(クリックで問題へ)", GetColor(255, 255, 255));*/
	return;
}

//ルール画面の処理
VOID MY_PLAY_RULE_PROC(VOID)
{
	//クリックすると問題画面へ
	if (mouse.Point.x >= BOX_SIZE_X+150
		&& mouse.Point.x <= BOX_SIZE_X + 350
		&& mouse.Point.y >= BOX_SIZE_Y +50
		&& mouse.Point.y <= BOX_SIZE_Y + 90)
	{
		if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
		{
			//BGMが流れているなら
			if (CheckSoundMem(BGM_RULE.handle) != 0)
			{
				StopSoundMem(BGM_RULE.handle);	//BGMを止める
			}
			//ゲームのシーンをプレイ画面にする
			GameScene = GAME_SCENE_QUES_EASY;

			return;
		}
	}

	//BGMが流れていないなら
	if (CheckSoundMem(BGM_RULE.handle) == 0)
	{
		//BGMの音量を下げる
		ChangeVolumeSoundMem(255 * 50 / 100, BGM_RULE.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(BGM_RULE.handle, DX_PLAYTYPE_LOOP);
	}
		

	
	return;
}

//ルール画面の描画
VOID MY_PLAY_RULE_DRAW(VOID)
{
	//背景を描画する
	DrawGraph(ImagePlayBack.x, ImagePlayBack.y, ImagePlayBack.handle, TRUE);
	//ルール説明を描画
	DrawGraph(ImageRule.x, ImageRule.y, ImageRule.handle, TRUE);
	
	return;
}

//問題画面
VOID MY_PLAY_QUES_EASY()
{
	MY_PLAY_QUES_EASY_DRAW();
	MY_PLAY_QUES_EASY_PROC();

	/*DrawString(0, 0, "問題画面", GetColor(255, 255, 255));
	DrawString(0, 20, "プレイ画面(Sキーでストーリーへ)", GetColor(255, 255, 255));
	DrawString(0, 40, "プレイ画面(Tキーでタイトルへ)", GetColor(255, 255, 255));*/
	return;
}

VOID MY_PLAY_QUES_EASY_PROC()
{
	QuesKind = QUES_END_WRONG;

	//BGMが流れていないなら
	if (CheckSoundMem(BGM_QUES.handle) == 0)
	{
		//BGMの音量を下げる
		ChangeVolumeSoundMem(255 * 50 / 100, BGM_QUES.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(BGM_QUES.handle, DX_PLAYTYPE_LOOP);
	}

	//if (ImageQuesBack.rate < ImageQuesBack.rateMAX)   //背景を拡大
	//{
	//	ImageQuesBack.rate += IMAGE_ROGO_ROTA;
	//	ImageQuesBack.image.y=  GAME_HEIGHT / 2 + sin(ImageQuesBack.radian) * GAME_HANKEI;
	//	if (ImageQuesBack.radian < 2 * GAME_PI)	//0.00(0°) 〜 1π(180°) 〜2π (360°)
	//		ImageQuesBack.radian += 0.01;
	//	else
	//		ImageQuesBack.radian = 0.0;
	//}
	//else
	if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
	{
		ImageQues[CountQues].IsDraw = TRUE;     //拡大しきったら問題描画
	}

if (ImageQues[CountQues].IsDraw == TRUE)    //選択肢処理
{
	if (CountQues == 0)
	{
		if (mouse.Point.x >= BOX_SIZE_X
			&& mouse.Point.x <= BOX_SIZE_X + 100
			&& mouse.Point.y >= BOX_SIZE_Y
			&& mouse.Point.y <= BOX_SIZE_Y + 20)
		{
			DrawString(BOX_SIZE_X + 46, BOX_SIZE_Y + 2, "A", GetColor(0, 0, 0));
			if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
			{
				QuesKind = QUES_END_WRONG;
				PlaySoundMem(BGM_WRONG.handle, DX_PLAYTYPE_NORMAL);
				CountWrong++;
			}
		}
		else if (mouse.Point.x >= BOX_SIZE_X
			&& mouse.Point.x <= BOX_SIZE_X + 300
			&& mouse.Point.y >= BOX_SIZE_Y
			&& mouse.Point.y <= BOX_SIZE_Y + 20)
		{
			DrawString(BOX_SIZE_X + 246, BOX_SIZE_Y + 2, "B", GetColor(0, 0, 0));
			if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
			{
				QuesKind = QUES_END_RIGHT;
				ImageQues[CountQues].IsDraw = FALSE;
				CountRight++;
				CountQues++;
				//BGMが流れているなら
				if (CheckSoundMem(BGM_QUES.handle) != 0)
				{
					StopSoundMem(BGM_QUES.handle);	//BGMを止める
				}
				GameScene = GAME_SCENE_STORY;
				return;
			}
		}
		else if (mouse.Point.x >= BOX_SIZE_X
			&& mouse.Point.x <= BOX_SIZE_X + 500
			&& mouse.Point.y >= BOX_SIZE_Y
			&& mouse.Point.y <= BOX_SIZE_Y + 20)
		{
			DrawString(BOX_SIZE_X + 446, BOX_SIZE_Y + 2, "C", GetColor(0, 0, 0));
			if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
			{
				QuesKind = QUES_END_WRONG;
				PlaySoundMem(BGM_WRONG.handle, DX_PLAYTYPE_NORMAL);
				CountWrong++;
			}
		}
	}
	else if (CountQues == 1)
	{
		if (mouse.Point.x >= BOX_SIZE_X
			&& mouse.Point.x <= BOX_SIZE_X + 100
			&& mouse.Point.y >= BOX_SIZE_Y
			&& mouse.Point.y <= BOX_SIZE_Y + 20)
		{
			DrawString(BOX_SIZE_X + 46, BOX_SIZE_Y + 2, "A", GetColor(0, 0, 0));
			if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
			{
				QuesKind = QUES_END_WRONG;
				PlaySoundMem(BGM_WRONG.handle, DX_PLAYTYPE_NORMAL);
				CountWrong++;
			}
		}
		else if (mouse.Point.x >= BOX_SIZE_X
			&& mouse.Point.x <= BOX_SIZE_X + 300
			&& mouse.Point.y >= BOX_SIZE_Y
			&& mouse.Point.y <= BOX_SIZE_Y + 20)
		{
			DrawString(BOX_SIZE_X + 246, BOX_SIZE_Y + 2, "B", GetColor(0, 0, 0));
			if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
			{
				QuesKind = QUES_END_WRONG;
				PlaySoundMem(BGM_WRONG.handle, DX_PLAYTYPE_NORMAL);
				CountWrong++;
			}
		}
		else if (mouse.Point.x >= BOX_SIZE_X
			&& mouse.Point.x <= BOX_SIZE_X + 500
			&& mouse.Point.y >= BOX_SIZE_Y
			&& mouse.Point.y <= BOX_SIZE_Y + 20)
		{
			DrawString(BOX_SIZE_X + 446, BOX_SIZE_Y + 2, "C", GetColor(0, 0, 0));
			if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
			{
				QuesKind = QUES_END_RIGHT;
				ImageQues[CountQues].IsDraw = FALSE;
				CountRight++;
				CountQues++;
				//BGMが流れているなら
				if (CheckSoundMem(BGM_QUES.handle) != 0)
				{
					StopSoundMem(BGM_QUES.handle);	//BGMを止める
				}
				GameScene = GAME_SCENE_STORY;
				return;
			}
		}
	}	
	

	if (CountRight == 2)
	{
		//BGMが流れているなら
		if (CheckSoundMem(BGM_QUES.handle) != 0)
		{
			StopSoundMem(BGM_QUES.handle);	//BGMを止める
		}
		GameScene = GAME_SCENE_END;
		return;
	}
	if (COUNTDIST - CountWrong == 0)
	{
		//BGMが流れているなら
		if (CheckSoundMem(BGM_QUES.handle) != 0)
		{
			StopSoundMem(BGM_QUES.handle);	//BGMを止める
		}
		GameScene = GAME_SCENE_END;
		return;
	}
}
	
	return;
}

VOID MY_PLAY_QUES_EASY_DRAW()
{

	//背景を描画する
	DrawGraph(ImageQuesBack.image.x, ImageQuesBack.image.y, ImageQuesBack.image.handle, TRUE);
	//院長との距離描画
	DrawFormatString(1100, 20, GetColor(255, 0, 0), "院長との距離 %d", COUNTDIST - CountWrong);

	//問題を描画する
	if (ImageQues[CountQues].IsDraw == TRUE)
	{
		DrawGraph(ImageQues[CountQues].x, ImageQues[CountQues].y, ImageQues[CountQues].handle, TRUE);
		DrawBox(BOX_SIZE_X, BOX_SIZE_Y, BOX_SIZE_X + 100, BOX_SIZE_Y + 20, GetColor(255, 0, 0), TRUE);
		DrawString(BOX_SIZE_X + 46, BOX_SIZE_Y + 2, "A", GetColor(255, 255, 255));

		DrawBox(BOX_SIZE_X + 200, BOX_SIZE_Y, BOX_SIZE_X + 300, BOX_SIZE_Y + 20, GetColor(0, 255, 0), TRUE);
		DrawString(BOX_SIZE_X + 246, BOX_SIZE_Y + 2, "B", GetColor(255, 255, 255));

		DrawBox(BOX_SIZE_X + 400, BOX_SIZE_Y, BOX_SIZE_X + 500, BOX_SIZE_Y + 20, GetColor(0, 0, 255), TRUE);
		DrawString(BOX_SIZE_X + 446, BOX_SIZE_Y + 2, "C", GetColor(255, 255, 255));
			
	}
	//血しぶき描画(不正解処理)
	/*if (CountWrong > 0)
	{
		DrawGraph(ImageWrong.x, ImageWrong.y, ImageWrong.handle, TRUE);
	}*/


	return;
}



//ストーリー画面
VOID MY_PLAY_STORY()
{
	MY_PLAY_STORY_PROC();
	MY_PLAY_STORY_DRAW();

	/*DrawString(0, 0, "ストーリー画面", GetColor(255, 255, 255));
	DrawString(0, 20, "プレイ画面(Qキーで問題へ)", GetColor(255, 255, 255));
	DrawString(0, 40, "プレイ画面(Rキーでルート分岐へ)", GetColor(255, 255, 255));*/


	return;
}

VOID MY_PLAY_STORY_PROC()
{
	
	//BGMが流れていないなら
	if (CheckSoundMem(BGM_STORY.handle) == 0)
	{
		//BGMの音量を下げる
		ChangeVolumeSoundMem(255 * 50 / 100, BGM_STORY.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(BGM_STORY.handle, DX_PLAYTYPE_LOOP);
	}

	if (CountQues == 0)
	{
		if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
		{
			setMessage("ある日怪我をしてしまったあなたは最近できた病院に入院することに。　　　　　　　　　　　　　　　　　　"
				"しかしその病院の院長は診断を偽り、病人の健康な臓器を切除し収集する趣味を持つ、狂ったやつだった。　　"
				"偶然、そのことを聞いてしまったあなたは、自分の番になる前に病院から脱出することに決めた。　　　　　　"
				"病人を逃がしたくない院長は、あらかじめあらゆる場所を施錠し、閉じ込めていた。　　　　　　　　　　　　"
				"そして、まるでこれは脱出ゲームだと言うように謎を各所に置いていた。　　　　　　　　　　　　　　　　　"
				"いきなり謎が置いてあることに困惑しつつも他に脱出方法も無さそうなので謎を解くことにする…");
		}
	}
	if (CountQues == 1)
	{
		if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
		{
			setMessage("どんどん謎を解いていくあなた。"
			           "特に襲われることもなく、勢いよく部屋を移動していく。　　　　　　　　　"
			           "「何かをしてくる気配もないし、謎も簡単だ」と余裕をかますあなたであった。　　　　　　　　　　　　　　"
			           "それがあなたを嘲笑い、観察するための院長の策略とも知らず…");
		}
	}
	if (CountQues == 2)
	{
		if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
		{
			setMessage("謎をすべて解き出口までやってきたあなた。"
			           "そこにはあの院長が出口の前で仁王立ちしていた。　　　　　　　"
			           "無理やりにでも通ろうと思っていたが院長は「謎をすべて解いたのだから出してやろう」と道を開ける。　　　"
			           "いかにも怪しいが出れるならと勢いよく走り出す。"
			           "やっと出れると安堵したのも束の間、　　　　　　　　　　視界の上側で赤く光った　　　　　気がした。　　"
				       "　　　　　　　　　　　　　　　　　　　　　　　　　　　"
			           "振り　　　　返ると　　　　　　何　も　　　　　　感じ　　　　　なく　　　　　　　　なった。　　　　　"
			           "　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　残ったのは嘲笑と歓喜の声だった。");
		}
	}

	//クリックしたら、プレイシーンへ移動する
	if (mouse.Point.x >= ImageNextBtn.x
		&& mouse.Point.x <= ImageNextBtn.x + ImageNextBtn.width
		&& mouse.Point.y >= ImageNextBtn.y
		&& mouse.Point.y <= ImageNextBtn.y + ImageNextBtn.height)
	{
		if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
		{
			//BGMが流れているなら
			if (CheckSoundMem(BGM_STORY.handle) != 0)
			{
				StopSoundMem(BGM_STORY.handle);	//BGMを止める
			}

			//ゲームのシーンをルール説明画面にする
			if (CountQues == 0)
			{
				GameScene = GAME_SCENE_RULE;
			}
			else
			{
				GameScene = GAME_SCENE_QUES_EASY;
			}

			return;
		}
	}
	
	return;
}

VOID MY_PLAY_STORY_DRAW()
{
	//背景を描画する
	DrawGraph(ImageStoryBack.x, ImageStoryBack.y, ImageStoryBack.handle, TRUE);
	drawMessage();

	DrawGraph(ImageNextBtn.x, ImageNextBtn.y, ImageNextBtn.handle, TRUE);
	if (mouse.Point.x >= ImageNextBtn.x
		&& mouse.Point.x <= ImageNextBtn.x + ImageNextBtn.width
		&& mouse.Point.y >= ImageNextBtn.y
		&& mouse.Point.y <= ImageNextBtn.y + ImageNextBtn.height)
	{
		DrawGraph(ImageNextBtn2.x, ImageNextBtn2.y, ImageNextBtn2.handle, TRUE);
	}
	return;
}

//ルート分岐画面
VOID MY_PLAY_ROOT()
{
	MY_PLAY_ROOT_PROC();
	MY_PLAY_ROOT_DRAW();

	//DrawString(0, 0, "プレイ画面(Qキーで問題へ)", GetColor(255, 255, 255));
	
	return;
}

VOID MY_PLAY_ROOT_PROC()
{
	
	//Qキーを押したら、問題シーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_Q) == TRUE)
	{
		//BGMが流れているなら
		if (CheckSoundMem(BGM_STORY.handle) != 0)
		{
			StopSoundMem(BGM_STORY.handle);	//BGMを止める
		}

		//ゲームのシーンを問題画面にする
		GameScene = GAME_SCENE_QUES_EASY;

		return;
	}

	//BGMが流れていないなら
	if (CheckSoundMem(BGM_STORY.handle) == 0)
	{

		//BGMの音量を下げる
		ChangeVolumeSoundMem(255 * 50 / 100, BGM_STORY.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(BGM_STORY.handle, DX_PLAYTYPE_LOOP);
	}

	return;
}

VOID MY_PLAY_ROOT_DRAW()
{
	return;
}

//エンド画面
VOID MY_END(VOID)
{
	MY_END_PROC();	//エンド画面の処理
	MY_END_DRAW();	//エンド画面の描画


	return;
}

//エンド画面の処理
VOID MY_END_PROC(VOID)
{
	//クリックしたら、スタートシーンへ移動する
	if (mouse.Point.x >= BOX_SIZE_X + 200
		&& mouse.Point.x <= BOX_SIZE_X + 350
		&& mouse.Point.y >= BOX_SIZE_Y
		&& mouse.Point.y <= BOX_SIZE_Y + 20)
	{
		if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
		{
			//BGMが流れているなら
			if (CheckSoundMem(BGM_END.handle) != 0)
			{
				StopSoundMem(BGM_END.handle);	//BGMを止める
			}
			//BGMが流れているなら
			if (CheckSoundMem(BGM_END2.handle) != 0)
			{
				StopSoundMem(BGM_END2.handle);	//BGMを止める
			}

			QuesKind = QUES_END_WRONG;
			CountWrong = 0;
			GameScene = GAME_SCENE_START;
			MY_GAME_INIT();

			return;
		}
	}
	
	if (CountRight == 2)
	{
		//BGMが流れていないなら
		if (CheckSoundMem(BGM_END.handle) == 0)
		{

			//BGMの音量を下げる
			ChangeVolumeSoundMem(255 * 50 / 100, BGM_END.handle);	//50%の音量にする

			//BGMを流す
			//DX_PLAYTYPE_NORMAL:　ノーマル再生
			//DX_PLAYTYPE_BACK  : バックグラウンド再生
			//DX_PLAYTYPE_LOOP  : ループ再生
			PlaySoundMem(BGM_END.handle, DX_PLAYTYPE_LOOP);
		}
	}
	else if(COUNTDIST - CountWrong == 0)
	{
		// BGMが流れていないなら
		if (CheckSoundMem(BGM_END2.handle) == 0)
		{

			//BGMの音量を下げる
			ChangeVolumeSoundMem(255 * 50 / 100, BGM_END2.handle);	//50%の音量にする

			//BGMを流す
			//DX_PLAYTYPE_NORMAL:　ノーマル再生
			//DX_PLAYTYPE_BACK  : バックグラウンド再生
			//DX_PLAYTYPE_LOOP  : ループ再生
			PlaySoundMem(BGM_END2.handle, DX_PLAYTYPE_LOOP);
		}
	}
	
	return;
}

//エンド画面の描画
VOID MY_END_DRAW(VOID)
{
	if (CountRight == 2)
	{
		//クリアを描画する
		DrawGraph(ImageEndClear.x, ImageEndClear.y, ImageEndClear.handle, TRUE);
		DrawString(600, 600, "Click to Title", GetColor(255, 255, 255));
	}
	else
	{
		//ゲームオーバーを描画する
		DrawGraph(ImageEndBack.x, ImageEndBack.y, ImageEndBack.handle, TRUE);
		DrawString(600, 600, "Click to Title", GetColor(255, 255, 255));
	}
	

	return;
}

//画像をまとめて読み込む関数
BOOL MY_LOAD_IMAGE(VOID)
{
	//タイトル背景画像
	strcpy_s(ImageTitleBack.path, IMAGE_TITLE_BACK_PATH);		//パスの設定
	ImageTitleBack.handle = LoadGraph(ImageTitleBack.path);	//読み込み
	if (ImageTitleBack.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleBack.handle, &ImageTitleBack.width, &ImageTitleBack.height);	//画像の幅と高さを取得
	ImageTitleBack.x = GAME_WIDTH / 2 - ImageTitleBack.width / 2;		//左右中央揃え
	ImageTitleBack.y = GAME_HEIGHT / 2 - ImageTitleBack.height / 2;	//上下中央揃え

	//タイトルロゴ
	strcpy_s(ImageTitleRogo.path, IMAGE_TITLE_ROGO_PATH);					//パスの設定
	ImageTitleRogo.handle = LoadGraph(ImageTitleRogo.path);			//読み込み
	if (ImageTitleRogo.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleRogo.handle, &ImageTitleRogo.width, &ImageTitleRogo.height);	      //画像の幅と高さを取得
	ImageTitleRogo.x = GAME_WIDTH / 2 - ImageTitleRogo.width / 2;		                      //中央寄せ
	ImageTitleRogo.y = GAME_HEIGHT / 2 - ImageTitleRogo.height / 2 - 100;				      //中央寄せ+画像幅

	//イージーロゴ
	strcpy_s(ImageEasyRogo.path, IMAGE_EASY_ROGO_PATH);					//パスの設定
	ImageEasyRogo.handle = LoadGraph(ImageEasyRogo.path);			//読み込み
	if (ImageEasyRogo.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_EASY_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEasyRogo.handle, &ImageEasyRogo.width, &ImageEasyRogo.height);	                  //画像の幅と高さを取得
	ImageEasyRogo.x = GAME_WIDTH / 2 - ImageEasyRogo.width - ImageEasyRogo.width / 2;                     //中央寄せ-画像幅
	ImageEasyRogo.y = GAME_HEIGHT / 2 + ImageEasyRogo.height*2;				            //中央寄せ+画像幅
	
	//イージーロゴHover
	strcpy_s(ImageEasyRogo2.path, IMAGE_EASY_ROGO2_PATH);					//パスの設定
	ImageEasyRogo2.handle = LoadGraph(ImageEasyRogo2.path);			//読み込み
	if (ImageEasyRogo2.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_EASY_ROGO2_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEasyRogo2.handle, &ImageEasyRogo2.width, &ImageEasyRogo2.height);	                  //画像の幅と高さを取得
	ImageEasyRogo2.x = GAME_WIDTH / 2 - ImageEasyRogo2.width - ImageEasyRogo2.width / 2;                     //中央寄せ-画像幅
	ImageEasyRogo2.y = GAME_HEIGHT / 2 + ImageEasyRogo2.height * 2;				            //中央寄せ+画像幅


	//ハードロゴ
	strcpy_s(ImageHardRogo.path, IMAGE_HARD_ROGO_PATH);					//パスの設定
	ImageHardRogo.handle = LoadGraph(ImageHardRogo.path);			//読み込み
	if (ImageHardRogo.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_HARD_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageHardRogo.handle, &ImageHardRogo.width, &ImageHardRogo.height);	//画像の幅と高さを取得
	ImageHardRogo.x = GAME_WIDTH / 2 + ImageHardRogo.width/2;	                        //中央寄せ+画像幅
	ImageHardRogo.y = GAME_HEIGHT / 2 + ImageHardRogo.height*2;				            //中央寄せ+画像幅
	
	//ハードロゴHover
	strcpy_s(ImageHardRogo2.path, IMAGE_HARD_ROGO2_PATH);					//パスの設定
	ImageHardRogo2.handle = LoadGraph(ImageHardRogo2.path);			//読み込み
	if (ImageHardRogo2.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_HARD_ROGO2_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageHardRogo2.handle, &ImageHardRogo2.width, &ImageHardRogo2.height);	//画像の幅と高さを取得
	ImageHardRogo2.x = GAME_WIDTH / 2 + ImageHardRogo2.width / 2;	                        //中央寄せ+画像幅
	ImageHardRogo2.y = GAME_HEIGHT / 2 + ImageHardRogo2.height * 2;				            //中央寄せ+画像幅

	//ルール背景画像
	strcpy_s(ImagePlayBack.path, IMAGE_PLAY_BACK_PATH);		//パスの設定
	ImagePlayBack.handle = LoadGraph(ImagePlayBack.path);	//読み込み
	if (ImagePlayBack.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_PLAY_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImagePlayBack.handle, &ImagePlayBack.width, &ImagePlayBack.height);	//画像の幅と高さを取得
	ImagePlayBack.x = GAME_WIDTH / 2 - ImagePlayBack.width / 2;		//左右中央揃え
	ImagePlayBack.y = GAME_HEIGHT / 2 - ImagePlayBack.height / 2;	//上下中央揃え


	//ルール説明画像
	strcpy_s(ImageRule.path, IMAGE_RULE_PATH);		//パスの設定
	ImageRule.handle = LoadGraph(ImageRule.path);	//読み込み
	if (ImageRule.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_RULE_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageRule.handle, &ImageRule.width, &ImageRule.height);	//画像の幅と高さを取得
	ImageRule.x = GAME_WIDTH / 2 - ImageRule.width / 2;		//左右中央揃え
	ImageRule.y = GAME_HEIGHT / 2 - ImageRule.height / 2 + 50;	//上下中央揃え

	//問題背景画像
	strcpy_s(ImageQuesBack.image.path, IMAGE_QUES_BACK_PATH);		//パスの設定
	ImageQuesBack.image.handle = LoadGraph(ImageQuesBack.image.path);	//読み込み
	if (ImageQuesBack.image.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_QUES_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageQuesBack.image.handle, &ImageQuesBack.image.width, &ImageQuesBack.image.height);	//画像の幅と高さを取得
	ImageQuesBack.image.x = GAME_WIDTH / 2 - ImageQuesBack.image.width / 2;		//左右中央揃え
	ImageQuesBack.image.y = GAME_HEIGHT / 2 - ImageQuesBack.image.height / 2;	//上下中央揃え
	ImageQuesBack.rate = 0.0;					       //拡大率
	ImageQuesBack.rateMAX = IMAGE_ROGO_ROTA_MAX;	   //拡大率MAX
	ImageQuesBack.radian = 0.0;

	//問題１画像
	strcpy_s(ImageQues[0].path, IMAGE_QUES_1_PATH);		//パスの設定
	ImageQues[0].handle = LoadGraph(ImageQues[0].path);	//読み込み
	if (ImageQues[0].handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_QUES_1_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageQues[0].handle, &ImageQues[0].width, &ImageQues[0].height);	//画像の幅と高さを取得
	ImageQues[0].x = GAME_WIDTH / 2 - ImageQues[0].width / 2;		//左右中央揃え
	ImageQues[0].y = GAME_HEIGHT / 2 - ImageQues[0].height / 2;	    //上下中央揃え
	ImageQues[0].IsDraw = FALSE;

	//問題2画像
	strcpy_s(ImageQues[1].path, IMAGE_QUES_2_PATH);		//パスの設定
	ImageQues[1].handle = LoadGraph(ImageQues[1].path);	//読み込み
	if (ImageQues[1].handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_QUES_2_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageQues[1].handle, &ImageQues[1].width, &ImageQues[1].height);	//画像の幅と高さを取得
	ImageQues[1].x = GAME_WIDTH / 2 - ImageQues[1].width / 2;		//左右中央揃え
	ImageQues[1].y = GAME_HEIGHT / 2 - ImageQues[1].height / 2;	    //上下中央揃え
	ImageQues[1].IsDraw = FALSE;

	//不正解画像
	strcpy_s(ImageWrong.path, IMAGE_WRONG_PATH);		//パスの設定
	ImageWrong.handle = LoadGraph(ImageWrong.path);	    //読み込み
	if (ImageWrong.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_WRONG_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageWrong.handle, &ImageWrong.width, &ImageWrong.height);	//画像の幅と高さを取得
	ImageWrong.x = GAME_WIDTH / 2 - ImageWrong.width / 2;		//左右中央揃え
	ImageWrong.y = GAME_HEIGHT / 2 - ImageWrong.height / 2;	    //上下中央揃え
	ImageWrong.IsDraw = FALSE;

	//ストーリー背景画像
	strcpy_s(ImageStoryBack.path, IMAGE_STORY_BACK_PATH);		//パスの設定
	ImageStoryBack.handle = LoadGraph(ImageStoryBack.path);	//読み込み
	if (ImageStoryBack.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_STORY_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageStoryBack.handle, &ImageStoryBack.width, &ImageStoryBack.height);	//画像の幅と高さを取得
	ImageStoryBack.x = GAME_WIDTH / 2 - ImageStoryBack.width / 2;		//左右中央揃え
	ImageStoryBack.y = GAME_HEIGHT / 2 - ImageStoryBack.height / 2;	//上下中央揃え

	//つぎへボタン
	strcpy_s(ImageNextBtn.path, IMAGE_NEXT_PATH);					//パスの設定
	ImageNextBtn.handle = LoadGraph(ImageNextBtn.path);			//読み込み
	if (ImageNextBtn.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_NEXT_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageNextBtn.handle, &ImageNextBtn.width, &ImageNextBtn.height);	    //画像の幅と高さを取得
	ImageNextBtn.x = GAME_WIDTH / 2 + ImageNextBtn.width * 2;                           //中央寄せ+画像幅×２
	ImageNextBtn.y = GAME_HEIGHT / 2 + ImageNextBtn.height * 3;				            //中央寄せ+画像幅×２

	//つぎへボタンHover
	strcpy_s(ImageNextBtn2.path, IMAGE_NEXT2_PATH);					//パスの設定
	ImageNextBtn2.handle = LoadGraph(ImageNextBtn2.path);			//読み込み
	if (ImageNextBtn2.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_NEXT2_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageNextBtn2.handle, &ImageNextBtn2.width, &ImageNextBtn2.height);	                  //画像の幅と高さを取得
	ImageNextBtn2.x = GAME_WIDTH / 2 + ImageNextBtn.width * 2;                              //中央寄せ-画像幅×２
	ImageNextBtn2.y = GAME_HEIGHT / 2 + ImageNextBtn2.height * 3;				            //中央寄せ+画像幅×２

	//ゲームオーバー画像
	strcpy_s(ImageEndBack.path, IMAGE_END_BACK_PATH);		//パスの設定
	ImageEndBack.handle = LoadGraph(ImageEndBack.path);	//読み込み
	if (ImageEndBack.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_END_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEndBack.handle, &ImageEndBack.width, &ImageEndBack.height);	//画像の幅と高さを取得
	ImageEndBack.x = GAME_WIDTH / 2 - ImageEndBack.width / 2;		//左右中央揃え
	ImageEndBack.y = GAME_HEIGHT / 2 - ImageEndBack.height / 2;	//上下中央揃え

	//クリア画像
	strcpy_s(ImageEndClear.path, IMAGE_END_CLEAR_PATH);		//パスの設定
	ImageEndClear.handle = LoadGraph(ImageEndClear.path);	//読み込み
	if (ImageEndClear.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), IMAGE_END_CLEAR_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEndClear.handle, &ImageEndClear.width, &ImageEndClear.height);	//画像の幅と高さを取得
	ImageEndClear.x = GAME_WIDTH / 2 - ImageEndClear.width / 2;		//左右中央揃え
	ImageEndClear.y = GAME_HEIGHT / 2 - ImageEndClear.height / 2;	//上下中央揃え


	return TRUE;
}

//画像をまとめて削除する関数
VOID MY_DELETE_IMAGE(VOID)
{
	DeleteGraph(ImageTitleBack.handle);
	DeleteGraph(ImageTitleRogo.handle);
	DeleteGraph(ImageEasyRogo.handle);
	DeleteGraph(ImageEasyRogo2.handle);
	DeleteGraph(ImageHardRogo.handle);
	DeleteGraph(ImageRule.handle);
	DeleteGraph(ImageQues[1].handle);
	DeleteGraph(ImageRight.handle);
	DeleteGraph(ImageWrong.handle);
	DeleteGraph(ImageEndBack.handle);
	return;
}

//音楽をまとめて読み込む関数
BOOL MY_LOAD_MUSIC(VOID)
{
	//タイトル背景音楽
	strcpy_s(BGM_TITLE.path, MUSIC_TITLE_PATH);		//パスの設定
	BGM_TITLE.handle = LoadSoundMem(BGM_TITLE.path);	//読み込み
	if (BGM_TITLE.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_TITLE_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//ルール説明背景音楽
	strcpy_s(BGM_RULE.path, MUSIC_RULE_PATH);		//パスの設定
	BGM_RULE.handle = LoadSoundMem(BGM_RULE.path);	//読み込み
	if (BGM_RULE.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_RULE_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//問題背景音楽
	strcpy_s(BGM_QUES.path, MUSIC_QUES_PATH);		//パスの設定
	BGM_QUES.handle = LoadSoundMem(BGM_QUES.path);	//読み込み
	if (BGM_QUES.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_QUES_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//不正解音楽
	strcpy_s(BGM_WRONG.path, MUSIC_WRONG_PATH);		//パスの設定
	BGM_WRONG.handle = LoadSoundMem(BGM_WRONG.path);	//読み込み
	if (BGM_WRONG.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_WRONG_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//ストーリー背景音楽
	strcpy_s(BGM_STORY.path, MUSIC_STORY_PATH);		//パスの設定
	BGM_STORY.handle = LoadSoundMem(BGM_STORY.path);	//読み込み
	if (BGM_STORY.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_STORY_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//エンド背景音楽
	strcpy_s(BGM_END.path, MUSIC_END_PATH);		//パスの設定
	BGM_END.handle = LoadSoundMem(BGM_END.path);	//読み込み
	if (BGM_END.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_END_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	//バッドエンド背景音楽
	strcpy_s(BGM_END2.path, MUSIC_END2_PATH);		//パスの設定
	BGM_END2.handle = LoadSoundMem(BGM_END2.path);	//読み込み
	if (BGM_END2.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_END2_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	return TRUE;
}

//音楽をまとめて削除する関数
VOID MY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(BGM_TITLE.handle);
	DeleteSoundMem(BGM_RULE.handle);
	DeleteSoundMem(BGM_QUES.handle);
	DeleteSoundMem(BGM_WRONG.handle);
	DeleteSoundMem(BGM_STORY.handle);
	DeleteSoundMem(BGM_END.handle);
	DeleteSoundMem(BGM_END2.handle);
	return;
}

int isJapanese(unsigned char code)
{
	if ((code >= 0x81 && code <= 0x9F) ||
		(code >= 0xE0 && code <= 0xFC))
	{
		return 1;
	}
	return 0;
}

// m e s s a g eで指定した文章を start の 位 置 か ら len 文 字 分 表 示 す る
 void writeSubstring(char* message, int start, int len,
	 int posX, int posY, int color, int bufferLine)
	 {
	 int i;
	 //文字数
		 int maxLen = strlen(message);
	
		 // s t a r t の 位 置 を 変 更 す る
		 // s t a r t の 位 置 ま で に 日 本 語 が で て き て い た ら ， 1 を 足 し て い く
		 for (i = 0; i < start && message[i] != '\0'; ) {
		 if (isJapanese(message[i])) {
			 //日本語の場合，2バ イ ト 分 す す め る
				 i += 2;
			 // s t a r tに1バイト分足す
				 start++;
		}
		else {
		 //半角文字の場合，1バ イ ト 分 進 め る
				 i++;
		}
	}
		 // s t a r t の 位 置 が 表 示 し た い 最 大 文 字 数 よ り 大 き い 場 合
		 if (start >= maxLen) {
		 return;
		
	}
	
		 //指定した位置から l e n 文 字 分 表 示 す る
		 for (i = 0; i < len && message[start + i] != '\0'; ) {
		 if (isJapanese(message[start + i])) {
			 //日本語の場合，2文 字 分 b u f f e rにセット
				messageBuffer[bufferLine][i] = message[start + i];
			    messageBuffer[bufferLine][i + 1] = message[start + i + 1];
			 // l e n は 日 本 語 な の で ， 1 バ イ ト 分 追 加 す る
				 len++;
			 //2バ イ ト 分 進 め る
				 i += 2;
		}
		else {
				 //半角文字1文 字 を セ ッ ト
				 messageBuffer[bufferLine][i] = message[start + i]; 
				 i++; //1バ イ ト 分 進 め る
		}
	}
		 messageBuffer[bufferLine][i] = '\0';
	 //メッセージ描画
		 DrawString(posX, posY, messageBuffer[bufferLine], color);
 }


//メッセージ描画
VOID drawMessage()
{
	int i; 
	
	//メッセージボックス描画
	   DrawGraph(MESSAGE_BOX_X, MESSAGE_BOX_Y, messageBoxGraphHandle, FALSE);

		 if (message[currentCursor] != '\0') {
		    currentCursor++;
	}
	
		// MESSAGE_MAX_LENGTH ま で 文 字 を 描 画 し た ら 段 落 を 切 り 替 え る
		if (currentCursor % MESSAGE_MAX_LENGTH == 0) {
		if (message[currentCursor] != '\0') {
			 currentLineCursor++;
		}
		
	}
	
		 //メッセージ描画部分
		 for (i = 0; i < MESSAGE_MAX_LINE; i++) {
		 if (i == currentLineCursor) {
			 //メッセージ風に表示
				 writeSubstring(message, i * MESSAGE_MAX_LENGTH,
					 currentCursor - MESSAGE_MAX_LENGTH * i,
					 MESSAGE_BOX_X + 200,
					 MESSAGE_BOX_Y + MESSAGE_FONT_SIZE * i + 250,
					 whiteColor, i);
			 break;
			
		}
		else {
			 //メッセージをそのまま表示
			 writeSubstring(message, i * MESSAGE_MAX_LENGTH,
					 MESSAGE_MAX_LENGTH, MESSAGE_BOX_X + 200,
					 MESSAGE_BOX_Y + MESSAGE_FONT_SIZE * i + 250,
					 whiteColor, i);
			
		}
		
	}
	 }


VOID setMessage(const char* ms)  //描画したいメッセージをセット
{
	currentCursor = 0;
	currentLineCursor = 0;

	strncpy(message, ms, MESSAGE_MAX_LENGTH * MESSAGE_MAX_LINE);   //メッセージをコピー
}