//########## �w�b�_�[�t�@�C���ǂݍ��� ##########
#include "DxLib.h"
#include "resource.h"

//########## �}�N����` ##########
#define GAME_WIDTH			1280	//��ʂ̉��̑傫��
#define GAME_HEIGHT			720	    //��ʂ̏c�̑傫��
#define GAME_COLOR			32	    //��ʂ̃J���[�r�b�g

#define GAME_WINDOW_BAR		0					//�^�C�g���o�[�̓f�t�H���g�ɂ���
#define GAME_WINDOW_NAME	"GAME TITLE"		//�E�B���h�E�̃^�C�g��

#define GAME_FPS			60	//FPS�̐��l	

//�}�E�X�̃{�^��
#define MOUSE_BUTTON_CODE	129	//0x0000�`0x0080�܂�

//�L�[�{�[�h�̎��
#define KEY_CODE_KIND		256	//256���

//�p�X�̒���
#define PATH_MAX			255	//255�����܂�
#define NAME_MAX			255	//255�����܂�

//�t�H���g
#define FONT_ONRYO_PATH			TEXT(".\\FONT\\onryou.TTF")	        //�t�H���g�̏ꏊ
#define FONT_ONRYO_NAME			TEXT("����")			            //�t�H���g�̖��O

//�G���[���b�Z�[�W
#define FONT_INSTALL_ERR_TITLE	TEXT("�t�H���g�C���X�g�[���G���[")
#define FONT_CREATE_ERR_TITLE	TEXT("�t�H���g�쐬�G���[")

//�G���[���b�Z�[�W
#define IMAGE_LOAD_ERR_TITLE	TEXT("�摜�ǂݍ��݃G���[")

//�摜�̃p�X
#define IMAGE_TITLE_BACK_PATH   TEXT(".\\IMAGE\\hospital_Aroom.jpg")
#define IMAGE_TITLE_ROGO_PATH   TEXT(".\\IMAGE\\Title.png")
#define IMAGE_EASY_ROGO_PATH    TEXT(".\\IMAGE\\Easy_logo.png")
#define IMAGE_EASY_ROGO2_PATH    TEXT(".\\IMAGE\\Easy_logo2.png")
#define IMAGE_HARD_ROGO_PATH    TEXT(".\\IMAGE\\Hard_logo.png")
#define IMAGE_HARD_ROGO2_PATH    TEXT(".\\IMAGE\\Hard_logo2.png")
#define IMAGE_RULE_PATH         TEXT(".\\IMAGE\\Rule.png")
#define IMAGE_QUES_BACK_PATH    TEXT(".\\IMAGE\\")
#define IMAGE_QUES_1_PATH       TEXT(".\\IMAGE\\Qusetion1.png")

//�摜�֘A
#define IMAGE_ROGO_ROTA         1.0      //�g�嗦
#define IMAGE_ROGO_ROTA_MAX     2.0      //�g�嗦MAX

//�G���[���b�Z�[�W
#define MUSIC_LOAD_ERR_TITLE	TEXT("���y�ǂݍ��݃G���[")

//���y�̃p�X
#define MUSIC_TITLE_PATH        TEXT(".\\MUSIC\\�q���̖�.mp3")

//����{�^�����������Ƃ�
#define MSG_CLOSE_TITLE			TEXT("�I�����b�Z�[�W")
#define MSG_CLOSE_CAPTION		TEXT("�Q�[�����I�����܂����H")

//��萔
#define QUES_MAX                5

//���̓^�u�T�C�Y
#define BOX_SIZE_X             GAME_WIDTH / 2 - 300
#define BOX_SIZE_Y             GAME_HEIGHT - 120

enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_QUES,
	GAME_SCENE_ROOT,
	GAME_SCENE_STORY,
	GAME_SCENE_END,
};	//�Q�[���̃V�[��

enum QUES_END {
	QUES_END_RIGHT,	//����
	QUES_END_WRONG	//�s����
};	//���̐���


//int�^��POINT�\����
typedef struct STRUCT_I_POINT
{
	int x = -1;	//���W��������
	int y = -1;	//���W��������
}iPOINT;

//�}�E�X�\����
typedef struct STRUCT_MOUSE
{
	int InputValue = 0;	//GetMouseInput�̒l������
	int WheelValue = 0;	//�}�E�X�z�C�[���̉�]��(���̓v���X�l / ��O�̓}�C�i�X�l)
	iPOINT Point;		//�}�E�X�̍��W������
	iPOINT OldPoint;	//�}�E�X�̍��W(���O)������
	int OldButton[MOUSE_BUTTON_CODE] = { 0 };	//�}�E�X�̃{�^������(���O)������
	int Button[MOUSE_BUTTON_CODE] = { 0 };	//�}�E�X�̃{�^�����͂�����
}MOUSE;

//�t�H���g�\����
typedef struct STRUCT_FONT
{
	char path[PATH_MAX];		//�p�X
	char name[NAME_MAX];		//�t�H���g��
	int handle;					//�n���h��
	int size;					//�傫��
	int bold;					//����
	int type;					//�^�C�v
	//���^�C�v�́Ahttps://dxlib.xsrv.jp/function/dxfunc_graph2.html#R17N10�@���Q��

}FONT;

