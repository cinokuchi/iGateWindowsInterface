#pragma once
#include <WinDef.h>
#include <stdint.h>
#include "Queue.h"

class WriteArg {
public:
	char* buffer;
	int numChars;
	WriteArg(const char* buffer, int numChars);
	WriteArg(const WriteArg &src);
	WriteArg& operator=(const WriteArg &src);
	~WriteArg();
};

class ComPortManager {
private:
	static const char* TAG;
	Queue<WriteArg> messageQueue;
	HWND mUiWindow;
	wchar_t* mComPortNameW;
	char* mComPortName;
	wchar_t* mFilePathW;
	char* mFilePath;
	HANDLE killEvent, writeRequestEvent;
	HANDLE hCom, hFile;
	uint32_t baudRate, dataBits, parity, stopBits;
	void* threadContext;
	int mPaneID;

	bool connect();
	void loop();
	void disconnect();
	void handleWrite(int, const char*);

	static void threadFtn(void*);
	static void log(const char* errorMsg);
	static bool logAndQuit(const char* errorMsg);

	ComPortManager(const ComPortManager &src);
	ComPortManager& operator=(const ComPortManager &src);

public:
	void handleRead(const BYTE*, DWORD);
	ComPortManager(const wchar_t* comPortName, const wchar_t* fileName, HWND uiWindow, int paneID);
	~ComPortManager();
	void startThread(uint32_t baudRate, uint32_t dataBits, uint32_t parity, uint32_t stopBits);
	bool endThread();
	/*
		It is the caller's responsibility to free the buffer.
		The buffer may be freed immediately.
	*/
	void writeRaw(const char* buffer, int numChars);
	bool writeHex(const char* buffer, int numChars);

	void getPortName(char* buffer);
	void getFileName(char* buffer);
	int getPaneID();
};