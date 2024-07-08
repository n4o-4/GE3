#pragma once
#include <fstream>
#include <wrl.h>
#include <xaudio2.h>

#pragma comment(lib,"xaudio2.lib")

class Audio {

private:
	// チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];   // チャンク毎のID
		int32_t size; // チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk; // "RIFF"
		char type[4];      // WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk; // "fmt"
		WAVEFORMATEX fmt;  // 波系フォーマット
	};

	// 音声データ
	struct SoundData
	{
		// 波系フォーマット
		WAVEFORMATEX wfex;

		// バッファの先頭アドレス
		BYTE* pBuffer;

		// バッファのサイズ
		unsigned int bufferSize;
	};

public:

private:

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;

	IXAudio2MasteringVoice* masterVoice;

	SoundData soundData{};

public:

	~Audio();

	void Initialize();

	void SoundLoadWave(const char* filename);
	
	void SoundUnload();
	
	// 音声再生
	void SoundPlayWave();
	

private:

	IXAudio2SourceVoice* pSourceVoice = nullptr;

};