typedef struct STRUCT_IMAGE
{
	char path[PATH_MAX];		//�p�X
	int handle;					//�n���h��
	int x;						//X�ʒu
	int y;						//Y�ʒu
	int width;					//��
	int height;					//����
	BOOL IsDraw = FALSE;		//�`��ł��邩

	RECT coll;					//�����蔻��
	iPOINT collBeforePt;		//������O�̍��W

}IMAGE;	//�摜�\����

typedef struct STRUCT_IMAGE_ROTA
{
	IMAGE image;		//IMAGE�\����
	double angle;		//��]��
	double angleMAX;	//��]��MAX
	double rate;		//�g�嗦
	double rateMAX;		//�g�嗦MAX

}IMAGE_ROTA;	//��]�g�傷��摜�̍\����

typedef struct STRUCT_MUSIC
{
	char path[PATH_MAX];		//�p�X
	int handle;					//�n���h��
}MUSIC;	//���y�\����


//########## �O���[�o���ϐ� ##########
//FPS�֘A
int StartTimeFps;				//����J�n����
int CountFps;					//�J�E���^
float CalcFps;					//�v�Z����
int SampleNumFps = GAME_FPS;	//���ς����T���v����

//�L�[�{�[�h�̓��͂��擾
char AllKeyState[KEY_CODE_KIND] = { '\0' };		//���ׂẴL�[�̏�Ԃ�����
char OldAllKeyState[KEY_CODE_KIND] = { '\0' };	//���ׂẴL�[�̏��(���O)������
char Answer[31];                                //�����̕����z��ۑ�

//�}�E�X�̍��W���擾
MOUSE mouse;

//�Q�[���֘A
int GameScene;		//�Q�[���V�[�����Ǘ�

//�w�i�֘A
IMAGE ImageTitleBack;          //�^�C�g���w�i�摜
IMAGE ImagePlayBack;           //�v���C�w�i�摜
IMAGE ImageTitleRogo;          //�^�C�g�����S�摜
IMAGE ImageEasyRogo;           //�C�[�W�[���S�摜
IMAGE ImageEasyRogo2;           //�C�[�W�[���S�摜
IMAGE ImageHardRogo;           //�n�[�h���S�摜
IMAGE ImageHardRogo2;           //�n�[�h���S�摜
IMAGE ImageRule;                //���[�������摜
IMAGE ImageQuesBack;           //����ʔw�i�摜
IMAGE ImageQues[QUES_MAX];     //���摜

//���y�֘A
MUSIC BGM;			//�Q�[����BGM

int QuesKind;					//���̐�����

//��萔�J�E���g
int CountQues;


//########## �v���g�^�C�v�錾 ##########
VOID MY_FPS_UPDATE(VOID);			//FPS�l���v���A�X�V����
VOID MY_FPS_DRAW(VOID);				//FPS�l��`�悷��
VOID MY_FPS_WAIT(VOID);				//FPS�l���v�����A�҂�

VOID MY_ALL_KEYDOWN_UPDATE(VOID);	//�L�[�̓��͏�Ԃ��X�V����
BOOL MY_KEY_DOWN(int);				//�L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_UP(int);				//�L�[�������グ�����A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEYDOWN_KEEP(int, int);		//�L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f����

VOID MY_MOUSE_UPDATE(VOID);			//�}�E�X�̓��͏����X�V����
BOOL MY_MOUSE_DOWN(int);			//�{�^���������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����
BOOL MY_MOUSE_UP(int);				//�{�^���������グ�����A�}�E�X�R�[�h�Ŕ��f����
BOOL MY_MOUSEDOWN_KEEP(int, int);	//�{�^�������������Ă��邩�A�}�E�X�R�[�h�Ŕ��f����

BOOL MY_FONT_INSTALL_ONCE(VOID);	//�t�H���g�����̃\�t�g�p�ɁA�ꎞ�I�ɃC���X�g�[��
VOID MY_FONT_UNINSTALL_ONCE(VOID);	//�t�H���g�����̃\�t�g�p�ɁA�ꎞ�I�ɃA���C���X�g�[��
BOOL MY_FONT_CREATE(VOID);			//�t�H���g���쐬����
VOID MY_FONT_DELETE(VOID);			//�t�H���g���폜����

VOID MY_START(VOID);		//�X�^�[�g���
VOID MY_START_PROC(VOID);	//�X�^�[�g��ʂ̏���
VOID MY_START_DRAW(VOID);	//�X�^�[�g��ʂ̕`��

VOID MY_PLAY_INIT(VOID);	//�v���C��ʏ�����
VOID MY_PLAY(VOID);			//�v���C���
VOID MY_PLAY_PROC(VOID);	//�v���C��ʂ̏���
VOID MY_PLAY_DRAW(VOID);	//�v���C��ʂ̕`��

VOID MY_PLAY_QUES(VOID);       //�����
VOID MY_PLAY_QUES_PROC(VOID);  //����ʂ̏���
VOID MY_PLAY_QUES_DRAW(VOID);  //����ʂ̕`��

