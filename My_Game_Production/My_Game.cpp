//########## ヘッダーファイル読み込み ##########
#include "DxLib.h"
#include "resource.h"

//########## マクロ定義 ##########
#define GAME_WIDTH			1280	//画面の横の大きさ
#define GAME_HEIGHT			720	    //画面の縦の大きさ
#define GAME_COLOR			32	    //画面のカラービット

#define GAME_WINDOW_BAR		0					//タイトルバーはデフォルトにする
#define GAME_WINDOW_NAME	"GAME TITLE"		//ウィンドウのタイトル

#define GAME_FPS			60	//FPSの数値	

//マウスのボタン
#define MOUSE_BUTTON_CODE	129	//0x0000～0x0080まで

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
#define IMAGE_TITLE_BACK_PATH   TEXT(".\\IMAGE\\hospital_Aroom.jpg")
#define IMAGE_TITLE_ROGO_PATH   TEXT(".\\IMAGE\\Title.png")
#define IMAGE_EASY_ROGO_PATH    TEXT(".\\IMAGE\\Easy_logo.png")
#define IMAGE_EASY_ROGO2_PATH    TEXT(".\\IMAGE\\Easy_logo2.png")
#define IMAGE_HARD_ROGO_PATH    TEXT(".\\IMAGE\\Hard_logo.png")
#define IMAGE_HARD_ROGO2_PATH    TEXT(".\\IMAGE\\Hard_logo2.png")
#define IMAGE_RULE_PATH         TEXT(".\\IMAGE\\Rule.png")
#define IMAGE_QUES_BACK_PATH    TEXT(".\\IMAGE\\")
#define IMAGE_QUES_1_PATH       TEXT(".\\IMAGE\\Qusetion1.png")

//画像関連
#define IMAGE_ROGO_ROTA         1.0      //拡大率
#define IMAGE_ROGO_ROTA_MAX     2.0      //拡大率MAX

//エラーメッセージ
#define MUSIC_LOAD_ERR_TITLE	TEXT("音楽読み込みエラー")

//音楽のパス
#define MUSIC_TITLE_PATH        TEXT(".\\MUSIC\\衛星の夜.mp3")

//閉じるボタンを押したとき
#define MSG_CLOSE_TITLE			TEXT("終了メッセージ")
#define MSG_CLOSE_CAPTION		TEXT("ゲームを終了しますか？")

//問題数
#define QUES_MAX                5

//入力タブサイズ
#define BOX_SIZE_X             GAME_WIDTH / 2 - 300
#define BOX_SIZE_Y             GAME_HEIGHT - 120

enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_QUES,
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
char Answer[31];                                //答えの文字配列保存

//マウスの座標を取得
MOUSE mouse;

//ゲーム関連
int GameScene;		//ゲームシーンを管理

//背景関連
IMAGE ImageTitleBack;          //タイトル背景画像
IMAGE ImagePlayBack;           //プレイ背景画像
IMAGE ImageTitleRogo;          //タイトルロゴ画像
IMAGE ImageEasyRogo;           //イージーロゴ画像
IMAGE ImageEasyRogo2;           //イージーロゴ画像
IMAGE ImageHardRogo;           //ハードロゴ画像
IMAGE ImageHardRogo2;           //ハードロゴ画像
IMAGE ImageRule;                //ルール説明画像
IMAGE ImageQuesBack;           //問題画面背景画像
IMAGE ImageQues[QUES_MAX];     //問題画像

//音楽関連
MUSIC BGM;			//ゲームのBGM

int QuesKind;					//問題の正誤状態

//問題数カウント
int CountQues;


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

VOID MY_PLAY_INIT(VOID);	//プレイ画面初期化
VOID MY_PLAY(VOID);			//プレイ画面
VOID MY_PLAY_PROC(VOID);	//プレイ画面の処理
VOID MY_PLAY_DRAW(VOID);	//プレイ画面の描画

