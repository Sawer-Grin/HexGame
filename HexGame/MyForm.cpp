#include "MyForm.h"
#include "controller.h"
#include <Windows.h>

using namespace HexGame; // �������� �������
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MyForm);
	while (true){}
	return 0;
}