VOID MY_PLAY_STORY(VOID);       //�X�g�[���[���
VOID MY_PLAY_STORY_PROC(VOID);  //�X�g�[���[��ʂ̏���
VOID MY_PLAY_STORY_DRAW(VOID);  //�X�g�[���[��ʂ̕`��

VOID MY_PLAY_ROOT(VOID);       //���[�g������
VOID MY_PLAY_ROOT_PROC(VOID);  //���[�g�����ʂ̏���
VOID MY_PLAY_ROOT_DRAW(VOID);  //���[�g�����ʂ̕`��

VOID MY_END(VOID);			//�G���h���
VOID MY_END_PROC(VOID);		//�G���h��ʂ̏���
VOID MY_END_DRAW(VOID);		//�G���h��ʂ̕`��

BOOL MY_LOAD_IMAGE(VOID);		//�摜���܂Ƃ߂ēǂݍ��ފ֐�
VOID MY_DELETE_IMAGE(VOID);		//�摜���܂Ƃ߂č폜����֐�

BOOL MY_LOAD_MUSIC(VOID);		//���y���܂Ƃ߂ēǂݍ��ފ֐�
VOID MY_DELETE_MUSIC(VOID);		//���y���܂Ƃ߂č폜����֐�

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�w��̐��l�ŃE�B���h�E��\������
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�^�C�g���o�[�̓f�t�H���g�ɂ���
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));			//�E�B���h�E�̃^�C�g���̕���
	SetAlwaysRunFlag(TRUE);								//��A�N�e�B�u�ł����s����
	SetWindowIconID(IDI_ICON1);							//�A�C�R���t�@�C����Ǎ�
	SetWindowUserCloseEnableFlag(FALSE);				//����{�^���ŁA����ɃE�B���h�E�����Ȃ��悤�ɂ���

	if (DxLib_Init() == -1) { return -1; }	//�c�w���C�u��������������

	//�摜��ǂݍ���
	if (MY_LOAD_IMAGE() == FALSE) { return -1; }

	//���y��ǂݍ���
	if (MY_LOAD_MUSIC() == FALSE) { return -1; }

	//�t�H���g���ꎞ�I�ɃC���X�g�[��
	if (MY_FONT_INSTALL_ONCE() == FALSE) { return -1; }
	//�t�H���g�n���h�����쐬
	if (MY_FONT_CREATE() == FALSE) { return -1; }


	GameScene = GAME_SCENE_START;	//�Q�[���V�[���̓X�^�[�g��ʂ���
	SetDrawScreen(DX_SCREEN_BACK);	//Draw�n�֐��͗���ʂɕ`��

	//�������[�v
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W�����̌��ʂ��G���[�̂Ƃ��A�����I��
		if (ClearDrawScreen() != 0) { break; }	//��ʂ������ł��Ȃ������Ƃ��A�����I��

		//��ʂ̕���{�^���������ꂽ�Ƃ�
		if (GetWindowUserCloseFlag(TRUE))
		{
			//�I���_�C�A���O��\��
			int Ret = MessageBox(GetMainWindowHandle(), MSG_CLOSE_CAPTION, MSG_CLOSE_TITLE, MB_YESNO | MB_ICONASTERISK);
			if (Ret == IDYES) { break; }	//YES�Ȃ�A�Q�[���𒆒f����
		}

		MY_ALL_KEYDOWN_UPDATE();				//�����Ă���L�[��Ԃ��擾

		MY_MOUSE_UPDATE();						//�}�E�X�̏�Ԃ��擾

		MY_FPS_UPDATE();	//FPS�̏���[�X�V]

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();	//�X�^�[�g���
			break;
		case GAME_SCENE_PLAY:
			MY_PLAY();	//�v���C���
			break;
		case GAME_SCENE_QUES:
			MY_PLAY_QUES();	//�����
			break;
		case GAME_SCENE_STORY:
			MY_PLAY_STORY();	//�X�g�[���[���
			break;
		case GAME_SCENE_ROOT:
			MY_PLAY_ROOT();	//���[�g������
			break;
		case GAME_SCENE_END:
			MY_END();	//�G���h���
			break;
		}

		MY_FPS_DRAW();		//FPS�̏���[�`��]

		ScreenFlip();		//���j�^�̃��t���b�V�����[�g�̑����ŗ���ʂ��ĕ`��

		MY_FPS_WAIT();		//FPS�̏���[�҂�]
	}

	//�t�H���g�n���h����j��
	MY_FONT_DELETE();

	//�ꎞ�I�ɃC���X�g�[�������t�H���g���A���C���X�g�[��
	MY_FONT_UNINSTALL_ONCE();

	//�摜�n���h����j��
	MY_DELETE_IMAGE();

	//���y�n���h����j��
	MY_DELETE_MUSIC();

	DxLib_End();	//�c�w���C�u�����g�p�̏I������

	return 0;
}

//########## FPS�l���v���A�X�V����֐� ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1�t���[���ڂȂ玞�����L��
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == SampleNumFps) //60�t���[���ڂȂ畽�ς��v�Z
	{
		int now = GetNowCount();
		//���݂̎��Ԃ���A0�t���[���ڂ̎��Ԃ������AFPS�̐��l�Ŋ��遁1FPS�ӂ�̕��ώ��Ԃ��v�Z�����
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)SampleNumFps);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