VOID MY_PLAY_QUES(VOID);       //問題画面
VOID MY_PLAY_QUES_PROC(VOID);  //問題画面の処理
VOID MY_PLAY_QUES_DRAW(VOID);  //問題画面の描画

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
		case GAME_SCENE_PLAY:
			MY_PLAY();	//プレイ画面
			break;
		case GAME_SCENE_QUES:
			MY_PLAY_QUES();	//問題画面
			break;
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
		if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
		{
			//BGMが流れているなら
			if (CheckSoundMem(BGM.handle) != 0)
			{
				StopSoundMem(BGM.handle);	//BGMを止める
			}

			//ゲームの終了状態を初期化する
			QuesKind = QUES_END_WRONG;

			//ゲームのシーンをプレイ画面にする
			GameScene = GAME_SCENE_STORY;

			return;
		}

	}
	if (mouse.Point.x >= ImageHardRogo.x
		&& mouse.Point.x <= ImageHardRogo.x + ImageHardRogo.width
		&& mouse.Point.y >= ImageHardRogo.y
		&& mouse.Point.y <= ImageHardRogo.y + ImageHardRogo.height)
	{
		if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
		{
			//BGMが流れているなら
			if (CheckSoundMem(BGM.handle) != 0)
			{
				StopSoundMem(BGM.handle);	//BGMを止める
			}

			//ゲームの終了状態を初期化する
			QuesKind = QUES_END_WRONG;

			//ゲームのシーンをプレイ画面にする
			GameScene = GAME_SCENE_STORY;

			return;
		}

	}
		
	//BGMが流れていないなら
	if (CheckSoundMem(BGM.handle) == 0)
	{
		//BGMの音量を下げる
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
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
	if (mouse.Point.x >= ImageHardRogo.x
		&& mouse.Point.x <= ImageHardRogo.x + ImageHardRogo.width
		&& mouse.Point.y >= ImageHardRogo.y
		&& mouse.Point.y <= ImageHardRogo.y + ImageHardRogo.height)
	{
		DrawGraph(ImageHardRogo2.x, ImageHardRogo2.y, ImageHardRogo2.handle, TRUE);
	}
	return;
}

//プレイ画面初期化
VOID MY_PLAY_INIT(VOID)
{
	
	return;
}

//プレイ画面
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//プレイ画面の処理
	MY_PLAY_DRAW();	//プレイ画面の描画

	DrawString(0, 40, "プレイ画面(クリックで問題へ)", GetColor(255, 255, 255));
	return;
}

//プレイ画面の処理
VOID MY_PLAY_PROC(VOID)
{
	//クリックすると問題画面へ
	if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
	{
		//BGMが流れているなら
		if (CheckSoundMem(BGM.handle) != 0)
		{
			StopSoundMem(BGM.handle);	//BGMを止める
		}
		//ゲームのシーンをプレイ画面にする
		GameScene = GAME_SCENE_QUES;

		return;
	}
	
	//BGMが流れていないなら
	if (CheckSoundMem(BGM.handle) == 0)
	{
		//BGMの音量を下げる
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
	}
		

	
	return;
}

//プレイ画面の描画
VOID MY_PLAY_DRAW(VOID)
{
	//背景を描画する
	DrawGraph(ImagePlayBack.x, ImagePlayBack.y, ImagePlayBack.handle, TRUE);
	//ルール説明を描画
	DrawGraph(ImageRule.x, ImageRule.y, ImageRule.handle, TRUE);

	return;
}

//問題画面
VOID MY_PLAY_QUES()
{
	MY_PLAY_QUES_PROC();
	MY_PLAY_QUES_DRAW();

	DrawString(0, 0, "プレイ画面(スペースキーでエンドへ)", GetColor(255, 255, 255));
	DrawString(0, 20, "プレイ画面(Sキーでストーリーへ)", GetColor(255, 255, 255));
	DrawString(0, 40, "プレイ画面(Tキーでタイトルへ)", GetColor(255, 255, 255));
	DrawString(0, 60, "プレイ画面(Rキーでルート分岐へ)", GetColor(255, 255, 255));

	return;
}

VOID MY_PLAY_QUES_PROC()
{
	//Sキーを押したら、ストーリーシーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_S) == TRUE)
	{
		//ゲームのシーンをエンド画面にする
		GameScene = GAME_SCENE_STORY;

		return;
	}
	//Rキーを押したら、ルート分岐シーンへ移動する
	else if (MY_KEY_DOWN(KEY_INPUT_R) == TRUE)
	{
		//ゲームのシーンをエンド画面にする
		GameScene = GAME_SCENE_ROOT;

		return;
	}
	//Tキーを押したら、タイトルシーンへ移動する
	else if (MY_KEY_DOWN(KEY_INPUT_T) == TRUE)
	{
		//ゲームのシーンをエンド画面にする
		GameScene = GAME_SCENE_START;

		return;
	}
	//スペースキーを押したら、エンドシーンへ移動する
	else if (MY_KEY_DOWN(KEY_INPUT_SPACE) == TRUE)
	{
		//ゲームのシーンをエンド画面にする
		GameScene = GAME_SCENE_END;

		return;
	}

	//BGMが流れていないなら
	if (CheckSoundMem(BGM.handle) == 0)
	{
		//BGMの音量を下げる
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
	}

	if (MY_MOUSE_DOWN(MOUSE_INPUT_LEFT) == TRUE)
	{
		ImageQues[CountQues].IsDraw = TRUE;
		/*if (QuesKind == QUES_END_RIGHT)
		{
			CountQues++;
			QuesKind = QUES_END_WRONG;
		}
		else if (QuesKind == QUES_END_WRONG)
		{
			CountQues++;
		}*/
	}



	return;
}

