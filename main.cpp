
#include "main.h"

HWND hWnd = NULL;													// main window handle
FILE *f = NULL;
HDC hdc = NULL, hdcreal = NULL;
HBITMAP hBitmap = NULL;

RECT myrect = { 0 , 0 , 0 , 0 };
int screen_width = 0, screen_height = 0;					// screen geom

char string[80];
unsigned int starttime = 0;
unsigned int frames = 0;
const float max_fps = 30.;
float fps = max_fps; 

void (*Error)(char *);										// pointer to current error func

int DDInit(HWND);
int DDShutdown();
void DDDraw();												// main frame draw func

int addlog(char*);

void Errors_before_window_cycle(char *message)
{
	MessageBox(0,message,"Error",MB_OK | MB_ICONSTOP);		
															
	addlog(message);										// in any case add message to log
	gcleanup();												// call of ginit() is before DDInit()
															// so it can be so, that ginit() was
															// already called
	fclose(f);
	if (hWnd)
		DestroyWindow(hWnd);
	ExitProcess(0);
}

void Errors_before_window_created(char *message)
{
	MessageBox(0,message,"Error",MB_OK | MB_ICONSTOP);
	addlog(message);
	fclose(f);
	ExitProcess(0);
}

void Errors_can_continue(char *message)
{
	addlog(message);
}

void Errors_while_direct_draw(char *message)
{
	addlog(message);							// add log, but not display message
	gcleanup();									// do cleanup and shutdown
	DDShutdown();
	ShowCursor(TRUE);
	addlog("Log closing with error");
	fclose(f);
	if (hWnd)
		DestroyWindow(hWnd);
	ExitProcess(0);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc0;

	switch (message)
	{
	case WM_DESTROY:

		if (hdcreal)
		{
			if (hdc)
				DeleteDC(hdc);
			if (hBitmap)
				DeleteObject(hBitmap);
			ReleaseDC(hWnd,hdcreal);
			hdc = NULL;
		}

		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc0 = BeginPaint(hWnd,&ps);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostMessage(hWnd,WM_DESTROY,0,0);
		return 0;
	default:
		return DefWindowProc(hWnd,message,wParam,lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	WNDCLASSEX wc;
	MSG mesg;

	Error = Errors_before_window_created;		// process errors before window creation
	wc.cbSize = 48;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hIcon = LoadIcon(NULL,IDI_WINLOGO);
	wc.hIconSm = 0;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASSNAME;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc))
		Error("Window class invalid");

	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_height = GetSystemMetrics(SM_CYSCREEN);
	hWnd = CreateWindowEx(0,CLASSNAME,"Pure rotating demo",
		WS_POPUP | WS_VISIBLE,0,0,screen_width,screen_height,0,NULL,hInstance,NULL);

	if (!hWnd)
		Error("Bad params to CreateWindow");

	Error = Errors_before_window_cycle;		// now process errors with running window

	if (!(f = fopen(LOGFILENAME,"wt")))
		Error("Logfile open fails");

	srand(GetTickCount());
	addlog("Program started");
		
	ShowCursor(FALSE);
	if (!ginit(screen_width,screen_height))
		Error("Initialization failed");

	Error = Errors_while_direct_draw;

#ifndef DEBUG
	DDInit(hWnd);
#endif

	starttime = GetTickCount();

	while (1)
	{
		if (PeekMessage(&mesg,0,0,0,PM_REMOVE))
		{
			if (mesg.message == WM_QUIT)
				break;
			TranslateMessage(&mesg);
			DispatchMessage(&mesg);
		}

		DDDraw();
		
		frames++;

		unsigned int dt = GetTickCount() - starttime;
	
		if (dt >= 1000)
		{
			fps = (float_t)frames * 1000. / (float_t)dt;
			frames = 0; starttime = GetTickCount();		
		}
		
	}

	Error = Errors_before_window_created;		// here window is already destroyed

	gcleanup();

#ifndef DEBUG
	DDShutdown();
#endif

	ShowCursor(TRUE);
	addlog("Log closing");
	fclose(f);

	return mesg.wParam;
}

void DDDraw()
{
	if (!gmain(screen_width,screen_height))
		Error("Algorithm error");

	sprintf(string,"%.2f fps",fps);
	SetTextColor(hdc,RGB(255,255,255));
	SetBkMode(hdc,TRANSPARENT);
	TextOut(hdc,10,10,string,strlen(string));
	sprintf(string,"SPACE - change figure");
	TextOut(hdc,10,30,string,strlen(string));
	sprintf(string,"ESC - quit");
	TextOut(hdc,10,50,string,strlen(string));

	BitBlt(hdcreal,0,0,screen_width,screen_height,hdc,0,0,SRCCOPY);
}

int DDInit(HWND hWnd)
{
	hdcreal = GetDC(hWnd);
	hdc = CreateCompatibleDC(hdcreal);
	hBitmap = CreateCompatibleBitmap(hdcreal,screen_width,screen_height);
	SelectObject(hdc,hBitmap);
	return 1;
}

int DDShutdown()
{
	return 1;
}

int addlog(char *line)
{
	if (f)
		fprintf(f,"%s\n",line);

	return 1;
}