//########## FPS�l��`�悷��֐� ##########
VOID MY_FPS_DRAW(VOID)
{
	//�������`��
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

//########## FPS�l���v�����A�҂֐� ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//������������
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;	//�҂ׂ�����

	if (waitTime > 0)		//�w���FPS�l���������ꍇ
	{
		WaitTimer(waitTime);	//�҂�
	}
	return;
}

//########## �L�[�̓��͏�Ԃ��X�V����֐� ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{

	//�ꎞ�I�ɁA���݂̃L�[�̓��͏�Ԃ��i�[����
	char TempKey[KEY_CODE_KIND];

	//���O�̃L�[���͂��Ƃ��Ă���
	for (int i = 0; i < KEY_CODE_KIND; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�

	for (int i = 0; i < KEY_CODE_KIND; i++)
	{
		if (TempKey[i] != 0)	//������Ă���L�[�̃L�[�R�[�h�������Ă���Ƃ�
		{
			AllKeyState[i]++;	//������Ă���
		}
		else
		{
			AllKeyState[i] = 0;	//������Ă��Ȃ�
		}
	}
	return;
}

//�L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
//���@���Fint�F�L�[�R�[�h�FKEY_INPUT_???
BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	//�L�[�R�[�h�̃L�[�������Ă��鎞
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//�L�[�������Ă���
	}
	else
	{
		return FALSE;	//�L�[�������Ă��Ȃ�
	}
}

//�L�[�������グ�����A�L�[�R�[�h�Ŕ��f����
//���@���Fint�F�L�[�R�[�h�FKEY_INPUT_???
BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	//���O�͉����Ă���
		&& AllKeyState[KEY_INPUT_] == 0)	//���͉����Ă��Ȃ��Ƃ�
	{
		return TRUE;	//�L�[�������グ�Ă���
	}
	else
	{
		return FALSE;	//�L�[�������グ�Ă��Ȃ�
	}
}

//�L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f����
//���@���Fint�F�L�[�R�[�h�FKEY_INPUT_???
//���@���Fint�F�L�[�����������鎞��
BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	//���������鎞��=�b���~FPS�l
	//��j60FPS�̃Q�[���ŁA1�b�ԉ���������Ȃ�A1�b�~60FPS
	int UpdateTime = DownTime * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//���������Ă���
	}
	else
	{
		return FALSE;	//���������Ă��Ȃ�
	}
}

//�}�E�X�̓��͏����X�V����
VOID MY_MOUSE_UPDATE(VOID)
{
	//�}�E�X�̈ȑO�̍��W���擾
	mouse.OldPoint = mouse.Point;

	//�}�E�X�̈ȑO�̃{�^�����͂��擾
	for (int i = 0; i < MOUSE_BUTTON_CODE; i++) { mouse.OldButton[i] = mouse.Button[i]; }

	//�}�E�X�̍��W���擾
	GetMousePoint(&mouse.Point.x, &mouse.Point.y);

	//�}�E�X�̉����Ă���{�^�����擾
	mouse.InputValue = GetMouseInput();

	//���{�^���������Ă��邩�`�F�b�N(TRUE��0�ȊO�Ȃ̂ŁA2��4��TRUE)
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) == MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT]++; }		//�����Ă���
	if ((mouse.InputValue & MOUSE_INPUT_LEFT) != MOUSE_INPUT_LEFT) { mouse.Button[MOUSE_INPUT_LEFT] = 0; }		//�����Ă��Ȃ�

	//���{�^���������Ă��邩�`�F�b�N(TRUE��0�ȊO�Ȃ̂ŁA2��4��TRUE)
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) == MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE]++; }	//�����Ă���
	if ((mouse.InputValue & MOUSE_INPUT_MIDDLE) != MOUSE_INPUT_MIDDLE) { mouse.Button[MOUSE_INPUT_MIDDLE] = 0; }	//�����Ă��Ȃ�

	//�E�{�^���������Ă��邩�`�F�b�N(TRUE��0�ȊO�Ȃ̂ŁA2��4��TRUE)
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) == MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT]++; }		//�����Ă���
	if ((mouse.InputValue & MOUSE_INPUT_RIGHT) != MOUSE_INPUT_RIGHT) { mouse.Button[MOUSE_INPUT_RIGHT] = 0; }	//�����Ă��Ȃ�

	//�z�C�[���̉�]�ʂ��擾
	mouse.WheelValue = GetMouseWheelRotVol();

	return;
}

//�{�^���������Ă��邩�A�}�E�X�[�R�[�h�Ŕ��f����
//���@���Fint�F�}�E�X�R�[�h�FMOUSE_INPUT_???
BOOL MY_MOUSE_DOWN(int MOUSE_INPUT_)
{
	//�}�E�X�R�[�h�̃{�^���������Ă��鎞
	if (mouse.Button[MOUSE_INPUT_] != 0)
	{
		return TRUE;	//�{�^���������Ă���
	}
	else
	{
		return FALSE;	//�{�^���������Ă��Ȃ�
	}
}

