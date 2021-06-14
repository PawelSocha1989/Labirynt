#ifndef DZWIEKI_H
#define DZWIEKI_H

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class Dzwieki
{
private:
	struct WaveHeaderType
	{
		CHAR chunkId[4];
		unsigned long chunkSize;
		CHAR format[4];
		CHAR subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		CHAR dataChunkId[4];
		unsigned long dataSize;
	};

public:
	Dzwieki();
	Dzwieki(const Dzwieki&);
	~Dzwieki();

	BOOL Initialize(HWND);
	void Shutdown();

	BOOL InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	BOOL LoadWaveFile(CHAR*, IDirectSoundBuffer8**, IDirectSound3DBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**, IDirectSound3DBuffer8**);

	BOOL PlayWaveFile(LONG IVolume, IDirectSoundBuffer8* m_secondaryBuffer, IDirectSound3DBuffer8* m_secondary3DBuffer, DWORD Loop);

	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSound3DListener8* m_listener;
	IDirectSoundBuffer8* m_secondaryBuffer1;
	IDirectSound3DBuffer8* m_secondary3DBuffer1;

	IDirectSoundBuffer8* m_secondaryBuffer2;
	IDirectSound3DBuffer8* m_secondary3DBuffer2;

	IDirectSoundBuffer8* m_secondaryBuffer3;
	IDirectSound3DBuffer8* m_secondary3DBuffer3;

	BOOL pmenu, potwierdzenie, muzyka_tlo;

	void dzwiek_potwierdzenie(LONG glosnosc_efektow);
	void dzwiek_menu(INT odtwarzanie_dzwieku_menu[20][18], INT odtworzone[20][18], LONG glosnosc_efektow);
	void muza_tlo(LONG glosnosc_tla);
	BOOL muzyka_tlo_volume(LONG glosnosc_tla);
};

#endif