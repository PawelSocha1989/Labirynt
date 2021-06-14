#include "Dzwieki.h"

Dzwieki::Dzwieki()
{
	m_DirectSound = 0;
	m_primaryBuffer = 0;
	m_listener = 0;
	m_secondaryBuffer1 = 0;
	m_secondary3DBuffer1 = 0;
	m_secondaryBuffer2 = 0;
	m_secondary3DBuffer2 = 0;
	m_secondaryBuffer3 = 0;
	m_secondary3DBuffer3 = 0;
}


Dzwieki::Dzwieki(const Dzwieki& other)
{
}


Dzwieki::~Dzwieki()
{
}


BOOL Dzwieki::Initialize(HWND hwnd)
{
	// Initialize direct sound and the primary sound buffer.
	pmenu = InitializeDirectSound(hwnd);
	if (!pmenu)
		return false;

	potwierdzenie = InitializeDirectSound(hwnd);
	if (!potwierdzenie)
		return false;

	muzyka_tlo = InitializeDirectSound(hwnd);
	if (!muzyka_tlo)
		return false;

	// Load a wave audio file onto a secondary buffer.
	pmenu = LoadWaveFile("../Release/Sounds/pmenu.wav", &m_secondaryBuffer1, &m_secondary3DBuffer1);
	if (!pmenu)
		return false;

	potwierdzenie = LoadWaveFile("../Release/Sounds/potwierdz.wav", &m_secondaryBuffer2, &m_secondary3DBuffer2);
	if (!potwierdzenie)
		return false;

	muzyka_tlo = LoadWaveFile("../Release/Sounds/chill.wav", &m_secondaryBuffer3, &m_secondary3DBuffer3);
	if (!muzyka_tlo)
		return false;

	return true;
}

void Dzwieki::Shutdown()
{
	// Release the secondary buffer.
	ShutdownWaveFile(&m_secondaryBuffer1, &m_secondary3DBuffer1);
	ShutdownWaveFile(&m_secondaryBuffer2, &m_secondary3DBuffer2);
	ShutdownWaveFile(&m_secondaryBuffer3, &m_secondary3DBuffer3);

	// Shutdown the Direct Sound API.
	ShutdownDirectSound();

	return;
}

BOOL Dzwieki::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;


	// Initialize the direct sound INTerface poINTer for the default sound device.
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
		return false;

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
		return false;

	// Setup the primary buffer description.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
		return false;

	// Setup the format of the primary sound bufffer.
	// In this case it is a .WAV file recorded at 44,100 samples per second in 16-bit stereo (cd audio format).
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified.
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
		return false;

	// Obtain a listener INTerface.
	result = m_primaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_listener);
	if (FAILED(result))
		return false;

	// Set the initial position of the listener to be in the middle of the scene.
	m_listener->SetPosition(0.0f, 0.0f, 0.0f, DS3D_IMMEDIATE);

	return true;
}

void Dzwieki::ShutdownDirectSound()
{
	// Release the listener INTerface.
	if (m_listener)
	{
		m_listener->Release();
		m_listener = 0;
	}

	// Release the primary sound buffer poINTer.
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	// Release the direct sound INTerface poINTer.
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}

	return;
}

BOOL Dzwieki::LoadWaveFile(CHAR* filename, IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
{
	INT error;
	FILE* filePtr;
	UINT count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	UCHAR *waveData;
	UCHAR *bufferPtr;
	ULONG bufferSize;


	// Open the wave file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
		return false;

	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
		return false;

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
		return false;

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
		return false;

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
		return false;

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
		return false;

	// Check that the wave file was recorded in mono format.
	if (waveFileHeader.numChannels != 1)
		return false;

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (waveFileHeader.sampleRate != 44100)
		return false;

	// Ensure that the wave file was recorded in 16 bit format.
	if (waveFileHeader.bitsPerSample != 16)
		return false;

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
		return false;

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
		return false;

	// Test the buffer format against the direct sound 8 INTerface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
		return false;

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new UCHAR[waveFileHeader.dataSize];
	if (!waveData)
		return false;

	// Read in the wave file data INTo the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
		return false;

	// Close the file ONCE done reading.
	error = fclose(filePtr);
	if (error != 0)
		return false;

	// Lock the secondary buffer to write wave data INTo it.
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
		return false;

	// Copy the wave data INTo the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
		return false;

	// Release the wave data since it was copied INTo the secondary buffer.
	delete[] waveData;
	waveData = 0;

	// Get the 3D INTerface to the secondary sound buffer.
	result = (*secondaryBuffer)->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&*secondary3DBuffer);
	if (FAILED(result))
		return false;

	return true;
}


void Dzwieki::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer, IDirectSound3DBuffer8** secondary3DBuffer)
{
	// Release the 3D INTerface to the secondary sound buffer.
	if (*secondary3DBuffer)
	{
		(*secondary3DBuffer)->Release();
		*secondary3DBuffer = 0;
	}

	// Release the secondary sound buffer.
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}


BOOL Dzwieki::PlayWaveFile(LONG IVolume, IDirectSoundBuffer8* m_secondaryBuffer, IDirectSound3DBuffer8* m_secondary3DBuffer, DWORD Loop)
{
	HRESULT result;
	FLOAT positionX, positionY, positionZ;

	// Set the 3D position of where the sound should be located.
	positionX = 0.0f;
	positionY = 0.0f;
	positionZ = 0.0f;

	// Set position at the beginning of the sound buffer.
	result = m_secondaryBuffer->SetCurrentPosition(0);
	if (FAILED(result))
		return false;

	// Set volume of the buffer
	result = m_secondaryBuffer->SetVolume(IVolume);
	if (FAILED(result))
		return false;

	// Set the 3D position of the sound.
	m_secondary3DBuffer->SetPosition(positionX, positionY, positionZ, DS3D_IMMEDIATE);

	// Play the contents of the secondary sound buffer.
	result = m_secondaryBuffer->Play(0, 0, Loop);
	if (FAILED(result))
		return false;

	return true;
}

void Dzwieki::dzwiek_potwierdzenie(LONG glosnosc_efektow)
{
	potwierdzenie = PlayWaveFile(glosnosc_efektow, m_secondaryBuffer2, m_secondary3DBuffer2, 0);
}

void Dzwieki::dzwiek_menu(INT odtwarzanie_dzwieku_menu[20][18], INT odtworzone[20][18], LONG glosnosc_efektow)
{
	for (INT fram = 0; fram < 20; fram++)
	{
		for (INT framik = 0; framik < 18; framik++)
		{
			if ((odtwarzanie_dzwieku_menu[fram][framik] == 1) && (odtworzone[fram][framik] == 1))
			{
				pmenu = PlayWaveFile(glosnosc_efektow, m_secondaryBuffer1, m_secondary3DBuffer1, 0);
				odtwarzanie_dzwieku_menu[fram][framik] = 0;
				odtworzone[fram][framik] = 0;
			}
		}
	}
}

void Dzwieki::muza_tlo(LONG glosnosc_tla)
{
	muzyka_tlo = PlayWaveFile(glosnosc_tla, m_secondaryBuffer3, m_secondary3DBuffer3, DSBPLAY_LOOPING);
}

BOOL Dzwieki::muzyka_tlo_volume(LONG glosnosc_tla)
{
	HRESULT result;
	// Set volume of the buffer
	result = m_secondaryBuffer3->SetVolume(glosnosc_tla);
	if (FAILED(result))
		return false;

	return true;
}