//�{�^���������グ�����A�}�E�X�R�[�h���f����
//���@���Fint�F�}�E�X�R�[�h�FMOUSE_INPUT_???
BOOL MY_MOUSE_UP(int MOUSE_INPUT_)
{
	if (mouse.OldButton[MOUSE_INPUT_] >= 1	//���O�͉����Ă���
		&& mouse.Button[MOUSE_INPUT_] == 0)	//���͉����Ă��Ȃ��Ƃ�
	{
		return TRUE;	//�{�^���������グ�Ă���
	}
	else
	{
		return FALSE;	//�{�^���������グ�Ă��Ȃ�
	}
}

//�{�^�������������Ă��邩�A�}�E�X�R�[�h���f����
//���@���Fint�F�}�E�X�R�[�h�FMOUSE_INPUT_???
//���@���Fint�F�{�^�������������鎞��
BOOL MY_MOUSEDOWN_KEEP(int MOUSE_INPUT_, int DownTime)
{
	//���������鎞��=�b���~FPS�l
	//��j60FPS�̃Q�[���ŁA1�b�ԉ���������Ȃ�A1�b�~60FPS
	int UpdateTime = DownTime * GAME_FPS;

	if (mouse.Button[MOUSE_INPUT_] > UpdateTime)
	{
		return TRUE;	//���������Ă���
	}
	else
	{
		return FALSE;	//���������Ă��Ȃ�
	}
}

//�t�H���g�����̃\�t�g�p�ɁA�ꎞ�I�ɃC���X�g�[��
BOOL MY_FONT_INSTALL_ONCE(VOID)
{
	//�t�H���g���ꎞ�I�ɓǂݍ���(WinAPI)
	if (AddFontResourceEx(FONT_ONRYO_PATH, FR_PRIVATE, NULL) == 0)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), FONT_ONRYO_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

//�t�H���g�����̃\�t�g�p�ɁA�ꎞ�I�ɃA���C���X�g�[��
VOID MY_FONT_UNINSTALL_ONCE(VOID)
{
	//�ꎞ�I�ɓǂݍ��񂾃t�H���g���폜(WinAPI)
	RemoveFontResourceEx(FONT_ONRYO_PATH, FR_PRIVATE, NULL);

	return;
}

//�t�H���g��ǂݍ��ފ֐�
//���@���Fconst char *�F�ǂݍ��ރt�H���g�̖��O
//���@���Fint�@�F�t�H���g�̑傫��
//���@���Fint�@�F�t�H���g�̑���
//���@���Fint�@�F�t�H���g�̃^�C�v
//�߂�l�FBOOL �F�G���[����FALSE / ���펞��TRUE
BOOL MY_FONT_CREATE(VOID)
{
	return TRUE;
}

//�t�H���g���폜����֐�
VOID MY_FONT_DELETE(VOID)
{

	return;
}

//�X�^�[�g���
VOID MY_START(VOID)
{
	MY_START_PROC();	//�X�^�[�g��ʂ̏���
	MY_START_DRAW();	//�X�^�[�g��ʂ̕`��


	return;
}

//�X�^�[�g��ʂ̏���
VOID MY_START_PROC(VOID)
{
	//��Փx����������A�v���C�V�[���ֈړ�����
	if (mouse.Point.x >= ImageEasyRogo.x 
		&& mouse.Point.x <= ImageEasyRogo.x+ImageEasyRogo.width
		&& mouse.Point.y >= ImageEasyRogo.y 
		&& mouse.Point.y <= ImageEasyRogo.y+ImageEasyRogo.height)
	{
		if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
		{
			//BGM������Ă���Ȃ�
			if (CheckSoundMem(BGM.handle) != 0)
			{
				StopSoundMem(BGM.handle);	//BGM���~�߂�
			}

			//�Q�[���̏I����Ԃ�����������
			QuesKind = QUES_END_WRONG;

			//�Q�[���̃V�[�����v���C��ʂɂ���
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
			//BGM������Ă���Ȃ�
			if (CheckSoundMem(BGM.handle) != 0)
			{
				StopSoundMem(BGM.handle);	//BGM���~�߂�
			}

			//�Q�[���̏I����Ԃ�����������
			QuesKind = QUES_END_WRONG;

			//�Q�[���̃V�[�����v���C��ʂɂ���
			GameScene = GAME_SCENE_STORY;

			return;
		}

	}
		
	//BGM������Ă��Ȃ��Ȃ�
	if (CheckSoundMem(BGM.handle) == 0)
	{
		//BGM�̉��ʂ�������
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);	//50%�̉��ʂɂ���

		//BGM�𗬂�
		//DX_PLAYTYPE_NORMAL:�@�m�[�}���Đ�
		//DX_PLAYTYPE_BACK  : �o�b�N�O���E���h�Đ�
		//DX_PLAYTYPE_LOOP  : ���[�v�Đ�
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
	}

	return;
}