VOID MY_PLAY_QUES_DRAW()
{
	//問題を描画する
	if (ImageQues[CountQues].IsDraw == TRUE)
	{
		DrawGraph(ImageQues[CountQues].x, ImageQues[CountQues].y, ImageQues[CountQues].handle, TRUE);
		DrawBox(BOX_SIZE_X, BOX_SIZE_Y, BOX_SIZE_X+600, BOX_SIZE_Y+20, GetColor(255, 255, 255), TRUE);
		if (mouse.Point.x >= BOX_SIZE_X
			&& mouse.Point.x <= BOX_SIZE_X + 600
			&& mouse.Point.y >= BOX_SIZE_Y
			&& mouse.Point.y <= BOX_SIZE_Y + 20)
		{
			KeyInputString(BOX_SIZE_X, BOX_SIZE_Y,
				30, &Answer[0],
				TRUE);
		}
	}
	


	return;
}

//ストーリー画面
VOID MY_PLAY_STORY()
{
	MY_PLAY_STORY_PROC();
	MY_PLAY_STORY_DRAW();

	DrawString(0, 20, "プレイ画面(Qキーで問題へ)", GetColor(255, 255, 255));
	DrawString(0, 40, "プレイ画面(Rキーでルート分岐へ)", GetColor(255, 255, 255));


	return;
}

VOID MY_PLAY_STORY_PROC()
{
	//Pキーを押したら、プレイシーンへ移動する
	if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
	{
		//ゲームのシーンをエンド画面にする
		GameScene = GAME_SCENE_PLAY;

		return;
	}
	//Rキーを押したら、ルート分岐シーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_R) == TRUE)
	{
		//ゲームのシーンをエンド画面にする
		GameScene = GAME_SCENE_ROOT;

		return;
	}
	//Qキーを押したら、問題シーンへ移動する
	else if (MY_KEY_DOWN(KEY_INPUT_Q) == TRUE)
	{
		//ゲームのシーンを問題画面にする
		GameScene = GAME_SCENE_QUES;

		return;
	}

	//BGMが流れていないなら
	else if (CheckSoundMem(BGM.handle) == 0)
	{
		//BGMの音量を下げる
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
	}

	return;
}

VOID MY_PLAY_STORY_DRAW()
{
	
	return;
}

//ルート分岐画面
VOID MY_PLAY_ROOT()
{
	MY_PLAY_ROOT_PROC();
	MY_PLAY_ROOT_DRAW();

	DrawString(0, 0, "プレイ画面(スペースキーでエンドへ)", GetColor(255, 255, 255));
	DrawString(0, 20, "プレイ画面(Qキーで問題へ)", GetColor(255, 255, 255));

	return;
}

VOID MY_PLAY_ROOT_PROC()
{
	//スペースキーを押したら、エンドシーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_SPACE) == TRUE)
	{
		//ゲームのシーンをエンド画面にする
		GameScene = GAME_SCENE_END;

		return;
	}

	//Qキーを押したら、問題シーンへ移動する
	else if (MY_KEY_DOWN(KEY_INPUT_Q) == TRUE)
	{
		//ゲームのシーンを問題画面にする
		GameScene = GAME_SCENE_QUES;

		return;
	}

	//BGMが流れていないなら
	if (CheckSoundMem(BGM.handle) == 0)
	{
		//BGMの音量を下げる
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);	//50%の音量にする

		//BGMを流す
		//DX_PLAYTYPE_NORMAL:　ノーマル再生
		//DX_PLAYTYPE_BACK  : バックグラウンド再生
		//DX_PLAYTYPE_LOOP  : ループ再生
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
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
	//エスケープキーを押したら、スタートシーンへ移動する
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		GameScene = GAME_SCENE_START;
	}

	DrawString(0, 0, "エンド画面(エスケープキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//エンド画面の描画
VOID MY_END_DRAW(VOID)
{

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
	
	//イージーロゴ
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
	
	//ハードロゴ
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


	//タイトル背景画像
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
	ImageRule.y = GAME_HEIGHT / 2 - ImageRule.height / 2;	//上下中央揃え

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
	return;
}

//音楽をまとめて読み込む関数
BOOL MY_LOAD_MUSIC(VOID)
{
	//背景音楽
	strcpy_s(BGM.path, MUSIC_TITLE_PATH);		//パスの設定
	BGM.handle = LoadSoundMem(BGM.path);	//読み込み
	if (BGM.handle == -1)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), MUSIC_TITLE_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

//音楽をまとめて削除する関数
VOID MY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(BGM.handle);
	return;
}