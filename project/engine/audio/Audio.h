#pragma once
#include <fstream>
#include <wrl.h>
#include <xaudio2.h>
#include <memory>

#include <unordered_map>

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

	static std::unique_ptr<Audio> instance;

	friend std::unique_ptr<Audio> std::make_unique<Audio>();
	friend std::default_delete<Audio>;

	Audio() = default;
	~Audio() = default;
	Audio(Audio&) = delete;
	Audio& operator=(Audio&) = delete;

public:

	// シングルトンインスタンスの取得
	static Audio* GetInstance();

	void Initialize();

	void Finalize();

	void SoundLoadWave(const char* filename);
	
	void SoundUnload();
	
	// 音声再生
	void SoundPlayWave(const char* filename);
	

private:

	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;

	IXAudio2MasteringVoice* masterVoice;

	SoundData soundData{};

	std::unordered_map<std::string, SoundData> soundDatas;

	IXAudio2SourceVoice* pSourceVoice = nullptr;

};