//�X�^�[�g��ʂ̕`��
VOID MY_START_DRAW(VOID)
{
	//�w�i��`�悷��
	DrawGraph(ImageTitleBack.x, ImageTitleBack.y, ImageTitleBack.handle, TRUE);

	//�^�C�g�����S��`�悷��
	DrawGraph(ImageTitleRogo.x, ImageTitleRogo.y, ImageTitleRogo.handle, TRUE);

	//��Փx���S�`��
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

//�v���C��ʏ�����
VOID MY_PLAY_INIT(VOID)
{
	
	return;
}

//�v���C���
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//�v���C��ʂ̏���
	MY_PLAY_DRAW();	//�v���C��ʂ̕`��

	DrawString(0, 40, "�v���C���(�N���b�N�Ŗ���)", GetColor(255, 255, 255));
	return;
}

//�v���C��ʂ̏���
VOID MY_PLAY_PROC(VOID)
{
	//�N���b�N����Ɩ���ʂ�
	if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
	{
		//BGM������Ă���Ȃ�
		if (CheckSoundMem(BGM.handle) != 0)
		{
			StopSoundMem(BGM.handle);	//BGM���~�߂�
		}
		//�Q�[���̃V�[�����v���C��ʂɂ���
		GameScene = GAME_SCENE_QUES;

		return;
	}
	
	//BGM������Ă��Ȃ��Ȃ�
	if (CheckSoundMem(BGM.handle) == 0)
	{
		//BGM�̉��ʂ�������
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);	//50%�̉��ʂɂ���

		//BGM�𗬂�
		//DX_PLAYTYPE_NORMAL:�@�m�[�}���Đ�
		//DX_PLAYTYPE_BACK  : �o�b�N�O���E���h�Đ�
		//DX_PLAYTYPE_LOOP  : ���[�v�Đ�
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
	}
		

	
	return;
}

//�v���C��ʂ̕`��
VOID MY_PLAY_DRAW(VOID)
{
	//�w�i��`�悷��
	DrawGraph(ImagePlayBack.x, ImagePlayBack.y, ImagePlayBack.handle, TRUE);
	//���[��������`��
	DrawGraph(ImageRule.x, ImageRule.y, ImageRule.handle, TRUE);

	return;
}

//�����
VOID MY_PLAY_QUES()
{
	MY_PLAY_QUES_PROC();
	MY_PLAY_QUES_DRAW();

	DrawString(0, 0, "�v���C���(�X�y�[�X�L�[�ŃG���h��)", GetColor(255, 255, 255));
	DrawString(0, 20, "�v���C���(S�L�[�ŃX�g�[���[��)", GetColor(255, 255, 255));
	DrawString(0, 40, "�v���C���(T�L�[�Ń^�C�g����)", GetColor(255, 255, 255));
	DrawString(0, 60, "�v���C���(R�L�[�Ń��[�g�����)", GetColor(255, 255, 255));

	return;
}

VOID MY_PLAY_QUES_PROC()
{
	//S�L�[����������A�X�g�[���[�V�[���ֈړ�����
	if (MY_KEY_DOWN(KEY_INPUT_S) == TRUE)
	{
		//�Q�[���̃V�[�����G���h��ʂɂ���
		GameScene = GAME_SCENE_STORY;

		return;
	}
	//R�L�[����������A���[�g����V�[���ֈړ�����
	else if (MY_KEY_DOWN(KEY_INPUT_R) == TRUE)
	{
		//�Q�[���̃V�[�����G���h��ʂɂ���
		GameScene = GAME_SCENE_ROOT;

		return;
	}
	//T�L�[����������A�^�C�g���V�[���ֈړ�����
	else if (MY_KEY_DOWN(KEY_INPUT_T) == TRUE)
	{
		//�Q�[���̃V�[�����G���h��ʂɂ���
		GameScene = GAME_SCENE_START;

		return;
	}
	//�X�y�[�X�L�[����������A�G���h�V�[���ֈړ�����
	else if (MY_KEY_DOWN(KEY_INPUT_SPACE) == TRUE)
	{
		//�Q�[���̃V�[�����G���h��ʂɂ���
		GameScene = GAME_SCENE_END;

		return;
	}

	//BGM������Ă��Ȃ��Ȃ�
	if (CheckSoundMem(BGM.handle) == 0)
	{
		//BGM�̉��ʂ�������
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);	//50%�̉��ʂɂ���

		//BGM�𗬂�
		//DX_PLAYTYPE_NORMAL:�@�m�[�}���Đ�
		//DX_PLAYTYPE_BACK  : �o�b�N�O���E���h�Đ�
		//DX_PLAYTYPE_LOOP  : ���[�v�Đ�
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
	//����`�悷��
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

//�X�g�[���[���
VOID MY_PLAY_STORY()
{
	MY_PLAY_STORY_PROC();
	MY_PLAY_STORY_DRAW();

	DrawString(0, 20, "�v���C���(Q�L�[�Ŗ���)", GetColor(255, 255, 255));
	DrawString(0, 40, "�v���C���(R�L�[�Ń��[�g�����)", GetColor(255, 255, 255));


	return;
}

VOID MY_PLAY_STORY_PROC()
{
	//P�L�[����������A�v���C�V�[���ֈړ�����
	if (MY_MOUSE_UP(MOUSE_INPUT_LEFT) == TRUE)
	{
		//�Q�[���̃V�[�����G���h��ʂɂ���
		GameScene = GAME_SCENE_PLAY;

		return;
	}
	//R�L�[����������A���[�g����V�[���ֈړ�����
	if (MY_KEY_DOWN(KEY_INPUT_R) == TRUE)
	{
		//�Q�[���̃V�[�����G���h��ʂɂ���
		GameScene = GAME_SCENE_ROOT;

		return;
	}
	//Q�L�[����������A���V�[���ֈړ�����
	else if (MY_KEY_DOWN(KEY_INPUT_Q) == TRUE)
	{
		//�Q�[���̃V�[�������ʂɂ���
		GameScene = GAME_SCENE_QUES;

		return;
	}

	//BGM������Ă��Ȃ��Ȃ�
	else if (CheckSoundMem(BGM.handle) == 0)
	{
		//BGM�̉��ʂ�������
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);	//50%�̉��ʂɂ���

		//BGM�𗬂�
		//DX_PLAYTYPE_NORMAL:�@�m�[�}���Đ�
		//DX_PLAYTYPE_BACK  : �o�b�N�O���E���h�Đ�
		//DX_PLAYTYPE_LOOP  : ���[�v�Đ�
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
	}

	return;
}

VOID MY_PLAY_STORY_DRAW()
{
	
	return;
}

//���[�g������
VOID MY_PLAY_ROOT()
{
	MY_PLAY_ROOT_PROC();
	MY_PLAY_ROOT_DRAW();

	DrawString(0, 0, "�v���C���(�X�y�[�X�L�[�ŃG���h��)", GetColor(255, 255, 255));
	DrawString(0, 20, "�v���C���(Q�L�[�Ŗ���)", GetColor(255, 255, 255));

	return;
}

VOID MY_PLAY_ROOT_PROC()
{
	//�X�y�[�X�L�[����������A�G���h�V�[���ֈړ�����
	if (MY_KEY_DOWN(KEY_INPUT_SPACE) == TRUE)
	{
		//�Q�[���̃V�[�����G���h��ʂɂ���
		GameScene = GAME_SCENE_END;

		return;
	}

	//Q�L�[����������A���V�[���ֈړ�����
	else if (MY_KEY_DOWN(KEY_INPUT_Q) == TRUE)
	{
		//�Q�[���̃V�[�������ʂɂ���
		GameScene = GAME_SCENE_QUES;

		return;
	}

	//BGM������Ă��Ȃ��Ȃ�
	if (CheckSoundMem(BGM.handle) == 0)
	{
		//BGM�̉��ʂ�������
		ChangeVolumeSoundMem(255 * 50 / 100, BGM.handle);	//50%�̉��ʂɂ���

		//BGM�𗬂�
		//DX_PLAYTYPE_NORMAL:�@�m�[�}���Đ�
		//DX_PLAYTYPE_BACK  : �o�b�N�O���E���h�Đ�
		//DX_PLAYTYPE_LOOP  : ���[�v�Đ�
		PlaySoundMem(BGM.handle, DX_PLAYTYPE_LOOP);
	}

	return;
}

VOID MY_PLAY_ROOT_DRAW()
{
	return;
}

//�G���h���
VOID MY_END(VOID)
{
	MY_END_PROC();	//�G���h��ʂ̏���
	MY_END_DRAW();	//�G���h��ʂ̕`��


	return;
}

//�G���h��ʂ̏���
VOID MY_END_PROC(VOID)
{
	//�G�X�P�[�v�L�[����������A�X�^�[�g�V�[���ֈړ�����
	if (MY_KEY_DOWN(KEY_INPUT_ESCAPE) == TRUE)
	{
		GameScene = GAME_SCENE_START;
	}

	DrawString(0, 0, "�G���h���(�G�X�P�[�v�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

//�G���h��ʂ̕`��
VOID MY_END_DRAW(VOID)
{

	return;
}

//�摜���܂Ƃ߂ēǂݍ��ފ֐�
BOOL MY_LOAD_IMAGE(VOID)
{
	//�^�C�g���w�i�摜
	strcpy_s(ImageTitleBack.path, IMAGE_TITLE_BACK_PATH);		//�p�X�̐ݒ�
	ImageTitleBack.handle = LoadGraph(ImageTitleBack.path);	//�ǂݍ���
	if (ImageTitleBack.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_BACK_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleBack.handle, &ImageTitleBack.width, &ImageTitleBack.height);	//�摜�̕��ƍ������擾
	ImageTitleBack.x = GAME_WIDTH / 2 - ImageTitleBack.width / 2;		//���E��������
	ImageTitleBack.y = GAME_HEIGHT / 2 - ImageTitleBack.height / 2;	//�㉺��������

	//�^�C�g�����S
	strcpy_s(ImageTitleRogo.path, IMAGE_TITLE_ROGO_PATH);					//�p�X�̐ݒ�
	ImageTitleRogo.handle = LoadGraph(ImageTitleRogo.path);			//�ǂݍ���
	if (ImageTitleRogo.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_TITLE_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageTitleRogo.handle, &ImageTitleRogo.width, &ImageTitleRogo.height);	      //�摜�̕��ƍ������擾
	ImageTitleRogo.x = GAME_WIDTH / 2 - ImageTitleRogo.width / 2;		                      //������
	ImageTitleRogo.y = GAME_HEIGHT / 2 - ImageTitleRogo.height / 2 - 100;				      //������+�摜��

	//�C�[�W�[���S
	strcpy_s(ImageEasyRogo.path, IMAGE_EASY_ROGO_PATH);					//�p�X�̐ݒ�
	ImageEasyRogo.handle = LoadGraph(ImageEasyRogo.path);			//�ǂݍ���
	if (ImageEasyRogo.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_EASY_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEasyRogo.handle, &ImageEasyRogo.width, &ImageEasyRogo.height);	                  //�摜�̕��ƍ������擾
	ImageEasyRogo.x = GAME_WIDTH / 2 - ImageEasyRogo.width - ImageEasyRogo.width / 2;                     //������-�摜��
	ImageEasyRogo.y = GAME_HEIGHT / 2 + ImageEasyRogo.height*2;				            //������+�摜��
	
	//�C�[�W�[���S
	strcpy_s(ImageEasyRogo2.path, IMAGE_EASY_ROGO2_PATH);					//�p�X�̐ݒ�
	ImageEasyRogo2.handle = LoadGraph(ImageEasyRogo2.path);			//�ǂݍ���
	if (ImageEasyRogo2.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_EASY_ROGO2_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageEasyRogo2.handle, &ImageEasyRogo2.width, &ImageEasyRogo2.height);	                  //�摜�̕��ƍ������擾
	ImageEasyRogo2.x = GAME_WIDTH / 2 - ImageEasyRogo2.width - ImageEasyRogo2.width / 2;                     //������-�摜��
	ImageEasyRogo2.y = GAME_HEIGHT / 2 + ImageEasyRogo2.height * 2;				            //������+�摜��


	//�n�[�h���S
	strcpy_s(ImageHardRogo.path, IMAGE_HARD_ROGO_PATH);					//�p�X�̐ݒ�
	ImageHardRogo.handle = LoadGraph(ImageHardRogo.path);			//�ǂݍ���
	if (ImageHardRogo.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_HARD_ROGO_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageHardRogo.handle, &ImageHardRogo.width, &ImageHardRogo.height);	//�摜�̕��ƍ������擾
	ImageHardRogo.x = GAME_WIDTH / 2 + ImageHardRogo.width/2;	                        //������+�摜��
	ImageHardRogo.y = GAME_HEIGHT / 2 + ImageHardRogo.height*2;				            //������+�摜��
	
	//�n�[�h���S
	strcpy_s(ImageHardRogo2.path, IMAGE_HARD_ROGO2_PATH);					//�p�X�̐ݒ�
	ImageHardRogo2.handle = LoadGraph(ImageHardRogo2.path);			//�ǂݍ���
	if (ImageHardRogo2.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_HARD_ROGO2_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageHardRogo2.handle, &ImageHardRogo2.width, &ImageHardRogo2.height);	//�摜�̕��ƍ������擾
	ImageHardRogo2.x = GAME_WIDTH / 2 + ImageHardRogo2.width / 2;	                        //������+�摜��
	ImageHardRogo2.y = GAME_HEIGHT / 2 + ImageHardRogo2.height * 2;				            //������+�摜��


	//�^�C�g���w�i�摜
	strcpy_s(ImageRule.path, IMAGE_RULE_PATH);		//�p�X�̐ݒ�
	ImageRule.handle = LoadGraph(ImageRule.path);	//�ǂݍ���
	if (ImageRule.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_RULE_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageRule.handle, &ImageRule.width, &ImageRule.height);	//�摜�̕��ƍ������擾
	ImageRule.x = GAME_WIDTH / 2 - ImageRule.width / 2;		//���E��������
	ImageRule.y = GAME_HEIGHT / 2 - ImageRule.height / 2;	//�㉺��������

	//���P�摜
	strcpy_s(ImageQues[0].path, IMAGE_QUES_1_PATH);		//�p�X�̐ݒ�
	ImageQues[0].handle = LoadGraph(ImageQues[0].path);	//�ǂݍ���
	if (ImageQues[0].handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), IMAGE_QUES_1_PATH, IMAGE_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}
	GetGraphSize(ImageQues[0].handle, &ImageQues[0].width, &ImageQues[0].height);	//�摜�̕��ƍ������擾
	ImageQues[0].x = GAME_WIDTH / 2 - ImageQues[0].width / 2;		//���E��������
	ImageQues[0].y = GAME_HEIGHT / 2 - ImageQues[0].height / 2;	    //�㉺��������
	ImageQues[0].IsDraw = FALSE;

	return TRUE;
}

//�摜���܂Ƃ߂č폜����֐�
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

//���y���܂Ƃ߂ēǂݍ��ފ֐�
BOOL MY_LOAD_MUSIC(VOID)
{
	//�w�i���y
	strcpy_s(BGM.path, MUSIC_TITLE_PATH);		//�p�X�̐ݒ�
	BGM.handle = LoadSoundMem(BGM.path);	//�ǂݍ���
	if (BGM.handle == -1)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), MUSIC_TITLE_PATH, MUSIC_LOAD_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

//���y���܂Ƃ߂č폜����֐�
VOID MY_DELETE_MUSIC(VOID)
{
	DeleteSoundMem(BGM.handle);
	return;
}